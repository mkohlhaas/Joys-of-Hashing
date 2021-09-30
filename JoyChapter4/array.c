#include "array.h"
#include <assert.h>
#include <stdlib.h>

Array*
new_array(int initial_size)
{
  Array* array = malloc(sizeof *array);
  array->array = malloc(initial_size * sizeof(int));
  array->used = 0;
  array->size = initial_size;
  return array;
}

void
delete_array(Array* array)
{
  free(array->array);
  free(array);
}

static void
resize(Array* array, unsigned int new_size)
{
  assert(new_size >= array->used);
  array->array = realloc(array->array, new_size * sizeof(int));
}

void
append(Array* array, int value)
{
  if (array->used == array->size) resize(array, 2 * array->size);
  array->array[array->used++] = value;
}

int
get(Array* array, int index)
{
  assert(array->used > index);
  return array->array[index];
}

void
set(Array* array, int index, int value)
{
  assert(array->used > index);
  array->array[index] = value;
}

int
pop(Array* array)
{
  assert(array->used > 0);
  int top = array->array[--(array->used)];
  if (array->used < array->size / 4) resize(array, array->size / 2);
  return top;
}
