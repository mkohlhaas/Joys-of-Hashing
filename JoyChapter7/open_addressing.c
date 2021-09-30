#include "open_addressing.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void resize              (HashTable* table, unsigned int new_size);
static void rehash              (HashTable* table);
static void insert_key_internal (HashTable* table, uint32_t key);

void
tabulation_sample(uint32_t* start, uint32_t* end)
{
  while (start != end)
    *(start++) = rand();
}

// tabulation hashing, r=4, q=32
uint32_t
hash(uint32_t x, uint8_t* T)
{
  const int r = 4;
  const uint32_t no_cols = 1 << r;
  const uint32_t mask = (1 << r) - 1;

  uint32_t* T_ = (uint32_t*)T;
  uint32_t y = T_[0 * no_cols + (x & mask)];
  x >>= r;
  y ^= T_[1 * no_cols + (x & mask)];
  x >>= r;
  y ^= T_[2 * no_cols + (x & mask)];
  x >>= r;
  y ^= T_[3 * no_cols + (x & mask)];
  x >>= r;
  y ^= T_[4 * no_cols + (x & mask)];
  x >>= r;
  y ^= T_[5 * no_cols + (x & mask)];
  x >>= r;
  y ^= T_[6 * no_cols + (x & mask)];
  x >>= r;
  y ^= T_[7 * no_cols + (x & mask)];

  return y;
}

// Linear probing
static unsigned int
p(unsigned int k, unsigned int i, unsigned int m)
{
  return (k + i) & (m - 1);
}

HashTable*
empty_table(unsigned int size, float rehash_factor)
{
  HashTable* table = malloc(sizeof *table);
  table->table     = malloc(size * sizeof(Bin));
  Bin* end         = table->table + size;
  for (Bin* bin = table->table; bin != end; ++bin) {
    bin->is_free    = true;
    bin->is_deleted = false;
  }

  table->size   = size;
  table->active = 0;
  table->used   = 0;

  // setting up tabulation hashing table
  int p        = 32;
  int r        = 4;
  int q        = 32;
  int no_cols  = (1 << r);
  int t        = p / r;
  int bytes    = t * no_cols * q / 8;
  table->T     = malloc(bytes);
  table->T_end = table->T + bytes;
  tabulation_sample((uint32_t*)table->T, (uint32_t*)table->T_end);

  table->rehash_factor           = rehash_factor;
  table->probe_limit             = rehash_factor * size;
  table->operations_since_rehash = 0;

  return table;
}

void
delete_table(HashTable* table)
{
  free(table->table);
  free(table->T);
  free(table);
}

void
insert_key(HashTable* table, uint32_t key)
{
  table->operations_since_rehash++;
  if (table->operations_since_rehash > table->probe_limit)
    rehash(table);
  insert_key_internal(table, key);
  if (table->active > table->size / 2) resize(table, table->size * 2);
}

static void
insert_key_internal(HashTable* table, uint32_t key)
{
  uint32_t hash_key = hash(key, table->T);
  for (unsigned int i = 0; i < table->size; ++i) {
    uint32_t index = p(hash_key, i, table->size);
    Bin* bin       = &table->table[index];

    if (bin->is_free) {
      bin->key        = key;
      bin->is_free    = false;
      bin->is_deleted = false;
      // we have one more active element and one more unused cell is occupied
      table->active++;
      table->used++;
      break;
    }

    if (bin->is_deleted) {
      bin->key        = key;
      bin->is_free    = false;
      bin->is_deleted = false;
      // we have one more active element but we do not use more cells since the
      // deleted cell was already used.
      table->active++;
      break;
    }

    if (bin->key == key) return; // nothing to be done
  }
}

bool
contains_key(HashTable* table, uint32_t key)
{
  table->operations_since_rehash++;
  if (table->operations_since_rehash > table->probe_limit)
    rehash(table);

  uint32_t hash_key = hash(key, table->T);

  for (unsigned int i = 0; i < table->size; ++i) {
    unsigned int index = p(hash_key, i, table->size);
    Bin*         bin   = &table->table[index];
    if (bin->is_free)                        return false;
    if (!bin->is_deleted && bin->key == key) return true;
  }
  return false;
}

void
delete_key(HashTable* table, uint32_t key)
{
  table->operations_since_rehash++;
  if (table->operations_since_rehash > table->probe_limit)
    rehash(table);

  uint32_t hash_key = hash(key, table->T);
  for (unsigned int i = 0; i < table->size; ++i) {
    unsigned int index = p(hash_key, i, table->size);
    Bin*         bin   = &table->table[index];

    if (bin->is_free) return;

    if (!bin->is_deleted && bin->key == key) {
      bin->is_deleted = true;
      table->active--;
      break;
    }
  }

  if (table->active < table->size / 8) resize(table, table->size / 2);
}

static void
resize(HashTable* table, unsigned int new_size)
{
  // nothing good comes from tables of size 0
  if (new_size == 0) return;
  // remember the old bins until we have moved them.
  Bin*         old_bins = table->table;
  unsigned int old_size = table->size;

  // Update table so it now contains the new bins (that are empty)
  table->table = malloc(new_size * sizeof(Bin));
  Bin* end     = table->table + new_size;
  for (Bin* bin = table->table; bin != end; ++bin) {
    bin->is_free    = true;
    bin->is_deleted = false;
  }
  table->size   = new_size;
  table->active = 0;
  table->used   = 0;
  for (Bin* bin = table->table; bin != end; ++bin) {
    // printf("bin %p => %u %u\n", bin, bin->is_deleted, bin->is_free);
  }

  // Update hash function
  tabulation_sample((uint32_t*)table->T, (uint32_t*)table->T_end);

  // Then move the values from the old bins to the new,
  // using the new hash function from the insertion function
  end = old_bins + old_size;
  for (Bin* bin = old_bins; bin != end; ++bin) {
    if (bin->is_free || bin->is_deleted) continue;
    insert_key_internal(table, bin->key);
  }

  for (Bin* bin = table->table; bin != end; ++bin) {
    // printf("bin %p => %u %u\n", bin, bin->is_deleted, bin->is_free);
  }

  // Update rehash limit
  table->probe_limit = table->rehash_factor * new_size;
  table->operations_since_rehash = 0;
  // finally, free memory for old bins
  free(old_bins);
}

static void
rehash(HashTable* table)
{
  Bin* old_bins = table->table;
  table->table  = malloc(table->size * sizeof(Bin));
  Bin* end      = table->table + table->size;
  for (Bin* bin = table->table; bin != end; ++bin) {
    bin->is_free    = true;
    bin->is_deleted = false;
  }
  // Update hash function
  tabulation_sample((uint32_t*)table->T, (uint32_t*)table->T_end);
  table->operations_since_rehash = 0;

  Bin* old_end = old_bins + table->size;
  for (Bin* bin = old_bins; bin != old_end; ++bin) {
    if (bin->is_free || bin->is_deleted) continue;
    insert_key_internal(table, bin->key);
  }

  // Update rehash limit
  table->probe_limit = table->rehash_factor * table->size;
  table->operations_since_rehash = 0;
}
