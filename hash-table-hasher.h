#ifndef __HASH_TABLE_HASHER_H
#define __HASH_TABLE_HASHER_H

#include <stdint.h>
#include "hash-table.h"

#ifdef __cplusplus
extern "C"
{
#endif


/*
 * Assumes that `key` is a C string (const char *)
 */
uint64_t ht_hash_c_string(ht_key_t key);

/*
 * Assumes that `key` is a uint64 (cast key into uint64_t)
 */
uint64_t ht_hash_uint64(ht_key_t key);

uint64_t ht_get_murmur_seed_value();
void ht_set_murmur_seed_value(uint64_t v);

#ifdef __cplusplus
}
#endif

#endif
