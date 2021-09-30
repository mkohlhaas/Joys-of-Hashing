#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef void (*DestructorFunc) (void*);
typedef bool (*CompareFunc)    (void*, void*);

typedef struct bin
{
  bool      is_free    : 1;
  bool      is_deleted : 1;
  uint32_t  hash_key;
  void*     key;
} Bin;

typedef struct hash_table
{
  Bin*            table;
  uint32_t        size;
  uint32_t        used;
  uint32_t        active;
  CompareFunc     cmp;
  DestructorFunc  destructor;
} HashTable;

HashTable* empty_table  (uint32_t size, CompareFunc cmp, DestructorFunc destructor);
void       delete_table (HashTable* table);
void       insert_key   (HashTable* table, uint32_t hash_key, void* key);
bool       contains_key (HashTable* table, uint32_t hash_key, void* key);
void       delete_key   (HashTable* table, uint32_t hash_key, void* key);
