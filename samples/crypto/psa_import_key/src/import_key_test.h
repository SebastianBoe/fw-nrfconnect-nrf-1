
void fake_flashing_of_psa_keys(struct psa_import_key_args_in_ram *args_in_ram)
{
	args_in_ram->num_psa_keys = 2;

	struct psa_import_key_args *key1 = (struct psa_import_key_args *)args_in_ram->psa_keys;
    key1->magic = 0x5eb0;
    key1->key = 0x41012500;    // Example key ID

	key1->attributes = PSA_KEY_ATTRIBUTES_INIT;

	psa_set_key_id(key1->attributes, key1->key);
	psa_set_key_lifetime(key1->attributes, LOCATION_CRACEN);
	psa_set_key_type(key1->attributes, PSA_KEY_TYPE_AES);
	psa_set_key_bits(key1->attributes, 255);
	psa_set_key_usage_flags(key1->attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_EXPORT);
	psa_set_key_algorithm(key1->attributes, PSA_ALG_GCM);

    key1->data_length = sizeof(key_data1);
    memcpy((uint8_t *)key1->data, key_data1, sizeof(key_data1));
}
