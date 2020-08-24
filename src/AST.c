#include "include/AST.h"
#include <stdlib.h>
#include <string.h>

AST_T* init_ast(int type)
{
    AST_T* ast = (AST_T*) calloc(1, sizeof(struct AST_STRUCT));
    ast->type = type;

    ast->ignore_value = 0;
    ast->raw_value = 0;
    ast->comp_value = 0;
    ast->raw_child = 0;
    ast->string_value = 0;
    ast->var_value = 0;
    ast->var_name = 0;
    ast->parent = 0;

    ast->group_items = (void*) 0;
    ast->group_items_size = 0;
    ast->result = 0;

    ast->buffered = 0;

    return ast;
}

static char* ast_string_to_string(AST_T* ast)
{
    char* value = (char*) calloc(strlen(ast->string_value) + 1, sizeof(char));
    strcpy(value, ast->string_value);

    return value;
}

static char* ast_var_to_string(AST_T* ast)
{
    char* value = (char*) calloc(strlen(ast->var_name) + 1, sizeof(char));
    strcpy(value, ast->var_name);

    return value;
}

static char* ast_default_to_string(AST_T* ast)
{
    const char* template = "COULD_CONVERT_TO_STRING";
    char* val = calloc(strlen(template) + 1, sizeof(char));
    strcpy(val, template);

    return val;
}

char* ast_to_string(AST_T* ast)
{
    switch (ast->type)
    {
        case AST_STRING: return ast_string_to_string(ast); break;
        case AST_VAR: return ast_var_to_string(ast); break;
        default: return ast_default_to_string(ast); break;
    }
}
