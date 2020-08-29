#include "include/AST_utils.h"
#include "include/utils.h"
#include <string.h>
#include <stdio.h>

AST_T* ast_object_get_value_by_key(AST_T* ast_object, const char* key)
{
    assert_not_nil(ast_object, "ast_object is nil\n");

    for (int i = 0; i < (int) ast_object->object_vars_size; i++)
    {
        AST_T* var = ast_object->object_vars[i];

        if (strcmp(var->var_name, key) == 0)
        {
            if (var->var_value)
                return var->var_value;
        }
    }

    return 0;
}

AST_T* ast_object_get_keys(AST_T* ast_object)
{
    assert_not_nil(ast_object, "ast_object is nil\n");

    AST_T* ast = init_ast(AST_GROUP);

    for (int i = 0; i < ast_object->object_vars_size; i++)
    {
        AST_T* var = ast_object->object_vars[i];

        AST_T* string = init_ast(AST_STRING);
        string->string_value = calloc(strlen(var->var_name) + 1, sizeof(char));
        strcpy(string->string_value, var->var_name);

        ast->group_items_size += 1;

        if (ast->group_items == (void*) 0)
        {
            ast->group_items = calloc(1, sizeof(struct AST_STRUCT*));
            ast->group_items[0] = string;
        }
        else
        {
            ast->group_items = realloc(ast->group_items, sizeof(struct AST_STRUCT*) * ast->group_items_size);
            ast->group_items[ast->group_items_size-1] = string;
        }
    }

    return ast;
}

AST_T* ast_group_get_value_by_index(AST_T* ast_group, int index)
{
    assert_not_nil(ast_group, "ast_group is nil\n");
    
    if (((int)ast_group->group_items_size) >= index)
        return ast_group->group_items[index];

    return 0;
}
