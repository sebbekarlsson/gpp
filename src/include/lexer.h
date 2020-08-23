#ifndef GPP_LEXER_H
#define GPP_LEXER_H
#include "token.h"
typedef struct LEXER_STRUCT {
    unsigned int i;
    unsigned int len;
    char c;
    char* src;
} lexer_T;

lexer_T* init_lexer(char* src);

void lexer_advance(lexer_T* lexer);

token_T* lexer_advance_token(lexer_T* lexer, token_T* token);

void lexer_skip_whitespace(lexer_T* lexer);

token_T* lexer_parse_left_brace(lexer_T* lexer);
token_T* lexer_parse_right_brace(lexer_T* lexer);
token_T* lexer_parse_left_paren(lexer_T* lexer);
token_T* lexer_parse_right_paren(lexer_T* lexer);

token_T* lexer_parse_id(lexer_T* lexer);

token_T* lexer_parse_string(lexer_T* lexer);

token_T* lexer_next_token(lexer_T* lexer);
#endif
