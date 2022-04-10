#include <gpp/io.h>
#include <gpp/json.h>
#include <gpp/main.h>
#include <gpp/parser.h>
#include <gpp/visitor.h>
#include <gpp/gpp.h>
#include <gpp/utils.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

AST_T *gpp_load_context(char *filepath) {
  return (!(access(filepath, F_OK) != -1)) ? init_ast(AST_NOOP)
                                           : json_load(gpp_read_file(filepath));
}

gpp_result_T *init_gpp_result(char *res, AST_T *node) {
  gpp_result_T *result =
      (gpp_result_T *)calloc(1, sizeof(struct GPP_RESULT_STRUCT));
  result->res = res;
  result->node = node;

  return result;
}

gpp_result_T *gpp_eval(char *source, unsigned int lazy, AST_T *parent,
                       AST_T *context, GPPEnv* env) {

  if (!env->base_dir) {
    env->base_dir = gpp_dirname(env->base_path);
  }



  lexer_T *lexer = init_lexer(source);
  parser_T *parser = init_parser(lexer, env);
  AST_T *root = parser_parse(parser, parent);
  char *res = 0;

  if (!lazy) {
    AST_T *context_object = context ? context : gpp_load_context(CONTEXT_FILE);
    visitor_T *visitor = init_visitor(context_object, env);
    AST_T *resroot = visitor_visit(visitor, root, 0, 0);

    res = ast_to_string(resroot);

    free(visitor);
  }

  free(lexer);

  return init_gpp_result(res, root);
}
