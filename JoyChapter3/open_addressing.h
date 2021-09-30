#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct bin
{
  int        is_free    : 1;
  int        is_deleted : 1;
  uint32_t   key;
} Bin;

typedef struct hash_table
{
  Bin*       table;
  size_t     size;
} HashTable;

HashTable* empty_table  (size_t size);
void       delete_table (HashTable* table);
void       insert_key   (HashTable* table, uint32_t key);
bool       contains_key (HashTable* table, uint32_t key);
void       delete_key   (HashTable* table, uint32_t key);
