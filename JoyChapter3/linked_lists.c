#include "linked_lists.h"

/* We represent linked lists using a sentinel link at the beginning of the list.
   This makes it easier to insert and remove elements without having to return
   updated lists.
 */

LinkedList*
new_linked_list()
{
  LinkedList* sentinel;
  sentinel       = malloc(sizeof *sentinel);
  sentinel->key  = 0;
  sentinel->next = NULL;
  return sentinel;
}

void
delete_linked_list(LinkedList* list)
{
  while (list) {
    LinkedList* next = list->next;
    free(list);
    list = next;
  }
}

static LinkedList*
get_previous_link(LinkedList* list, uint32_t key)
{
  // because of sentinel list != 0
  while (list->next) {
    if (list->next->key == key) return list;
    list = list->next;
  }
  // if we get to list->next == 0, we didn't find the key
  return NULL;
}

void
add_element(LinkedList* list, uint32_t key)
{
  // Build link and put it at the front of the list.
  // The hash table checks for duplicates if we want to
  // avoid those
  LinkedList* link;
  link       = malloc(sizeof *link);
  link->key  = key;
  link->next = list->next;
  list->next = link;
}

void
delete_element(LinkedList* list, uint32_t key)
{
  LinkedList* link = get_previous_link(list, key);
  if (!link) return; // key isn't in the list

  // we need to get rid of link->next
  LinkedList* to_delete;
  to_delete  = link->next;
  link->next = to_delete->next;
  free(to_delete);
}

bool
contains_element(LinkedList* list, uint32_t key)
{
  return get_previous_link(list, key) != 0;
}

uint32_t
search_length(LinkedList* list, uint32_t key)
{
  uint32_t probed = 1;
  while (list->next) {
    probed++;
    if (list->next->key == key) break;
    list = list->next;
  }
  return probed;
}
