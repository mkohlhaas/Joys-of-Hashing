#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct bin
{
  int      is_free : 1;
  uint32_t key;
} Bin;

typedef struct hash_table
{
  Bin*        table;
  uint32_t    size;
} HashTable;

HashTable* new_table    (uint32_t size);
void       delete_table (HashTable* table);
void       insert_key   (HashTable* table, uint32_t key);
bool       contains_key (HashTable* table, uint32_t key);
void       delete_key   (HashTable* table, uint32_t key);
