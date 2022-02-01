#ifndef __TESTS_UTILITY_H
#define __TESTS_UTILITY_H

#include <stdio.h>

#include "../hash-table.h"


static void clear_func(ht_key_t key, ht_value_t value, void * ctx)
{
	const char * k = (const char *) key;
	uint64_t v = (uint64_t) value;

	printf("Cleared pair '%s': %lu\n", k, v);
}


static uint64_t hash_func(ht_key_t key)
{
	uint64_t h = 0;
	const char * s = (const char *) key;
	if (s == NULL)
		return 0;

	for (; *s != '\0'; ++s)
	{
		h *= 31;
		h += *s;
	}

	return h;
}

#endif
