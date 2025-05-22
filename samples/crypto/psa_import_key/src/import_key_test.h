#define PSA_KEY_LOCATION_CRACEN ((psa_key_location_t)(0x800000 | ('N' << 8)))

#define NRF_CRYPTO_EXAMPLE_ECDSA_TEXT_SIZE (100)

#define NRF_CRYPTO_EXAMPLE_ECDSA_PUBLIC_KEY_SIZE (65)
#define NRF_CRYPTO_EXAMPLE_ECDSA_SIGNATURE_SIZE (64)
#define NRF_CRYPTO_EXAMPLE_ECDSA_HASH_SIZE (32)

static uint8_t m_pub_key[NRF_CRYPTO_EXAMPLE_ECDSA_PUBLIC_KEY_SIZE];

static psa_key_id_t keypair_id;
static uint8_t m_pub_key[NRF_CRYPTO_EXAMPLE_ECDSA_PUBLIC_KEY_SIZE];

int generate_ecdsa_keypair(void)
{
	psa_status_t status;
	size_t olen;

	/* Configure the key attributes */
	psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;

	psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_SIGN_HASH);
	psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_VOLATILE);
	psa_set_key_algorithm(&key_attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_256));
	psa_set_key_type(&key_attributes, PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1));
	psa_set_key_bits(&key_attributes, 256);

	status = psa_generate_key(&key_attributes, &keypair_id);
	if (status != PSA_SUCCESS) {
		LOG_INF("psa_generate_key failed! (Error: %d)", status);
		return 1;
	}

	/* Export the public key */
	status = psa_export_public_key(keypair_id, m_pub_key, sizeof(m_pub_key), &olen);
	if (status != PSA_SUCCESS) {
		LOG_INF("psa_export_public_key failed! (Error: %d)", status);
		return 1;
	}

	/* Reset key attributes and free any allocated resources. */
	psa_reset_key_attributes(&key_attributes);

	return 0;
}

void fake_flashing_of_psa_keys(struct psa_import_key_args_in_ram *args_in_ram)
{
	int err = generate_ecdsa_keypair();
	if(err) {
		printf("Failed\n");
		k_oops();
	}

	args_in_ram->num_psa_keys = 3;

	struct psa_import_key_args *key = (struct psa_import_key_args *)args_in_ram->psa_keys;

	uint8_t * args_bytes = (uint8_t *)key;
	uint32_t data_length;

    key->magic = 0x5eb0;
    key->key = 0x40000001;    // Observed value

	key->attributes = psa_key_attributes_init();

	psa_set_key_usage_flags(&key->attributes, PSA_KEY_USAGE_VERIFY_HASH);
	psa_set_key_lifetime(&key->attributes, PSA_KEY_LIFETIME_VOLATILE);
	psa_set_key_algorithm(&key->attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_256));
	psa_set_key_type(&key->attributes, PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1));
	psa_set_key_bits(&key->attributes, 256);

	data_length = sizeof(m_pub_key);

    key->data_length = data_length;
    memcpy((uint8_t *)key->data, m_pub_key, key->data_length);

	{
		args_bytes += sizeof(struct psa_import_key_args);
		args_bytes += data_length;

		key = (struct psa_import_key_args *)args_bytes;

		memset(key, 0, sizeof(*key));

		key->magic = 0x5eb0;

		key->key = 0x40000002;    // Observed value

		psa_set_key_type(&key->attributes, PSA_KEY_TYPE_AES);
		psa_set_key_bits(&key->attributes, 128);
		psa_set_key_usage_flags(&key->attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
		psa_set_key_algorithm(&key->attributes, PSA_ALG_CBC_NO_PADDING);

		// Define the raw key material (16 bytes for AES-128)
		uint8_t raw_key[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
			0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };

		data_length = sizeof(raw_key);

		key->data_length = data_length;
		memcpy((uint8_t *)key->data, raw_key, key->data_length);
	}
	{
		args_bytes += sizeof(struct psa_import_key_args);
		args_bytes += data_length;

		key = (struct psa_import_key_args *)args_bytes;

		memset(key, 0, sizeof(*key));

		key->magic = 0x5eb0;

		key->key = PSA_KEY_ID_USER_MAX - 1;

		psa_set_key_type(&key->attributes, PSA_KEY_TYPE_AES);
		psa_set_key_bits(&key->attributes, 128);
		psa_set_key_usage_flags(&key->attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
		psa_set_key_algorithm(&key->attributes, PSA_ALG_CBC_NO_PADDING);

		psa_set_key_lifetime(&key->attributes, PSA_KEY_LIFETIME_PERSISTENT);

		/* We set the ID because it is a persistent key */
		psa_set_key_id(&key->attributes, key->key);

		// Define the raw key material (16 bytes for AES-128)
		uint8_t raw_key[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
			0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };

		data_length = sizeof(raw_key);

		key->data_length = data_length;
		memcpy((uint8_t *)key->data, raw_key, key->data_length);
	}
}
