#ifndef __HASH_TABLE_H
#define __HASH_TABLE_H

#include <stdint.h>
#include <stdbool.h>

#include "linked-list/linked-list.h"

typedef void * ht_key_t;
typedef void * ht_value_t;

typedef uint64_t (*hash_func_t)(ht_key_t);
typedef void (*ht_consume_func_t)(ht_key_t, ht_value_t, void * ctx);

#define HT_DEFAULT_LOAD_FACTOR (0.75f)
#define HT_DEFAULT_INITIAL_CAPACITY ((uint64_t) 1 << 4)

#define HT_MAX_ALLOCATED ((uint64_t) 1 << 30)

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
 * the macros `HT_DEFAULT_LOAD_FACTOR` & `HT_DEFAULT_INITIAL_CAPACITY` can be used for
 * the corresponding arguments, also,
 * if load_factor == 0.0f -> load_factor = HT_DEFAULT_LOAD_FACTOR
 * if initial_capacity == 0 -> initial_capacity = HT_DEFAULT_INITIAL_CAPACITY
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

bool ht_add_value(hash_table_t * ht, ht_key_t key, ht_value_t value, bool * had_key, ht_value_t * old_value);

bool ht_search_key(const hash_table_t * ht, ht_key_t key, ht_value_t * value, ht_key_t * stored_key);
#define ht_get_value(ht, key, value) (ht_search_key((ht), (key), (value), NULL))
#define ht_contains(ht, key) (ht_get_value((ht), (key), NULL))

#define ht_len(ht) (((const hash_table_t *) (ht))->size)
#define ht_is_empty(ht) (ht_len((ht)) == 0)

bool ht_remove_value(hash_table_t * ht, ht_key_t key, ht_value_t * value);

void ht_foreach(const hash_table_t * ht, ht_consume_func_t f, void * ctx);

bool ht_resize(hash_table_t * ht, uint64_t min_size);

#endif
