#include "include/AST_utils.h"
#include "include/utils.h"
#include <stdio.h>
#include <string.h>

AST_T *ast_object_get_var_by_name(AST_T *ast_object, const char *key) {
  assert_not_nil(ast_object, "ast_object is nil\n");

  for (int i = 0; i < (int)ast_object->object_vars_size; i++) {
    AST_T *var = ast_object->object_vars[i];

    if (strcmp(var->var_name, key) == 0) {
      return var;
    }
  }

  return 0;
}

AST_T *ast_object_get_value_by_key(AST_T *ast_object, const char *key) {
  assert_not_nil(ast_object, "ast_object is nil\n");

  for (int i = 0; i < (int)ast_object->object_vars_size; i++) {
    AST_T *var = ast_object->object_vars[i];

    if (strcmp(var->var_name, key) == 0) {
      if (var->var_value)
        return var->var_value;
    }
  }

  return 0;
}

unsigned int ast_object_has_var(AST_T *ast_object, const char *key) {
  assert_not_nil(ast_object, "ast_object is nil\n");

  for (int i = 0; i < (int)ast_object->object_vars_size; i++) {
    AST_T *var = ast_object->object_vars[i];

    if (strcmp(var->var_name, key) == 0) {
      return 1;
    }
  }

  return 0;
}

AST_T *ast_object_redefine_var(AST_T *ast_object, const char *key,
                               AST_T *value) {
  assert_not_nil(ast_object, "ast_object is nil\n");

  for (int i = 0; i < (int)ast_object->object_vars_size; i++) {
    AST_T *var = ast_object->object_vars[i];

    if (strcmp(var->var_name, key) == 0) {
      ast_object->object_vars[i]->var_value = value;
      return ast_object->object_vars[i];
    }
  }

  return value;
}

AST_T *ast_object_get_keys(AST_T *ast_object) {
  assert_not_nil(ast_object, "ast_object is nil\n");

  AST_T *ast = init_ast(AST_GROUP);

  for (int i = 0; i < ast_object->object_vars_size; i++) {
    AST_T *var = ast_object->object_vars[i];

    AST_T *string = init_ast(AST_STRING);
    string->string_value = calloc(strlen(var->var_name) + 1, sizeof(char));
    strcpy(string->string_value, var->var_name);

    ast->group_items_size += 1;

    if (ast->group_items == (void *)0) {
      ast->group_items = calloc(1, sizeof(struct AST_STRUCT *));
      ast->group_items[0] = string;
    } else {
      ast->group_items = realloc(ast->group_items, sizeof(struct AST_STRUCT *) *
                                                       ast->group_items_size);
      ast->group_items[ast->group_items_size - 1] = string;
    }
  }

  return ast;
}

AST_T *ast_object_push_var(AST_T *ast_object, AST_T *ast_var) {
  assert_not_nil(ast_object, "ast_object is nil\n");
  assert_not_nil(ast_var, "ast_var is nil\n");

  ast_object->object_vars_size += 1;

  if (ast_object->object_vars == (void *)0) {
    ast_object->object_vars = calloc(1, sizeof(struct AST_STRUCT *));
    ast_object->object_vars[0] = ast_var;
  } else {
    ast_object->object_vars =
        realloc(ast_object->object_vars,
                sizeof(struct AST_STRUCT *) * ast_object->object_vars_size);
    ast_object->object_vars[ast_object->object_vars_size - 1] = ast_var;
  }

  return ast_var;
}

AST_T *ast_group_get_value_by_index(AST_T *ast_group, int index) {
  assert_not_nil(ast_group, "ast_group is nil\n");

  if (((int)ast_group->group_items_size) >= index)
    return ast_group->group_items[index];

  return 0;
}

AST_T *ast_group_push_item(AST_T *ast_group, AST_T *item) {
  assert_not_nil(ast_group, "ast_group is nil\n");

  ast_group->group_items_size += 1;

  if (ast_group->group_items == (void *)0) {
    ast_group->group_items = calloc(1, sizeof(struct AST_STRUCT *));
    ast_group->group_items[0] = item;
  } else {
    ast_group->group_items =
        realloc(ast_group->group_items,
                sizeof(struct AST_STRUCT *) * ast_group->group_items_size);
    ast_group->group_items[ast_group->group_items_size - 1] = item;
  }

  return ast_group;
}

AST_T *ast_root_push_item(AST_T *ast_root, AST_T *item) {
  assert_not_nil(ast_root, "ast_root is nil\n");

  ast_root->root_items_size += 1;

  if (ast_root->root_items == (void *)0) {
    ast_root->root_items = calloc(1, sizeof(struct AST_STRUCT *));
    ast_root->root_items[0] = item;
  } else {
    ast_root->root_items =
        realloc(ast_root->root_items,
                sizeof(struct AST_STRUCT *) * ast_root->root_items_size);
    ast_root->root_items[ast_root->root_items_size - 1] = item;
  }

  return ast_root;
}
