#include "include/builtins.h"
#include "include/AST_utils.h"
#include "include/io.h"
#include <string.h>


static void builtins_register_fptr(visitor_T* visitor, const char* name, AST_T* (*fptr)(
      visitor_T* visitor,
      AST_T* node,
      int argc,
      AST_T** argv,
      int caller_argc,
      AST_T** caller_argv
      )) {
  AST_T* var = init_ast(AST_VAR);
  var->var_name = (char*) calloc(strlen(name) + 1, sizeof(char));
  strcpy(var->var_name, name);
  var->fptr = fptr;
  ast_object_push_var(visitor->object, var);
}

void builtins_register(visitor_T* visitor)
{
  builtins_register_fptr(visitor, "map", builtin_fptr_map);
  builtins_register_fptr(visitor, "cat", builtin_fptr_cat);
}

AST_T* builtin_fptr_map(visitor_T* visitor, AST_T* node, int argc, AST_T** argv, int caller_argc, AST_T** caller_argv)
{ 

  if (argc < 2)
    return node;

  int iterable_type = ((AST_T*)argv[0])->type;
  AST_T* iterable = (iterable_type == AST_VAR || iterable_type == AST_CALL) ? visitor_visit(visitor, argv[0], caller_argc, caller_argv) : ((AST_T*)argv[0]);
  AST_T* mapping = (AST_T*) argv[1];

  AST_T* ast_group = init_ast(AST_GROUP);

  for (int i = 0; i < (int)iterable->group_items_size; i++)
  {
    AST_T* item = visitor_visit(visitor, iterable->group_items[i], caller_argc, caller_argv);

    AST_T* dot = visitor_visit(visitor, mapping, 1, (AST_T*[]){ item });

    ast_group_push_item(ast_group, dot);
  }

  return ast_group;
}

AST_T* builtin_fptr_cat(visitor_T* visitor, AST_T* node, int argc, AST_T** argv, int caller_argc, AST_T** caller_argv)
{
  if (argc < 1)
    return node;

  AST_T* ast_group = init_ast(AST_GROUP);

  for (int i = 0; i < argc; i++)
  {
    AST_T* string = (AST_T*) argv[i];

    if (!string->string_value)
      continue;

    char* contents = gpp_read_file(string->string_value);
    AST_T* ast_string = init_ast(AST_STRING);
    ast_string->string_value = calloc(strlen(contents) + 1, sizeof(char));
    strcpy(ast_string->string_value, contents);
    ast_group_push_item(ast_group, ast_string);
    free(contents);
  }

  return ast_group;
}
