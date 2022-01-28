#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "hash-table.h"
#include "_ht-node.h"

static int cmp_ll_vals(ll_value_t v1, ll_value_t v2);


bool ht_init(hash_table_t * ht, hash_func_t f, float load_factor, uint64_t initial_capacity)
{
	linked_list_t * array;
	uint64_t i;

	if (load_factor == 0.0f) load_factor = DEFAULT_LOAD_FACTOR;
	if (initial_capacity == 0) initial_capacity = DEFAULT_INITIAL_CAPACITY;

	if (ht == NULL || f == NULL || load_factor < 0.05f || 0.95f < load_factor)
	{
		errno = EINVAL;
		return false;
	}

	memset(ht, '\0', sizeof(*ht));
	if ((array = malloc(initial_capacity * sizeof(linked_list_t))) == NULL)
		return false;

	for (i = 0; i < initial_capacity; ++i)
		ll_init(array + i, cmp_ll_vals);

	ht->load_factor = load_factor;
	ht->hash_func = f;
	/* ht->size already equal 0 */
	ht->allocated = initial_capacity;
	ht->table = array;

	return true;
}



static int cmp_ll_vals(ll_value_t v1, ll_value_t v2)
{
	ht_node_t dummy = {.hash = 0};
	ht_node_t * n1 = (v1 != NULL) ? v1 : &dummy;
	ht_node_t * n2 = (v2 != NULL) ? v2 : &dummy;

	return (n1->hash == n2->hash) ? 0 : ((n1->hash < n2->hash) ? -1 : 1);
}
