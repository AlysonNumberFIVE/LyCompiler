#include "types.h"
#include "parser.h"
#include "utils.h"

t_node  *parse_if_statement(t_parser *prs)
{
   // t_node      *node;
    t_node      *condition;
    t_node      *else_condition = NULL;
    t_token     *token;
    t_node *body = NULL;
    t_node *node;
    t_node  *body_list;
    t_node *body_head = NULL;

   
    token = parser_advance(prs);
    printf("token entering if statement %s\n", token->value);
    if (token == NULL)
        return NULL;

    if (token->type != TOKEN_KW_IF)
        return NULL;
    
    token = parser_advance(prs);
    if (token == NULL)
        return NULL;

    printf("token entering if statement %s\n", token->value);
    if (token->type != TOKEN_L_PAREN)
        return NULL;

    condition = parse_logical_or(prs);
    if (condition == NULL)
        return NULL;

    token = parser_advance(prs);
    if (token == NULL)
        return NULL;

    printf("token entering if statement %s\n", token->value);
    if (token->type != TOKEN_R_PAREN)
        return NULL;

    token = parser_advance(prs);
    if (token == NULL)
        return NULL;

    if (token->type != TOKEN_L_BRACE)
        return NULL;

    printf("token if_stmt is %s\n", token->value);
    token = parser_peek(prs);
        printf("token if_stmt is %s\n", token->value);
    if (token && is_statement_intro(token->type))
    {
        body_list = NULL;
        body_head = NULL;
        while (parser_peek(prs) && is_statement_intro(parser_peek(prs)->type))
        {       
            body = parse_statement(prs);
            if (token == NULL)
                return NULL;
            

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
    printf("after blck we're at %s\n", parser_peek(prs)->value);

    token = parser_advance(prs);
    if (token == NULL)
        return NULL;

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

        printf("tokne is KW_ELSE %s\n", token->value);
        if (token->type == TOKEN_KW_IF) 
        {

            else_condition = parse_if_statement(prs);
            printf("after if statement\n");
        }
        else if (token->type == TOKEN_L_BRACE)
        {
            printf("here we are\n");
        } 
    } 

    printf("token if_stmt is %s\n", token->value);
    body = body_head;

    node = new_if_stmt(condition, body_head, else_condition);

    print_ast(node, 1);
    return node;
    
}   












