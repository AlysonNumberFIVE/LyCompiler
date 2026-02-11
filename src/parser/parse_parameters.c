

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

    pointer_level = 0;
    token = parser_advance(prs);
    if (token && !(is_primitive(token->type) || token->type == TOKEN_IDENTIFIER))
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
        parser_advance(prs);
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

    // IDENTIFIER <type>
    parameter = new_param(name, type);
    if (parameter == NULL)
        return NULL;
    
    // <param>            ::= IDENTIFIER <type>
    return parameter;
}

t_node  *parse_parameter_list(t_parser *prs)
{
    t_token *token = parser_peek(prs);
    //t_node *parameter;
 //   t_node *node_next;

    printf("before token %s\n", token->value);
     parse_parameter(prs);

//    node_next = parameter;
    token = parser_peek(prs);
    if (token->type == TOKEN_COMMA)
    {
        token = parser_advance(prs);
        while (token)
        {
            parse_parameter(prs);

            token = parser_peek(prs); 
            if (token->type != TOKEN_COMMA)
                break;
            parser_advance(prs);
        }
    }
    printf("after token %s\n", token->value); 
    return NULL;
}