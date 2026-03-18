

#include "types.h"
#include "utils.h"
#include "parser.h"

t_node  *parse_while_stmt(t_parser *prs)
{
    t_node  *node;
    t_token *token;
    t_node  *body;
    t_node  *body_list;
    t_node  *body_head;
    t_node  *condition;

    body_list = NULL;
    body_head = NULL;

    // consume 'while'
    token = parser_advance(prs);
    if (token == NULL || token->type != TOKEN_KW_WHILE)
        return NULL;

    // consume '('
    token = parser_advance(prs);
    if (token == NULL || token->type != TOKEN_L_PAREN)
        return NULL;

    condition = parse_logical_or(prs);
    if (condition == NULL)
        return NULL;

    // consume ')'
    token = parser_advance(prs);
    if (token == NULL || token->type != TOKEN_R_PAREN)
        return NULL;

    // consume '{'
    token = parser_advance(prs);
    if (token == NULL || token->type != TOKEN_L_BRACE)
        return NULL;

    // parse body
    while (parser_peek(prs) && is_statement_intro(parser_peek(prs)->type))
    {
        body = parse_statement(prs);
        if (body == NULL)
            break;
        if (body_list == NULL)
        {
            body_list = body;
            body_head = body_list;
        }
        else
        {
            body_list->next = body;
            body_list = body_list->next;
        }
    }

    // consume '}'
    token = parser_advance(prs);
    if (token == NULL || token->type != TOKEN_R_BRACE)
        return NULL;

    node = new_while_stmt(condition, body_head);
    return node;
}
