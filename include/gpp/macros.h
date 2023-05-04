#ifndef GPP_MACROS_H
#define GPP_MACROS_H
#include <stdio.h>

#ifndef OR
#define OR(a, b) (a ? a : b)
#endif

#ifndef SWAP
#define SWAP(T, x, y)                                                          \
  {                                                                            \
    T tmp = x;                                                                 \
    x = y;                                                                     \
    y = tmp;                                                                   \
  }
#endif

#define GPP_BUBBLE_SORT(T, arr, n, cmp)                                        \
  {                                                                            \
    for (int i = 0; i < n - 1; i++) {                                          \
      for (int j = 0; j < n - i - 1; j++) {                                    \
        T a = arr[j];                                                          \
        T b = arr[j + 1];                                                      \
        if (cmp) {                                                             \
          SWAP(T, arr[j], arr[j + 1]);                                         \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  }

#define GPP_WARNING(...)                                                       \
  {                                                                            \
    printf("# (GPP)(Warning)(%s): \n", __func__);                              \
    fprintf(__VA_ARGS__);                                                      \
  }
#define GPP_WARNING_RETURN(ret, ...)                                           \
  {                                                                            \
    printf("\n****\n");                                                        \
    printf("# (GPP)(Warning)(%s): \n", __func__);                              \
    fprintf(__VA_ARGS__);                                                      \
    printf("\n****\n");                                                        \
    return ret;                                                                \
  }
#endif
