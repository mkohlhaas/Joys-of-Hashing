#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct bin
{
  bool          is_free    : 1;
  bool          is_deleted : 1;
  unsigned int  key;
} Bin;

typedef struct hash_table
{
  Bin*          table;
  unsigned int  size;
  unsigned int  used;
  unsigned int  active;
  uint8_t       *T;               // For tabulation hashing.
  uint8_t       *T_end;           // For tabulation hashing.
  float         rehash_factor;
  unsigned int  probe_limit;
  unsigned int  operations_since_rehash;
} HashTable;

HashTable* empty_table  (uint32_t size, float rehash_factor);
void       delete_table (HashTable* table);
void       insert_key   (HashTable* table, uint32_t key);
bool       contains_key (HashTable* table, uint32_t key);
void       delete_key   (HashTable* table, uint32_t key);
