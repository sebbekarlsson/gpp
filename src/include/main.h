#ifndef GPP_MAIN_H
#define GPP_MAIN_H
#include "AST.h"
typedef struct GPP_RESULT_STRUCT
{
    char* res;
    AST_T* node;
} gpp_result_T;

gpp_result_T* init_gpp_result(char* res, AST_T* node);
gpp_result_T* gpp_eval(char* source, unsigned int eval);
#endif
