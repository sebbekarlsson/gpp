#ifndef GPP_AST_UTILS_H
#define GPP_AST_UTILS_H
#include "AST.h"
AST_T *ast_object_get_var_by_name(AST_T *ast_object, const char *key);
AST_T *ast_object_get_value_by_key(AST_T *ast_object, const char *key);
unsigned int ast_object_has_var(AST_T *ast_object, const char *key);
AST_T *ast_object_redefine_var(AST_T *ast_object, const char *key,
                               AST_T *value);
AST_T *ast_object_get_keys(AST_T *ast_object);
AST_T *ast_object_push_var(AST_T *ast_object, AST_T *ast_var);
AST_T *ast_group_get_value_by_index(AST_T *ast_group, int index);
AST_T *ast_group_push_item(AST_T *ast_group, AST_T *item);
AST_T *ast_root_push_item(AST_T *ast_root, AST_T *item);
#endif
