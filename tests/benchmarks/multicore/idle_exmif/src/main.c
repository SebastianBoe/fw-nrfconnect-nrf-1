/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/cache.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/mspi.h>
#include <zephyr/pm/device_runtime.h>
#include <zephyr/sys/printk.h>

#define MSPI_BUS	       DT_BUS(DT_ALIAS(external_memory))
#define MSPI_TARGET	       DT_ALIAS(external_memory)
#define SPI_NOR_CMD_RDID       0x9F
#define SPI_NOR_CMD_WREN       0x06
#define SPI_NOR_CMD_WR_CFGREG2 0x72
#define JESD216_OCMD_READ_ID   0x9F60
#define SPI_NOR_OCMD_READ      0xFD  /* Octal read command */
#define MSPI_FREQUENCY_MHZ     MHZ(1)
#define NUMBER_OF_TEST_READS   10
#define READ_DATA_SIZE         256
#define SEARCH_CHUNK_SIZE      512   /* Read in 512 byte chunks for searching */
#define MAX_SEARCH_SIZE        (1024 * 1024)  /* Search at most 1MB */

static const struct device *controller = DEVICE_DT_GET(MSPI_BUS);
static struct mspi_dev_id dev_id = MSPI_DEVICE_ID_DT(MSPI_TARGET);
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led), gpios);

static void set_qspi_mode(const struct device *controller, struct mspi_dev_id *dev_id)
{
	int ret;

	const struct mspi_xfer_packet write_enable[] = {
		{
			.dir = MSPI_TX,
			.cmd = SPI_NOR_CMD_WREN,
			.address = 0,
			.num_bytes = 0,
		},
	};

	const struct mspi_xfer wren_xfer = {
		.xfer_mode = MSPI_PIO,
		.rx_dummy = 0,
		.tx_dummy = 0,
		.cmd_length = 1,
		.addr_length = 0,
		.packets = write_enable,
		.num_packet = sizeof(write_enable) / sizeof(struct mspi_xfer_packet),
		.priority = 1,
		.timeout = 30,
	};

	static const uint8_t set_qspi[] = {0x01};
	const struct mspi_xfer_packet enable_qspi[] = {
		{
			.dir = MSPI_TX,
			.cmd = SPI_NOR_CMD_WR_CFGREG2,
			.address = 0,
			.data_buf = (uint8_t *)&set_qspi,
			.num_bytes = sizeof(set_qspi),
		},
	};

	const struct mspi_xfer set_qspi_xfer = {
		.xfer_mode = MSPI_PIO,
		.rx_dummy = 0,
		.tx_dummy = 0,
		.cmd_length = 1,
		.addr_length = 4,
		.packets = enable_qspi,
		.num_packet = sizeof(enable_qspi) / sizeof(struct mspi_xfer_packet),
		.priority = 1,
		.timeout = 30,
	};

	printk("Setting QSPI mode\n");

	ret = mspi_transceive(controller, dev_id, &wren_xfer);
	printk("'mspi_transceive (WREN)' return code: %d\n", ret);

	ret = mspi_transceive(controller, dev_id, &set_qspi_xfer);
	printk("'mspi_transceive (SET QSPI)' return code: %d\n", ret);
}

static void read_jedec_id(const struct device *controller, struct mspi_dev_id *dev_id)
{
	int ret;
	uint8_t rx_data[3] = {0};
	const struct mspi_xfer_packet dev_id_packet[] = {
		{
			.dir = MSPI_RX,
			.cmd = JESD216_OCMD_READ_ID,
			.address = 0,
			.num_bytes = sizeof(rx_data),
			.data_buf = rx_data,
		},
	};

	const struct mspi_xfer device_id_read_xfer = {
		.xfer_mode = MSPI_PIO,
		.rx_dummy = 4,
		.cmd_length = 2,
		.addr_length = 4,
		.packets = dev_id_packet,
		.num_packet = sizeof(dev_id_packet) / sizeof(struct mspi_xfer_packet),
		.priority = 1,
		.timeout = 30,
	};

	printk("Read JEDEC ID\n");
	ret = mspi_transceive(controller, dev_id, &device_id_read_xfer);
	printk("'mspi_transceive' return code: %d\n", ret);
	__ASSERT_NO_MSG(ret == 0);

	k_msleep(10);
	printk("Manufacturer ID: 0x%x\n", rx_data[0]);
	printk("Memory type: 0x%x\n", rx_data[1]);
	printk("Memory density: 0x%x\n", rx_data[2]);
	__ASSERT_NO_MSG(rx_data[0] != 0);
	__ASSERT_NO_MSG(rx_data[1] != 0);
	__ASSERT_NO_MSG(rx_data[2] != 0);
}

static void read_external_memory(const struct device *controller, struct mspi_dev_id *dev_id)
{
	int ret;
	uint32_t search_addr = 0;
	uint32_t found_addr = 0;
	bool found = false;
	uint8_t search_buffer[SEARCH_CHUNK_SIZE];
	uint8_t rx_data[READ_DATA_SIZE] = {0};

	/* Search for non-zero data */
	printk("Searching for non-zero data in external memory (max 1MB)...\n");
	
	while (search_addr < MAX_SEARCH_SIZE && !found) {
		struct mspi_xfer_packet search_packet[] = {
			{
				.dir = MSPI_RX,
				.cmd = SPI_NOR_OCMD_READ,
				.address = search_addr,
				.num_bytes = SEARCH_CHUNK_SIZE,
				.data_buf = search_buffer,
			},
		};

		struct mspi_xfer search_xfer = {
			.xfer_mode = MSPI_DMA,
			.rx_dummy = 8,  /* Typical dummy cycles for octal read */
			.tx_dummy = 0,
			.cmd_length = 1,  /* Octal read command is 1 byte (0xFD) */
			.addr_length = 4,
			.packets = search_packet,
			.num_packet = sizeof(search_packet) / sizeof(struct mspi_xfer_packet),
			.priority = 1,
			.timeout = 100,
		};

		ret = mspi_transceive(controller, dev_id, &search_xfer);
		if (ret != 0) {
			printk("'mspi_transceive (search)' failed at address 0x%08x, return code: %d\n", 
			       search_addr, ret);
			break;
		}

		/* Check for non-zero bytes in this chunk */
		for (uint32_t i = 0; i < SEARCH_CHUNK_SIZE; i++) {
			if (search_buffer[i] != 0) {
				found_addr = search_addr + i;
				found = true;
				printk("Found non-zero data at address 0x%08x (value: 0x%02x)\n", 
				       found_addr, search_buffer[i]);
				break;
			}
		}

		if (!found) {
			search_addr += SEARCH_CHUNK_SIZE;
			if (search_addr % (64 * 1024) == 0) {
				printk("Searched up to address 0x%08x...\n", search_addr);
			}
		}
	}

	if (!found) {
		printk("No non-zero data found in first 1MB of external memory\n");
		return;
	}

	/* Read 256 bytes starting from the found address */
	printk("Reading %d bytes from address 0x%08x\n", READ_DATA_SIZE, found_addr);
	
	struct mspi_xfer_packet read_packet[] = {
		{
			.dir = MSPI_RX,
			.cmd = SPI_NOR_OCMD_READ,
			.address = found_addr,
			.num_bytes = READ_DATA_SIZE,
			.data_buf = rx_data,
		},
	};

	struct mspi_xfer read_xfer = {
		.xfer_mode = MSPI_DMA,
		.rx_dummy = 8,  /* Typical dummy cycles for octal read */
		.tx_dummy = 0,
		.cmd_length = 1,  /* Octal read command is 1 byte (0xFD) */
		.addr_length = 4,
		.packets = read_packet,
		.num_packet = sizeof(read_packet) / sizeof(struct mspi_xfer_packet),
		.priority = 1,
		.timeout = 100,
	};

	ret = mspi_transceive(controller, dev_id, &read_xfer);
	printk("'mspi_transceive (read memory)' return code: %d\n", ret);
	__ASSERT_NO_MSG(ret == 0);

	/* Print all 256 bytes as hex dump */
	printk("256 bytes of data starting from address 0x%08x:\n", found_addr);
	for (int i = 0; i < READ_DATA_SIZE; i++) {
		if (i % 16 == 0) {
			printk("%04x: ", i);
		}
		printk("%02x ", rx_data[i]);
		if (i % 16 == 15) {
			printk("\n");
		}
	}
	printk("\n");
}

static void configure_exmif_for_test(const struct device *controller, struct mspi_dev_id *dev_id)
{
	int ret;
	const struct mspi_dev_cfg spi_transfer_cfg = {
		.freq = MSPI_FREQUENCY_MHZ,
		.io_mode = MSPI_IO_MODE_SINGLE,
	};

	const struct mspi_dev_cfg qspi_transfer_cfg = {
		.freq = MSPI_FREQUENCY_MHZ,
		.io_mode = MSPI_IO_MODE_OCTAL,
	};

	printk("Configuring memory device\n");

	ret = device_is_ready(controller);
	printk("'device_is_ready' return code: %d\n", ret);
	__ASSERT_NO_MSG(ret == 1);

	pm_device_runtime_get(controller);

	ret = mspi_dev_config(controller, dev_id,
			      MSPI_DEVICE_CONFIG_FREQUENCY | MSPI_DEVICE_CONFIG_IO_MODE,
			      &spi_transfer_cfg);
	printk("'mspi_dev_config (SPI)'  return code: %d\n", ret);
	__ASSERT_NO_MSG(ret == 0);

	set_qspi_mode(controller, dev_id);

	ret = mspi_dev_config(controller, dev_id,
			      MSPI_DEVICE_CONFIG_FREQUENCY | MSPI_DEVICE_CONFIG_IO_MODE,
			      &qspi_transfer_cfg);
	printk("'mspi_dev_config (QSPI)' return code: %d\n", ret);
	__ASSERT_NO_MSG(ret == 0);

	read_jedec_id(controller, dev_id);
}

int main(void)
{
	int ret;
	int test_repetitions = 3;

	printk("Multicore idle exmif test on %s\n", CONFIG_BOARD_TARGET);
	k_msleep(10);

	ret = gpio_is_ready_dt(&led);
	printk("'gpio_is_ready_dt' return code: %d\n", ret);
	__ASSERT_NO_MSG(ret == 1);
	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
	printk("'gpio_pin_configure_dt' return code: %d\n", ret);
	__ASSERT_NO_MSG(ret == 0);

	configure_exmif_for_test(controller, &dev_id);
	k_msleep(500);

	/* Read 256 bytes from the beginning of external memory */
	read_external_memory(controller, &dev_id);

#if defined(CONFIG_COVERAGE)
	printk("Coverage analysis enabled\n");
	while (test_repetitions--)
#else
	while (test_repetitions)
#endif
	{
		printk("Wake up\n");
		for (int counter = 0; counter < NUMBER_OF_TEST_READS; counter++) {
			read_jedec_id(controller, &dev_id);
		}
		pm_device_runtime_put(controller);
		printk("Go to sleep\n");
		gpio_pin_set_dt(&led, 0);
		k_msleep(1000);
		gpio_pin_set_dt(&led, 1);
	}

#if defined(CONFIG_COVERAGE)
	printk("Coverage analysis start\n");
#endif
	return 0;
}
