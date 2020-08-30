#include "include/visitor.h"
#include "include/main.h"
#include "include/io.h"
#include "include/utils.h"
#include "include/AST_utils.h"
#include "include/lexer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

static char* charstr(char c)
{
    char* new_str = (char*) calloc(2, sizeof(char));
    new_str[0] = c;
    new_str[1] = '\0';

    return new_str;
}

static char* sh(char* binpath, char* source)
{
  char* output = (char*) calloc(1, sizeof(char));
  output[0] = '\0';

  FILE *fp;
  char path[1035];

  char* cmd_template = "%s <<EOF\n%s\nEOF";
  char* cmd = (char*) calloc(strlen(cmd_template) + strlen(binpath) + strlen(source) + 128, sizeof(char));
  sprintf(cmd, cmd_template, binpath, source);

  fp = popen(cmd, "r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }

  while (fgets(path, sizeof(path), fp) != NULL) {
    output = (char*) realloc(output, (strlen(output) + strlen(path) + 1) * sizeof(char));
    strcat(output, path);
  }

  pclose(fp);

  free(cmd);

  return output;
}

static AST_T* obj_var_lookup(AST_T* object, char* var_name)
{
    if (object == (void*)0) return (void*)0;

    for (int i = 0; i < (int) object->object_vars_size; i++)
    {
        AST_T* var = object->object_vars[i];


        if (!var)
            continue;

        if (!var->var_name || !var_name)
            continue;

        if (strcmp(var->var_name, var_name) == 0)
            return var->var_value;
    }

    return (void*)0;
}

visitor_T* init_visitor(AST_T* object)
{
    visitor_T* visitor = (visitor_T*) calloc(1, sizeof(struct VISITOR_STRUCT));
    visitor->buffer = (void*)0;
    visitor->object = object;
    visitor->follow_pointers = 1;

    return visitor;
}

void visitor_buffer(visitor_T* visitor, char* buffstr)
{
    if (visitor->buffer == (void*) 0)
    {
        visitor->buffer = calloc(strlen(buffstr) + 1, sizeof(char));
        strcpy(visitor->buffer, buffstr);
    }
    else
    {
        visitor->buffer = realloc(visitor->buffer, (strlen(visitor->buffer) + strlen(buffstr) + 2) * sizeof(char));
        strcat(visitor->buffer, buffstr);
    }
}

AST_T* visitor_visit_ptr(visitor_T* visitor, AST_T* node, int argc, AST_T** argv)
{
    AST_T* left = node->left;

    if (left->type == AST_OBJECT)
    {
        AST_T* var = obj_var_lookup(left, node->var_name);


        if (var) return var;
    }

    return visitor_visit(visitor, left, argc, argv);

}

AST_T* visitor_visit(visitor_T* visitor, AST_T* node, int argc, AST_T** argv)
{
    assert_not_nil(node, "[Visitor.visit]: Node is nil.");

    switch (node->type)
    {
        case AST_ROOT: return visitor_visit_root(visitor, node, argc, argv); break;
        case AST_RAW: return visitor_visit_raw(visitor, node, argc, argv); break;
        case AST_COMP: return visitor_visit_comp(visitor, node, argc, argv); break;
        case AST_ASSIGN: return visitor_visit_assign(visitor, node, argc, argv); break;
        case AST_STRING: return visitor_visit_string(visitor, node, argc, argv); break;
        case AST_FLOAT: return visitor_visit_float(visitor, node, argc, argv); break;
        case AST_INT: return visitor_visit_int(visitor, node, argc, argv); break;
        case AST_VAR: return visitor_visit_var(visitor, node, argc, argv); break;
        case AST_CALL: return visitor_visit_call(visitor, node, argc, argv); break;
        case AST_GROUP: return visitor_visit_group(visitor, node, argc, argv); break;
        case AST_OBJECT: return visitor_visit_object(visitor, node, argc, argv); break;
        case AST_COMMENT: return visitor_visit_comment(visitor, node, argc, argv); break;
        default: printf("[Visitor]: Unhandled node of type `%d`\n", node->type); exit(1); break;
    }
    return init_ast(AST_NOOP);
}

AST_T* visitor_visit_root(visitor_T* visitor, AST_T* node, int argc, AST_T** argv)
{
    AST_T* extend_root = 0;
    unsigned int skip = node->skip;

    if (!node->skip_comments)
    {
        for (int i = 0; i < (int)node->root_items_size; i++)
        {
            AST_T* comment = node->root_items[i];

            if (comment->type != AST_COMMENT)
                continue;

            char* comment_value = comment->comment_value;

            if (comment_value[0] == '%')
            {
                lexer_T* lexer = init_lexer(comment_value);
                token_T* tok = 0;

                char* op = 0;
                char* val = 0;
                while ((tok = lexer_next_token_simple(lexer))->type != TOKEN_EOF)
                {
                    if (tok->type == TOKEN_ID)
                    {
                        op = tok->value;
                        tok = lexer_next_token_simple(lexer);
                    }
                    if (tok->type == TOKEN_ID || tok->type == TOKEN_STRING)
                    {
                        val = tok->value;
                    }

                    if (op && val)
                        break;
                }

                if (op && val)
                {
                    if (strcmp(op, "block") == 0)
                    {
                        if (ast_object_has_var(visitor->object, val))
                        {
                            AST_T* var_value = ast_object_get_value_by_key(visitor->object, val);
                            var_value->skip_comments = 1;
                            return visitor_visit(visitor, var_value, argc, argv);
                        }

                        gpp_result_T* gpp_res = gpp_eval(node->raw_value, 1, 0);
                    
                        AST_T* var = init_ast(AST_VAR);
                        var->var_name = calloc(strlen(val) + 1, sizeof(char));
                        var->var_value = gpp_res->node;
                        strcpy(var->var_name, val);
                        ast_object_push_var(visitor->object, var);
                        free(gpp_res);

                        skip = 1;
                    }
                    else
                    if (strcmp(op, "extends") == 0)
                    {
                        gpp_result_T* gpp_res = gpp_eval(gpp_read_file(val), 1, 0);
                        extend_root = gpp_res->node;
                        free(gpp_res);
                    }
                }
            }
            else
            {
                if (node->raw_value && comment)
                {
                    gpp_result_T* gpp_res = gpp_eval(node->raw_value, 0, 0);

                    char* value = sh(comment->comment_value, gpp_res->res);
                    visitor_buffer(visitor, value);
                    free(value);
                    free(gpp_res->res);
                    free(gpp_res->node);
                    free(gpp_res);

                    return node;
                }
            } 
        }
    } 

    if (!skip)
    {
        for (int i = 0; i < (int)node->root_items_size; i++)
        {
            AST_T* child = node->root_items[i];

            if (child->type == AST_COMMENT)
                continue;
            
            visitor_visit(visitor, child, argc, argv);
        }
    }

    if (extend_root)
        return visitor_visit(visitor, extend_root, argc, argv);

    return node;
}

AST_T* visitor_visit_raw(visitor_T* visitor, AST_T* node, int argc, AST_T** argv)
{
    assert_not_reached();

    if (node->result)
    {
        if (node->raw_child)
        {
            if (node->raw_child->type == AST_ROOT)
            {
                AST_T* comment = (void*)0;

                if (node->raw_child->root_items_size > 0)
                {
                    AST_T* first = node->raw_child->root_items[0];

                    comment = first->type == AST_COMMENT ? first : (void*)0;
                }

                if (comment)
                {
                    char* comment_value = comment->comment_value;
                    char* value = sh(comment_value, node->result);
                    visitor_buffer(visitor, value);
                    free(value);
                    return node;
                }
            }
        }

        //visitor_buffer(visitor, node->result);
        return visitor_visit(visitor, node->raw_child, argc, argv);
    }
    else
    {
        if (node->raw_child)
        {
             if (node->raw_child->type == AST_ROOT)
            {
                AST_T* comment = (void*)0;

                if (node->raw_child->root_items_size > 0)
                {
                    AST_T* first = node->raw_child->root_items[0];

                    comment = first->type == AST_COMMENT ? first : (void*)0;
                }

                if (comment)
                {
                    char* comment_value = comment->comment_value;

                    gpp_result_T* gpp_res = gpp_eval(node->raw_value, 0, 0);
                    char* value = sh(comment_value, gpp_res->res);
                    visitor_buffer(visitor, value);
                    free(value);
                    free(gpp_res->res);
                    free(gpp_res->node);
                    free(gpp_res);
                    return node;
                }
            }

            return visitor_visit(visitor, node->raw_child, argc, argv);
        }

        visitor_buffer(visitor, node->raw_value);
    }

    return node;
}

AST_T* visitor_visit_comp(visitor_T* visitor, AST_T* node, int argc, AST_T** argv)
{
    for (int i = 0; i < (int)node->group_items_size; i++)
    {
        visitor_visit(visitor, node->group_items[i], argc, argv);
    }

    return node;
}

AST_T* visitor_visit_assign(visitor_T* visitor, AST_T* node, int argc, AST_T** argv)
{
    if (node->parent == (void*)0)
    {
        printf("[Visitor.visit_assign]: Node has no parent.\n");
        exit(1);
    }

    if (strcmp(node->var_name, "@") == 0)
    {
        // TODO: fix
    }

    return node;
}

AST_T* visitor_visit_string(visitor_T* visitor, AST_T* node, int argc, AST_T** argv)
{
    visitor_buffer(visitor, node->string_value);

    return node;
}

AST_T* visitor_visit_comment(visitor_T* visitor, AST_T* node, int argc, AST_T** argv)
{
    return node;
}

AST_T* visitor_visit_float(visitor_T* visitor, AST_T* node, int argc, AST_T** argv)
{
    const char* template = "%12.6f";
    char* string_value = calloc(strlen(template) + 128, sizeof(char));
    sprintf(string_value, template, node->float_value);
    visitor_buffer(visitor, string_value);
    free(string_value);

    return node;
}

AST_T* visitor_visit_int(visitor_T* visitor, AST_T* node, int argc, AST_T** argv)
{
    const char* template = "%d";
    char* string_value = calloc(strlen(template) + 128, sizeof(char));
    sprintf(string_value, template, node->int_value);
    visitor_buffer(visitor, string_value);
    free(string_value);

    return node;
}

AST_T* step_left_lookup (visitor_T* visitor, AST_T* node, char* varname, int argc, AST_T** argv)
{
    AST_T* var = obj_var_lookup(node, varname);
    if (var) return var;

    if (node->left)
        return step_left_lookup(visitor, visitor_visit(visitor, node->left, argc, argv), varname, argc, argv);

    return node;
}

AST_T* visitor_visit_var(visitor_T* visitor, AST_T* node, int argc, AST_T** argv)
{
    if (node->var_value)
        return node->var_value; 

    AST_T* look = visitor->object;

    if (argc)
    {
        look = argv[0];
    }

    AST_T* var = (void*)0;

    AST_T* global_value = obj_var_lookup(look, node->var_name);

    if (global_value)
    {
        if (global_value->var_value)
            var = visitor_visit(visitor, global_value->var_value, argc, argv);
        else
            var = visitor_visit(visitor, global_value, argc, argv);
    }
    else
    {
        if (node->var_name[0] == '$')
        {
            char* newstr = charstr(node->var_name[1]);
            int index = atoi(newstr);
            free(newstr);

            if (argc >= index && argc != 0)
               var = visitor_visit(visitor, argv[index], argc, argv);
        }
    }
    
    if (node->right)
    {
        return visitor_visit(visitor, node->right, 1, (AST_T*[]) { var });
    }
    else
    {
        if (var)
        {
            return var;
        }
    }

    printf("[Visitor.visit_var]: `%s` is not defined.\n", node->var_name);
    exit(1);
}

AST_T* visitor_visit_call(visitor_T* visitor, AST_T* node, int argc, AST_T** argv)
{
    AST_T** args = node->call_group->group_items;
    int args_size = (int) node->call_group->group_items_size;

    // TODO: abstract these away from the visitor.

    if (strcmp(node->var_name, "map") == 0)
    {
        if (args_size < 2)
            return node;


        int iterable_type = ((AST_T*)args[0])->type;
        AST_T* iterable = (iterable_type == AST_VAR || iterable_type == AST_CALL) ? visitor_visit(visitor, args[0], argc, argv) : ((AST_T*)args[0]);
        AST_T* mapping = (AST_T*) args[1];

        for (int i = 0; i < (int)iterable->group_items_size; i++)
        {
            AST_T* item = iterable->group_items[i];

            visitor_visit(visitor, mapping, 1, (AST_T*[]){ item });
        }
    }
    else
    if (strcmp(node->var_name, "cat") == 0)
    {
        if (args_size < 1)
            return node;

        for (int i = 0; i < args_size; i++)
        {
            AST_T* string = (AST_T*) args[i];

            if (!string->string_value)
                continue;

            char* contents = gpp_read_file(string->string_value);
            visitor_buffer(visitor, contents);
            free(contents);
        }
    }
    else
    if (strcmp(node->var_name, "cate") == 0)
    {
        if (args_size < 1)
            return node;

        for (int i = 0; i < args_size; i++)
        {
            AST_T* string = (AST_T*) args[i];

            if (!string->string_value)
                continue;

            char* contents = gpp_read_file(string->string_value);
            gpp_result_T* res = gpp_eval(contents, 0, 0);
            visitor_buffer(visitor, res->res);
            free(contents);
            free(res->res);
            free(res);
        }
    }
    else
    if (strcmp(node->var_name, "floor") == 0)
    {
        if (args_size < 1)
            return node;

        AST_T* number = (AST_T*) args[0];
        AST_T* new_number = init_ast(AST_INT);
        new_number->int_value = floor(number->float_value);

        return visitor_visit(visitor, new_number, argc, argv);
    }
    else
    if (strcmp(node->var_name, "key") == 0)
    {
        if (args_size < 2)
            return node;

        AST_T* object = (AST_T*) visitor_visit(visitor, args[0], argc, argv);
        AST_T* ast_index = (AST_T*) args[1];

        if (object->object_vars_size >= ast_index->int_value)
        {
            AST_T* var = object->object_vars[ast_index->int_value];
            AST_T* string = init_ast(AST_STRING);
            string->string_value = var->var_name;

            return visitor_visit(visitor, string, argc, argv);
        }
    } 
    else if (strcmp(node->var_name, "value") == 0)
    {
        if (args_size < 2)
            return node;

        AST_T* object = (AST_T*) visitor_visit(visitor, args[0], argc, argv);
        AST_T* ast_index = (AST_T*) args[1];

        if (object->object_vars_size >= ast_index->int_value)
        {
            AST_T* var = object->object_vars[ast_index->int_value];
            
            if (var->var_value)
                return visitor_visit(visitor, var->var_value, argc, argv);
        }
    }
    else
    if (strcmp(node->var_name, "get") == 0)
    {
        if (args_size < 2)
            return node;

        AST_T* object = (AST_T*) visitor_visit(visitor, args[0], argc, argv);
        AST_T* ast_key = (AST_T*) args[1];
        AST_T* value = 0;

        if (object->type == AST_OBJECT)
        {
            value = ast_object_get_value_by_key(object, ast_key->string_value);
        }
        else
        if (object->type == AST_GROUP)
        {
            value = ast_group_get_value_by_index(object, ((int)ast_key->float_value));
        }

        if (value)
            return visitor_visit(visitor, value, argc, argv);
    }
    else
    if (strcmp(node->var_name, "keys") == 0)
    {
        if (!args_size)
            return node;

        AST_T* object = (AST_T*) visitor_visit(visitor, args[0], argc, argv);
        AST_T* value = 0;

        if (object->type == AST_OBJECT)
            value = ast_object_get_keys(object);

        /** Might implement this some time
        * else
        * if (object->type == AST_GROUP)
        * {
        * } **/

        if (value)
            return visitor_visit(visitor, value, argc, argv);
    }

    return node;
}

AST_T* visitor_visit_group(visitor_T* visitor, AST_T* node, int argc, AST_T** argv)
{
    return node;
}

AST_T* visitor_visit_object(visitor_T* visitor, AST_T* node, int argc, AST_T** argv)
{
    return node;
}
