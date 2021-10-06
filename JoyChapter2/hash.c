#include "hash.h"
#include <stdlib.h>
#include <string.h>

HashTable*
empty_table(uint32_t size)
{
  HashTable* table = malloc(sizeof *table);
  table->table     = malloc(size * sizeof *table->table);
  for (uint32_t i = 0; i < size; ++i) {
    Bin* bin     = &table->table[i];
    bin->is_free = true;
  }
  table->size = size;
  return table;
}

void
delete_table(HashTable* table)
{
  free(table->table);
  free(table);
}

void
insert_key(HashTable* table, uint32_t key)
{
  uint32_t mask  = table->size - 1;
  uint32_t index = key & mask; // or key % table->size;
  Bin* bin = &table->table[index];
  if (bin->is_free) {
    bin->key     = key;
    bin->is_free = false;
  } else {
    // There is already a key here, so we have a
    // collision. We cannot deal with this yet.
  }
}

bool
contains_key(HashTable* table, uint32_t key)
{
  uint32_t mask  = table->size - 1;
  uint32_t index = key & mask; // or key % table->size;
  Bin* bin = &table->table[index];
  if (!bin->is_free && bin->key == key) return true;
  else                                  return false;
}

void
delete_key(HashTable* table, uint32_t key)
{
  uint32_t mask  = table->size - 1;
  uint32_t index = key & mask; // or key % table->size;
  Bin* bin       = &table->table[index];
  if (bin->key == key) bin->is_free = true;
}
