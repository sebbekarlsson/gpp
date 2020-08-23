#include "include/token.h"
#include <stdlib.h>
#include <string.h>


token_T* init_token(char* value, int type)
{
    token_T* token = (token_T*) calloc(1, sizeof(struct TOKEN_STRUCT));
    token->value = (char*) calloc(strlen(value) + 1, sizeof(char));
    strcpy(token->value, value);
    token->type = type;

    return token;
}

const char* token_to_str(token_T* token)
{
    switch (token->type)
    {
        case TOKEN_EOF: return "TOKEN_EOF"; break;
        case TOKEN_TEMPLATE_BEGIN: return "TOKEN_TEMPLATE_BEGIN"; break;
        case TOKEN_TEMPLATE_END: return "TOKEN_TEMPLATE_END"; break;
        case TOKEN_IGNORE_BEGIN: return "TOKEN_IGNORE_BEGIN"; break;
        case TOKEN_IGNORE_END: return "TOKEN_IGNORE_END"; break;
        case TOKEN_COMMA: return "TOKEN_COMMA"; break;
        case TOKEN_LPAREN: return "TOKEN_LPAREN"; break;
        case TOKEN_RPAREN: return "TOKEN_RPAREN"; break;
        case TOKEN_EQUALS: return "TOKEN_EQUALS"; break;
        case TOKEN_ID: return "TOKEN_ID"; break;
        case TOKEN_STRING: return "TOKEN_STRING"; break;
        default: return "TOKEN_NOT_STRINGABLE"; break;
    }
}
