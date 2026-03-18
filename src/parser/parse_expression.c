

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
    {
        node = new_string_literal(token->value); 
    }
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
//                         | <call>
t_node  *parse_primary(t_parser *prs)
{
    t_token     *token;
    t_node      *left;

    token = parser_peek(prs);
    if (token == NULL)
        return NULL;
    if (is_literal(token->type) == true || token->type == TOKEN_L_PAREN)
        left = parse_literal(prs);
    else if (token->type == TOKEN_IDENTIFIER)
    {
        token = parser_lookahead(prs);
        if (token == NULL)
            return NULL;

        if (token->type == TOKEN_L_PAREN)
        {
            left = parse_func_call(prs);
        }        
        else 
        {       
            token = parser_peek(prs);
            if (token == NULL)
                return NULL; 
            left = new_identifier(token->value);

            token = parser_advance(prs);
            if (token == NULL)
                return NULL;   
        }
    }
    else if (token->type == TOKEN_L_BLOCK)
        left = parse_array(prs);
    else if (token->type == TOKEN_AT)
        left = parse_struct_init(prs);
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
            parser_advance(prs); // consume '['
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

    left = parse_logical_and(prs);
    while (42) 
    {
        token = parser_peek(prs);     
        if (token == NULL || token->type != TOKEN_OP_OR)
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

t_node  *parse_assignment(t_parser *prs)
{
    t_token *token;
    t_node  *left;
    t_node  *value;

    left = parse_logical_or(prs);
    if (left == NULL)
        return NULL;

    token = parser_peek(prs);
    if (token != NULL && token->type == TOKEN_OP_ASSIGN)
    {
        parser_advance(prs); // consume '='
        value = parse_assignment(prs);
        if (value == NULL)
            return NULL;
        return new_assignment(left, value);
    }
    return left;
}

t_node      *parse_struct_init(t_parser *prs)
{
    t_token *token;
    t_node  *node;
    t_node  *field;
    t_node  *field_head;
    t_node  *field_list;
    char    *struct_name;
    char    *field_name;
    t_node  *field_value;

    // consume '@'
    token = parser_advance(prs);
    if (token == NULL || token->type != TOKEN_AT)
        return NULL;

    // consume struct name
    token = parser_advance(prs);
    if (token == NULL || token->type != TOKEN_IDENTIFIER)
        return NULL;
    struct_name = strdup(token->value);

    // consume '{'
    token = parser_advance(prs);
    if (token == NULL || token->type != TOKEN_L_BRACE)
    {
        free(struct_name);
        return NULL;
    }

    field_head = NULL;
    field_list = NULL;
    while (parser_peek(prs) && parser_peek(prs)->type != TOKEN_R_BRACE)
    {
        // field name (IDENTIFIER)
        token = parser_advance(prs);
        if (token == NULL || token->type != TOKEN_IDENTIFIER)
        {
            free(struct_name);
            return NULL;
        }
        field_name = strdup(token->value);

        // ':'
        token = parser_advance(prs);
        if (token == NULL || token->type != TOKEN_COLON)
        {
            free(struct_name);
            free(field_name);
            return NULL;
        }

        // value expression
        field_value = parse_logical_or(prs);
        if (field_value == NULL)
        {
            free(struct_name);
            free(field_name);
            return NULL;
        }

        field = new_struct_field(field_name, field_value);
        free(field_name);
        if (field_head == NULL)
        {
            field_head = field;
            field_list = field_head;
        }
        else
        {
            field_list->next = field;
            field_list = field_list->next;
        }

        // optional ','
        if (parser_peek(prs) && parser_peek(prs)->type == TOKEN_COMMA)
            parser_advance(prs);
    }

    // consume '}'
    token = parser_advance(prs);
    if (token == NULL || token->type != TOKEN_R_BRACE)
    {
        free(struct_name);
        return NULL;
    }

    node = new_struct_initializer(struct_name, field_head);
    free(struct_name);
    return node;
}

t_node      *parse_array(t_parser *prs)
{
    t_token *token;
    t_node  *node;
    t_node  *item;
    t_node  *item_list;
    t_node  *item_head;

    // consume '['
    token = parser_advance(prs);
    if (token == NULL || token->type != TOKEN_L_BLOCK)
        return NULL;

    item_head = NULL;
    item_list = NULL;

    // handle empty array []
    token = parser_peek(prs);
    if (token != NULL && token->type == TOKEN_R_BLOCK)
    {
        parser_advance(prs);
        return new_array(NULL);
    }

    // parse first item
    item = parse_logical_or(prs);
    if (item == NULL)
        return NULL;
    item_head = item;
    item_list = item_head;

    // parse remaining comma-separated items
    while (parser_peek(prs) && parser_peek(prs)->type == TOKEN_COMMA)
    {
        parser_advance(prs); // consume ','
        item = parse_logical_or(prs);
        if (item == NULL)
            return NULL;
        item_list->next = item;
        item_list = item_list->next;
    }

    // consume ']'
    token = parser_advance(prs);
    if (token == NULL || token->type != TOKEN_R_BLOCK)
        return NULL;

    node = new_array(item_head);
    return node;
}