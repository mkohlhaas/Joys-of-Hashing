#include "hash.h"
#include <stdio.h>
#include <stdlib.h>

#define TABLE_SIZE 64

static unsigned int
random_key()
{
  unsigned int key = random();
  // unsigned int key = random() % 100000;
  if (key == 0 || key == 1) return 2;
  return key;
}

int
main()
{
  // initialize random number generator
  unsigned random_seed;
  FILE *fp = fopen("/dev/urandom", "r");
  fread(&random_seed, sizeof random_seed, 1, fp);
  fclose(fp);
  srand(random_seed);

  // fill an array with random keys
  int n = 10;
  size_t keys[n];
  for (int i = 0; i < n; ++i) {
    keys[i] = random_key();
  }

  printf("Table size: %d\n", TABLE_SIZE);

  printf("\n================================\n");
  printf("========== Insertions ==========\n");
  printf("================================\n");
  HashTable* table = new_table(TABLE_SIZE);
  for (int i = 0; i < n; ++i) {
    printf("Key %10lu, ", keys[i]);
    insert_key(table, keys[i]);
  }

  printf("\n================================\n");
  printf("========== Collisions ==========\n");
  printf("================================\n");
  int n_collisions = 0;
  for (int i = 0; i < n; ++i) {
    int b_contains = contains_key(table, keys[i]);
    if (!b_contains) n_collisions++;
    printf("Key %10lu in table? %s\n", keys[i], b_contains ? "yes" : "no");
  }
  printf("\nNumber of collisions: %d\n", n_collisions);

  printf("\n================================\n");
  printf("============ Removals ==========\n");
  printf("================================\n");
  printf("Removing keys 3 and 4.\n");
  delete_key(table, keys[3]);
  delete_key(table, keys[4]);
  printf("\n");

  for (int i = 0; i < n; ++i)
    printf("Key %10lu in table? %d\n", keys[i], contains_key(table, keys[i]));

  delete_table(table);
}
