#pragma once

typedef struct array
{
  int*          array;
  unsigned int  size;
  unsigned int  used;
} Array;

Array* new_array    (int initial_size);
void   delete_array (Array* array);
void   append       (Array* array, int value);
int    get          (Array* array, int index);
void   set          (Array* array, int index, int value);
int    pop          (Array* array);
