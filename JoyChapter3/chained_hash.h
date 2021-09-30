#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct hash_table
{
  struct     linked_list** table;
  size_t     size;
} HashTable;

HashTable* empty_table  (size_t size);
void       delete_table (HashTable* table);
void       insert_key   (HashTable* table, uint32_t key);
bool       contains_key (HashTable* table, uint32_t key);
void       delete_key   (HashTable* table, uint32_t key);
