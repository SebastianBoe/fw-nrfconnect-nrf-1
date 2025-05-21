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

	LOG_INF("Generating random ECDSA keypair...");

	/* Configure the key attributes */
	psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;

	/* Configure the key attributes */
	psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_SIGN_HASH);
	psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_VOLATILE);
	psa_set_key_algorithm(&key_attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_256));
	psa_set_key_type(&key_attributes, PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1));
	psa_set_key_bits(&key_attributes, 256);

	/* Generate a random keypair. The keypair is not exposed to the application,
	 * we can use it to sign hashes.
	 */
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

	args_in_ram->num_psa_keys = 1;

	struct psa_import_key_args *key1 = (struct psa_import_key_args *)args_in_ram->psa_keys;
    key1->magic = 0x5eb0;
    key1->key = 0x40000001;    // Observed value

	key1->attributes = psa_key_attributes_init();

	psa_set_key_usage_flags(&key1->attributes, PSA_KEY_USAGE_VERIFY_HASH);
	psa_set_key_lifetime(&key1->attributes, PSA_KEY_LIFETIME_VOLATILE);
	psa_set_key_algorithm(&key1->attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_256));
	psa_set_key_type(&key1->attributes, PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1));
	psa_set_key_bits(&key1->attributes, 256);

    key1->data_length = sizeof(m_pub_key);
    memcpy((uint8_t *)key1->data, m_pub_key, key1->data_length);
}
