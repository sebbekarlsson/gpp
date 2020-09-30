#include "include/token.h"
#include <stdlib.h>
#include <string.h>


token_T* init_token(char* value, int type)
{
    token_T* token = (token_T*) calloc(1, sizeof(struct TOKEN_STRUCT));
    token->value = (char*) calloc(strlen(value) + 1, sizeof(char));
    strcpy(token->value, value);
    token->type = type;
    token->x = 0;
    token->y = 0;

    return token;
}

const char* token_to_str(token_T* token)
{
    switch (token->type)
    {
        case TOKEN_EOF: return "TOKEN_EOF"; break;
        case TOKEN_COMMA: return "TOKEN_COMMA"; break;
        case TOKEN_LPAREN: return "TOKEN_LPAREN"; break;
        case TOKEN_RPAREN: return "TOKEN_RPAREN"; break;
        case TOKEN_EQUALS: return "TOKEN_EQUALS"; break;
        case TOKEN_ID: return "TOKEN_ID"; break;
        case TOKEN_STRING: return "TOKEN_STRING"; break;
        case TOKEN_NUMBER: return "TOKEN_NUMBER"; break;
        case TOKEN_COMMENT: return "TOKEN_COMMENT"; break;
        case TOKEN_RAW_BEGIN: return "TOKEN_RAW_BEGIN"; break;
        case TOKEN_COMP_BEGIN: return "TOKEN_COMP_BEGIN"; break;
        case TOKEN_RAW: return "TOKEN_RAW"; break;
        case TOKEN_RAW_END: return "TOKEN_RAW_END"; break;
        case TOKEN_COMP_END: return "TOKEN_COMP_END"; break;
        case TOKEN_LBRACKET: return "TOKEN_LBRACKET"; break;
        case TOKEN_RBRACKET: return "TOKEN_RBRACKET"; break;
        default: return "TOKEN_NOT_STRINGABLE"; break;
    }
}
