#ifndef _UTILS_H
#define _UTILS_H

#include "types.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>


bool            push_token(t_lexer *lexer, token_type type, char *value, int line, int column);
void            print_tokens(t_token *head);
void            free_tokens(t_token *head);

void            print_files(t_sourcefile *files);
void            free_files(t_sourcefile *files);
bool            push_file(t_sourcefile **head, char *path, char *buffer, size_t buffer_size);

bool            push_error(t_error_control *error, char *line_message, char *code_block, int line, int column);

t_sourcefile    *parse_cmd(int argc, char **argv);
void            scan_token(t_lexer *lx, char *buffer, size_t length);
char            peek(t_lexer *lx, char *buffer, size_t length);
char            advance(t_lexer *lx, char *buffer, size_t length);
void            handle_double(t_lexer *lx, 
        char *buffer, size_t length, char expected, 
        token_type double_type, char *double_lit,
        token_type single_type, char *single_lit);
t_lexer         *lex_files(t_sourcefile *files); 
char            *charpush(char *str, char c);


#endif 