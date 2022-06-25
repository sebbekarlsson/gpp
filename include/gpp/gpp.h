#ifndef GPP_H
#define GPP_H
#include <gpp/AST.h>
#include <gpp/env.h>

typedef struct GPP_RESULT_STRUCT {
  char *res;
  AST_T *node;
} gpp_result_T;

AST_T *gpp_load_context(char *filepath);

gpp_result_T *init_gpp_result(char *res, AST_T *node);
gpp_result_T *gpp_eval(char *source, unsigned int lazy, AST_T *parent,
                       AST_T *context, GPPEnv* env, GPPEnv* global_env);


unsigned int gpp_has_included(GPPEnv* global_env, const char* path);
#endif
