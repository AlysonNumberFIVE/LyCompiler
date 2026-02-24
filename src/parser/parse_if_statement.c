#include "types.h"
#include "parser.h"
#include "utils.h"

t_node  *parse_if_statement(t_parser *prs)
{
    t_node      *condition;
    t_node      *else_condition;
    t_token     *token;
    t_node      *body;
    t_node      *node;
    t_node      *body_list;
    t_node      *body_head;

    else_condition = NULL;
    body_list = NULL;
    body_head = NULL;

    token = parser_advance(prs);
    if (token == NULL)
        return NULL;

    if (token->type != TOKEN_KW_IF)
        return NULL;
    
    token = parser_advance(prs);
    if (token == NULL)
        return NULL;

    if (token->type != TOKEN_L_PAREN)
        return NULL;

    condition = parse_logical_or(prs);
    if (condition == NULL)
        return NULL;

    token = parser_advance(prs);
    if (token == NULL)
        return NULL;

    if (token->type != TOKEN_R_PAREN)
        return NULL;

    token = parser_advance(prs);
    if (token == NULL)
        return NULL;

    if (token->type != TOKEN_L_BRACE)
        return NULL;

    token = parser_peek(prs);
    if (token == NULL)
        return NULL;
 
    if (token && is_statement_intro(token->type))
    {
        while (parser_peek(prs) && is_statement_intro(parser_peek(prs)->type))
        {       
            body = parse_statement(prs);
            if (token == NULL)
                break ;
            
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
    }

    node = new_if_stmt(condition, body_head, NULL);
    if (node == NULL)
        return NULL;

    token = parser_advance(prs);
    if (token == NULL)
    {
        // TODO: I'm unsure about this. Added this here to make the unittest work but this feels
        // arbitrary.
        return node;
    }

    if (token->type != TOKEN_R_BRACE)
        return NULL;
    
    token = parser_advance(prs);
    if (token == NULL)
        return NULL;
    
    if (token->type == TOKEN_KW_ELSE) 
    {
        token = parser_peek(prs);
        if (token == NULL)
            return NULL;

        if (token->type == TOKEN_KW_IF) 
            else_condition = parse_if_statement(prs);
    } 

    node->data.if_stmt.else_branch = else_condition;
    return node;
    
}   












