#include "chained_hash.h"
#include "linked_lists.h"
#include <stdio.h>
#include <stdlib.h>

#if 0
// Using insert and delete to move keys
static void resize(HashTable *table,
                   uint32_t new_size)
{
   // Remember these...
    uint32_t old_size = table->size;
    LinkedList **old_bins = table->table;

    // Set up the new table
    table->table = malloc(new_size * sizeof(LinkedList *));
    for (size_t i = 0; i < new_size; ++i) {
        table->table[i] = new_linked_list();
    }
    table->size = new_size;
    table->used = 0;

    // Copy keys
    for (size_t i = 0; i < old_size; ++i) {
    LinkedList *list = old_bins[i];
        while ( (list = list->next) ) {
            insert_key(table, list->key);
        }
    }

    // Delete old table
    for (size_t i = 0; i < old_size; ++i) {
        delete_linked_list(old_bins[i]);
    }
    free(old_bins);
}

#else

// This version copies links.
static void
resize(HashTable* table, size_t new_size)
{
  // remember these...
  uint32_t     old_size = table->size;
  LinkedList** old_bins = table->table;

  // set up the new table
  table->table = malloc(new_size * sizeof(LinkedList*));
  for (size_t i = 0; i < new_size; ++i)
    table->table[i] = new_linked_list();

  table->size = new_size;
  table->used = 0;

  // copy keys
  uint32_t mask = table->size - 1;
  for (size_t i = 0; i < old_size; ++i) {
    LinkedList* sentinel = old_bins[i];
    LinkedList* list     = sentinel->next;
    while (list) {
      uint32_t    index     = list->key & mask;
      LinkedList* next_link = list->next;
      LinkedList* bin       = table->table[index];
      list->next            = bin->next;
      bin->next             = list;
      table->used++;
      list = next_link;
    }
  }

  // delete old table
  // Free instead of delete -- we have moved the links and we only need to remove the sentinels
  for (size_t i = 0; i < old_size; ++i)
    free(old_bins[i]);

  free(old_bins);
}
#endif

HashTable*
empty_table(size_t size)
{
  HashTable* table;
  table        = malloc(sizeof(HashTable));
  table->table = malloc(size * sizeof(LinkedList*));
  for (int i = 0; i < size; ++i)
    table->table[i] = new_linked_list();
  table->size = size;
  table->used = 0;
  return table;
}

void
delete_table(HashTable* table)
{
  for (int i = 0; i < table->size; ++i)
    delete_linked_list(table->table[i]);
  free(table->table);
  free(table);
}

void
insert_key(HashTable* table, uint32_t key)
{
  uint32_t mask  = table->size - 1;
  uint32_t index = key & mask;

  if (!contains_element(table->table[index], key)) {
    add_element(table->table[index], key);
    table->used++;
  }

  if (table->used > table->size / 2) resize(table, table->size * 2);
}

bool
contains_key(HashTable* table, uint32_t key)
{
  uint32_t mask  = table->size - 1;
  uint32_t index = key & mask;
  return contains_element(table->table[index], key);
}

void
delete_key(HashTable* table, uint32_t key)
{
  uint32_t mask  = table->size - 1;
  uint32_t index = key & mask;

  if (contains_element(table->table[index], key)) {
    delete_element(table->table[index], key);
    table->used--;
  }

  if (table->used < table->size / 8) resize(table, table->size / 2);
}
