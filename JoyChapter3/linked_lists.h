#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct linked_list
{
  uint32_t     key;
  struct       linked_list* next;
} LinkedList;

LinkedList* new_linked_list    ();
void        delete_linked_list (LinkedList* list);
void        add_element        (LinkedList* list, unsigned int key);
void        delete_element     (LinkedList* list, unsigned int key);
bool        contains_element   (LinkedList* list, unsigned int key);
