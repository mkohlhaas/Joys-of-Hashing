#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct linked_list
{
  uint32_t             hash_key;
  void*                key;
  struct linked_list*  next;
} LinkedList;

typedef void (*DestructorFunc) (void*);
typedef bool (*CompareFunc)    (void*, void*);

LinkedList* new_linked_list    ();
void        delete_linked_list (LinkedList* list, DestructorFunc destructor, bool free_keys);
void        list_insert_key    (LinkedList* list, uint32_t hash_key, void* key, CompareFunc cmp);
bool        list_contains_key  (LinkedList* list, uint32_t hash_key, void* key, CompareFunc cmp);
void        list_delete_key    (LinkedList* list, uint32_t hash_key, void* key, CompareFunc cmp, DestructorFunc destructor);
