#include "include/AST.h"
#include <stdlib.h>

AST_T* init_ast(int type)
{
    AST_T* ast = (AST_T*) calloc(1, sizeof(struct AST_STRUCT));
    ast->type = type;

    return ast;
}
