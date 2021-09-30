#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct linked_list
{
  uint32_t              hash_key;
  void*                 key;
  void*                 value;
  struct linked_list*   next;
} LinkedList;

typedef void (*DestructorFunc) (void*);
typedef bool (*CompareFunc)    (void*, void*);

LinkedList*  new_linked_list     ();
void         delete_linked_list  (LinkedList* list, DestructorFunc key_destructor, DestructorFunc val_destructor, bool free_resouces);
void         list_add_map        (LinkedList* list, uint32_t hash_key, void* key, void* value);
bool         list_delete_key     (LinkedList* list, uint32_t hash_key, void* key, CompareFunc key_cmp, DestructorFunc key_destructor, DestructorFunc value_destructor);
LinkedList*  list_lookup_key     (LinkedList* list, uint32_t hash_key, void* key, CompareFunc key_cmp);
