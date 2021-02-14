#include "include/AST.h"
#include "include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AST_T *init_ast(int type) {
  AST_T *ast = (AST_T *)calloc(1, sizeof(struct AST_STRUCT));
  ast->type = type;

  ast->ignore_value = 0;
  ast->raw_value = 0;
  ast->comp_value = 0;
  ast->raw_child = 0;
  ast->string_value = 0;
  ast->float_value = 0;
  ast->int_value = 0;
  ast->var_value = 0;
  ast->var_name = 0;
  ast->parent = 0;
  ast->call_group = 0;

  ast->group_items = (void *)0;
  ast->group_items_size = 0;
  ast->result = 0;
  ast->interpreter_path = 0;
  ast->comment_value = 0;
  ast->object_vars = 0;
  ast->object_vars_size = 0;
  ast->left = 0;
  ast->right = 0;
  ast->is_block = 0;
  ast->comments = 0;
  ast->comments_size = 0;

  ast->buffered = 0;
  ast->skip = 0;
  ast->skip_comments = 0;
  ast->function_body = 0;
  ast->function_args = 0;

  ast->fptr = 0;

  ast->x = 0;
  ast->y = 0;

  return ast;
}

static char *ast_newline_to_string(AST_T *ast) {
  char *nl = calloc(2, sizeof(char));
  nl[0] = '\n';
  nl[1] = '\0';
  return nl;
}

static char *ast_string_to_string(AST_T *ast) {
  char *value = (char *)calloc(strlen(ast->string_value) + 1, sizeof(char));
  strcpy(value, ast->string_value);

  return value;
}

static char *ast_var_to_string(AST_T *ast) {
  char *value = (char *)calloc(strlen(ast->var_name) + 1, sizeof(char));
  strcpy(value, ast->var_name);

  return value;
}

static char *ast_comment_to_string(AST_T *ast) {
  /*char* value = (char*) calloc(strlen(ast->comment_value) + 1, sizeof(char));
  strcpy(value, ast->comment_value);

  return value;*/

  char *str = (char *)calloc(1, sizeof(char));
  str[0] = '\0';

  return str;
}

static char *ast_default_to_string(AST_T *ast) {
  const char *template = "COULD_CONVERT_TO_STRING %d";
  char *val = calloc(strlen(template) + 128, sizeof(char));
  sprintf(val, template, ast->type);

  return val;
}

static char *ast_comp_to_string(AST_T *ast) {
  char *val = (char *)calloc(1, sizeof(char));
  val[0] = '\0';

  for (int i = 0; i < (int)ast->group_items_size; i++) {
    char *res = ast_to_string(ast->group_items[i]);
    val = (char *)realloc(val, (strlen(val) + strlen(res) + 1) * sizeof(char));
    strcat(val, res);
  }

  return val;
}

static char *ast_group_to_string(AST_T *ast) {
  char *val = (char *)calloc(1, sizeof(char));
  val[0] = '\0';

  for (int i = 0; i < (int)ast->group_items_size; i++) {
    char *res = ast_to_string(ast->group_items[i]);
    val = (char *)realloc(val, (strlen(val) + strlen(res) + 1) * sizeof(char));

    strcat(val, res);
  }

  return val;
}

static char *ast_root_to_string(AST_T *ast) {
  char *val = (char *)calloc(1, sizeof(char));
  val[0] = '\0';

  for (int i = 0; i < (int)ast->root_items_size; i++) {
    char *res = ast_to_string(ast->root_items[i]);
    val = (char *)realloc(val, (strlen(val) + strlen(res) + 1) * sizeof(char));
    strcat(val, res);
  }

  if (ast->x) {
    char *newstr = remove_indent(val, ast->x);
    free(val);
    return newstr;
  }

  return val;
}

static char *ast_call_to_string(AST_T *ast) {
  const char *template = "CALL `%s`";
  char *str =
      calloc(strlen(template) + strlen(ast->var_name) + 8, sizeof(char));
  sprintf(str, template, ast->var_name);

  return str;
}

static char *ast_function_to_string(AST_T *ast) {
  char *str = (char *)calloc(1, sizeof(char));
  str[0] = '\0';

  return str;
}

static char *ast_int_to_string(AST_T *ast) {
  const char *template = "%d";
  char *val = calloc(strlen(template) + 128, sizeof(char));
  sprintf(val, template, ast->int_value);

  return val;
}

static char *ast_float_to_string(AST_T *ast) {
  const char *template = "%12.6f";
  char *val = calloc(strlen(template) + 128, sizeof(char));
  sprintf(val, template, ast->float_value);

  return val;
}

char *ast_assign_to_string(AST_T *ast) {
  char *buff = calloc(2, sizeof(char));
  buff[0] = '\0';
  buff[1] = '\0';

  return buff;
}

char *ast_to_string(AST_T *ast) {
  if (!ast) {
    char *str = calloc(5, sizeof(char));
    strcpy(str, "NULL");
    return str;
  }

  switch (ast->type) {
  case AST_STRING:
    return ast_string_to_string(ast);
    break;
  case AST_ASSIGN:
    return ast_assign_to_string(ast);
    break;
  case AST_INT:
    return ast_int_to_string(ast);
    break;
  case AST_FLOAT:
    return ast_float_to_string(ast);
    break;
  case AST_VAR:
    return ast_var_to_string(ast);
    break;
  case AST_COMMENT:
    return ast_comment_to_string(ast);
    break;
  case AST_COMP:
    return ast_comp_to_string(ast);
    break;
  case AST_ROOT:
    return ast_root_to_string(ast);
    break;
  case AST_GROUP:
    return ast_group_to_string(ast);
    break;
  case AST_CALL:
    return ast_call_to_string(ast);
    break;
  case AST_FUNCTION:
    return ast_function_to_string(ast);
    break;
  case AST_NEWLINE:
    return ast_newline_to_string(ast);
    break;
  default:
    return ast_default_to_string(ast);
    break;
  }
}
