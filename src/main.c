#include "include/lexer.h"
#include "include/parser.h"
#include "include/visitor.h"
#include "include/io.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Error, please specify file.\n");
        exit(1);
    }

    lexer_T* lexer = init_lexer(gpp_read_file(argv[1]));
    parser_T* parser = init_parser(lexer);
    AST_T* root = parser_parse(parser, (void*)0);
    visitor_T* visitor = init_visitor();
    visitor_visit(visitor, root);

    printf("address of root node:\t%p\n", root);
    printf("--- START OF GENERATED OUTPUT ---\n");
    printf("%s\n", visitor->buffer);
    printf("--- END OF GENERATED OUTPUT ---\n");
}
