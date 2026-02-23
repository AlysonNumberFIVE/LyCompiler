

#include "types.h"
#include "utils.h"
#include "parser.h"
#include <errno.h>
#include <unistd.h>

#include <stdio.h>

// BNF Definition
// <expression>            ::= <assignment>
// <assignment>            ::= <logical_or>
//                         | IDENTIFIER "=" <assignment>

// <logical_or>            ::= <logical_and> { "||" <logical_and> }
// <logical_and>           ::= <equality>    { "&&" <equality> }
// <equality>              ::= <additive>    { ("==" | "!=") <additive> }
// <additive>              ::= <multiplicative> { ("+" | "-") <multiplicative> }
// <multiplicative>        ::= <primary> { ( “*” | “/” ) <primary>  } 
// <postfix>               ::= <primary> { "->" IDENTIFIER | "." IDENTIFIER | "[" <expression> "]" }
// <primary>               ::= IDENTIFIER 
//                         | <literal>
//                         | “(“ <expression> “)”  
//                         | <struct_initializer>      
// <literal>               ::= CHAR_LITERAL | INT_LITERAL | STRING_LITERAL


// <literal>               ::= CHAR_LITERAL | INT_LITERAL | STRING_LITERAL
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
    else if (token->type == TOKEN_L_PAREN) 
    {
        node = parse_logical_or(prs);

        token = parser_peek(prs);
        if (token == NULL)
            return NULL;

        if (token->type != TOKEN_R_PAREN)
            return NULL;
        
        parser_advance(prs);
        
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
    token = parser_peek(prs);
    if (token == NULL)
        return NULL;

    if (is_literal(token->type) == true || token->type == TOKEN_L_PAREN)
        left = parse_literal(prs);
    else if (token->type == TOKEN_IDENTIFIER)
    {
        left = new_identifier(token->value);
        token = parser_advance(prs);
        if (token == NULL)
            return NULL;
    }
    else 
        return NULL;
    
    return left;
}

t_node *parse_postfix(t_parser *prs)
{
    t_node      *node;
    t_node      *index;
    t_token     *token;
    bool        is_arrow;
    char        *name;

    node = parse_primary(prs);
    if (node == NULL)
        return NULL;
    
    while (42)
    {
        token = parser_peek(prs);
        if (token == NULL)
            break ;
        
        if (token->type == TOKEN_OP_ARROW || token->type == TOKEN_OP_DOT)
        {
            is_arrow = (token->type == TOKEN_OP_ARROW);
            token = parser_advance(prs);
            if (token == NULL)
                break ;

            token = parser_advance(prs);
            if (token == NULL)
                break ;
        
            name = strdup(token->value);
            if (name == NULL)
                return NULL;    

            node = new_member_access(node, name, is_arrow);
        }
        else if (token->type == TOKEN_L_BLOCK)
        {
            index = parse_assignment(prs); // Expressions are allowed in []
            
            if (parser_peek(prs) && parser_peek(prs)->type == TOKEN_R_BLOCK)
                parser_advance(prs); // Consume ']'
            
            node = new_array_access(node, index);
        }
        else 
        {
            break ;
        }
    }

    return node;
}

// <multiplicative>       ::= <primary> { ( “*” | “/” ) <primary>  } 
t_node *parse_multiplicative(t_parser *prs)
{
    t_token     *token;
    t_node      *left;
    t_node      *right;
    char        *op;

    left = parse_postfix(prs);
    if (left == NULL)
        return NULL;
 
    while (42) 
    {
        token = parser_peek(prs);     
        if (token == NULL || (token->type != TOKEN_OP_STAR && token->type != TOKEN_OP_SLASH))
            break ;

        token = parser_advance(prs);
        if (token == NULL)
            break;
    
        op = strdup(token->value);
        if (op == NULL)
            return NULL;
        
        right = parse_postfix(prs);
        if (right == NULL)
            return left; 

        left = new_binary_expr(left, op, right);
    }

    return left;
}

// <additive>              ::= <multiplicative> { ("+" | "-") <multiplicative> }
t_node *parse_additive(t_parser *prs)
{
    t_token     *token;
    t_node      *left;
    t_node      *right;
    char        *op;

    left = parse_multiplicative(prs);
    if (left == NULL)
        return NULL;

    while (42) 
    {
        token = parser_peek(prs);     
        if (token == NULL || (token->type != TOKEN_OP_PLUS && token->type != TOKEN_OP_MINUS))
            break ;

        token = parser_advance(prs);
         if (token == NULL)
            break;

        op = strdup(token->value);
        if (op == NULL)
            return NULL;

        right = parse_multiplicative(prs);
        if (right == NULL)
            return left; 

        left = new_binary_expr(left, op, right);
    }

    return left;
}

// <equality>              ::= <additive>    { ("==" | "!=") <additive> }
t_node  *parse_equality(t_parser *prs)
{
    t_token     *token;
    t_node      *left;
    t_node      *right;
    char        *op;

    left = parse_additive(prs);
    if (left == NULL)
        return NULL;

    while (42) 
    {
        token = parser_peek(prs);     
        if (token == NULL || (token->type != TOKEN_OP_EQ && token->type != TOKEN_OP_NEQ))
            break ;

        token = parser_advance(prs);
        if (token == NULL)
            break;

        op = strdup(token->value);
        if (op == NULL)
            return NULL;

        right = parse_additive(prs);
        if (right == NULL)
            return left; 

        left = new_binary_expr(left, op, right);
    } 

    return left;
}

// <logical_and>        ::= <equality> { "&&" <equality> }
t_node  *parse_logical_and(t_parser *prs)
{
    t_token     *token;
    t_node      *left;
    t_node      *right;
    char        *op;

    left = parse_equality(prs);

    while (42) 
    {
        token = parser_peek(prs);     
        if (token == NULL || token->type != TOKEN_OP_AND)
            break ;

        token = parser_advance(prs);
        if (token == NULL)
            break;

        op = strdup(token->value);
        if (op == NULL)
            return NULL;

        right = parse_equality(prs);
        if (right == NULL)
            return left; 

        left = new_binary_expr(left, op, right);
    } 
    
    return left;
}

// <logical_or>            ::= <logical_and> { "||" <logical_and> }
t_node  *parse_logical_or(t_parser *prs) 
{
    t_token     *token;
    t_node      *left;
    t_node      *right;
    char        *op;

    printf("token in logical_or is %s\n", parser_peek(prs)->value);
    left = parse_logical_and(prs);
    while (42) 
    {
        token = parser_peek(prs);     
        if (token == NULL || token->type != TOKEN_OP_AND)
            break ;

        token = parser_advance(prs);
        if (token == NULL)
            break;

        op = strdup(token->value);
        if (op == NULL)
            return NULL;

        right = parse_logical_and(prs);
        if (right == NULL)
            return left; 

        left = new_binary_expr(left, op, right);
    } 
    
    return left;
}


// <assignment>            ::= <logical_or>
t_node  *parse_assignment(t_parser *prs)
{
    t_token     *token;
    t_node      *node;

    token = parser_advance(prs);
    if (token == NULL)
        return NULL;

    printf("token parse_assignment is %s\n", token->value);
    node = parse_logical_or(prs);
    printf("binary_expr: \n");
    print_ast(node  , 1);
    printf("======================\n");
    return node;
}