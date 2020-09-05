#include "include/parser.h"
#include "include/main.h"
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
        printf("[Parser.eat]: Unexpected (`%s` `%s`)\nWas expecting `%d`\n", token_to_str(parser->token), parser->token->value, type);
        exit(1);
    }
}

AST_T* parser_parse(parser_T* parser, AST_T* parent) {
    AST_T* ast = parent ? init_ast(AST_ROOT) : init_ast(AST_ROOT);

    while (parser->token->type != TOKEN_EOF)
    {
        AST_T* item = parser_parse_expr(parser, parent ? parent : ast);

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
    if (parser->token->type == TOKEN_EOF)
        return init_ast(AST_NOOP);

    AST_T* ast = (void*) 0;

    switch (parser->token->type)
    {
        case TOKEN_RAW: ast = parser_parse_raw(parser, parent); break;
        case TOKEN_COMP_BEGIN: ast = parser_parse_comp(parser, parent); break;
        case TOKEN_ID: ast = parser_parse_id(parser, parent); break;
        case TOKEN_STRING: ast = parser_parse_string(parser, parent); break;
        case TOKEN_NUMBER: ast = parser_parse_number(parser, parent); break;
        case TOKEN_LPAREN: ast = parser_parse_group(parser, parent); break;
        case TOKEN_LBRACKET: ast = parser_parse_group(parser, parent); break;
        case TOKEN_COMMENT: ast = parser_parse_comment(parser, parent); break;
        default: if (parser->token->type != TOKEN_EOF && parser->token->type != TOKEN_DOT ) { printf("[Parser]: Unexpected `%s` `%s`\n", token_to_str(parser->token), parser->token->value); exit(1); } break;
    }


    if (ast != (void*) 0)
    {
        ast->parent = parent;
        
        if (parser->token->type == TOKEN_DOT)
        {
            parser_eat(parser, TOKEN_DOT);
            AST_T* ptrast = parser_parse_expr(parser, parent);

            ast->right = ptrast;
        }

        return ast;
    }

    return init_ast(AST_NOOP);
}

// TODO: implement
AST_T* parser_parse_raw(parser_T* parser, AST_T* parent)
{
    char* value = parser->token->value;

    parser_eat(parser, TOKEN_RAW);
    
    AST_T* ast = init_ast(AST_ROOT);
    ast->parent = parent;
    ast->raw_value = (char*) calloc(strlen(value) + 1, sizeof(char));
    strcpy(ast->raw_value, value);
    gpp_result_T* evres = gpp_eval(ast->raw_value, 1, ast, 0);
    ast->raw_child = evres->node;
    ast->result = evres->res;
    evres->node->raw_value = ast->raw_value;

    return evres->node;
}

// TODO: implement
AST_T* parser_parse_comp(parser_T* parser, AST_T* parent)
{
    parser_eat(parser, TOKEN_COMP_BEGIN);

    AST_T* ast = init_ast(AST_COMP);

    while (parser->token->type != TOKEN_COMP_END && parser->token->type != TOKEN_EOF)
    {
        AST_T* item = parser_parse_expr(parser, ast);

        ast->group_items_size += 1;

        if (ast->group_items == (void*) 0)
        {
            ast->group_items = calloc(ast->group_items_size, sizeof(struct AST_STRUCT*));
            ast->group_items[ast->group_items_size-1] = item;
        }
        else
        {
            ast->group_items = realloc(ast->group_items, sizeof(struct AST_STRUCT*) * ast->group_items_size);
            ast->group_items[ast->group_items_size-1] = item;
        }
    }

    parser_eat(parser, TOKEN_COMP_END); 

    return ast;
}

AST_T* parser_parse_id(parser_T* parser, AST_T* parent)
{
    parser_eat(parser, TOKEN_ID);

    if (parser->token->type == TOKEN_EQUALS)
        return parser_parse_assign(parser, parent);

    char* var_name = (char*) calloc(strlen(parser->prev_token->value) + 1, sizeof(char));
    strcpy(var_name, parser->prev_token->value);

    if(parser->token->type == TOKEN_LBRACKET || parser->token->type == TOKEN_LPAREN)
    {
        AST_T* ast_call = init_ast(AST_CALL);
        ast_call->var_name = var_name;
        AST_T* ast_group = parser_parse_group(parser, parent);
        ast_call->call_group = ast_group;

        return ast_call;
    }
   
    AST_T* ast = init_ast(AST_VAR);
    ast->var_name = var_name;
    ast->var_value = (void*)0;

    return ast;
}

// TODO: implement
AST_T* parser_parse_assign(parser_T* parser, AST_T* parent)
{
    char* varname = (char*) calloc(strlen(parser->prev_token->value) + 1, sizeof(char));
    strcpy(varname, parser->prev_token->value);
    parser_eat(parser, TOKEN_EQUALS);
    AST_T* var_value = parser_parse_expr(parser, parent);
    
    if (parser->token->type == TOKEN_ARROW_RIGHT)
    {
        if (var_value->type != AST_GROUP)
        {
            printf("[Parser]: Function arguments needs to be wrapped in a group.\n");
            exit(1);
        }

        parser_eat(parser, TOKEN_ARROW_RIGHT);
        AST_T* ast_func = init_ast(AST_FUNCTION);
        ast_func->parent = parent;
        ast_func->var_name = varname;
        ast_func->function_body = parser_parse_expr(parser, ast_func);
        ast_func->function_args = var_value;

        return ast_func;
    }

    AST_T* ast = init_ast(AST_ASSIGN);
    ast->var_value = var_value;
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

AST_T* parser_parse_number(parser_T* parser, AST_T* parent)
{
    char* value = parser->token->value;
    parser_eat(parser, TOKEN_NUMBER);

    AST_T* ast = init_ast(AST_FLOAT);
    ast->float_value = atof(value);

    return ast;
}

AST_T* parser_parse_comment(parser_T* parser, AST_T* parent)
{
    AST_T* p = parent;

    char* value = parser->token->value;
    parser_eat(parser, TOKEN_COMMENT);

    AST_T* ast = init_ast(AST_COMMENT);
    ast->comment_value = value;

    ast->parent = p;

    return ast;
}

AST_T* parser_parse_group(parser_T* parser, AST_T* parent)
{
    unsigned int is_bracket = 0;

    if (parser->token->type == TOKEN_LBRACKET)
    {
        parser_eat(parser, TOKEN_LBRACKET);
        is_bracket = 1;
    }
    else
    {
        parser_eat(parser, TOKEN_LPAREN);
    }

    AST_T* ast = init_ast(AST_GROUP);

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

    while (parser->token->type == TOKEN_COMMA)
    {
        parser_eat(parser, TOKEN_COMMA);

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

    if (is_bracket)
    {
        parser_eat(parser, TOKEN_RBRACKET);
    }
    else
    {
        parser_eat(parser, TOKEN_RPAREN);
    }

    return ast;
}
