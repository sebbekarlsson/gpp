#ifndef GPP_LEXER_H
#define GPP_LEXER_H
#include "token.h"

#define VFB0 '('
#define VFB1 '@'

#define VFE0 '@'
#define VFE1 ')'

typedef struct LEXER_STRUCT {
    unsigned int i;
    unsigned int len;
    char c;
    char* src;
    unsigned int raw;
    unsigned int comp;
    unsigned int raws;
    unsigned int x;
    unsigned int y;
} lexer_T;

lexer_T* init_lexer(char* src);

void lexer_advance(lexer_T* lexer);

token_T* lexer_advance_token(lexer_T* lexer, token_T* token);

void lexer_skip_whitespace(lexer_T* lexer);

token_T* lexer_parse_left_paren(lexer_T* lexer);
token_T* lexer_parse_right_paren(lexer_T* lexer);

token_T* lexer_parse_id(lexer_T* lexer);

token_T* lexer_parse_string(lexer_T* lexer);
token_T* lexer_parse_number(lexer_T* lexer);
token_T* lexer_parse_comment(lexer_T* lexer);

token_T* lexer_parse_arrow_right(lexer_T* lexer);

token_T* lexer_parse_comp(lexer_T* lexer);
token_T* lexer_parse_raw(lexer_T* lexer, unsigned int all);

token_T* lexer_next_token_simple(lexer_T* lexer);
token_T* lexer_next_token(lexer_T* lexer);

char lexer_peek(lexer_T* lexer, unsigned int index);
#endif
