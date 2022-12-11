#ifndef GPP_MACROS_H
#define GPP_MACROS_H
#include <stdio.h>

#ifndef OR
#define OR(a, b) (a ? a : b)
#endif

#define GPP_WARNING(...)                                                      \
  {                                                                            \
    printf("# (GPP)(Warning)(%s): \n", __func__);   \
    fprintf(__VA_ARGS__);                                                      \
  }
#define GPP_WARNING_RETURN(ret, ...)                                          \
  {                                                                            \
    printf("\n****\n");                                                        \
    printf("# (GPP)(Warning)(%s): \n", __func__);   \
    fprintf(__VA_ARGS__);                                                      \
    printf("\n****\n");                                                        \
    return ret;                                                                \
  }
#endif
