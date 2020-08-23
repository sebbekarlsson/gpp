#include "include/lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char* charstr(char c)
{
    char* new_str = (char*) calloc(2, sizeof(char));
    new_str[0] = c;
    new_str[1] = '\0';

    return new_str;
}

lexer_T* init_lexer(char* src, unsigned int eval)
{
    lexer_T* lexer = (lexer_T*) calloc(1, sizeof(struct LEXER_STRUCT));
    lexer->src = (char*) calloc(strlen(src) + 1, sizeof(char));
    strcpy(lexer->src, src);
    lexer->i = 0;
    lexer->c = lexer->src[lexer->i];
    lexer->len = strlen(lexer->src);
    lexer->eval = eval;

    return lexer;
}

void lexer_advance(lexer_T* lexer)
{
    if (lexer->c != '\0' && lexer->i < lexer->len) {
        lexer->i = lexer->i + 1;
        lexer->c = lexer->src[lexer->i];
    } else {
        printf("[Lexer]: Cannot advance anymore.\n");
    }
}

token_T* lexer_advance_token(lexer_T* lexer, token_T* token)
{
    lexer_advance(lexer);
    return token;
}

void lexer_skip_whitespace(lexer_T* lexer)
{
    while (lexer->c == ' ' || lexer->c == 10 || lexer->c == '\n')
        lexer_advance(lexer);
}

token_T* lexer_parse_left_paren(lexer_T* lexer) {
    lexer_advance(lexer);

    if (lexer->c != '(')
        return init_token(charstr(lexer->c), TOKEN_LPAREN);

    return lexer_parse_raw(lexer);
}

token_T* lexer_parse_right_paren(lexer_T* lexer) {
    lexer_advance(lexer);

    return init_token(charstr(lexer->c), TOKEN_RPAREN); 
}

token_T* lexer_parse_id(lexer_T* lexer)
{
    char* value = (char*) calloc(1, sizeof(char));
    value[0] = '\0';

    do 
    {
        value = (char*) realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, charstr(lexer->c));
        lexer_advance(lexer);
    } while (lexer->c != '\0' && isalpha(lexer->c));

    return init_token(value, TOKEN_ID);
}

token_T* lexer_parse_string(lexer_T* lexer)
{
    lexer_advance(lexer);

    char* value = (char*) calloc(1, sizeof(char));
    value[0] = '\0';

    do 
    {
        value = (char*) realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, charstr(lexer->c));
        lexer_advance(lexer);
    } while (lexer->c != '\0' && lexer->c != '"');

    return init_token(value, TOKEN_STRING);
}

static char* lexer_scrape(lexer_T* lexer, char cond_c, char cond_b, unsigned int close_i)
{
    char* value = (char*) calloc(1, sizeof(char));
    value[0] = '\0';
    unsigned int nr_closes = 0;

    while (lexer->c != '\0')
    {
        if (cond_c)
        {
            if (lexer->c == cond_c && lexer_peek(lexer, 1) == cond_c)
                nr_closes += 1;
        }

        if (cond_b)
        {
            if (lexer->c == cond_b && lexer_peek(lexer, 1) == cond_b)
                nr_closes += 1;
        }

        if (!close_i)
        {
            if (nr_closes)
                break;
        }

        if (close_i)
        {
            if (nr_closes >= close_i)
                break;
        }

        if (close_i && nr_closes == close_i-1)
        {
            if (cond_c)
            {
                if (lexer->c == cond_c && lexer_peek(lexer, 1) == cond_c)
                    continue;
            }

            if (cond_b)
            {
                if (lexer->c == cond_b && lexer_peek(lexer, 1) == cond_b)
                    continue;
            }
        }

        value = (char*) realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, charstr(lexer->c));
        lexer_advance(lexer); 
    }


    return value;
}

token_T* lexer_parse_raw(lexer_T* lexer)
{
    float close_i = 0;

    while(lexer->c == '(')
    {
        close_i += 0.5f;
        lexer_advance(lexer);
        lexer_skip_whitespace(lexer);
    }

    token_T* tok = init_token(lexer_scrape(lexer, ')', '{', (int) close_i), TOKEN_RAW);

    while (lexer->c == ')')
    {
        lexer_advance(lexer);
        lexer_skip_whitespace(lexer);
    }

    return tok;
}

token_T* lexer_parse_comp(lexer_T* lexer)
{
    float close_i = 0;

    while(lexer->c == '{')
    {
        close_i += 0.5f;
        lexer_advance(lexer);
        lexer_skip_whitespace(lexer);
    }

    token_T* tok = init_token(lexer_scrape(lexer, '}', 0, (int) close_i), TOKEN_COMP);

    while(lexer->c == '}')
    {
        lexer_advance(lexer);
        lexer_skip_whitespace(lexer);
    }

    return tok;
}

token_T* lexer_next_token_c(lexer_T* lexer)
{
    while (lexer->c != '\0' && lexer->c != '}' && lexer_peek(lexer, 1) != '}') 
    { 
       lexer_skip_whitespace(lexer);

       if (lexer->c == '(' && lexer_peek(lexer, 1) == '(')
           return lexer_parse_raw(lexer);

       if (lexer->c == '\0') break;

       if (isalpha(lexer->c) || lexer->c == '@' || lexer->c == '$') return lexer_advance_token(lexer, lexer_parse_id(lexer));

       switch (lexer->c)
       {
           case '(': return lexer_parse_left_paren(lexer); break;
           case ')': return lexer_advance_token(lexer, lexer_parse_right_paren(lexer)); break;
           case '=': return lexer_advance_token(lexer, init_token(charstr(lexer->c), TOKEN_EQUALS)); break;
           case '"': return lexer_advance_token(lexer, lexer_parse_string(lexer)); break;
           case '[': return lexer_advance_token(lexer, init_token(charstr(lexer->c), TOKEN_LBRACKET)); break;
           case ']': return lexer_advance_token(lexer, init_token(charstr(lexer->c), TOKEN_RBRACKET)); break;
           default: printf("[Lexer]: Unexpected `%c`\n", lexer->c); exit(1); break;
       } 
    }

    return init_token(charstr(lexer->c), TOKEN_EOF);
}

token_T* lexer_next_token(lexer_T* lexer)
{
    while (lexer->c != '\0') 
    {
       if (lexer->eval)
           return lexer_next_token_c(lexer);

       if (lexer->c == '(' && lexer_peek(lexer, 1) == '(')
           return lexer_parse_raw(lexer);
       else if (lexer->c == '{' && lexer_peek(lexer, 1) == '{')
           return lexer_parse_comp(lexer);
       else
           return lexer_parse_raw(lexer);
    }

    return init_token(charstr(lexer->c), TOKEN_EOF); 
}

char lexer_peek(lexer_T* lexer, unsigned int index)
{
    return lexer->i < lexer->len ? lexer->src[lexer->i + index] : '\0';
}
