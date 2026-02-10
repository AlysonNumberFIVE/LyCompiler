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

// Parser =====================================================

t_node          *new_struct_decl(char *name, t_node *members);
t_node          *new_struct_member(char *name, t_node *type_node);
t_node          *new_func_decl(char *name, t_node *params, t_node *return_type, t_node *body);
t_node          *new_param(char *name, t_node *type);
t_node          *new_var_decl(char *name, t_node *type, t_node *initializer);
t_node          *new_return_stmt(t_node *expression);
t_node          *new_break_stmt(void);
t_node          *new_continue_stmt(void);
t_node          *new_if_stmt(t_node *condition, t_node *then_branch, t_node *else_branch);
t_node          *new_while_stmt(t_node *condition, t_node *body);
t_node          *new_expr_stmt(t_node *expression);
t_node          *new_binary_expr(t_node *left, char *op, t_node *right);
t_node          *new_assignment(t_node *target, t_node *value);
t_node          *new_member_access(t_node *struct_expr, char *member_name, bool is_arrow);
t_node          *new_call(t_node *callee, t_node *args);
t_node          *new_struct_initializer(char *struct_name, t_node *fields);
t_node          *new_struct_field(char *name ,t_node *value);
t_node          *new_identifier(char *name);
t_node          *new_type_spec(char *base_type, int pointer_level);

#endif 