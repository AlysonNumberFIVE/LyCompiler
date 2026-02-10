

#include "types.h"


// BNF Definition:
// <param_list>       ::= <param> { “,” <param> } 
// <param>            ::= <identifier> <type>  
// <identifier>       ::= IDENTIFIER  
// <type>             ::= <type_spec> { “*” | “&” }
// <type_spec>        ::= “i64” 
//                    | “char” 
//                    | “struct” IDENTIFIER


t_node  *parse_parameter(parser *prs)
{
    t_token *token;

    token = parser_advance(prs);
    if (token && token->type != TOKEN_IDENTIFIER)
        return NULL;

    token = parser_advance(prs);
    if (token->type !=)
}

t_node  *parse_parameter_list(t_parser *prs)
{
    t_token *token;



}