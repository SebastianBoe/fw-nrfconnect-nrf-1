/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <stdint.h>

/* Used by EXT_API declaration */

typedef void (*implementation_id_value_read_t)(uint8_t * ptr);

typedef size_t (*implementation_id_len_t)(void);

struct bl_storage_ext_api {
	/* Actual EXT_API/data goes here. */
	implementation_id_value_read_t implementation_id_value_read;
	implementation_id_len_t implementation_id_len;
};
