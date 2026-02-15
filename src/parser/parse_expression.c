

#include "types.h"
#include "utils.h"
#include "parser.h"
#include <errno.h>

t_node  *parse_literal(t_parser *prs)
{
    t_token     *token;
    t_node      *node;
    long        integer;
    char        *strlol_ptr;

    token = parser_advance(prs);
    if (token == NULL)
        return token;

    if (token->type == TOKEN_INT_LITERAL)
    { 
        integer = strtol(token->value, &strlol_ptr, 10);

        if (errno == ERANGE) 
            return NULL;

        if (strlol_ptr == token->value || *strlol_ptr != '\0')
            return NULL;
    
        node = new_int_literal(integer);
    }
    else if (token->type == TOKEN_STRING_LITERAL)
        node = new_string_literal(token->value);
    
    else if (token->type == TOKEN_CHAR_LITERAL)
    {
        if (strlen(token->value) != 1)
            return NULL
        node = new_char_literal(token->value[0]);
    } 
    else 
        return NULL;

    return node;
}

t_node  *parse_logical_and(t_parser *prs)
{
    t_node *token;
    t_node *left;
    t_node *right; 

    // left = 
}


t_node  *parse_logical_or(t_parser *prs) 
{
    t_node *token;
    t_node *left;
    t_node *right;

    // left = parse_logical_and()


}

t_node  *parse_assignment(t_parser *parser)
{
    t_node *token;

    token = parser_advance(prs);
    if (token == NULL)
        return NULL;

    
}