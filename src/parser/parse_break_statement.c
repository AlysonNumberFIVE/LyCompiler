

#include "types.h"
#include "parser.h"
#include "utils.h"

t_node  *parse_break_stmt(t_parser *prs)
{
    t_token *token;
    t_node  *node;

    token = parser_advance(prs);
    if (token == NULL)
        return NULL;

    if (token->type != TOKEN_KW_BREAK)
        return NULL;

    token = parser_advance(prs);
    if (token == NULL)
        return NULL;

    if (token->type != TOKEN_SEMICOLON)
        return NULL;

    node = new_break_stmt();
    if (node == NULL)
        return NULL;

    return node;
}   