#include "include/main.h"
#include "include/lexer.h"
#include "include/parser.h"
#include "include/visitor.h"
#include "include/io.h"
#include "include/json.h"
#include <stdio.h>
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

gpp_result_T* gpp_eval(char* source, unsigned int force_raw, unsigned int lazy)
{
    lexer_T* lexer = init_lexer(source, force_raw);
    parser_T* parser = init_parser(lexer);
    AST_T* root = parser_parse(parser, (void*)0);
    char* res = 0;

    if (!lazy)
    {
        AST_T* context_object = gpp_load_context();
        visitor_T* visitor = init_visitor(context_object);
        visitor_visit(visitor, root, 0, (void*)0);

        if (visitor->buffer)
        {
            res = (char*) calloc(strlen(visitor->buffer) + 1, sizeof(char));
            strcpy(res, visitor->buffer);
        }

        free(visitor->buffer);
        free(visitor);
    }

    free(lexer);
    

    return init_gpp_result(res, root);
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Error, please specify file.\n");
        exit(1);
    }

    gpp_result_T* res = gpp_eval(gpp_read_file(argv[1]), 0, 0);

    if (res->res)
        printf("%s\n", res->res);
}
