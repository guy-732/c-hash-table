#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "hash-table.h"
#include "_ht-node.h"


typedef struct ht_to_ll_ctx_t
{
	ht_consume_func_t f;
	void * ctx;
} ht_to_ll_ctx_t;


static void ht_func_consumer(ll_value_t v, void * ctx);


bool ht_init(hash_table_t * ht, hash_func_t f, float load_factor, uint64_t initial_capacity)
{
	linked_list_t * array;
	uint64_t i;

	if (load_factor == 0.0f) load_factor = HT_DEFAULT_LOAD_FACTOR;
	if (initial_capacity == 0) initial_capacity = HT_DEFAULT_INITIAL_CAPACITY;

	if (ht == NULL || f == NULL || load_factor < 0.05f || 0.95f < load_factor)
	{
		errno = EINVAL;
		return false;
	}

	memset(ht, '\0', sizeof(*ht));
	if ((array = malloc(initial_capacity * sizeof(linked_list_t))) == NULL)
		return false;

	for (i = 0; i < initial_capacity; ++i)
		ll_init(array + i, _ht_compare_node);

	ht->load_factor = load_factor;
	ht->hash_func = f;
	/* ht->size already equal 0 */
	ht->threshold = initial_capacity * load_factor;
	ht->allocated = initial_capacity;
	ht->table = array;

	return true;
}

void ht_clear(hash_table_t * ht, ht_consume_func_t f, void * ctx)
{
	ht_to_ll_ctx_t c = {.f = f, .ctx = ctx};
	uint64_t i;

	if (ht == NULL || ht->table == NULL)
	{
		errno = EINVAL;
		return;
	}

	for (i = 0; i < ht->allocated; ++i)
	{
		ll_clear(ht->table + i, ht_func_consumer, (void *) &c);
	}

	ht->size = 0;
}


void ht_finalize(hash_table_t * ht)
{
	if (ht == NULL)
	{
		errno = EINVAL;
		return;
	}

	ht_clear(ht, NULL, NULL);
	free(ht->table);
	ht->table = NULL;
	ht->allocated = 0;
	ht->threshold = 0;
}


void ht_foreach(const hash_table_t * ht, ht_consume_func_t f, void * ctx)
{
	ht_to_ll_ctx_t c = {.f = f, .ctx = ctx};
	uint64_t i;
	if (ht == NULL || f == NULL)
	{
		errno = EINVAL;
		return;
	}

	for (i = 0; i < ht->allocated; ++i)
		ll_foreach(ht->table + i, ht_func_consumer, &c);
}


static void ht_func_consumer(ll_value_t v, void * ctx)
{
	ht_node_t * n = (ht_node_t *) v;
	ht_to_ll_ctx_t * c = (ht_to_ll_ctx_t *) ctx;
	if (n == NULL || c == NULL || c->f == NULL)
		return;

	c->f(n->key, n->value, c->ctx);
}
