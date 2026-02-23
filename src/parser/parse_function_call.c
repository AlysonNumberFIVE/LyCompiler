#include "types.h"
#include "utils.h"
#include "parser.h"

t_node      *parse_func_call(t_parser *prs)
{
    t_token     *token;
    t_node      *args;
    t_node      *func_name;
    t_node      *node; 
    t_node      *next;

    args = NULL;
    func_name = NULL;
    token = parser_advance(prs);
    if (token == NULL)
        return NULL;

    if (token->type == TOKEN_IDENTIFIER)
        func_name = new_identifier(token->value);


    token = parser_advance(prs);
    if (token == NULL)
        return NULL;

    if (token->type == TOKEN_L_PAREN)  
        args = parse_logical_or(prs);

    token = parser_peek(prs);
    if (token == NULL)
        return NULL;

    if (token->type == TOKEN_COMMA)
    {
        next = args;
        token = parser_advance(prs);
        while (token)
        {

            next->next = parse_logical_or(prs);
            if (next->next == NULL)
                return NULL;

            token = parser_peek(prs);
            if (token->type != TOKEN_COMMA)
                break;

            token = parser_advance(prs);
            if (token == NULL)
                break ;

            next = next->next;
        }
    }

    node = new_call(func_name, args);
    printf("binary_expr func_call: \n");
    print_ast(node  , 1);
    printf("======================\n");
    return node;
}