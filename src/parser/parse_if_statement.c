#include "types.h"
#include "parser.h"
#include "utils.h"

t_node  *parse_if_statement(t_parser *prs)
{
    t_node      *condition;
    t_node      *else_branch;
    t_token     *token;
    t_node      *body;
    t_node      *body_list;
    t_node      *body_head;
    t_node      *else_list;
    t_node      *else_head;

    else_branch = NULL;
    body_list = NULL;
    body_head = NULL;
    else_list = NULL;
    else_head = NULL;

    // consume 'if'
    token = parser_advance(prs);
    if (token == NULL || token->type != TOKEN_KW_IF)
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

    // parse then-body
    while (parser_peek(prs) && is_statement_intro(parser_peek(prs)->type))
    {
        body = parse_statement(prs);
        if (body == NULL)
            break;
        if (body_head == NULL)
        {
            body_head = body;
            body_list = body_head;
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

    // check for 'else'
    token = parser_peek(prs);
    if (token != NULL && token->type == TOKEN_KW_ELSE)
    {
        parser_advance(prs); // consume 'else'
        token = parser_peek(prs);
        if (token == NULL)
            return NULL;

        if (token->type == TOKEN_KW_IF)
        {
            // else if (...) { ... }
            else_branch = parse_if_statement(prs);
        }
        else if (token->type == TOKEN_L_BRACE)
        {
            parser_advance(prs); // consume '{'
            while (parser_peek(prs) && is_statement_intro(parser_peek(prs)->type))
            {
                body = parse_statement(prs);
                if (body == NULL)
                    break;
                if (else_head == NULL)
                {
                    else_head = body;
                    else_list = else_head;
                }
                else
                {
                    else_list->next = body;
                    else_list = else_list->next;
                }
            }
            // consume '}'
            token = parser_advance(prs);
            if (token == NULL || token->type != TOKEN_R_BRACE)
                return NULL;
            else_branch = else_head;
        }
    }

    return new_if_stmt(condition, body_head, else_branch);
}   












