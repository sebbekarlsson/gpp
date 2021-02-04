#include "include/json.h"
#include <stdio.h>

AST_T *json_load(char *source) {
  json_parser_T *parser = init_json_parser(init_json_lexer(source));

  // parse the file
  json_ast_T *ast = json_parser_parse(parser);

  return json_visit(ast);

  // don't forget to free some memory.
  // json_parser_free(parser);
  // json_ast_free(ast);
}

AST_T *json_visit(json_ast_T *jnode) {
  switch (jnode->type) {
  case JSON_AST_KEY_VALUE_LIST:
    return json_visit_key_value_list(jnode);
    break;
  case JSON_AST_KEY_VALUE:
    return json_visit_key_value(jnode);
    break;
  case JSON_AST_STRING:
    return json_visit_string(jnode);
    break;
  case JSON_AST_INTEGER:
    return json_visit_integer(jnode);
    break;
  case JSON_AST_FLOAT:
    return json_visit_float(jnode);
    break;
  case JSON_AST_LIST:
    return json_visit_list(jnode);
    break;
  default: {
    printf("[json_visit]: Unhandled node.\n");
    exit(1);
  } break;
  }
}

AST_T *json_visit_key_value_list(json_ast_T *jnode) {
  AST_T *ast = init_ast(AST_OBJECT);

  for (int i = 0; i < jnode->key_value_list_size; i++) {
    json_ast_T *jitem = (json_ast_T *)jnode->key_value_list_value[i];
    AST_T *item = json_visit(jitem);

    ast->object_vars_size += 1;

    if (ast->object_vars == (void *)0) {
      ast->object_vars = calloc(1, sizeof(struct AST_STRUCT *));
      ast->object_vars[0] = item;
    } else {
      ast->object_vars = realloc(ast->object_vars, sizeof(struct AST_STRUCT *) *
                                                       ast->object_vars_size);
      ast->object_vars[ast->object_vars_size - 1] = item;
    }
  }

  return ast;
}

AST_T *json_visit_key_value(json_ast_T *jnode) {
  AST_T *ast = init_ast(AST_VAR);
  ast->var_name = jnode->key_value_key;
  ast->var_value = json_visit(jnode->key_value_value);

  return ast;
}

AST_T *json_visit_string(json_ast_T *jnode) {
  AST_T *ast = init_ast(AST_STRING);
  ast->string_value = jnode->string_value;

  return ast;
}

AST_T *json_visit_integer(json_ast_T *jnode) {
  AST_T *ast = init_ast(AST_INT);
  ast->int_value = jnode->integer_value;
  return ast;
}

AST_T *json_visit_float(json_ast_T *jnode) {
  AST_T *ast = init_ast(AST_FLOAT);
  ast->float_value = jnode->float_value;
  return ast;
}

AST_T *json_visit_list(json_ast_T *jnode) {
  AST_T *ast = init_ast(AST_GROUP);

  for (int i = 0; i < jnode->list_size; i++) {
    AST_T *item = json_visit(jnode->list_value[i]);

    ast->group_items_size += 1;

    if (ast->group_items == (void *)0) {
      ast->group_items = calloc(1, sizeof(struct AST_STRUCT *));
      ast->group_items[0] = item;
    } else {
      ast->group_items = realloc(ast->group_items, sizeof(struct AST_STRUCT *) *
                                                       ast->group_items_size);
      ast->group_items[ast->group_items_size - 1] = item;
    }
  }

  return ast;
}
