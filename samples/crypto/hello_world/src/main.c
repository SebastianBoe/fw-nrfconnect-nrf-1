/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(fault_handler, LOG_LEVEL_ERR);

int main(void)
{
	printf("Hello World! main function at %x\n", (int)main);

	return 0;
}

extern uint32_t __start;

void k_sys_fatal_error_handler(unsigned int reason, const struct arch_esf *esf) {
    LOG_ERR("Custom fault handler called");
    LOG_ERR("Reason: %u", reason);

    // Perform custom actions, such as logging or resetting hardware
    if (reason == 20) {
        LOG_ERR("? occurred!");
		// This is a known crash. We don't know why it happens. But it
		// only happens on the first execution of the image so we set
		// PC back to the reset vector and try again.

		void (*hacky_jump_back_to_reset)(void) = (void (*)(void))__start;
		hacky_jump_back_to_reset();

	a: goto a;
		CODE_UNREACHABLE;
	}

    // Halt the system or take necessary recovery action
    k_panic();
}
