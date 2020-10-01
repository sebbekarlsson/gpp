#include "include/main.h"
#include "include/gpp.h"
#include "include/io.h"
#include <stdio.h>


int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    printf("Error, please specify file.\n");
    exit(1);
  }

  gpp_result_T* res = gpp_eval(gpp_read_file(argv[1]), 0, 0, 0);

  if (res->res)
    printf("%s\n", res->res);
}
