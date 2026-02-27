
#include "utils.h"
#include "types.h"
#include "parser.h"

t_node      *parse_struct(t_parser *prs)
{
    t_token     *token;
    t_node      *node;
    t_node      *parameter;
    t_node      *parameter_list;
    t_node      *parameter_head;
    char        *name;

    token = parser_advance(prs);
    if (token == NULL)
        return NULL;

    if (token->type != TOKEN_KW_STRUCT)
        return NULL;

    token = parser_advance(prs);
    if (token == NULL)
        return NULL;

    if (token->type != TOKEN_IDENTIFIER)
        return NULL;
    
    name = strdup(token->value);
    if (name == NULL)
        return NULL;

    token = parser_advance(prs);
    if (token == NULL)
        return NULL;

    if (token->type != TOKEN_L_BRACE)
        return NULL;

    parameter_list = NULL;
    parameter_head = NULL;
    while (token && token->type != TOKEN_R_BRACE)
    {
        token = parser_peek(prs);
        if (token == NULL)
            break ;

        if (token->type == TOKEN_R_BRACE)
            break;
    
        parameter = parse_parameter(prs);
        if (parameter_list == NULL)
        {
            parameter_list = parameter;
            parameter_head = parameter_list;
        }
        else
        {
            parameter_list->next = parameter;
            parameter_list = parameter_list->next;
        }

        token = parser_advance(prs);
        if (token == NULL)
            break ;

        if (token->type != TOKEN_SEMICOLON)
             break;    
    }

    node = new_struct_decl(name, parameter_head);
    if (node == NULL)
        return NULL;

    return node; 
}