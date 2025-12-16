#include "dynamic_array.h"
#include <stdio.h>

int main(void) {
  DA_INIT(nums, int);

  DA_PUSH_T(nums, int, 10);
  DA_PUSH_T(nums, int, 20);
  DA_PUSH_T(nums, int, 30);

  int x;
  DA_PEEK_T(nums, int, &x);
  printf("peek = %d\n", x);

  while (!DA_is_empty(&nums)) {
    DA_POP_T(nums, int, &x);
    printf("pop = %d\n", x);
  }

  DA_FREE(nums);
  return 0;
}
