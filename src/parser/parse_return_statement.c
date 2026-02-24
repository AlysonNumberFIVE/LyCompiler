
#include "utils.h"
#include "types.h"
#include "parser.h"

t_node  *parse_return_stmt(t_parser *prs)
{
    t_node      *node;
    t_token     *token;
    t_node      *return_stmt;

    token = parser_advance(prs);
    if (token == NULL)
        return NULL;

    if (token->type != TOKEN_KW_RETURN)
        return NULL;

    node = parse_logical_or(prs);
    if (node == NULL)
        return NULL;

    return_stmt = new_return_stmt(node);
    if (return_stmt == NULL)
        return NULL;

    token = parser_peek(prs);
    if (token == NULL)
        return NULL;
 
    if (token->type != TOKEN_SEMICOLON)
        return NULL; 

    return return_stmt;
}

