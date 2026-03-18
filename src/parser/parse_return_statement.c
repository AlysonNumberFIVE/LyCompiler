
#include "utils.h"
#include "types.h"
#include "parser.h"

t_node  *parse_return_stmt(t_parser *prs)
{
    t_token     *token;
    t_node      *value;
    t_node      *return_stmt;

    // consume 'return'
    token = parser_advance(prs);
    if (token == NULL || token->type != TOKEN_KW_RETURN)
        return NULL;

    // handle bare 'return;'
    token = parser_peek(prs);
    if (token == NULL)
        return NULL;
    if (token->type == TOKEN_SEMICOLON)
    {
        parser_advance(prs); // consume ';'
        return new_return_stmt(NULL);
    }

    value = parse_logical_or(prs);
    if (value == NULL)
        return NULL;

    // consume ';'
    token = parser_advance(prs);
    if (token == NULL || token->type != TOKEN_SEMICOLON)
        return NULL;

    return_stmt = new_return_stmt(value);
    return return_stmt;
}

