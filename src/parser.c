#include "include/parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


parser_T* init_parser(lexer_T* lexer)
{
    parser_T* parser = (parser_T*) calloc(1, sizeof(struct PARSER_STRUCT));
    parser->lexer = lexer;
    parser->token = lexer_next_token(parser->lexer);
    parser->prev_token = parser->token;

    return parser;
}

token_T* parser_eat(parser_T* parser, int type)
{
    if (parser->token->type == type)
    {
        parser->prev_token = parser->token;
        parser->token = lexer_next_token(parser->lexer);
        return parser->token;
    }
    else
    {
        printf("[Parser.eat]: Unexpected `%s`\n", token_to_str(parser->token));
        exit(1);
    }
}

AST_T* parser_parse(parser_T* parser, AST_T* parent) {
    AST_T* ast = init_ast(AST_ROOT);

    while (parser->token->type != TOKEN_EOF)
    {
        AST_T* item = parser_parse_expr(parser, parent);

        ast->root_items_size += 1;

        if (ast->root_items == (void*) 0)
        {
            ast->root_items = calloc(1, sizeof(struct AST_STRUCT*));
            ast->root_items[0] = item;
        }
        else
        {
            ast->root_items = realloc(ast->root_items, sizeof(struct AST_STRUCT*) * ast->root_items_size);
            ast->root_items[ast->root_items_size-1] = item;
        }
    }

    return ast;
}

// TODO: implement
AST_T* parser_parse_expr(parser_T* parser, AST_T* parent)
{
    AST_T* ast = (void*) 0;

    switch (parser->token->type)
    {
        case TOKEN_RAW: ast = parser_parse_raw(parser, parent); break;
        case TOKEN_TEMPLATE_BEGIN: ast = parser_parse_template(parser, parent); break;
        case TOKEN_ID: ast = parser_parse_id(parser, parent); break;
        case TOKEN_STRING: ast = parser_parse_string(parser, parent); break;
        case TOKEN_LPAREN: ast = parser_parse_group(parser, parent); break;
        default: printf("[Parser]: Unexpected `%s`\n", token_to_str(parser->token)); exit(1); break;
    }

    if (ast != (void*) 0)
    {
        ast->parent = parent;
        return ast;
    }

    return init_ast(AST_NOOP);
}

// TODO: implement
AST_T* parser_parse_raw(parser_T* parser, AST_T* parent)
{
    char* value = parser->token->value;
    parser_eat(parser, TOKEN_RAW);
    AST_T* ast = init_ast(AST_RAW);
    ast->raw_value = (char*) calloc(strlen(value) + 1, sizeof(char));
    strcpy(ast->raw_value, value);

    if (parser->token->type != TOKEN_EOF)
        ast->raw_child = parser_parse(parser, parent);

    return ast;
}

// TODO: implement
AST_T* parser_parse_template(parser_T* parser, AST_T* parent)
{
    parser_eat(parser, TOKEN_TEMPLATE_BEGIN);
    AST_T* ast = init_ast(AST_TEMPLATE);
    ast->template_value = parser_parse(parser, ast);
    parser_eat(parser, TOKEN_TEMPLATE_END);

    ast->template_var = init_ast(AST_VAR);
    ast->template_var->var_value = (void*)0;

    if (parser->token->type != TOKEN_EOF)
        ast->template_child = parser_parse(parser, parent);

    return ast;
}

AST_T* parser_parse_id(parser_T* parser, AST_T* parent)
{
    parser_eat(parser, TOKEN_ID);

    if (parser->token->type == TOKEN_EQUALS)
        return parser_parse_assign(parser, parent);

    AST_T* ast = init_ast(AST_VAR);
    ast->var_name = (char*) calloc(strlen(parser->prev_token->value) + 1, sizeof(char));
    strcpy(ast->var_name, parser->prev_token->value);
    ast->var_value = (void*)0;

    return ast;
}

// TODO: implement
AST_T* parser_parse_assign(parser_T* parser, AST_T* parent)
{
    char* varname = (char*) calloc(strlen(parser->prev_token->value) + 1, sizeof(char));
    strcpy(varname, parser->prev_token->value);
    parser_eat(parser, TOKEN_EQUALS);
    AST_T* ast = init_ast(AST_ASSIGN);
    ast->var_value = parser_parse_expr(parser, parent);
    ast->var_name = varname;

    return ast;
}

AST_T* parser_parse_string(parser_T* parser, AST_T* parent)
{
    char* value = parser->token->value;
    parser_eat(parser, TOKEN_STRING);

    AST_T* ast = init_ast(AST_STRING);
    ast->string_value = value;

    return ast;
}

AST_T* parser_parse_group(parser_T* parser, AST_T* parent)
{
    parser_eat(parser, TOKEN_LPAREN);

    AST_T* ast = init_ast(AST_GROUP);
    
    while (parser->token->type != TOKEN_RPAREN)
    {
        AST_T* item = parser_parse_expr(parser, parent);

        ast->group_items_size += 1;

        if (ast->group_items == (void*) 0)
        {
            ast->group_items = calloc(1, sizeof(struct AST_STRUCT*));
            ast->group_items[0] = item;
        }
        else
        {
            ast->group_items = realloc(ast->group_items, sizeof(struct AST_STRUCT*) * ast->group_items_size);
            ast->group_items[ast->group_items_size-1] = item;
        }
    }

    parser_eat(parser, TOKEN_RPAREN);

    return ast;
}
