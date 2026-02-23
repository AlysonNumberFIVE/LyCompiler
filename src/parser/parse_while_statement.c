

#include "types.h"
#include "utils.h"
#include "parser.h"

t_node  *parse_while_stmt(t_parser *prs)
{
    t_node  *node;
    t_token *token;

    t_node      *body;
    t_node      *body_list;
    t_node      *body_head;
    t_node      *condition;

    body_list = NULL;
    body_head = NULL;

    token = parser_advance(prs);
    if (token == NULL)
        return NULL;

    if (token->type != TOKEN_KW_WHILE)
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
    
    printf("here is %s\n", token->value);
    if (token && is_statement_intro(token->type))
    {
        body_list = NULL;
        body_head = NULL;
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
    
    node = new_while_stmt(condition, body_head);
    if (node == NULL)
        return NULL;

    return node;
}
