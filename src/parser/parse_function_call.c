#include "types.h"
#include "utils.h"
#include "parser.h"


t_node      *parse_func_call(t_parser *prs)
{
    t_token     *token;
    t_node      *node; 
    t_node      *next;
    char        *name;

    token = parser_advance(prs);
    if (token == NULL)
        return NULL;

    name = strdup(token->value);
    if (name == NULL)
        return NULL;

    token = parser_advance(prs);
    if (token == NULL)
        return NULL;

    if (token->type == TOKEN_L_PAREN) 
    {
        node = parse_expression(prs);
        if (node == NULL)
            break ;
    }

    if (token->type == TOKEN_COMMA)
    {
        next = node;
        token = parser_advance(prs);
        while (token)
        {
            next->next = parse_expression(prs);
            if (next->next == NULL)
                return NULL;

            token = parser_peek(prs);
            if (token->type != TOKEN_COMMA)
                break;

            token = parser_advance(prs);
            if (token == NULL)
                break ;

            node_next = node_next->next;
        }

    }
}