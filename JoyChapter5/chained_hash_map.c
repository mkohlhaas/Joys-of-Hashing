#include "chained_hash_map.h"
#include "linked_lists_map.h"
#include <stdlib.h>

HashTable*
empty_table(uint32_t size, CompareFunc key_cmp, DestructorFunc key_destructor, DestructorFunc val_destructor)
{
  HashTable* table = malloc(sizeof *table);
  table->table     = malloc(size * sizeof(LinkedList*));
  for (int i = 0; i < size; ++i)
    table->table[i] = new_linked_list();
  table->size           = size;
  table->used           = 0;
  table->key_cmp        = key_cmp;
  table->key_destructor = key_destructor;
  table->val_destructor = val_destructor;
  return table;
}

void
delete_table(HashTable* table)
{
  for (int i = 0; i < table->size; ++i)
    delete_linked_list(table->table[i], table->key_destructor, table->val_destructor, true);
  free(table->table);
  free(table);
}

static void
resize(HashTable* table, uint32_t new_size)
{
  // Remember these...
  uint32_t     old_size = table->size;
  LinkedList** old_bins = table->table;
  // Set up the new table
  table->table = malloc(new_size * sizeof(LinkedList*));
  for (int i = 0; i < new_size; ++i)
    table->table[i] = new_linked_list();

  table->size = new_size;
  table->used = 0;

  // Copy maps
  for (int i = 0; i < old_size; ++i) {
    LinkedList* list = old_bins[i];
    while ((list = list->next))
      add_map(table, list->hash_key, list->key, list->value);
  }
  // Delete old table
  for (int i = 0; i < old_size; ++i)
    delete_linked_list(old_bins[i], table->key_destructor, table->val_destructor, false);

  free(old_bins);
}

void
add_map(HashTable* table, uint32_t hash_key, void* key, void* value)
{
  uint32_t    mask  = table->size - 1;
  uint32_t    index = hash_key & mask;
  LinkedList* list  = table->table[index];

  LinkedList* link = list_lookup_key(list, hash_key, key, table->key_cmp);

  if (link != NULL) {
    // the key exists in the table, replace the value.
    if (table->key_destructor) table->key_destructor(link->key);
    if (table->val_destructor) table->val_destructor(link->value);
    link->key   = key;
    link->value = value;
  } else {
    // the key is new, so insert it and the value
    list_add_map(list, hash_key, key, value);
    table->used++;
  }

  if (table->used > table->size / 2) resize(table, table->size * 2);
}

void
delete_key(HashTable* table, uint32_t hash_key, void* key)
{
  uint32_t    mask    = table->size - 1;
  uint32_t    index   = hash_key & mask;
  LinkedList* list    = table->table[index];
  bool        deleted = list_delete_key(list, hash_key, key, table->key_cmp, table->key_destructor, table->val_destructor);

  if (deleted) table->used--;

  if (table->used < table->size / 8) resize(table, table->size / 2);
}

void*
lookup_key(HashTable* table, uint32_t hash_key, void* key)
{
  uint32_t    mask  = table->size - 1;
  uint32_t    index = hash_key & mask;
  LinkedList* list  = table->table[index];
  LinkedList* link  = list_lookup_key(list, hash_key, key, table->key_cmp);
  return link ? link->value : NULL;
}
