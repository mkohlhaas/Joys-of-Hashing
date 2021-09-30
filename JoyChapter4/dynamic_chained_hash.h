#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct linked_list* SubTable;
typedef struct hash_table
{
  SubTable* tables;

  uint32_t  table_bits;
  uint32_t  k;
  uint32_t  subtable_mask;
  uint32_t  old_tables_mask;
  uint32_t  new_tables_mask;
  uint32_t  allocated_tables;
  uint32_t  split_count;
  uint32_t  max_bin_initialised;
} HashTable;

HashTable* empty_table  (size_t table_bits);
void       delete_table (HashTable* table);
void       insert_key   (HashTable* table, uint32_t key);
bool       contains_key (HashTable* table, uint32_t key);
void       delete_key   (HashTable* table, uint32_t key);
