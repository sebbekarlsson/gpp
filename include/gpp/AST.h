#ifndef GPP_AST_H
#define GPP_AST_H
#include <stdlib.h>
#include <stdbool.h>

// forward declaration
struct VISITOR_STRUCT;

typedef struct AST_STRUCT {
  enum {
    AST_NOOP,
    AST_COMP,
    AST_ASSIGN,
    AST_FUNCTION,
    AST_FUNCTION_CALL,
    AST_STRING,
    AST_FLOAT,
    AST_INT,
    AST_VAR,
    AST_GROUP,
    AST_ROOT,
    AST_COMMENT,
    AST_CALL,
    AST_OBJECT,
    AST_DOT,
    AST_NEWLINE,
    AST_NAME
  } type;

  char *ignore_value;
  char *raw_value;
  char *comp_value;
  struct AST_STRUCT *raw_child;
  char *interpreter_path;
  char *comment_value;
  struct AST_STRUCT *var_value;
  struct AST_STRUCT *parent;
  char *string_value;
  float float_value;
  int int_value;
  char *var_name;
  char *name;

  struct AST_STRUCT *call_group;

  struct AST_STRUCT **group_items;
  size_t group_items_size;

  struct AST_STRUCT **root_items;
  size_t root_items_size;

  struct AST_STRUCT **object_vars;
  size_t object_vars_size;

  struct AST_STRUCT **comments;
  size_t comments_size;

  struct AST_STRUCT *left;
  struct AST_STRUCT *right;

  struct AST_STRUCT *function_body;
  struct AST_STRUCT *function_args;

  unsigned int is_block;

  unsigned int buffered;
  unsigned int skip;
  unsigned int skip_comments;
  char *result;

  struct AST_STRUCT *(*fptr)(struct VISITOR_STRUCT *visitor,
                             struct AST_STRUCT *node, int argc,
                             struct AST_STRUCT **argv, int caller_argc,
                             struct AST_STRUCT **caller_argv);

  unsigned int x;
  unsigned int y;
} AST_T;

AST_T *init_ast(int type);

char *ast_to_string(AST_T *ast);

const char *ast_get_string_value(AST_T *ast);

char ast_get_char(AST_T* ast, int index);

char ast_get_char_lower(AST_T* ast, int index);

unsigned int ast_comment_is_meaningful(AST_T *ast);

bool ast_is_valid_object(AST_T* ast);
#endif
