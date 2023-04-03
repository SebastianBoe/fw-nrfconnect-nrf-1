/*
 * Copyright (c) 2018 Nordic Semiconductor ASA.
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#include <spm.h>
#include <aarch32/cortex_m/tz.h>
#include <stdio.h>
#include <secure_services.h>
#include <nrfx_nvmc.h>

__TZ_NONSECURE_ENTRY_FUNC
void rcrOtpRead_nse(size_t num)
{
  for (size_t i = 0; i < num; i++)
  {
    volatile uint32_t val32 = *(const uint32_t *)0xff8108;
    // volatile uint32_t val32 = *(const uint32_t *)0xff8;
  }
}


void main(void)
{
	spm_config();
	spm_jump();
}
