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
	const uint64_t initial_capacity = HT_DEFAULT_INITIAL_CAPACITY;
	hash_table_t ht;

	ht_key_t searched_key;
	ht_value_t searched_value;


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
	if (ht_search_key(&ht, (ht_key_t) keys[3], &searched_value, &searched_key))
	{
		printf("Searched key '%s':\n\t'%s': %lu\n\n", keys[3], (const char *) searched_key, (uint64_t) searched_value);
	}
	else if (errno != 0)
	{
		fprintf(stderr, "failed, errno: %s\n", strerror(errno));
	}

	const char * to_search = "Inexistant key";
	errno = 0;
	if (ht_search_key(&ht, (ht_key_t) to_search, &searched_value, &searched_key))
	{
		printf("Searched key '%s':\n\t'%s': %lu\n\n", to_search, (const char *) searched_key, (uint64_t) searched_value);
	}
	else if (errno != 0)
	{
		fprintf(stderr, "failed, errno: %s\n", strerror(errno));
	}
	else
	{
		printf("Key '%s' not in hash table (I didn't put it in, so it's normal)\n", to_search);
	}

	ht_clear(&ht, clear_func, NULL);
	ht_finalize(&ht);

	return 0;
}
