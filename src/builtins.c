#include "gpp/AST.h"
#include <gpp/AST_utils.h>
#include <gpp/builtins.h>
#include <gpp/gpp.h>
#include <gpp/io.h>
#include <gpp/utils.h>
#include <limits.h>
#include <string.h>
#include <gpp/macros.h>

static void builtins_register_fptr(visitor_T *visitor, const char *name,
                                   AST_T *(*fptr)(visitor_T *visitor,
                                                  AST_T *node, int argc,
                                                  AST_T **argv, int caller_argc,
                                                  AST_T **caller_argv)) {
  AST_T *var = init_ast(AST_VAR);
  var->var_name = (char *)calloc(strlen(name) + 1, sizeof(char));
  strcpy(var->var_name, name);
  var->fptr = fptr;
  ast_object_push_var(visitor->object, var);
}

void builtins_register(visitor_T *visitor) {
  builtins_register_fptr(visitor, "map", builtin_fptr_map);
  builtins_register_fptr(visitor, "sort", builtin_fptr_sort);
  builtins_register_fptr(visitor, "cat", builtin_fptr_cat);
  builtins_register_fptr(visitor, "join", builtin_fptr_join);
  builtins_register_fptr(visitor, "load", builtin_fptr_load);
  builtins_register_fptr(visitor, "newline", builtin_fptr_newline);
  builtins_register_fptr(visitor, "range", builtin_fptr_range);
}

AST_T *builtin_fptr_range(visitor_T *visitor, AST_T *node, int argc,
                          AST_T **argv, int caller_argc, AST_T **caller_argv) {

  if (argc < 1)
    return node;

  // unsigned int new_args_size = copy_args_into_args(argv, argc, &caller_argv,
  // caller_argc);

  int n = 0;

  AST_T *n_val = (AST_T *)argv[0];
  if (!n_val)
    return node;
  n_val = visitor_visit(visitor, n_val, argc, argv);
  if ((n_val->type != AST_INT && n_val->type != AST_FLOAT))
    return node;
  n = (int)MAX(n_val->int_value, n_val->float_value);
  if (n <= 0)
    return node;

  AST_T *ast_group = init_ast(AST_GROUP);

  for (int i = 0; i < n; i++) {
    AST_T *numeric = init_ast(AST_INT);
    numeric->int_value = i;

    ast_group_push_item(ast_group, numeric);
  }

  return ast_group;
}

AST_T *builtin_fptr_map(visitor_T *visitor, AST_T *node, int argc, AST_T **argv,
                        int caller_argc, AST_T **caller_argv) {
  if (argc < 2)
    return node;

  // unsigned int new_args_size = copy_args_into_args(argv, argc, &caller_argv,
  // caller_argc);

  int iterable_type = ((AST_T *)argv[0])->type;
  AST_T *iterable =
      (iterable_type == AST_VAR || iterable_type == AST_CALL)
          ? visitor_visit(visitor, argv[0], caller_argc, caller_argv)
          : ((AST_T *)argv[0]);
  AST_T *mapping = (AST_T *)argv[1];

  AST_T *ast_group = init_ast(AST_GROUP);

  for (int i = 0; i < (int)iterable->group_items_size; i++) {
    AST_T *item = visitor_visit(visitor, iterable->group_items[i], caller_argc,
                                caller_argv);

    /**
     * TODO: move this to somewhere else and cleanup memory
     * afterwards.
     */
    AST_T **child_args = calloc(1, sizeof(struct AST_STRUCT *));
    child_args[0] = item;
    unsigned int child_args_size = 1;
    for (int j = 0; j < (int)caller_argc; j++) {
      child_args_size += 1;
      child_args =
          realloc(child_args, (child_args_size) * sizeof(struct AST_STRUCT *));
      child_args[child_args_size - 1] = caller_argv[j];
    }

    AST_T *dot = visitor_visit(visitor, mapping, child_args_size, child_args);

    ast_group_push_item(ast_group, dot);
  }

  return ast_group;
}

AST_T *builtin_fptr_sort(visitor_T *visitor, AST_T *node, int argc,
                         AST_T **argv, int caller_argc, AST_T **caller_argv) {

  int iterable_type = ((AST_T *)argv[0])->type;
  AST_T *iterable =
      (iterable_type == AST_VAR || iterable_type == AST_CALL)
          ? visitor_visit(visitor, argv[0], caller_argc, caller_argv)
          : ((AST_T *)argv[0]);

  AST_T *ast_group = init_ast(AST_GROUP);

  for (int i = 0; i < (int)iterable->group_items_size; i++) {
    AST_T *item = visitor_visit(visitor, iterable->group_items[i], caller_argc,
                                caller_argv);

    ast_group_push_item(ast_group, item);
  }

  ast_sort_alphabetical(ast_group); 

  return ast_group;
}

AST_T *builtin_fptr_cat(visitor_T *visitor, AST_T *node, int argc, AST_T **argv,
                        int caller_argc, AST_T **caller_argv) {
  if (argc < 1)
    return node;

  AST_T *ast_group = init_ast(AST_GROUP);

  for (int i = 0; i < argc; i++) {
    AST_T *string = (AST_T *)argv[i];

    if (!string->string_value)
      continue;

    if (!gpp_file_exists(string->string_value)) {
      fprintf(stderr, "# GPP ERROR: no such file %s\n", string->string_value);
      return ast_group;
    }

    char *contents = gpp_read_file(string->string_value);
    AST_T *ast_string = init_ast(AST_STRING);
    ast_string->string_value = calloc(strlen(contents) + 1, sizeof(char));
    strcpy(ast_string->string_value, contents);
    ast_group_push_item(ast_group, ast_string);
    free(contents);
  }

  return ast_group;
}

AST_T *builtin_fptr_load(visitor_T *visitor, AST_T *node, int argc,
                         AST_T **argv, int caller_argc, AST_T **caller_argv) {
  if (argc < 1)
    return node;

  AST_T *context = 0;

  for (int i = 0; i < argc; i++) {
    AST_T *string = (AST_T *)argv[i];

    if (!string->string_value)
      continue;

    context = gpp_load_context(string->string_value);

    if (context)
      break;
  }

  return context ? context : init_ast(AST_NOOP);
}

AST_T *builtin_fptr_newline(visitor_T *visitor, AST_T *node, int argc,
                            AST_T **argv, int caller_argc,
                            AST_T **caller_argv) {
  return init_ast(AST_NEWLINE);
}

AST_T *builtin_fptr_join(visitor_T *visitor, AST_T *node, int argc,
                         AST_T **argv, int caller_argc, AST_T **caller_argv) {
  if (argc < 2)
    return node;

  int iterable_type = ((AST_T *)argv[0])->type;
  AST_T *iterable =
      (iterable_type == AST_VAR || iterable_type == AST_CALL)
          ? visitor_visit(visitor, argv[0], caller_argc, caller_argv)
          : ((AST_T *)argv[0]);
  AST_T *mapping = (AST_T *)argv[1];

  AST_T *ast_group = init_ast(AST_GROUP);

  for (int i = 0; i < (int)iterable->group_items_size; i++) {
    AST_T *item = visitor_visit(visitor, iterable->group_items[i], caller_argc,
                                caller_argv);
    ast_group_push_item(ast_group, item);

    if (i < iterable->group_items_size - 1)
      ast_group_push_item(ast_group, mapping);
  }

  return ast_group;
}
