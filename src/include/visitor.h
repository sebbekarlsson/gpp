#ifndef VISITOR_H
#define VISITOR_H
#include "AST.h"

typedef struct VISITOR_STRUCT
{
    char* buffer;
} visitor_T;

visitor_T* init_visitor();

void visitor_buffer(visitor_T* visitor, char* buffstr);

AST_T* visitor_visit(visitor_T* visitor, AST_T* node, int argc, AST_T** args);

AST_T* visitor_visit_root(visitor_T* visitor, AST_T* node, int argc, AST_T** args);

AST_T* visitor_visit_raw(visitor_T* visitor, AST_T* node, int argc, AST_T** args);

AST_T* visitor_visit_comp(visitor_T* visitor, AST_T* node, int argc, AST_T** args);

AST_T* visitor_visit_assign(visitor_T* visitor, AST_T* node, int argc, AST_T** args);

AST_T* visitor_visit_string(visitor_T* visitor, AST_T* node, int argc, AST_T** args);

AST_T* visitor_visit_var(visitor_T* visitor, AST_T* node, int argc, AST_T** args);

AST_T* visitor_visit_call(visitor_T* visitor, AST_T* node, int argc, AST_T** args);

AST_T* visitor_visit_group(visitor_T* visitor, AST_T* node, int argc, AST_T** args);
#endif
