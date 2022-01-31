#include <stdlib.h>
#include <setjmp.h>

#include "hash-table.h"
#include "_ht-node.h"
#include "_ht-table.h"


typedef struct _ht_resize_ctx
{
	uint64_t capacity;
	linked_list_t * table;
	jmp_buf jmp;
} _ht_resize_ctx;


static void _ht_table_resize_consumer(ll_value_t v, void * ctx);

bool _ht_table_resize(hash_table_t * ht, uint64_t new_size)
{
	linked_list_t * table, * tmp;
	uint64_t i, min_capacity, new_capacity, new_threshold, old_capacity;
	_ht_resize_ctx ctx;

	if (ht == NULL)
		return false;

	if (ht->threshold >= new_size)
		return true; /* no resize needed */

	min_capacity = (new_size / ht->load_factor) + 1;
	if (min_capacity > HT_MAX_ALLOCATED)
	{
		new_capacity = HT_MAX_ALLOCATED;
	}
	else if (ht->allocated == 0)
	{
		new_capacity = (min_capacity > HT_DEFAULT_INITIAL_CAPACITY) ? min_capacity : HT_DEFAULT_INITIAL_CAPACITY;
	}
	else
	{
		new_capacity = ht->allocated;
		while (new_capacity < min_capacity)
		{
			if ((new_capacity <<= 1) > HT_MAX_ALLOCATED)
			{
				new_capacity = HT_MAX_ALLOCATED;
				break;
			}
		}
	}

	if (new_capacity == ht->allocated)
		return true;

	old_capacity = ht->allocated;
	new_threshold = new_capacity * ht->load_factor;
	table = malloc(sizeof(linked_list_t) * new_capacity);
	if (table == NULL)
		return false;

	for (i = 0; i < new_capacity; ++i)
		ll_init(table + i, _ht_compare_node);

	ctx.capacity = new_capacity;
	ctx.table = table;
	if (setjmp(ctx.jmp) != 0)
	{
		for (i = 0; i < new_capacity; ++i)
			ll_clear(table + i, NULL, NULL);

		free(table);

		return false;
	}

	if (ht->table != NULL)
		for (i = 0; i < ht->allocated; ++i)
			ll_foreach(ht->table + i, _ht_table_resize_consumer, &ctx);

	tmp = ht->table;
	ht->table = table;
	ht->allocated = new_capacity;
	ht->threshold = new_threshold;

	if (tmp != NULL)
	{
		for (i = 0; i < old_capacity; ++i)
			ll_clear(tmp + i, NULL, NULL);

		free(tmp);
	}

	return true;
}

static void _ht_table_resize_consumer(ll_value_t v, void * ctx)
{
	ht_node_t * n = v;
	_ht_resize_ctx * c = ctx;
	uint64_t slot;

	if (n == NULL || c == NULL || c->table == NULL)
		return;

	slot = n->hash % c->capacity;
	if (!ll_insert_tail(c->table + slot, n))
		longjmp(c->jmp, 1);
}
