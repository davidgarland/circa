#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define CIRCA_STATIC

#include "../circa_mac.h"
#include "../circa_err.h"

#define T int
  #include "../circa_arr.h"
#undef T

int main() {
  arr_int xs;
  arr_int_alloc(&xs);
  arr_int_free(&xs);
  return 0;
}
