#include <stdlib.h>

#include "_ht-node.h"


ht_node_t * _ht_new_node(uint64_t hash, ht_key_t key, ht_value_t value)
{
	ht_node_t * n = malloc(sizeof(ht_node_t));
	if (n == NULL)
		return NULL;

	n->hash = hash;
	n->key = key;
	n->value = value;
	return n;
}
