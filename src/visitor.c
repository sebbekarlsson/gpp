#include "include/visitor.h"
#include "include/main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

visitor_T* init_visitor()
{
    visitor_T* visitor = (visitor_T*) calloc(1, sizeof(struct VISITOR_STRUCT));
    visitor->buffer = (void*)0;

    return visitor;
}

void visitor_buffer(visitor_T* visitor, char* buffstr)
{
    if (visitor->buffer == (void*) 0)
    {
        visitor->buffer = calloc(strlen(buffstr) + 1, sizeof(char));
        strcpy(visitor->buffer, buffstr);
    }
    else
    {
        visitor->buffer = realloc(visitor->buffer, (strlen(visitor->buffer) + strlen(buffstr) + 1) * sizeof(char));
        strcat(visitor->buffer, buffstr);
    }
}

AST_T* visitor_visit(visitor_T* visitor, AST_T* node)
{
    switch (node->type)
    {
        case AST_ROOT: return visitor_visit_root(visitor, node); break;
        case AST_RAW: return visitor_visit_raw(visitor, node); break;
        case AST_COMP: return visitor_visit_comp(visitor, node); break;
        case AST_ASSIGN: return visitor_visit_assign(visitor, node); break;
        case AST_STRING: return visitor_visit_string(visitor, node); break;
        case AST_VAR: return visitor_visit_var(visitor, node); break;
        case AST_GROUP: return visitor_visit_group(visitor, node); break;
        default: printf("[Visitor]: Unhandled node of type `%d`\n", node->type); exit(1); break;
    }
    return init_ast(AST_NOOP);
}

AST_T* visitor_visit_root(visitor_T* visitor, AST_T* node)
{
    for (int i = 0; i < (int)node->root_items_size; i++)
        visitor_visit(visitor, node->root_items[i]);

    return node;
}

AST_T* visitor_visit_raw(visitor_T* visitor, AST_T* node)
{
    for (int i = 0; i < (int)node->group_items_size; i++)
        visitor_buffer(visitor, ast_to_string(node->group_items[i]));

    return node;
}

AST_T* visitor_visit_comp(visitor_T* visitor, AST_T* node)
{
    for (int i = 0; i < (int)node->group_items_size; i++)
        visitor_visit(visitor, node->group_items[i]);

    return node;
}

AST_T* visitor_visit_assign(visitor_T* visitor, AST_T* node)
{
    if (node->parent == (void*)0)
    {
        printf("[Visitor.visit_assign]: Node has no parent.\n");
        exit(1);
    }

    if (strcmp(node->var_name, "@") == 0)
    {
        // TODO: fix
    }

    return node;
}

AST_T* visitor_visit_string(visitor_T* visitor, AST_T* node)
{
    visitor_buffer(visitor, node->string_value);

    return node;
}

AST_T* visitor_visit_var(visitor_T* visitor, AST_T* node)
{
    if (node->var_value == (void*)0)
    {
        printf("[Visitor.visit_var]: `%s` is not defined.\n", node->var_name);
        exit(1);
    }

    return visitor_visit(visitor, node->var_value);
}

AST_T* visitor_visit_group(visitor_T* visitor, AST_T* node)
{
    for (int i = 0; i < (int) node->group_items_size; i++)
    {
        visitor_visit(visitor, node->group_items[i]);
    }

    return node;
}
