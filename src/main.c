#include "include/lexer.h"
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
    token_T* tok;

    while ((tok = lexer_next_token(lexer))->type != TOKEN_EOF)
    {
        printf("%s:\t\t%s\n", token_to_str(tok), tok->value);
    }
}
