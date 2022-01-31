#ifndef ___HT_NODE_H
#define ___HT_NODE_H

#include <stdlib.h>

#include "hash-table.h"

typedef struct ht_node_t
{
	uint64_t hash;
	ht_key_t key;
	ht_value_t value;
} ht_node_t;

ht_node_t * _ht_new_node(uint64_t hash, ht_key_t key, ht_value_t value);
#define _ht_free_node(n) (free((n)))

int _ht_compare_node(ll_value_t v1, ll_value_t v2);

#endif
