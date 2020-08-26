#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include "AST.h"
typedef struct PARSER_STRUCT
{
    token_T* token;
    token_T* prev_token;
    lexer_T* lexer;
} parser_T;

parser_T* init_parser(lexer_T* lexer);

token_T* parser_eat(parser_T* parser, int type);

AST_T* parser_parse(parser_T* parser, AST_T* parent);

AST_T* parser_parse_expr(parser_T* parser, AST_T* parent);

AST_T* parser_parse_raw(parser_T* parser, AST_T* parent);

AST_T* parser_parse_comp(parser_T* parser, AST_T* parent);

AST_T* parser_parse_id(parser_T* parser, AST_T* parent);

AST_T* parser_parse_assign(parser_T* parser, AST_T* parent);

AST_T* parser_parse_string(parser_T* parser, AST_T* parent);

AST_T* parser_parse_number(parser_T* parser, AST_T* parent);

AST_T* parser_parse_comment(parser_T* parser, AST_T* parent);

AST_T* parser_parse_group(parser_T* parser, AST_T* parent);
#endif
