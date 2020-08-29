#ifndef GPP_AST_UTILS_H
#define GPP_AST_UTILS_H
#include "AST.h"
AST_T* ast_object_get_value_by_key(AST_T* ast_object, const char* key);
AST_T* ast_object_get_keys(AST_T* ast_object);
AST_T* ast_group_get_value_by_index(AST_T* ast_group, int index);
#endif
