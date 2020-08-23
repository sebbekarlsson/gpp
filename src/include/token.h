#ifndef GPP_TOKEN_H
#define GPP_TOKEN_H
typedef struct TOKEN_STRUCT {
    char* value;
    enum
    {
        TOKEN_EOF,
        TOKEN_COMP,
        TOKEN_RAW,
        TOKEN_COMMA,
        TOKEN_LPAREN,
        TOKEN_RPAREN,
        TOKEN_LBRACKET,
        TOKEN_RBRACKET,
        TOKEN_ID,
        TOKEN_EQUALS,
        TOKEN_STRING,
    } type;
} token_T;
#endif

token_T* init_token(char* value, int type);

const char* token_to_str(token_T* token);
