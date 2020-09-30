#include "../../src/include/io.h"
#include "../../src/include/gpp.h"
#include "include/utils.h"
#include <string.h>

void test_inputs()
{
    {
      gpp_result_T* res = gpp_eval(gpp_read_file("inputs/hello-world.html"), 0, 0, 0);
      test_assert(!!res->res, "res->res exists");
      test_assert(strcmp(res->res, "<h1>Hello World</h1>") == 0, "Receives correct output");
    }

    {
      gpp_result_T* res = gpp_eval(gpp_read_file("inputs/arrow-function.html"), 0, 0, 0);
      test_assert(!!res->res, "res->res exists");
      test_assert(strcmp(res->res, "<li>hello</li><li>world</li>") == 0, "Receives correct output");
    }
}
