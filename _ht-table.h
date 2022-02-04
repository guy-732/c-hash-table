#ifndef ___HT_TABLE_H
#define ___HT_TABLE_H
/* actions on table */

#include "hash-table.h"

bool _ht_table_resize(hash_table_t * ht, uint64_t new_size);
bool _ht_transfer(linked_list_t * dest, uint64_t dest_capacity, const linked_list_t * src, uint64_t src_capacity);

#endif
