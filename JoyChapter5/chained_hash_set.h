#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef uint32_t (*HashFunc)       (void*);
typedef void     (*DestructorFunc) (void*);
typedef bool     (*CompareFunc)    (void*, void*);

typedef struct linked_list LinkedList;

typedef struct hash_table
{
  LinkedList**    table;
  uint32_t        size;
  uint32_t        used;
  CompareFunc     cmp;
  DestructorFunc  destructor;
} HashTable;

HashTable* empty_table  (uint32_t size, CompareFunc cmp, DestructorFunc destructor);
void       delete_table (HashTable* table);
void       insert_key   (HashTable* table, uint32_t hash_key, void* key);
bool       contains_key (HashTable* table, uint32_t hash_key, void* key);
void       delete_key   (HashTable* table, uint32_t hash_key, void* key);
