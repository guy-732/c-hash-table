#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "hash-table.h"
#include "_ht-node.h"
#include "_ht-table.h"


typedef struct ht_to_ll_ctx_t
{
	ht_consume_func_t f;
	void * ctx;
	bool clearing;
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
	ht_to_ll_ctx_t c = {.f = f, .ctx = ctx, .clearing = true};
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


bool ht_add_value(hash_table_t * ht, ht_key_t key, ht_value_t value, bool * had_key, ht_value_t * old_value)
{
	uint64_t hash_res;

	if (ht == NULL || ht->hash_func == NULL)
	{
		errno = EINVAL;
		return false;
	}

	hash_res = ht->hash_func(key);
	if (ht->size != 0)
	{
		ht_node_t n = {.hash = hash_res}, * searched;
		if (ll_search(ht->table + (hash_res % ht->allocated), &n, NULL, (ll_value_t *) &searched))
		{
			if (old_value != NULL)
				*old_value = searched->value;

			searched->value = value;
			if (had_key != NULL)
				*had_key = true;

			return true;
		}
	}

	ht_node_t * n;
	if (!_ht_table_resize(ht, ht->size + 1))
		return false;

	n = _ht_new_node(hash_res, key, value);
	if (n == NULL)
		return false;

	hash_res %= ht->allocated;
	if (!ll_insert_head(ht->table + hash_res, n))
	{
		_ht_free_node(n);
		return false;
	}

	ht->size++;
	if (had_key != NULL)
		*had_key = false;

	return true;
}


void ht_foreach(const hash_table_t * ht, ht_consume_func_t f, void * ctx)
{
	ht_to_ll_ctx_t c = {.f = f, .ctx = ctx, .clearing = false};
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
	ht_key_t key;
	ht_value_t value;

	if (n == NULL || c == NULL || c->f == NULL)
		return;

	key = n->key;
	value = n->value;
	if (c->clearing)
		_ht_free_node(n);

	c->f(key, value, c->ctx);
}
