

#include "../inc/compiler.h"
#include "utils/utils.h"
#include "utils/types.h"

int main(int argc, char **argv)
{
    t_sourcefile *files;

    files = NULL;
    if (argc < 1) {
        fprintf(stderr, "Usage: %s <file1.ly> <file2.ly> ...\n", argv[0]);
        return (0);
    }

    files = parse_cmd(argc, argv);
    if (files == NULL) {
        fprintf(stderr, "file error");
        return (0);  
    }

    t_lexer *lex = lex_files(files);
    
   //  print_tokens(lex->head);

    parser(lex);

    return 0;   
}