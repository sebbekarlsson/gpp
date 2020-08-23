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

lexer_T* init_lexer(char* src)
{
    lexer_T* lexer = (lexer_T*) calloc(1, sizeof(struct LEXER_STRUCT));
    lexer->src = (char*) calloc(strlen(src) + 1, sizeof(char));
    strcpy(lexer->src, src);
    lexer->i = 0;
    lexer->c = lexer->src[lexer->i];
    lexer->len = strlen(lexer->src);

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
    while ((lexer->c == ' ' || lexer->c == 10 || lexer->c == '\n') && (lexer->i < lexer->len))
        lexer_advance(lexer);
}

token_T* lexer_parse_left_paren(lexer_T* lexer) {
    lexer_advance(lexer);

    return init_token(charstr(lexer->c), TOKEN_LPAREN);
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

token_T* lexer_parse_any(lexer_T* lexer)
{
    lexer_advance(lexer);

    char* value = (char*) calloc(1, sizeof(char));
    value[0] = '\0';

    do 
    {
        value = (char*) realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, charstr(lexer->c));
        lexer_advance(lexer);
    } while (lexer->c != '\0' && !(lexer->c == '{' && lexer_peek(lexer, 1) == '{'));

    return init_token(value, TOKEN_STRING);
}

token_T* lexer_parse_raw_begin(lexer_T* lexer)
{
    lexer_advance(lexer);
    lexer_advance(lexer);

    return init_token(charstr(lexer->c), TOKEN_RAW_BEGIN);
}

token_T* lexer_parse_raw_end(lexer_T* lexer)
{
    lexer_advance(lexer);
    lexer_advance(lexer);

    return init_token(charstr(lexer->c), TOKEN_RAW_END);
}

token_T* lexer_parse_comp_begin(lexer_T* lexer)
{
    lexer_advance(lexer);
    lexer_advance(lexer);

    return init_token(charstr(lexer->c), TOKEN_COMP_BEGIN);
}

token_T* lexer_parse_comp_end(lexer_T* lexer)
{
    lexer_advance(lexer);
    lexer_advance(lexer);

    return init_token(charstr(lexer->c), TOKEN_COMP_END);
}

token_T* lexer_next_token(lexer_T* lexer)
{
    while (lexer->c != '\0') 
    {
       lexer_skip_whitespace(lexer);

       if (lexer->c == '(' && lexer_peek(lexer, 1) == '(')
           return lexer_parse_raw_begin(lexer);
       else if (lexer->c == '{' && lexer_peek(lexer, 1) == '{')
           return lexer_parse_comp_begin(lexer);

       if (lexer->c == ')' && lexer_peek(lexer, 1) == ')')
           return lexer_parse_raw_end(lexer);
       else if (lexer->c == '}' && lexer_peek(lexer, 1) == '}')
           return lexer_parse_comp_end(lexer);

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
           default: return lexer_parse_any(lexer); break;
       } 
    }

    return init_token(charstr(lexer->c), TOKEN_EOF);
}

char lexer_peek(lexer_T* lexer, unsigned int index)
{
    return lexer->i < lexer->len ? lexer->src[lexer->i + index] : '\0';
}
