#include "AST.h"
#include <json/json_parser.h>

AST_T *json_load(char *source);
AST_T *json_visit(json_ast_T *jnode);
AST_T *json_visit_key_value_list(json_ast_T *jnode);
AST_T *json_visit_key_value(json_ast_T *jnode);
AST_T *json_visit_string(json_ast_T *jnode);
AST_T *json_visit_integer(json_ast_T *jnode);
AST_T *json_visit_float(json_ast_T *jnode);
AST_T *json_visit_list(json_ast_T *jnode);
AST_T *json_visit_id(json_ast_T *jnode);
