#ifndef VISITOR_H
#define VISITOR_H
#include "AST.h"
#include <gpp/env.h>

typedef struct VISITOR_STRUCT {
  AST_T *object;
  unsigned int follow_pointers;
  GPPEnv* env;
  GPPEnv* global_env;
} visitor_T;

visitor_T *init_visitor(AST_T *object, GPPEnv* env, GPPEnv* global_env);

AST_T *visitor_visit(visitor_T *visitor, AST_T *node, int argc, AST_T **argv);

AST_T *visitor_visit_root(visitor_T *visitor, AST_T *node, int argc,
                          AST_T **argv);

AST_T *visitor_visit_raw(visitor_T *visitor, AST_T *node, int argc,
                         AST_T **argv);

AST_T *visitor_visit_comp(visitor_T *visitor, AST_T *node, int argc,
                          AST_T **argv);

AST_T *visitor_visit_assign(visitor_T *visitor, AST_T *node, int argc,
                            AST_T **argv);

AST_T *visitor_visit_string(visitor_T *visitor, AST_T *node, int argc,
                            AST_T **argv);

AST_T *visitor_visit_comment(visitor_T *visitor, AST_T *node, int argc,
                             AST_T **argv);

AST_T *visitor_visit_float(visitor_T *visitor, AST_T *node, int argc,
                           AST_T **argv);

AST_T *visitor_visit_int(visitor_T *visitor, AST_T *node, int argc,
                         AST_T **argv);

AST_T *visitor_visit_var(visitor_T *visitor, AST_T *node, int argc,
                         AST_T **argv);

AST_T *visitor_visit_function(visitor_T *visitor, AST_T *node, int argc,
                              AST_T **argv);

AST_T *visitor_visit_call(visitor_T *visitor, AST_T *node, int argc,
                          AST_T **argv);

AST_T *visitor_visit_group(visitor_T *visitor, AST_T *node, int argc,
                           AST_T **argv);

AST_T *visitor_visit_object(visitor_T *visitor, AST_T *node, int argc,
                            AST_T **argv);
#endif
