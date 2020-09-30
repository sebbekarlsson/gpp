#include "include/gpp.h"
#include "include/main.h"
#include "include/parser.h"
#include "include/visitor.h"
#include "include/json.h"
#include "include/io.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

AST_T* gpp_load_context()
{
    return (!(access(CONTEXT_FILE, F_OK) != -1)) ? init_ast(AST_NOOP) : json_load(gpp_read_file(CONTEXT_FILE));
}

gpp_result_T* init_gpp_result(char* res, AST_T* node)
{
    gpp_result_T* result = (gpp_result_T*) calloc(1, sizeof(struct GPP_RESULT_STRUCT));
    result->res = res;
    result->node = node;

    return result;
}

gpp_result_T* gpp_eval(char* source, unsigned int lazy, AST_T* parent, AST_T* context)
{
    lexer_T* lexer = init_lexer(source);
    parser_T* parser = init_parser(lexer);
    AST_T* root = parser_parse(parser, parent);
    char* res = 0;

    if (!lazy)
    {
        AST_T* context_object = context ? context : gpp_load_context();
        visitor_T* visitor = init_visitor(context_object);
        AST_T* resroot = visitor_visit(visitor, root, 0, 0);

        res = ast_to_string(resroot);

        free(visitor);
    }

    free(lexer);

    return init_gpp_result(res, root);
}
