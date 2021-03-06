#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define CIRCA_STATIC

#include "../circa_mac.h"
#include "../circa_err.h"
#include "../circa_cmp.h"
#include "../circa_hash.h"

#define K int
#define V int
  #include "../circa_map.h"
#undef V
#undef K

#define SIZ 4000000

int main() {
  map_int_int m;
  map_int_int_alloc(&m);

  for (int i = 0; i < SIZ; i++)
    map_int_int_set(&m, &i, &i);

  int j;
  for (int i = 0; i < SIZ; i++) {
    map_int_int_get(&m, &i, &j);
    if (j != i) puts("oh no");
  }

  map_int_int_free(&m);

  return 0;
}
