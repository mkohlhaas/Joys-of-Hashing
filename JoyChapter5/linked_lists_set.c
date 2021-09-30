#include "linked_lists_set.h"
#include <assert.h>

/* We represent linked lists using a sentinel link at the beginning of the list.
   This makes it easier to insert and remove elements without having to return
   updated lists.
 */

LinkedList*
new_linked_list()
{
  LinkedList* sentinel;
  sentinel           = malloc(sizeof *sentinel);
  sentinel->hash_key = 0;
  sentinel->key      = 0;
  sentinel->next     = NULL;
  return sentinel;
}

void
delete_linked_list(LinkedList* list, DestructorFunc destructor, bool free_keys)
{
  while (list) {
    LinkedList* next = list->next;
    if (free_keys && destructor && list->key) destructor(list->key);
    free(list);
    list = next;
  }
}

static LinkedList*
get_previous_link(LinkedList* list, uint32_t hash_key, void* key, CompareFunc cmp)
{
  while (list->next) {
    if (list->next->hash_key == hash_key && cmp(list->next->key, key)) return list;
    list = list->next;
  }
  return NULL;
}

void
list_insert_key(LinkedList* list, uint32_t hash_key, void* key, CompareFunc cmp)
{
  // build link and put it at the front of the list.
  // the hash table checks for duplicates if we want to avoid those
  LinkedList* new_link;
  new_link           = malloc(sizeof *new_link);
  new_link->hash_key = hash_key;
  new_link->key      = key;
  new_link->next     = list->next;
  list->next         = new_link;
}

void
list_delete_key(LinkedList* list, uint32_t hash_key, void* key, CompareFunc cmp, DestructorFunc destructor)
{
  LinkedList* link = get_previous_link(list, hash_key, key, cmp);
  if (!link) return;

  LinkedList* to_delete;
  to_delete  = link->next;
  link->next = to_delete->next;
  if (destructor) destructor(to_delete->key);
  free(to_delete);
}

bool
list_contains_key(LinkedList* list, uint32_t hash_key, void* key, CompareFunc cmp)
{
  return get_previous_link(list, hash_key, key, cmp) != 0;
}
