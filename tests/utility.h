#ifndef __TESTS_UTILITY_H
#define __TESTS_UTILITY_H

#include <stdio.h>

#include "../hash-table.h"
#include "../hash-table-hasher.h"


static void clear_func(ht_key_t key, ht_value_t value, void * ctx)
{
	const char * k = (const char *) key;
	uint64_t v = (uint64_t) value;

	printf("Cleared pair '%s': %lu\n", k, v);
}


static const hash_func_t hash_func = ht_hash_c_string;

#endif
