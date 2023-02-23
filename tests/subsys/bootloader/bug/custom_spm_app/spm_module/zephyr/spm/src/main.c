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
bool lRcrOtpReadBytes_nse
(
  uint32_t  xAddr,
  uint8_t*  xpDest,
  size_t    xNumBytes
)
{
  bool      retval  = false;

  // Ensure that what we want to read is in OTP area
  if (xAddr >= 0 && (xAddr + xNumBytes) <= 0xFFFFFFFF &&
      (xNumBytes % 2u == 0u) && (xAddr % 2u == 0u))
  {
    uint16_t  otpRead = 0u;

    for (size_t i = 0u; i < xNumBytes; i = i + sizeof(uint16_t))
    {
      otpRead = nrfx_nvmc_otp_halfword_read(0xff8108);
      xpDest[i + 1u] = (uint8_t)(otpRead >> 8u);
      xpDest[i] = (uint8_t)otpRead;
    }
    retval = true;
  }

  return retval;
} // lRcrOtpReadBytes_nse

/**
 * @brief   Read the Serial Number in OTP
 *
 * @param[in] xpDest          Buffer to write serial number read.
 *                            Must not be NULL.
 * @param[in] xNumBytes       Size of the xpDest buffer
 *                            Should equal to PM_SERIAL_NUMBER_SIZE
 *
 * @return  true in case of success, false otherwise
 */
static bool lRcrOtpRead
(
  uint8_t*  xpDest,
  size_t   xNumBytes
)
{
  bool retval = false;

  if (4 == xNumBytes)
  {
    retval = lRcrOtpReadBytes_nse(0xff8108, xpDest, xNumBytes);
  }

  return retval;
} // lRcrOtpRead

__TZ_NONSECURE_ENTRY_FUNC
bool rcrOtpRead_nse
(
  uint8_t  xField,
  uint8_t*      xpDest,
  size_t*       xpNumBytes
)
{
  bool retval = false;

  for (;;)
  {
    if (xField >= 4 || NULL == xpDest || NULL == xpNumBytes)
    {
      retval = false;
      break;
    }
    switch (xField)
    {
      case 1:
        retval = lRcrOtpRead(xpDest, *xpNumBytes);
        break;

      default:
        break;
    } //switch

    break;
  } // for

	return retval;
}


int spm_request_random_number_nse(uint8_t *output, size_t len, size_t *olen)
{
	return 0;
}

void main(void)
{
	spm_config();
	spm_jump();
}
