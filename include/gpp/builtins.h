#ifndef GPP_BUILTINS_H
#define GPP_BUILTINS_H
#include "AST.h"
#include "visitor.h"

void builtins_register(visitor_T *visitor);
AST_T *builtin_fptr_map(visitor_T *visitor, AST_T *node, int argc, AST_T **argv,
                        int caller_argc, AST_T **caller_argv);

AST_T *builtin_fptr_sort(visitor_T *visitor, AST_T *node, int argc, AST_T **argv,
                        int caller_argc, AST_T **caller_argv);

AST_T *builtin_fptr_cat(visitor_T *visitor, AST_T *node, int argc, AST_T **argv,
                        int caller_argc, AST_T **caller_argv);
AST_T *builtin_fptr_join(visitor_T *visitor, AST_T *node, int argc,
                         AST_T **argv, int caller_argc, AST_T **caller_argv);
AST_T *builtin_fptr_load(visitor_T *visitor, AST_T *node, int argc,
                         AST_T **argv, int caller_argc, AST_T **caller_argv);

AST_T *builtin_fptr_newline(visitor_T *visitor, AST_T *node, int argc,
                            AST_T **argv, int caller_argc, AST_T **caller_argv);

AST_T *builtin_fptr_range(visitor_T *visitor, AST_T *node, int argc,
                            AST_T **argv, int caller_argc, AST_T **caller_argv);
#endif
