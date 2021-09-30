#include "linked_lists_map.h"
#include <assert.h>

/* We represent linked lists using a sentinel link at the beginning of the list.
   This makes it easier to insert and remove elements without having to return
   updated lists.
 */

LinkedList*
new_linked_list()
{
  LinkedList* sentinel = malloc(sizeof *sentinel);
  sentinel->hash_key   = 0;
  sentinel->key        = 0;
  sentinel->value      = 0;
  sentinel->next       = 0;
  return sentinel;
}

void
delete_linked_list(LinkedList* list, DestructorFunc key_destructor, DestructorFunc val_destructor, bool free_resources)
{
  while (list != NULL) {
    LinkedList* next = list->next;
    if (free_resources && key_destructor && list->key)   key_destructor(list->key);
    if (free_resources && val_destructor && list->value) val_destructor(list->value);
    free(list);
    list = next;
  }
}

void
list_add_map(LinkedList* list, uint32_t hash_key, void* key, void* value)
{
  LinkedList* link = malloc(sizeof *link);
  link->hash_key   = hash_key;
  link->key        = key;
  link->value      = value;
  link->next       = list->next;
  list->next       = link;
}

static LinkedList*
get_previous_link(LinkedList* list, uint32_t hash_key, void* key, CompareFunc key_cmp)
{
  while (list->next) {
    if (list->next->hash_key == hash_key && key_cmp(list->next->key, key))
      return list;
    list = list->next;
  }
  return NULL;
}

bool
list_delete_key(LinkedList* list, uint32_t hash_key, void* key, CompareFunc key_cmp, DestructorFunc key_destructor, DestructorFunc val_destructor)
{
  LinkedList* link = get_previous_link(list, hash_key, key, key_cmp);
  if (!link) return false;

  // we need to get rid of link->next
  LinkedList* to_delete = link->next;
  link->next            = to_delete->next;
  if (key_destructor) key_destructor(to_delete->key);
  if (val_destructor) val_destructor(to_delete->value);
  free(to_delete);
  return true;
}

LinkedList*
list_lookup_key(LinkedList* list, uint32_t hash_key, void* key, CompareFunc key_cmp)
{
  LinkedList* link = get_previous_link(list, hash_key, key, key_cmp);
  return link ? link->next : NULL;
}
