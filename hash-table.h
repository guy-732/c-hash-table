#ifndef __HASH_TABLE_H
#define __HASH_TABLE_H

#include <stdint.h>
#include <stdbool.h>

#include "linked-list/linked-list.h"

typedef void * ht_key_t;
typedef void * ht_value_t;

typedef uint64_t (*hash_func_t)(ht_key_t);
typedef void (*ht_consume_func_t)(ht_key_t, ht_value_t, void * ctx);

#define DEFAULT_LOAD_FACTOR (0.75f)
#define DEFAULT_INITIAL_CAPACITY ((uint64_t) 1 << 4)

typedef struct hash_table_t
{
	float load_factor;
	hash_func_t hash_func;

	uint64_t size;
	uint64_t threshold;
	uint64_t allocated;
	linked_list_t * table;
} hash_table_t;

/*
 * Initialize a hash table
 * the macros `DEFAULT_LOAD_FACTOR` & `DEFAULT_INITIAL_CAPACITY` can be used for
 * the corresponding arguments, also,
 * if load_factor == 0.0f -> load_factor = DEFAULT_LOAD_FACTOR
 * if initial_capacity == 0 -> initial_capacity = DEFAULT_INITIAL_CAPACITY
 *
 * ht == NULL || f == NULL -> errno = EINVAL
 * load_factor < 0.05f || 0.95f < load_factor -> errno = EINVAL
 */
bool ht_init(hash_table_t * ht, hash_func_t f, float load_factor, uint64_t initial_capacity);

/*
 * clear all elements from the hash table
 */
void ht_clear(hash_table_t * ht, ht_consume_func_t f, void * ctx);

/*
 * Finalize the hash table, `ht_init` may be called on a hash table after it has been finalized
 *
 * This function MUST be called on any initialized hash table after being used
 * failure to do so will result in memory leak(s)
 */
void ht_finalize(hash_table_t * ht);

#endif
