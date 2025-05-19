/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <psa/crypto.h>
#include <psa/crypto_extra.h>
#include <zephyr/logging/log.h>

#ifdef IMPORT_KEY_TEST
#include "import_key_test.h"
#endif

/* [0] As a special case, the last element of a structure with more
 * than one named member may have an incomplete array type; this is
 * called a flexible array member. In most situations, the flexible
 * array member is ignored. In particular, the size of the structure
 * is as if the flexible array member were omitted except that it may
 * have more trailing padding than the omission would imply.
 */

struct psa_import_key_args {
	uint32_t magic;
	psa_key_id_t key;
	const psa_key_attributes_t attributes;
	size_t data_length;
	/* Note that when we later calculate the sizeof this struct that
	 * the data member will be omitted[0].
	 */
	const uint8_t data[];
};

struct psa_import_key_args_in_ram {
	uint32_t num_psa_keys;
	struct psa_import_key_args psa_keys[];
};

struct psa_import_key_args_in_ram *args_in_ram = (struct psa_import_key_args_in_ram *)0x2f021000;

int main(void)
{
#ifdef IMPORT_KEY_TEST
	fake_flashing_of_psa_keys(args_in_ram);
#endif

	struct psa_import_key_args *args = args_in_ram->psa_keys;

	/* Create a pointer to a byte to be able to do pointer arithmetic. */
	uint8_t * args_bytes = (uint8_t *)args;


	for (int i = 0; i < args_in_ram->num_psa_keys; i++) {
		if(args->magic != 0x5eb0) {
			printf("magic value not found. Were the keys flashed correctly?\n");
			return 1;
		}

		psa_status_t status = psa_import_key(&(args->attributes), args->data,
						     args->data_length, &(args->key));
		if (status) {
			printf("psa_import_key failed with status %d\n", status);
			return 1;
		}

		/* see [0] */
		args_bytes += sizeof(struct psa_import_key_args);
		args_bytes += args->data_length;

		args = (struct psa_import_key_args *)args_bytes;
	}

	printf("finished importing %d psa keys", args_in_ram->num_psa_keys);
}
