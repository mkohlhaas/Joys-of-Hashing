#include "open_addressing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t static p(uint32_t k, uint32_t i, uint32_t m)
{
  return (k + i) & (m - 1);
}

static void
resize(HashTable* table, uint32_t new_size)
{
  // remember the old bins until we have moved them.
  Bin* old_bins      = table->table;
  uint32_t old_size  = table->size;
  table->table       = malloc(new_size * sizeof(Bin));  // Update table so it now contains the new bins (that are empty)
  Bin* end           = table->table + new_size;

  for (Bin* bin = table->table; bin != end; ++bin) {
    bin->is_free    = true;
    bin->is_deleted = false;
  }

  table->size   = new_size;
  table->active = table->used = 0;
  end           = old_bins + old_size;  // the move the values from the old bins to the new, using the table's insertion function
  for (Bin* bin = old_bins; bin != end; ++bin) {
    if (bin->is_free || bin->is_deleted) continue;
    insert_key(table, bin->key);
  }
  free(old_bins);
}

HashTable*
empty_table(size_t size, double load_limit)
{
  HashTable* table; Bin* end;
  table        = malloc(sizeof(HashTable));
  table->table = malloc(size * sizeof(Bin));
  end          = table->table + size;
  for (Bin* bin = table->table; bin != end; ++bin) {
    bin->is_free    = true;
    bin->is_deleted = false;
  }
  table->size       = size;
  table->active     = 0; 
  table->used       = 0;
  table->load_limit = load_limit;
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
  if (contains_key(table, key)) return;

  for (uint32_t i = 0; i < table->size; ++i) {
    uint32_t index = p(key, i, table->size);
    Bin*     bin   = &table->table[index];

    if (bin->is_free) {
      bin->key        = key;
      bin->is_free    = false;
      bin->is_deleted = false;
      // We have one more active element and one more unused cell changes character
      table->active++;
      table->used++;
      break;
    }
    if (bin->is_deleted) {
      bin->key        = key;
      bin->is_free    = false;
      bin->is_deleted = false;
      // We have one more active element but we do not use more cells since the deleted cell was already used.
      table->active++;
      break;
    }
  }
  if (table->used > table->load_limit * table->size) resize(table, table->size * 2);
}

bool
contains_key(HashTable* table, uint32_t key)
{
  for (uint32_t i = 0; i < table->size; ++i) {
    uint32_t index = p(key, i, table->size);
    Bin*     bin   = &table->table[index];

    if (bin->is_free)                        return false;
    if (!bin->is_deleted && bin->key == key) return true;
  }
  return false;
}

void
delete_key(HashTable* table, uint32_t key)
{
  for (uint32_t i = 0; i < table->size; ++i) {
    uint32_t index = p(key, i, table->size);
    Bin*     bin   = &table->table[index];

    if (bin->is_free) return;
    if (!bin->is_deleted && bin->key == key) {
      bin->is_deleted = true;
      table->active--;
      break;
    }
  }
  if (table->active < table->load_limit / 4 * table->size) resize(table, table->size / 2);
}
