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

int _ht_compare_node(ll_value_t v1, ll_value_t v2)
{
	ht_node_t dummy = {.hash = 0};
	ht_node_t * n1 = (v1 != NULL) ? v1 : &dummy;
	ht_node_t * n2 = (v2 != NULL) ? v2 : &dummy;

	return (n1->hash == n2->hash) ? 0 : ((n1->hash < n2->hash) ? 1 : -1);
}
