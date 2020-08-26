#ifndef GPP_AST_H
#define GPP_AST_H
#include <stdlib.h>
typedef struct AST_STRUCT {
    enum
    {
        AST_NOOP,
        AST_RAW,
        AST_COMP,
        AST_ASSIGN,
        AST_FUNCTION_CALL,
        AST_STRING,
        AST_FLOAT,
        AST_INT,
        AST_VAR,
        AST_GROUP,
        AST_ROOT,
        AST_COMMENT,
        AST_CALL,
    } type;

    char* ignore_value;
    char* raw_value;
    char* comp_value;
    struct AST_STRUCT* raw_child;
    char* interpreter_path;
    char* comment_value;
    struct AST_STRUCT* var_value;
    struct AST_STRUCT* parent;
    char* string_value;
    float float_value;
    int int_value;
    char* var_name;
    
    struct AST_STRUCT* call_group;
    
    struct AST_STRUCT** group_items;
    size_t group_items_size;

    struct AST_STRUCT** root_items;
    size_t root_items_size;

    unsigned int buffered;
    char* result;
} AST_T;

AST_T* init_ast(int type);

char* ast_to_string(AST_T* ast);
#endif
