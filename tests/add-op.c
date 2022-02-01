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
	const uint64_t initial_capacity = 4;
	hash_table_t ht;
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

	ht_clear(&ht, clear_func, NULL);
	ht_finalize(&ht);

	if (!ht_add_value(&ht, (ht_key_t) "Random key...", NULL, NULL, NULL))
		fprintf(stderr, "Couldn't add value after hash table finalized (should be possible): %s\n",
		strerror(errno));
	
	ht_finalize(&ht);

	return 0;
}
