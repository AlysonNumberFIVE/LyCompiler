

#include "types.h"
#include "utils.h"
#include "parser.h"
#include <errno.h>

// <literal>                 ::= CHAR_LITERAL | INT_LITERAL | STRING_LITERAL
t_node  *parse_literal(t_parser *prs)
{
    t_token     *token;
    t_node      *node;
    long        integer;
    char        *strlol_ptr;

    token = parser_advance(prs);
    if (token == NULL)
        return NULL;

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
            return NULL;
        node = new_char_literal(token->value[0]);
    } 
    else 
        return NULL;

    return node;
}

// <primary>               ::= IDENTIFIER 
//                         | <literal>
//                         | “(“ <expression> “)”  
//                         | <struct_initializer>   
t_node  *parse_primary(t_parser *prs)
{
    t_token  *token;
    t_node *left;
 //   t_node *right;   

    token = parser_peek(prs);
    if (token == NULL)
        return NULL;

    printf("token is %s\n", token->value);
    if (is_literal(token->type) == true)
        left = parse_literal(prs);
    else if (token->type == TOKEN_IDENTIFIER)
        left = new_identifier(token->value);
    else 
        return NULL;
    
    return left;
}

// <multiplicative>       ::= <primary> { ( “*” | “/” ) <primary>  } 
t_node *parse_multiplicative(t_parser *prs)
{
    t_token  *token;
    t_node *left;
    t_node *right;
    t_node *binary_exp;
    char *op;

    left = parse_primary(prs);
    if (left == NULL)
        return NULL;

    token = parser_advance(prs);
    if (token == NULL)
        return NULL;
    
    if (token->type != TOKEN_OP_STAR && token->type != TOKEN_OP_SLASH) 
        return left;

    op = strdup(token->value);
    if (op == NULL)
        return NULL;
    
    right = parse_primary(prs);
    if (right == NULL)
        return left;

    binary_exp = new_binary_expr(left, op, right);
    if (binary_exp == NULL)
        return NULL;

    return binary_exp;
}

t_node *parse_additive(t_parser *prs)
{
    t_token  *token;
    t_node *left;
    t_node *right;
    t_node *binary_exp;
    char *op;

    left = parse_multiplicative(prs);
    if (left == NULL)
        return NULL;

    token = parser_advance(prs);
    if (token == NULL)
        return NULL;
    
    if (token->type != TOKEN_OP_PLUS && token->type != TOKEN_OP_MINUS) 
        return left;

    op = strdup(token->value);
    if (op == NULL)
        return NULL;

    right = parse_multiplicative(prs);
    if (right == NULL)
        return NULL;

    binary_exp = new_binary_expr(left, op, right);
    if (binary_exp == NULL)
        return NULL;

    printf("end");

    return left;
}



t_node  *parse_equality(t_parser *prs)
{
  //  t_node *token;
    t_node *left;
  //  t_node *right;  

    left = parse_additive(prs);

    return left;
}

t_node  *parse_logical_and(t_parser *prs)
{
  //  t_node *token;
    t_node *left;
  //  t_node *right; 

     left = parse_equality(prs);

    return left;
}


t_node  *parse_logical_or(t_parser *prs) 
{
  //  t_node *token;
    t_node *left;
  //  t_node *right;

    left = parse_logical_and(prs);

    return left;
}

t_node  *parse_assignment(t_parser *prs)
{
    t_token *token;

    token = parser_advance(prs);
    if (token == NULL)
        return NULL;
    
    parse_logical_or(prs);

    return NULL;
}