/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

typedef struct {
#if defined(CONFIG_MULTITHREADING) && !defined(__NRF_TFM__)
        struct k_mutex *zephyr_mutex;
#else
        uint32_t dummy;
#endif
} nrf_security_mutex_t;

int nrf_security_mutex_lock(nrf_security_mutex_t mutex);

int nrf_security_mutex_unlock(nrf_security_mutex_t mutex);