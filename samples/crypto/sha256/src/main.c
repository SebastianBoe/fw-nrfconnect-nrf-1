/*
 * Copyright (c) 2021, 2025 Nordic Semiconductor ASA
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

#ifdef CONFIG_BUILD_WITH_TFM
#include <tfm_ns_interface.h>
#endif

#define APP_SUCCESS		(0)
#define APP_ERROR		(-1)
#define APP_SUCCESS_MESSAGE "Example finished successfully!"
#define APP_ERROR_MESSAGE "Example exited with error!"

#define PRINT_HEX(p_label, p_text, len)\
	({\
		LOG_INF("---- %s (len: %u): ----", p_label, len);\
		LOG_HEXDUMP_INF(p_text, len, "Content:");\
		LOG_INF("---- %s end  ----", p_label);\
	})

LOG_MODULE_REGISTER(sha256, LOG_LEVEL_DBG);

/* ====================================================================== */
/*				Global variables/defines for the SHA512 example			  */

#define NRF_CRYPTO_EXAMPLE_SHA512_TEXT_SIZE (200)
#define NRF_CRYPTO_EXAMPLE_SHA512_SIZE (64)

/* Below text is used as plaintext for computing/verifying the hash. */
static uint8_t m_plain_text[NRF_CRYPTO_EXAMPLE_SHA512_TEXT_SIZE] = {
	"Example string to demonstrate basic usage of SHA512."
	"That uses single and multi-part PSA crypto API's to "
	"perform a SHA-512 hashing operation. This text is longer "
	"to test the bug reproduction scenarios."
};

static uint8_t m_hash[NRF_CRYPTO_EXAMPLE_SHA512_SIZE];

/* ====================================================================== */

int crypto_init(void)
{
	psa_status_t status;

	/* Initialize PSA Crypto */
	status = psa_crypto_init();
	if (status != PSA_SUCCESS)
		return APP_ERROR;

	return APP_SUCCESS;
}

int hash_singlepart_sha512(void)
{
	uint32_t olen;
	psa_status_t status;

	LOG_INF("Hashing using SHA-512...");

	/* Calculate the SHA-512 hash */
	status = psa_hash_compute(
		PSA_ALG_SHA_512, m_plain_text, sizeof(m_plain_text), m_hash, sizeof(m_hash), &olen);
	if (status != PSA_SUCCESS) {
		LOG_INF("psa_hash_compute failed! (Error: %d)", status);
		return APP_ERROR;
	}

	LOG_INF("Hash computation successful!");
	PRINT_HEX("SHA-512 hash", m_hash, sizeof(m_hash));

	return APP_SUCCESS;
}

int hash_multipart_sha512(void)
{
	uint32_t olen;
	psa_status_t status;
	uint8_t *input_ptr = m_plain_text;
	psa_hash_operation_t hash_operation = {0};

	LOG_INF("Hashing using multi-part SHA-512...");

	/* Setup a multipart hash operation */
	status = psa_hash_setup(&hash_operation, PSA_ALG_SHA_512);
	if (status != PSA_SUCCESS) {
		LOG_ERR("Could not setup the hash operation! Error %d", status);
		return APP_ERROR;
	}


	/* Feed the chunks of the input data to the PSA driver */
	status = psa_hash_update(&hash_operation, input_ptr, 42);
	if (status != PSA_SUCCESS) {
		LOG_ERR("Could not hash the next chunk! Error %d", status);
		return APP_ERROR;

	}
	LOG_INF("Added %d bytes", 42);
	input_ptr += 42;


	status = psa_hash_update(&hash_operation, input_ptr, 58);
	if (status != PSA_SUCCESS) {
		LOG_ERR("Could not hash the next chunk! Error %d", status);
		return APP_ERROR;

	}
	LOG_INF("Added %d bytes", 58);
	input_ptr += 58;

	status = psa_hash_update(&hash_operation, input_ptr, 50);
	if (status != PSA_SUCCESS) {
		LOG_ERR("Could not hash the next chunk! Error %d", status);
		return APP_ERROR;

	}
	LOG_INF("Added %d bytes", 50);

	status = psa_hash_finish(&hash_operation, m_hash, sizeof(m_hash), &olen);
	if (status != PSA_SUCCESS) {
		LOG_ERR("Could not finish the hash operation! Error %d", status);
		return APP_ERROR;
	}

	LOG_INF("Hash computation successful!");
	PRINT_HEX("SHA-512 hash", m_hash, sizeof(m_hash));

	return APP_SUCCESS;
}

int verify_sha512(void)
{
	psa_status_t status;

	LOG_INF("Verifying the SHA-512 hash...");

	/* Verify the hash */
	status = psa_hash_compare(
		PSA_ALG_SHA_512, m_plain_text, sizeof(m_plain_text), m_hash, sizeof(m_hash));
	if (status != PSA_SUCCESS) {
		LOG_INF("psa_hash_compare failed! (Error: %d)", status);
		return APP_ERROR;
	}

	LOG_INF("SHA-512 verification successful!");

	return APP_SUCCESS;
}

/* Test case 1: Single call to psa_hash_update() with > 127 bytes */
int test_bug_single_update_over_127_bytes(void)
{
	uint32_t olen;
	psa_status_t status;
	psa_hash_operation_t hash_operation = {0};
	uint8_t test_data[200];
	uint8_t test_hash[NRF_CRYPTO_EXAMPLE_SHA512_SIZE];

	/* Initialize test data */
	memset(test_data, 0xAA, sizeof(test_data));

	LOG_INF("=== Test: Single psa_hash_update() with 200 bytes (should trigger bug) ===");

	/* Setup a multipart hash operation */
	status = psa_hash_setup(&hash_operation, PSA_ALG_SHA_512);
	if (status != PSA_SUCCESS) {
		LOG_ERR("Could not setup the hash operation! Error %d", status);
		return APP_ERROR;
	}

	/* This should trigger the bug: single update with 200 bytes (> 127) */
	status = psa_hash_update(&hash_operation, test_data, 200);
	if (status == PSA_ERROR_INVALID_ARGUMENT) {
		LOG_ERR("BUG REPRODUCED! psa_hash_update() returned PSA_ERROR_INVALID_ARGUMENT (-135)");
		LOG_ERR("This is the expected bug behavior for SHA-512 with > 127 bytes in single update");
		psa_hash_abort(&hash_operation);
		return APP_ERROR;
	} else if (status != PSA_SUCCESS) {
		LOG_ERR("psa_hash_update() failed with unexpected error: %d", status);
		psa_hash_abort(&hash_operation);
		return APP_ERROR;
	}

	LOG_INF("psa_hash_update() succeeded (bug not reproduced)");
	status = psa_hash_finish(&hash_operation, test_hash, sizeof(test_hash), &olen);
	if (status != PSA_SUCCESS) {
		LOG_ERR("Could not finish the hash operation! Error %d", status);
		return APP_ERROR;
	}

	return APP_SUCCESS;
}

/* Test case 2: Multiple calls to psa_hash_update() where accumulated bytes exceed 127 */
int test_bug_multiple_updates_exceed_127_bytes(void)
{
	uint32_t olen;
	psa_status_t status;
	psa_hash_operation_t hash_operation = {0};
	uint8_t test_data[200];
	uint8_t test_hash[NRF_CRYPTO_EXAMPLE_SHA512_SIZE];

	/* Initialize test data */
	memset(test_data, 0xBB, sizeof(test_data));

	LOG_INF("=== Test: Multiple psa_hash_update() calls totaling 200 bytes (should trigger bug) ===");

	/* Setup a multipart hash operation */
	status = psa_hash_setup(&hash_operation, PSA_ALG_SHA_512);
	if (status != PSA_SUCCESS) {
		LOG_ERR("Could not setup the hash operation! Error %d", status);
		return APP_ERROR;
	}

	/* First update: 100 bytes */
	status = psa_hash_update(&hash_operation, test_data, 100);
	if (status != PSA_SUCCESS) {
		LOG_ERR("First psa_hash_update() failed! Error %d", status);
		psa_hash_abort(&hash_operation);
		return APP_ERROR;
	}
	LOG_INF("Added %d bytes (total: 100)", 100);

	/* Second update: 100 bytes (total will be 200, which exceeds 127) */
	status = psa_hash_update(&hash_operation, test_data + 100, 100);
	if (status == PSA_ERROR_INVALID_ARGUMENT) {
		LOG_ERR("BUG REPRODUCED! psa_hash_update() returned PSA_ERROR_INVALID_ARGUMENT (-135)");
		LOG_ERR("This is the expected bug behavior for SHA-512 when accumulated bytes exceed 127");
		psa_hash_abort(&hash_operation);
		return APP_ERROR;
	} else if (status != PSA_SUCCESS) {
		LOG_ERR("psa_hash_update() failed with unexpected error: %d", status);
		psa_hash_abort(&hash_operation);
		return APP_ERROR;
	}

	LOG_INF("Added %d bytes (total: 200)", 100);
	LOG_INF("psa_hash_update() succeeded (bug not reproduced)");
	status = psa_hash_finish(&hash_operation, test_hash, sizeof(test_hash), &olen);
	if (status != PSA_SUCCESS) {
		LOG_ERR("Could not finish the hash operation! Error %d", status);
		return APP_ERROR;
	}

	return APP_SUCCESS;
}

int main(void)
{
	int status;

	LOG_INF("Starting SHA-512 example (bug reproduction test)...");

	status = crypto_init();
	if (status != APP_SUCCESS) {
		LOG_INF(APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	PRINT_HEX("Plaintext to hash", m_plain_text, sizeof(m_plain_text));

	status = hash_singlepart_sha512();
	if (status != APP_SUCCESS) {
		LOG_INF(APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	status = verify_sha512();
	if (status != APP_SUCCESS) {
		LOG_INF(APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	/* Reset the hash */
	memset(m_hash, 0, sizeof(m_hash));

	status = hash_multipart_sha512();
	if (status != APP_SUCCESS) {
		LOG_INF(APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	/* Test bug reproduction scenarios */
	LOG_INF("");
	LOG_INF("========================================");
	LOG_INF("Testing bug reproduction scenarios...");
	LOG_INF("========================================");

	status = test_bug_single_update_over_127_bytes();
	if (status == APP_ERROR) {
		LOG_INF("Bug reproduced in test case 1!");
	} else {
		LOG_INF("Test case 1 passed (bug not reproduced)");
	}

	status = test_bug_multiple_updates_exceed_127_bytes();
	if (status == APP_ERROR) {
		LOG_INF("Bug reproduced in test case 2!");
	} else {
		LOG_INF("Test case 2 passed (bug not reproduced)");
	}

	LOG_INF(APP_SUCCESS_MESSAGE);

	return APP_SUCCESS;
}
