#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct hashfn_name {
  HashFunc hash;
  char*    name;
} HashFnName;

void
test(HashFunc hash_fn, char* hashfn_name, int no_rep, int loop)
{
  clock_t tick, tack;

  for (int j = 0; j < no_rep; ++j) {
    tick = clock();
    for (int i = 0; i < loop; ++i)
      hash_fn(i, i);
    tack = clock();
    printf("%s %zu\n", hashfn_name, tack - tick);
  }
  printf("------------------------------\n");
}

int
main()
{
  HashFnName hash_fns[] = { { identity_hash,          "identity", },
	                    { additive_hash,          "additive", },
			    { rotating_hash,          "rotating", },
			    { rotating_hash_rev,      "rotating rev", },
			    { one_at_a_time_hash,     "one-at-a-time", },
			    { one_at_a_time_hash_rev, "one-at-time-rev", },
			    { jenkins_hash,           "jenkins", }};

  for (size_t i =0; i < sizeof hash_fns/ sizeof *hash_fns; i++) {
    test(hash_fns[i].hash, hash_fns[i].name, 10, 100000);
  }
}
