#include "include/parser.h"
#include <stdlib.h>


parser_T* init_parser(lexer_T* lexer)
{
    parser_T* parser = (parser_T*) calloc(1, sizeof(struct PARSER_STRUCT));
    parser->lexer = lexer;

    return parser;
}

// TODO: implement
AST_T* parser_parse(parser_T* parser)
{
    return parser_parse_template(parser);
}

// TODO: implement
AST_T* parser_parse_ignore(parser_T* parser)
{
    return init_ast(AST_IGNORE);
}

// TODO: implement
AST_T* parser_parse_template(parser_T* parser)
{
    return init_ast(AST_TEMPLATE);
}

// TODO: implement
AST_T* parser_parse_assign(parser_T* parser)
{
    return init_ast(AST_ASSIGN);
}
