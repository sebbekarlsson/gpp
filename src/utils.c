#include "include/utils.h"
#include <stdio.h>
#include <stdlib.h>


void assert_not_nil(void* x, const char* msg)
{
    if (!!x) return;
    printf("ASSERT FAILED. ITEM IS NIL (%p) - %s\n", x, msg);
    exit(1);
}

void assert_not_reached()
{
    printf("ASSERT FAILED. SHOULD NOT HAVE REACHED HERE.\n");
    exit(1);
}
