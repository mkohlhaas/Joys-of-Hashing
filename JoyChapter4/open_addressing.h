#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct bin
{
  bool      is_free    : 1;
  bool      is_deleted : 1;
  uint32_t  key;
} Bin;

typedef struct hash_table
{
  Bin*      table;
  size_t    size;
  size_t    used;
  size_t    active;
  double    load_limit;
  uint32_t  primes_idx; // only used in primes code, but we share the header, so...
} HashTable;

HashTable* empty_table  (size_t size, double load_limit);
void       delete_table (HashTable* table);
void       insert_key   (HashTable* table, uint32_t key);
bool       contains_key (HashTable* table, uint32_t key);
void       delete_key   (HashTable* table, uint32_t key);
