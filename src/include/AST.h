#ifndef GPP_AST_H
#define GPP_AST_H
typedef struct AST_STRUCT {
    enum
    {
        AST_TEMPLATE,
        AST_IGNORE,
        AST_ASSIGN,
        AST_FUNCTION_CALL,
    } type;
} AST_T;

AST_T* init_ast(int type);
#endif
