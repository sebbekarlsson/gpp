#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include "AST.h"
typedef struct PARSER_STRUCT
{
    token_T* token;
    lexer_T* lexer;
} parser_T;

parser_T* init_parser(lexer_T* lexer);

AST_T* parser_parse(parser_T* parser);

AST_T* parser_parse_ignore(parser_T* parser);

AST_T* parser_parse_template(parser_T* parser);

AST_T* parser_parse_assign(parser_T* parser);
#endif
