
#include "types.h"
#include "parser.h"
#include "utils.h"

t_node  *parse_continue_stmt(t_parser *prs)
{
    t_token *token;
    t_node  *node;

    token = parser_advance(prs);
    if (token == NULL)
        return NULL;

    if (token->type != TOKEN_KW_CONTINUE)
        return NULL;

    token = parser_advance(prs);
    if (token == NULL)
        return NULL;

    if (token->type != TOKEN_SEMICOLON)
        return NULL;

    node = new_continue_stmt();
    if (node == NULL)
        return NULL;

    return node;
}