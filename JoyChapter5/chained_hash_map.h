#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef void (*DestructorFunc) (void*);
typedef bool (*CompareFunc)    (void*, void*);

typedef struct linked_list LinkedList;

typedef struct hash_table
{
  LinkedList**   table;
  uint32_t       size;
  uint32_t       used;
  CompareFunc    key_cmp;
  DestructorFunc key_destructor;
  DestructorFunc val_destructor;
} HashTable;

HashTable* empty_table  (uint32_t size, CompareFunc key_cmp, DestructorFunc key_destructor, DestructorFunc val_destructor);
void       delete_table (HashTable* table);
void       add_map      (HashTable* table, uint32_t hash_key, void* key, void* value);
void       delete_key   (HashTable* table, uint32_t hash_key, void* key);
void*      lookup_key   (HashTable* table, uint32_t hash_key, void* key);
