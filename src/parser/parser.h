#ifndef _H_PARSER_SUPPORT_
#define _H_PARSER_SUPPORT_


#include "types.h"

bool        is_statement_intro(token_type type);

t_node          *parse_statement(t_parser *prs);
t_node          *parse_type(t_parser *prs);
t_parser        *init_parser(t_token *token);

t_node          *parse_assignment(t_parser *parser);
t_node          *parse_statement(t_parser *prs);

void            print_ast(t_node *node, int depth);
t_node          *parse_logical_or(t_parser *prs);
t_node          *parse_func_call(t_parser *prs);
t_node          *parse_if_statement(t_parser *prs);
t_node          *parse_while_stmt(t_parser *prs);
t_node          *parse_return_stmt(t_parser *prs);
t_node          *parse_continue_stmt(t_parser *prs);
t_node          *parse_break_stmt(t_parser *prs);
t_node          *parse_array(t_parser *prs);
t_node          *parse_struct(t_parser *prs);

t_node          *parse_parameter(t_parser *prs);
t_node          *parse_var_assign(t_parser *prs);
t_node          *init_error(char *message, int line, int column);

t_token         *search_for_recovery(t_parser *prs);
t_node          *parser_error(t_parser *prs, char *message, t_token *token);
void            print_parse_errors(t_parser *prs);
t_node          *parse_var_decl(t_parser *prs);
t_node          *parse_statement_scope(t_parser *prs);
bool            is_statement_scope(token_type type);
t_node          *parse_array(t_parser *prs);
t_node          *parse_struct_init(t_parser *prs);

#endif 

