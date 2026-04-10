/*
 * Copyright (c) 2026 Nordic Semiconductor ASA.
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/logging/log.h>
#include <ironside/se/boot_report.h>
#include <ironside/se/api.h>

LOG_MODULE_REGISTER(snapshot_capture_recover, CONFIG_LOG_DEFAULT_LEVEL);

#define IRONSIDE_NV_COUNTER_ID IRONSIDE_SE_COUNTER_0

/* When RUN_FOREVER is defined: capture/recover forever.
 * When undefined: stop capturing and recovering after NV counter reaches NV_COUNTER_STOP.
 */
/* #define RUN_FOREVER */
#define NV_COUNTER_STOP 5

static const char *snapshot_status_str(uint8_t status)
{
	switch (status) {
	case IRONSIDE_SE_SNAPSHOT_STATUS_NONE:
		return "None";
	case IRONSIDE_SE_SNAPSHOT_STATUS_CAPTURE_SUCCESSFUL:
		return "Capture Successful";
	case IRONSIDE_SE_SNAPSHOT_STATUS_CAPTURE_FAILED:
		return "Capture Failed";
	case IRONSIDE_SE_SNAPSHOT_STATUS_RECOVERY:
		return "Recovery";
	case IRONSIDE_SE_SNAPSHOT_STATUS_CORRUPTION_DETECTED:
		return "Corruption Detected";
	default:
		return "Unknown";
	}
}

int main(void)
{
	const struct ironside_se_boot_report *boot_report = IRONSIDE_SE_BOOT_REPORT;
	uint32_t nv_counter_value = 0;
	uint32_t nv_current;
	int rc;

	rc = ironside_se_counter_get(IRONSIDE_NV_COUNTER_ID, &nv_counter_value);
	if (rc != 0) {
		printf("NV counter get failed: %d\n", rc);
		nv_counter_value = 0;
	}

	/* At every boot: print NV counter at boot and after increment */
	rc = ironside_se_counter_set(IRONSIDE_NV_COUNTER_ID, nv_counter_value + 1);
	if (rc == 0) {
		printf("NV counter: %u -> %u\n", nv_counter_value, nv_counter_value + 1);
		nv_current = nv_counter_value + 1;
	} else {
		printf("NV counter at boot: %u (set failed: %d)\n", nv_counter_value, rc);
		nv_current = nv_counter_value;
	}

	/* Check if boot report is valid */
	if (boot_report == NULL || boot_report->magic != IRONSIDE_SE_BOOT_REPORT_MAGIC) {
		printf("ERROR: Invalid boot report\n");
		for (;;) {
			k_sleep(K_SECONDS(1));
		}
	}

	{
		uint8_t st = boot_report->snapshot.status;

		if (st <= IRONSIDE_SE_SNAPSHOT_STATUS_CORRUPTION_DETECTED) {
			printf("Snapshot status: %s\n", snapshot_status_str(st));
		} else {
			printf("Snapshot status: Unknown (0x%02x)\n", st);
		}
	}

#ifndef RUN_FOREVER
	/* Stop capture/recover after NV counter reaches limit */
	if (nv_current >= NV_COUNTER_STOP) {
		printf("NV counter %u >= %d, stopping capture/recover\n", nv_current,
		       NV_COUNTER_STOP);
		for (int i = 0; i < 100000; i++) {
			printf("Heartbeat\t%d\n", i);
			if (((i + 1) % 5) == 0) {
				printf("Doing lots of MRAM access to cause a cache miss and possibly detect corruption\n");

				for (volatile uint32_t *mram_addr = (volatile uint32_t *)0x0E042000;
				     (uint32_t)mram_addr < (0x0E042000 + 256 * 1024); mram_addr++) {
					volatile uint32_t mram = *mram_addr;
					(void)mram;
				}

				printf("Finished accessing MRAM\n");
			}
			k_sleep(K_SECONDS(3));
		}
	}
#endif

	/* Handle different snapshot statuses */
	switch (boot_report->snapshot.status) {
	case IRONSIDE_SE_SNAPSHOT_STATUS_NONE:
		printf("Capturing the value %u\n", nv_current);
		rc = ironside_se_snapshot_capture(IRONSIDE_SE_SNAPSHOT_CAPTURE_NO_INCREMENT);
		/* This should never be reached as snapshot_capture causes a reset */
		printf("ERROR: snapshot_capture returned: %d\n", rc);
		return rc;

	case IRONSIDE_SE_SNAPSHOT_STATUS_CAPTURE_SUCCESSFUL:
		printf("Requesting snapshot recovery.\n");
		rc = ironside_se_snapshot_recovery();
		/* This should never be reached as snapshot_recovery causes a reset */
		printf("ERROR: snapshot_recovery returned: %d\n", rc);
		return rc;

	case IRONSIDE_SE_SNAPSHOT_STATUS_RECOVERY:
		printf("Recovered the value %u\n", nv_counter_value);
		sys_reboot(SYS_REBOOT_COLD);
		break;

	case IRONSIDE_SE_SNAPSHOT_STATUS_CAPTURE_FAILED:
		for (;;) {
			k_sleep(K_SECONDS(1));
		}
		break;

	case IRONSIDE_SE_SNAPSHOT_STATUS_CORRUPTION_DETECTED:
		for (;;) {
			k_sleep(K_SECONDS(1));
		}
		break;

	default:
		for (;;) {
			k_sleep(K_SECONDS(1));
		}
		break;
	}

	/* Should never reach here */
	return 0;
}
