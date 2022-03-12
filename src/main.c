#include <gpp/main.h>
#include <gpp/gpp.h>
#include <gpp/io.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Error, please specify file.\n");
    exit(1);
  }

  const char* filepath = argv[1];
  GPPEnv env = (GPPEnv){ .base_path = filepath };
  gpp_result_T *res = gpp_eval(gpp_read_file(filepath), 0, 0, 0, &env);

  if (env.base_dir) {
    free(env.base_dir);
    env.base_dir = 0;
  }

  if (res->res)
    printf("%s\n", res->res);
}
