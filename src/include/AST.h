#ifndef GPP_AST_H
#define GPP_AST_H
typedef struct AST_STRUCT {
    enum
    {
        AST_NOOP,
        AST_TEMPLATE,
        AST_RAW,
        AST_ASSIGN,
        AST_FUNCTION_CALL,
        AST_STRING,
        AST_VAR,
    } type;

    char* ignore_value;
    char* raw_value;
    struct AST_STRUCT* raw_child;
    struct AST_STRUCT* template_value;
    struct AST_STRUCT* template_child;
    struct AST_STRUCT* template_var;
    struct AST_STRUCT* var_value;
    struct AST_STRUCT* parent;
    char* string_value;
    char* var_name;
} AST_T;

AST_T* init_ast(int type);

char* ast_to_string(AST_T* ast);
#endif
