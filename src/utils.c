#include "include/utils.h"
#include <stdio.h>


void assert_not_nil(void* x, const char* msg)
{
    if (!!x) return;
    printf("ASSERT FAILED. ITEM IS NIL (%p) - %s\n", x, msg);
}
