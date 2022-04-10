#include <gpp/gpp.h>
#include <gpp/io.h>
#include "include/utils.h"
#include <string.h>

void test_inputs(char* argv[]) {

  GPPEnv env = (GPPEnv){ .base_path = "inputs" };

  {
    printf("Running test: hello-world.html\n");
    gpp_result_T *res =
      gpp_eval(gpp_read_file("inputs/hello-world.html"), 0, 0, 0, &env);


    test_assert(!!res->res, "res->res exists");
    test_assert(strcmp(res->res, "<h1>Hello World</h1>") == 0,
                "Receives correct output");
  }

  {
    printf("Running test: arrow-function.html\n");
    gpp_result_T *res =
        gpp_eval(gpp_read_file("inputs/arrow-function.html"), 0, 0, 0, &env);
    test_assert(!!res->res, "res->res exists");
    test_assert(strcmp(res->res, "<li>hello</li><li>world</li>") == 0,
                "Receives correct output");
  }

  {
    printf("Running test: map-context.html\n");
    gpp_result_T *res =
        gpp_eval(gpp_read_file("inputs/map-context.html"), 0, 0, 0, &env);
    test_assert(!!res->res, "res->res exists");
    test_assert(strcmp(res->res, "<li>Not</li>\n"
                                 "<li>Made</li>\n"
                                 "<li>with</li>\n"
                                 "<li>Coffee</li>\n"
                                 "<li>and</li>\n"
                                 "<li>Love</li>\n"
                                 "<li>in</li>\n"
                                 "<li>San</li>\n"
                                 "<li>Francisco</li>\n") == 0,
                "Receives correct output");
  }

  {
    printf("Running test: escape.html\n");
    gpp_result_T *res = gpp_eval(gpp_read_file("inputs/escape.html"), 0, 0, 0, &env);
    test_assert(!!res->res, "res->res exists");
    test_assert(strcmp(res->res, "&lt;h1&gt;Hello&lt;/h1&gt;") == 0,
                "Receives correct output");
  }
}
