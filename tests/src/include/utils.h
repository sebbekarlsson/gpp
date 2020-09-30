#ifndef GPP_TESTS_UTILS_H
#define GPP_TESTS_UTILS_H
#include <stdio.h>
#include <stdlib.h>
void test_assert(unsigned int expr, const char* msg)
{
  if (!!expr) {
    printf("[OK]: %s\n", msg);
    return; 
  }

  printf("[FAILED]: %s = not true\n", msg);
  exit(1);
}
#endif
