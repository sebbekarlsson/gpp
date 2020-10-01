#ifndef GPP_BUILTINS_H
#define GPP_BUILTINS_H
#include "visitor.h"
#include "AST.h"

void builtins_register(visitor_T* visitor);
AST_T* builtin_fptr_map(visitor_T* visitor, AST_T* node, int argc, AST_T** argv, int caller_argc, AST_T** caller_argv);
AST_T* builtin_fptr_cat(visitor_T* visitor, AST_T* node, int argc, AST_T** argv, int caller_argc, AST_T** caller_argv);
#endif
