

#include "types.h"
#include "utils.h"


// BNF Definition:
// <param_list>       ::= <param> { “,” <param> } 
// <param>            ::= IDENTIFIER <type>  
// <type>             ::= <type_spec> { “*” }
// <type_spec>        ::= “i64” 
//                    | “char” 
//                    | IDENTIFIER


// <type>             ::= <type_spec> { "*" }
t_node  *parse_type(t_parser *prs)
{
    t_token     *token; 
    char        *base_type;
    int         pointer_level;
    t_node      *node;
    bool        is_valid;

    pointer_level = 0;
    token = parser_advance(prs);
    if (token == NULL) 
        return NULL;

    is_valid = is_primitive(token->type) || token->type == TOKEN_IDENTIFIER;
    if (!is_valid) 
        return NULL;

    // <type_spec>
    base_type = strdup(token->value);

    // { "*" }
    token = parser_peek(prs);
    if (token && token->type == TOKEN_OP_STAR) 
    {
        while (token && token->type == TOKEN_OP_STAR)
        {
            token = parser_advance(prs);
            pointer_level++;
            token = parser_lookahead(prs);
            if (token && token->type != TOKEN_OP_STAR)  
            {
                pointer_level++;
                break;
            }
        }
        token = parser_advance(prs);
        if (token == NULL)
            return NULL;
    }

    printf("base_type: %s nad pointer_level %d\n", base_type, pointer_level);
    // <type_spec> { "*" }
    node = new_type_spec(base_type, pointer_level); 
    if (node == NULL)
        return NULL;

    // <type>       ::= <type_spec> { "*" }
    return node;
}


// <param>            ::= IDENTIFIER <type>
t_node  *parse_parameter(t_parser *prs)
{
    t_token     *token;
    char        *name;
    t_node      *type;
    t_node      *parameter;

    // IDENTIFIER 
    token = parser_advance(prs);
    if (token && token->type != TOKEN_IDENTIFIER)
        return NULL;

    name = strdup(token->value);

    // fetch <type>
    type = parse_type(prs);
    if (type == NULL)
        return NULL;

    // IDENTIFIER <type>
    parameter = new_param(name, type);
    if (parameter == NULL)
        return NULL;
    
    // <param>            ::= IDENTIFIER <type>
    return parameter;
}


// <param_list>       ::= <param> { “,” <param> } 
t_node  *parse_parameter_list(t_parser *prs)
{
    t_token *token = parser_peek(prs);
    t_node *parameter;
    t_node *node_next;

    parameter = parse_parameter(prs);
    if (parameter == NULL)
        return NULL;

    node_next = parameter;
    token = parser_peek(prs);
    if (token->type == TOKEN_COMMA)
    {
        token = parser_advance(prs);
        while (token)
        {
            node_next->next = parse_parameter(prs);
            if (node_next->next == NULL)
                return NULL;

            token = parser_peek(prs); 
            if (token->type != TOKEN_COMMA)
                break;
            
            parser_advance(prs);
            node_next = node_next->next; 
        }
    }

    node_next = parameter;
    while (node_next)
    {
        printf("name:       %s\n", node_next->data.param.name);
        printf("type:       %s\n", node_next->data.param.type->data.type_spec.base_type);
        printf("ptr_level:  %d\n", node_next->data.param.type->data.type_spec.pointer_level);
        printf("=================\n");

        node_next = node_next->next;
    }
  
    return parameter;
}