#include <gpp/visitor.h>
#include <gpp/AST_utils.h>
#include <gpp/builtins.h>
#include <gpp/escape.h>
#include <gpp/gpp.h>
#include <gpp/io.h>
#include <gpp/lexer.h>
#include <gpp/utils.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *charstr(char c) {
  char *new_str = (char *)calloc(2, sizeof(char));
  new_str[0] = c;
  new_str[1] = '\0';

  return new_str;
}

static char *sh(char *binpath, char *source) {
  char *output = (char *)calloc(1, sizeof(char));
  output[0] = '\0';

  FILE *fp;
  char path[1035];

  char *cmd_template = "%s <<EOF\n%s\nEOF";
  char *cmd = (char *)calloc(strlen(cmd_template) + strlen(binpath) +
                                 strlen(source) + 128,
                             sizeof(char));
  sprintf(cmd, cmd_template, binpath, source);

  fp = popen(cmd, "r");

  if (fp == NULL) {
    printf("Failed to run command\n");
    exit(1);
  }

  while (fgets(path, sizeof(path), fp) != NULL) {
    output = (char *)realloc(output, (strlen(output) + strlen(path) + 1) *
                                         sizeof(char));
    strcat(output, path);
  }

  pclose(fp);

  free(cmd);

  return output;
}

visitor_T *init_visitor(AST_T *object, GPPEnv* env) {
  visitor_T *visitor = (visitor_T *)calloc(1, sizeof(struct VISITOR_STRUCT));
  visitor->object = object;
  visitor->follow_pointers = 1;
  builtins_register(visitor);
  visitor->env = env;

  return visitor;
}

AST_T *visitor_visit(visitor_T *visitor, AST_T *node, int argc, AST_T **argv) {
  assert_not_nil(node, "[Visitor.visit]: Node is nil.");

  switch (node->type) {
  case AST_ROOT:
    return visitor_visit_root(visitor, node, argc, argv);
    break;
  case AST_COMP:
    return visitor_visit_comp(visitor, node, argc, argv);
    break;
  case AST_ASSIGN:
    return visitor_visit_assign(visitor, node, argc, argv);
    break;
  case AST_STRING:
    return visitor_visit_string(visitor, node, argc, argv);
    break;
  case AST_FLOAT:
    return visitor_visit_float(visitor, node, argc, argv);
    break;
  case AST_INT:
    return visitor_visit_int(visitor, node, argc, argv);
    break;
  case AST_VAR:
    return visitor_visit_var(visitor, node, argc, argv);
    break;
  case AST_FUNCTION:
    return visitor_visit_function(visitor, node, argc, argv);
    break;
  case AST_CALL:
    return visitor_visit_call(visitor, node, argc, argv);
    break;
  case AST_GROUP:
    return visitor_visit_group(visitor, node, argc, argv);
    break;
  case AST_OBJECT:
    return visitor_visit_object(visitor, node, argc, argv);
    break;
  case AST_COMMENT:
    return visitor_visit_comment(visitor, node, argc, argv);
    break;
  default:
    printf("[Visitor]: Unhandled node of type `%d`\n", node->type);
    exit(1);
    break;
  }

  assert_not_reached();
}



AST_T *visitor_visit_root(visitor_T *visitor, AST_T *node, int argc,
                          AST_T **argv) {
  AST_T *new_root = init_ast(AST_ROOT);
  new_root->x = node->x;
  AST_T *extend_root = 0;
  unsigned int skip = node->skip;

  if (!node->skip_comments) {
    for (int i = 0; i < (int)node->root_items_size; i++) {
      AST_T *comment = node->root_items[i];

      if (comment->type != AST_COMMENT)
        continue;

      if (!ast_comment_is_meaningful(comment)) continue;

      char *comment_value = comment->comment_value;

      if (comment_value[1] == '%') {
        /**
         * Below we are doing some parsing of whatever is after
         * the "%" character.
         * This should probably be moved to a separate function.
         *
         * TODO: move this code to a separate function.
         */

        lexer_T *lexer = init_lexer(comment_value);
        lexer_advance(lexer);
        token_T *tok = 0;

        char *op = 0;
        char *val = 0;
        while ((tok = lexer_next_token_simple(lexer))->type != TOKEN_EOF) {
          if (tok->type == TOKEN_ID) {
            op = tok->value;
            tok = lexer_next_token_simple(lexer);
          }
          if (tok->type == TOKEN_ID || tok->type == TOKEN_STRING) {
            val = tok->value;
          }

          free(tok); // dont need you anymore

          if (op && val)
            break;
        }

        // free(lexer->src);
        free(lexer); // yeah, you can go to

        if (op && val) {
          if (strcmp(op, "block") == 0) {
            if (ast_object_has_var(visitor->object, val)) {
              AST_T *var_value =
                  ast_object_get_value_by_key(visitor->object, val);
              var_value->skip_comments = 1;
              free(op);
              free(val);
              return visitor_visit(visitor, var_value, argc, argv);
            }

            gpp_result_T *gpp_res = gpp_eval(node->raw_value, 1, 0, 0, visitor->env);

            AST_T *var = init_ast(AST_VAR);
            var->var_name = calloc(strlen(val) + 1, sizeof(char));
            var->var_value = gpp_res->node;
            strcpy(var->var_name, val);
            ast_object_push_var(visitor->object, var);
            free(gpp_res);
            skip = 1;
          } else if (strcmp(op, "extends") == 0) {
            gpp_result_T *gpp_res = gpp_eval(gpp_read_file(val), 1, 0, 0, visitor->env);
            extend_root = gpp_res->node;
            free(gpp_res);
          }

          free(op);
          free(val);
        }
      } else {
        if (node->raw_value && comment) {
          if (comment_value[1] == '!') {
            char *indented = remove_indent(node->raw_value, node->x);
            gpp_result_T *gpp_res = gpp_eval(indented, 0, 0, 0, visitor->env);
            free(indented);

            char *value = sh(comment->comment_value + 2, gpp_res->res);

            AST_T *ast_string = init_ast(AST_STRING);
            ast_string->string_value = calloc(strlen(value) + 1, sizeof(char));
            strcpy(ast_string->string_value, value);

            free(value);
            free(gpp_res->res);
            free(gpp_res->node);
            free(gpp_res);

            return ast_string;
          }
        }
      }
    }
  }

  if (!skip) {
    for (int i = 0; i < (int)node->root_items_size; i++) {
      AST_T *child = node->root_items[i];

      if (child->type == AST_COMMENT  && ast_comment_is_meaningful(child))
        continue;

      ast_root_push_item(new_root, visitor_visit(visitor, child, argc, argv));
    }
  }

  /**
   * If the current block is set to extend some template,
   * then we will visit that template instead of returning the
   * node.
   */
  return extend_root ? visitor_visit(visitor, extend_root, argc, argv)
                     : new_root;
}

AST_T *visitor_visit_comp(visitor_T *visitor, AST_T *node, int argc,
                          AST_T **argv) {
  AST_T *ast_group = init_ast(AST_GROUP);

  for (int i = 0; i < (int)node->group_items_size; i++)
    ast_group_push_item(
        ast_group, visitor_visit(visitor, node->group_items[i], argc, argv));

  return ast_group;
}

AST_T *visitor_visit_assign(visitor_T *visitor, AST_T *node, int argc,
                            AST_T **argv) {

  if (node->var_value) {
    node->var_value = visitor_visit(visitor, node->var_value, argc, argv);
    if (!ast_object_has_var(visitor->object, node->var_name))
      ast_object_push_var(visitor->object, node);
  }
  /*printf("[Visitor.visit_assign]: No assignments except for function "
         "assignments are implemented at the moment.\n");
  exit(1);*/

  return node;
}

AST_T *visitor_visit_string(visitor_T *visitor, AST_T *node, int argc,
                            AST_T **argv) {
  return node;
}

AST_T *visitor_visit_comment(visitor_T *visitor, AST_T *node, int argc,
                             AST_T **argv) {
  return node;
}

AST_T *visitor_visit_float(visitor_T *visitor, AST_T *node, int argc,
                           AST_T **argv) {
  return node;
}

AST_T *visitor_visit_int(visitor_T *visitor, AST_T *node, int argc,
                         AST_T **argv) {
  return node;
}

AST_T *visitor_visit_var(visitor_T *visitor, AST_T *node, int argc,
                         AST_T **argv) {
  if (node->var_value) {
    /**
     * If the value of the node is already known,
     * we don't need to perform any further operations.
     */
    return node->var_value;
  }

  AST_T *var = (void *)0;

  /**
   * The loop below acts as a guard to now perform any futher
   * operations if the "caller" has already provided information about
   * where we should look for the value of the variable.
   *
   * The main reason for this is that we might visit the current var node
   * from within a function body, and we want the arguments of the "caller"
   * to have highest priority when we do the lookup.
   */
  for (int i = 0; i < argc; i++) {
    AST_T *argument = argv[i];

    if (!argument)
      continue;

    if (!argument->var_name || !argument->var_value)
      continue;

    if (strcmp(argument->var_name, node->var_name) == 0) {
      var = visitor_visit(visitor, argument->var_value, argc, argv);
      break;
    }
  }

  AST_T *look = argc ? argv[0] : visitor->object;

  AST_T *global_value = ast_object_get_var_by_name(look, node->var_name);

  if (global_value) {
    if (global_value->var_value) {
      var = global_value->var_value
                ? visitor_visit(visitor, global_value->var_value, argc, argv)
                : visitor_visit(visitor, global_value, argc, argv);
    }
  } else {
    if (node->var_name[0] == '$') {
      /**
       * This is for accessing arguments from the "caller"
       * by index.
       * If a variable starts with a `$` dollar sign,
       * then the programmer wants to access an argument by index.
       * This is similar to how the shell & bash works.
       */
      char *newstr = charstr(node->var_name[1]);
      int index = atoi(newstr);
      free(newstr);

      if (argc >= index && argc != 0)
        var = visitor_visit(visitor, argv[index], argc, argv);
    }
  }

  if (node->right) {
    /**
     * This needs some explanation.
     * This is for doing property access.
     *
     * Such as "myobject.someproperty.someotherproperty"
     *
     * It works by recursively visiting the node to the right,
     * and passing along data from the previous node to the next one.
     *
     * The length of the arguments passed to the next node is always `1`
     * since accessing a property won't yield multiple returns.
     */
    return visitor_visit(visitor, node->right, (int)(!!var), (AST_T *[]){var});
  } else if (var) {
    return var;
  }

  printf("[Visitor.visit_var]: `%s` is not defined.\n", node->var_name);
  exit(1);
}

AST_T *visitor_visit_function(visitor_T *visitor, AST_T *node, int argc,
                              AST_T **argv) {
  if (!ast_object_has_var(visitor->object, node->var_name))
    ast_object_push_var(visitor->object, node);

  // return the function
  return node;
}

AST_T *visitor_visit_call(visitor_T *visitor, AST_T *node, int argc,
                          AST_T **argv) {
  AST_T **args = node->call_group->group_items;
  int args_size = (int)node->call_group->group_items_size;

  /**
   * Most of the "strcmp" stuff below are built-in function
   * implementations.
   * The plan is to abstract them away from here and instead use
   * function pointers and attach these function pointers to visitor->object
   * and just directly access them from there instead.
   * This would also increase the performance since we could get rid of the
   * "strcmp" stuff.
   *
   * TODO: do as described above.
   */

  if (strcmp(node->var_name, "cate") == 0) {
    if (args_size < 1)
      return node;

    AST_T *ast_group = init_ast(AST_GROUP);

    for (int i = 0; i < args_size; i++) {
      AST_T *string = (AST_T *)args[i];

      if (!string->string_value)
        continue;


      char* path = gpp_path_join(visitor->env->base_dir, string->string_value);

      char *contents = gpp_read_file(path);
      gpp_result_T *res = gpp_eval(contents, 0, 0, visitor->object, visitor->env);

      AST_T *ast_string = init_ast(AST_STRING);
      ast_string->string_value = calloc(strlen(res->res) + 1, sizeof(char));
      strcpy(ast_string->string_value, res->res);
      ast_group_push_item(ast_group, ast_string);

      free(contents);
      free(res->res);
      free(res);
      free(path);
    }

    return ast_group;
  } else if (strcmp(node->var_name, "floor") == 0) {
    if (args_size < 1)
      return node;

    AST_T *number = (AST_T *)args[0];
    AST_T *new_number = init_ast(AST_INT);
    new_number->int_value = floor(number->float_value);

    return visitor_visit(visitor, new_number, argc, argv);
  } else if (strcmp(node->var_name, "key") == 0) {
    if (args_size < 2)
      return node;

    AST_T *object = (AST_T *)visitor_visit(visitor, args[0], argc, argv);
    AST_T *ast_index = (AST_T *)args[1];

    if (object->object_vars_size >= ast_index->int_value) {
      AST_T *var = object->object_vars[ast_index->int_value];
      AST_T *string = init_ast(AST_STRING);
      string->string_value = var->var_name;

      return visitor_visit(visitor, string, argc, argv);
    }
  } else if (strcmp(node->var_name, "value") == 0) {
    if (args_size < 2)
      return node;

    AST_T *object = (AST_T *)visitor_visit(visitor, args[0], argc, argv);
    AST_T *ast_index = (AST_T *)args[1];

    if (object->object_vars_size >= ast_index->int_value) {
      AST_T *var = object->object_vars[ast_index->int_value];

      if (var->var_value)
        return visitor_visit(visitor, var->var_value, argc, argv);
    }
  } else if (strcmp(node->var_name, "get") == 0) {
    if (args_size < 2)
      return node;

    AST_T *object = (AST_T *)visitor_visit(visitor, args[0], argc, argv);
    AST_T *ast_key = (AST_T *)args[1];
    AST_T *value = 0;

    switch (object->type) {
    case AST_OBJECT:
      value = ast_object_get_value_by_key(object, ast_key->string_value);
      break;
    case AST_GROUP:
      value = ast_group_get_value_by_index(object, ((int)ast_key->float_value));
      break;
    default:
      value = 0;
      break;
    }

    /**
     * TODO: throw some error or return an AST of null if this is false.
     * I am not sure what behaviour is expected here right now.
     * But I think it would be preferable is the programmer was notified
     * that the value she is trying to access doesn't exist.
     */
    if (value)
      return visitor_visit(visitor, value, argc, argv);
  } else if (strcmp(node->var_name, "keys") == 0) {
    if (!args_size)
      return node;

    AST_T *object = (AST_T *)visitor_visit(visitor, args[0], argc, argv);
    AST_T *value = 0;

    if (object->type == AST_OBJECT)
      value = ast_object_get_keys(object);

    if (value)
      return visitor_visit(visitor, value, argc, argv);
  } else if (strcmp(node->var_name, "escapehtml") == 0) {
    if (args_size < 1)
      return node;

    AST_T *a = (AST_T *)visitor_visit(visitor, args[0], argc, argv);
    char *v = ast_to_string(a);
    AST_T *new_string = init_ast(AST_STRING);
    new_string->string_value = escape_html(v);
    free(v);

    return visitor_visit(visitor, new_string, argc, argv);
  }

  if (!ast_object_has_var(visitor->object, node->var_name)) {
    printf("[Visitor]: `%s` is not defined.\n", node->var_name);
    exit(1);
  }

  /**
   * Look and see if a definition exists for the encountered name.
   *
   * The programmer might have defined a function that we are not
   * yet aware of.
   */
  AST_T *var = ast_object_get_var_by_name(visitor->object, node->var_name);

  if (var) {
    /**
     * Okay, we found a definition.
     * Let's try and perform a function call using the definition we found.
     */

    if (var->fptr) {
      return var->fptr(visitor, node, args_size, args, argc, argv);
    }

    AST_T **new_args = (void *)0;
    size_t new_args_size = 0;

    for (int i = 0; i < (int)var->function_args->group_items_size; i++) {
      /**
       * Inside this loop, we are mapping function signature arguments
       * to the actual "caller" arguments.
       * We are doing this by generating a new list of variable definitions
       * and passing them to the scope of the function body.
       */
      AST_T *new_var = init_ast(AST_VAR);
      new_var->var_value = args[i];
      new_var->var_name =
          calloc(strlen(var->function_args->group_items[i]->var_name) + 1,
                 sizeof(char));
      strcpy(new_var->var_name, var->function_args->group_items[i]->var_name);

      new_args_size += 1;

      if (new_args == (void *)0) {
        new_args = calloc(1, sizeof(struct AST_STRUCT *));
        new_args[0] = new_var;
      } else {
        new_args =
            realloc(new_args, sizeof(struct AST_STRUCT *) * new_args_size);
        new_args[new_args_size - 1] = new_var;
      }
    }

    /**
     * Finally, we are ready to visit the function body.
     * While visiting it, we are also passing along the "caller" arguments
     * that we previously mapped.
     *
     * If no arguments were recieved from the "caller",
     * we will just pass along the arguments of the parent to the "caller".
     * This makes parent arguments available to the function body instead.
     * It is possible that we might want to concatinate these two sets of
     * arguments in the future.... I am just to lazy to do it now.
     */
    return visitor_visit(visitor, var->function_body,
                         new_args_size ? new_args_size : argc,
                         new_args_size ? new_args : argv);
  }

  // NOTE: I am not sure we will ever reach here.
  return node;
}

AST_T *visitor_visit_group(visitor_T *visitor, AST_T *node, int argc,
                           AST_T **argv) {
  AST_T *ast_group = init_ast(AST_GROUP);

  for (int i = 0; i < node->group_items_size; i++)
    ast_group_push_item(
        ast_group, visitor_visit(visitor, node->group_items[i], argc, argv));

  return ast_group;
}

AST_T *visitor_visit_object(visitor_T *visitor, AST_T *node, int argc,
                            AST_T **argv) {
  return node;
}
