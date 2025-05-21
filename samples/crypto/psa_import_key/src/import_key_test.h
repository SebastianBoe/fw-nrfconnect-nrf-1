#define PSA_KEY_LOCATION_CRACEN ((psa_key_location_t)(0x800000 | ('N' << 8)))

char * key_data1 = "01234567890123456789012345678901";

void fake_flashing_of_psa_keys(struct psa_import_key_args_in_ram *args_in_ram)
{
	args_in_ram->num_psa_keys = 2;

	struct psa_import_key_args *key1 = (struct psa_import_key_args *)args_in_ram->psa_keys;
    key1->magic = 0x5eb0;
    key1->key = 0x41012500;    // Example key ID

	key1->attributes = psa_key_attributes_init();

	psa_set_key_id(&key1->attributes, key1->key);

	psa_set_key_lifetime(&key1->attributes, PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(
				     PSA_KEY_PERSISTENCE_VOLATILE, PSA_KEY_LOCATION_CRACEN));

	psa_set_key_type(&key1->attributes, PSA_KEY_TYPE_AES);
	psa_set_key_bits(&key1->attributes, 255);
	psa_set_key_usage_flags(&key1->attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_EXPORT);
	psa_set_key_algorithm(&key1->attributes, PSA_ALG_GCM);

    key1->data_length = (255 + 1) / 8;
    memcpy((uint8_t *)key1->data, key_data1, key1->data_length);
}
