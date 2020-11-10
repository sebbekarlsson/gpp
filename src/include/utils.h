#ifndef GPP_UTILS_H
#define GPP_UTILS_H
#include "AST.h"
void assert_not_nil(void* x, const char* msg);
void assert_not_reached();

char* remove_indent(char* instr, unsigned int x);
#endif
