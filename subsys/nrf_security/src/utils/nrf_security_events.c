/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <stdbool.h>

#include <nrf_security_events.h>

#if defined(__NRF_TFM__)
#include "cmsis.h"
#include "utilities.h"
#endif

#if defined(CONFIG_EVENTS) && !defined(__NRF_TFM__)

#include <zephyr/kernel.h>

void nrf_security_event_init(struct nrf_security_event_t event)
{
	k_event_init(event.zephyr_event);
}

uint32_t nrf_security_event_wait(struct nrf_security_event_t event, uint32_t events)
{
	/* Don't clear the events before waiting and wait forever */
	return k_event_wait(event.zephyr_event, events, false, K_FOREVER);
}

void nrf_security_event_clear(struct nrf_security_event_t event, uint32_t events)
{
	k_event_clear(event.zephyr_event, events);
}

uint32_t nrf_security_event_set(struct nrf_security_event_t event, uint32_t events)
{
	return k_event_set(event.zephyr_event, events);
}

#else

void nrf_security_event_init(nrf_security_event_t event)
{
	event.val = 0;
}

uint32_t nrf_security_event_wait(nrf_security_event_t event, uint32_t events)
{
	/* Wait while we have none of the subscribed events */
	while ((event.val & events) == 0) {
		__WFE();
	}

	return event.val & events;
}

void nrf_security_event_clear(nrf_security_event_t event, uint32_t events)
{
	event.val &= ~events;
}

uint32_t nrf_security_event_set(nrf_security_event_t event, uint32_t events)
{
	uint32_t prev = event.val;

	event.val |= events;

	return prev;
}

#endif
