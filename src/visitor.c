#include "include/visitor.h"
#include "include/main.h"
#include "include/io.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

visitor_T* init_visitor()
{
    visitor_T* visitor = (visitor_T*) calloc(1, sizeof(struct VISITOR_STRUCT));
    visitor->buffer = (void*)0;

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

AST_T* visitor_visit(visitor_T* visitor, AST_T* node, int argc, AST_T** argv)
{
    switch (node->type)
    {
        case AST_ROOT: return visitor_visit_root(visitor, node, argc, argv); break;
        case AST_RAW: return visitor_visit_raw(visitor, node, argc, argv); break;
        case AST_COMP: return visitor_visit_comp(visitor, node, argc, argv); break;
        case AST_ASSIGN: return visitor_visit_assign(visitor, node, argc, argv); break;
        case AST_STRING: return visitor_visit_string(visitor, node, argc, argv); break;
        case AST_VAR: return visitor_visit_var(visitor, node, argc, argv); break;
        case AST_CALL: return visitor_visit_call(visitor, node, argc, argv); break;
        case AST_GROUP: return visitor_visit_group(visitor, node, argc, argv); break;
        case AST_COMMENT: return init_ast(AST_NOOP); break;
        default: printf("[Visitor]: Unhandled node of type `%d`\n", node->type); exit(1); break;
    }
    return init_ast(AST_NOOP);
}

AST_T* visitor_visit_root(visitor_T* visitor, AST_T* node, int argc, AST_T** argv)
{
    for (int i = 0; i < (int)node->root_items_size; i++)
        visitor_visit(visitor, node->root_items[i], argc, argv);

    return node;
}

AST_T* visitor_visit_raw(visitor_T* visitor, AST_T* node, int argc, AST_T** argv)
{
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
                    char* value = sh(comment->comment_value, node->result);
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

AST_T* visitor_visit_var(visitor_T* visitor, AST_T* node, int argc, AST_T** argv)
{
    if (node->var_name[0] == '$')
    {
        int index = (int) atoi((char[]){ node->var_name[1], '\0'});

        if (argc >= index && argc != 0)
            return visitor_visit(visitor, argv[index], argc, argv);
    }
    if (node->var_value == (void*)0)
    {
        printf("[Visitor.visit_var]: `%s` is not defined.\n", node->var_name);
        exit(1);
    }

    return visitor_visit(visitor, node->var_value, argc, argv);
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

        AST_T* iterable = (AST_T*) args[0];
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

    return node;
}

AST_T* visitor_visit_group(visitor_T* visitor, AST_T* node, int argc, AST_T** argv)
{
    for (int i = 0; i < (int) node->group_items_size; i++)
        visitor_visit(visitor, node->group_items[i], argc, argv);

    return node;
}
