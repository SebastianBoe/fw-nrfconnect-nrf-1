/*
 * Copyright (c) 2025 Nordic Semiconductor ASA.
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#ifdef CONFIG_SOC_NRF54H20_CPURAD
#define RUNNING_ON_RADIOCORE 1
#else
#define RUNNING_ON_RADIOCORE 0
#endif

int main(void)
{
	printk("=== Hello World from Secondary Image ===\n");

#if RUNNING_ON_RADIOCORE
	printk("*** VERIFIED: Running on RADIO CORE (cpurad) ***\n");
	printk("  - CONFIG_SOC_NRF54H20_CPURAD is enabled\n");
#ifdef CONFIG_BOARD_NRF54H20DK_NRF54H20_CPURAD
	printk("  - CONFIG_BOARD_NRF54H20DK_NRF54H20_CPURAD is enabled\n");
#endif
#else
	printk("*** WARNING: NOT running on radio core! ***\n");
#endif

	printk("Secondary image initialization complete\n");

	/* Keep the secondary image running */
	while (1) {
		k_msleep(3000);
		printk("Secondary image heartbeat\n");
	}

	return 0;
}
