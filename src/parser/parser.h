#ifndef _H_PARSER_SUPPORT_
#define _H_PARSER_SUPPORT_


#include "types.h"


t_node      *parse_statement(t_parser *prs);
t_node      *parse_type(t_parser *prs);
t_parser    *init_parser(t_token *token);

t_node      *parse_assignment(t_parser *parser);
t_node      *parse_statement(t_parser *prs);

void        print_ast(t_node *node, int depth);

#endif 

