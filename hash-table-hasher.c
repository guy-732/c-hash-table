#include <string.h>

#include "hash-table-hasher.h"


static uint64_t murmur2_64_x64(const void * data, size_t len, uint64_t seed);


static uint64_t murmur_seed = 104729;
uint64_t ht_get_murmur_seed_value()
{
	return murmur_seed;
}

void ht_set_murmur_seed_value(uint64_t v)
{
	murmur_seed = v;
}


uint64_t ht_hash_c_string(ht_key_t key)
{
	const uint64_t seed = murmur_seed;
	const char * s = (const char *) key;
	size_t length = (s != NULL) ? strlen(s) : 0;

	return murmur2_64_x64(s, length, seed);
}

uint64_t ht_hash_uint64(ht_key_t key)
{
	const uint64_t seed = murmur_seed;
	const uint64_t v = (uint64_t) key;

	return murmur2_64_x64(&v, sizeof(v), seed);
}


static uint64_t murmur2_64_x64(const void * data, size_t len, uint64_t seed)
{
	const uint64_t m = 0xc6a4a7935bd1e995ULL;
	const int rot = 47;
	const uint64_t * d = (const uint64_t *) data;
	const uint64_t * d_end = d + (len / sizeof(uint64_t));

	uint64_t h = seed ^ (len * m);

	const uint8_t * d2;
	uint64_t k;

	while (d != d_end)
	{
		k = *d;
		++d;

		k *= m;
		k ^= k >> rot;
		k *= m;

		h ^= k;
		h *= m;
	}

	d2 = (const uint8_t *) d;
	switch (len & 0b111)
	{
	case 7: h ^= ((uint64_t) d2[6]) << 48;
	case 6: h ^= ((uint64_t) d2[5]) << 40;
	case 5: h ^= ((uint64_t) d2[4]) << 32;
	case 4: h ^= ((uint64_t) d2[3]) << 24;
	case 3: h ^= ((uint64_t) d2[2]) << 16;
	case 2: h ^= ((uint64_t) d2[1]) << 8;
	case 1: h ^= ((uint64_t) d2[0]);
		h *= m;
	}

	h ^= h >> rot;
	h *= m;
	h ^= h >> rot;

	return h;
}
