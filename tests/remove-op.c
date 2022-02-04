#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "../hash-table.h"
#include "utility.h"


const char * keys[] = {
	"First key",
	"Second key",
	"Another key",
	"A fourth... I guess",
	"How am I supposed to know what to write here?",
	NULL
};


int main()
{
	uint64_t i;
	const float load_factor = HT_DEFAULT_LOAD_FACTOR;
	const uint64_t initial_capacity = ((uint64_t) 1 << 10); /* Overkill on purpose */
	hash_table_t ht;

	ht_value_t val;

	if (!ht_init(&ht, hash_func, load_factor, initial_capacity))
	{
		fprintf(stderr, "Failed to initialize (load factor: %f, initial capacity: %lu): %s\n",
		load_factor, initial_capacity, strerror(errno));
		return 1;
	}

	for (i = 0; keys[i] != NULL; ++i)
	{
		if (!ht_add_value(&ht, (ht_key_t) keys[i], (ht_value_t) i, NULL, NULL))
			fprintf(stderr, "Failed to add pair\n\t'%s': %lu\n\t%s\n", keys[i], i, strerror(errno));
	}

	errno = 0;
	if (!ht_remove_value(&ht, (ht_key_t) keys[2], &val))
		fprintf(stderr, "Failed to remove key '%s', (errno: %s)\n", keys[2], strerror(errno));
	else
		printf("Removed key '%s' (v = %lu)\n", keys[2], (uint64_t) val);

	errno = 0;
	if (!ht_resize(&ht, 0))
		fprintf(stderr, "ht_resize failed (%s)\n", strerror(errno));

	ht_clear(&ht, clear_func, NULL);
	ht_finalize(&ht);

	return 0;
}
