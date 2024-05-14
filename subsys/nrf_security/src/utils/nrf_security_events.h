/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

typedef struct nrf_security_event_t {
#if defined(CONFIG_EVENTS) && !defined(__NRF_TFM__)
        struct k_event *zephyr_event;
#else
	uint32_t volatile val;
#endif
} nrf_security_event_t;

void nrf_security_event_init(nrf_security_event_t event);

uint32_t nrf_security_event_wait(nrf_security_event_t event, uint32_t events);

void nrf_security_event_clear(nrf_security_event_t event, uint32_t events);

uint32_t nrf_security_event_set(nrf_security_event_t event, uint32_t events);