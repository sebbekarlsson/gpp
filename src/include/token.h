#ifndef GPP_TOKEN_H
#define GPP_TOKEN_H
typedef struct TOKEN_STRUCT {
    char* value;
    enum
    {
        TOKEN_EOF,
        TOKEN_RAW,
        TOKEN_RAW_BEGIN,
        TOKEN_RAW_END,
        TOKEN_COMP_BEGIN,
        TOKEN_COMP_END,
        TOKEN_COMMA,
        TOKEN_LPAREN,
        TOKEN_RPAREN,
        TOKEN_LBRACKET,
        TOKEN_RBRACKET,
        TOKEN_ID,
        TOKEN_EQUALS,
        TOKEN_STRING,
        TOKEN_NUMBER,
        TOKEN_COMMENT,
        TOKEN_DOT,
        TOKEN_ARROW_RIGHT,
    } type;
    unsigned int x;
    unsigned int y;
} token_T;
#endif

token_T* init_token(char* value, int type);

const char* token_to_str(token_T* token);
