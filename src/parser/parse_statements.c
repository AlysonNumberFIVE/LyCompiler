
#include "types.h"
#include "utils.h"

// <statement>        ::= <var_decl>
//                     | <return_stmt>
//                     | <if_stmt>
//                     | <while_stmt>
//                     | <expr_stmt>
//                     | <break_stmt>
//                     | <continue_stmt>


// <var_decl>           ::= “var” IDENTIFIER  <type> [ “[“ [ INT_LITERAL | IDENTIFIER ] “]”  ]  [ “=” <expr> ] “;”
// <type>                  ::= <type_spec> { “*” }
// <type_spec>        ::= “i64” 
//                               | “char” 
//                               | “struct” IDENTIFIER
// <expr>                  ::= IDENTIFIER | <literal> | <array>
// <array>                 ::= “[“ <expr> { “,” <expr> } “]” “;” 
// <literal>                ::= CHAR_LITERAL | INT_LITERAL | STRING_LITERAL



// [ “=” <expr> ] “;”
t_node  *parse_var_decl_assign(t_parser *prs)
{

}


// <var_decl>           ::= “var” IDENTIFIER  <type> [ “[“ [ INT_LITERAL | IDENTIFIER ] “]”  ]  [ “=” <expr> ] “;”
t_node  *parse_var_decl(t_parser *prs)
{
    t_token     *token;
    t_node      *node;
    char        *name;
    t_node      *type;

    token = parser_advance(prs);
    if (!token) 
        return NULL;

    if (token->type != TOKEN_KW_VAR) 
        return NULL;
    
    token = parser_advance(prs);
    if (!token)
        return NULL;

    if (token->type != TOKEN_IDENTIFIER)
        return NULL;

    name = strdup(token->value);
    if (name == NULL)
        return NULL;

    type = parse_type(prs);
    if (type == NULL)
        return NULL;

    token = parser_lookahead(prs);
    if (!token)
        return NULL;

    switch(token->type) 
    {
        case TOKEN_SEMICOLON: 
        {
            node = new_var_decl(name, type, NULL);
            break 
        }
        case TOKEN_OP_ASSIGN: 
        {

        }
    }

}


// <statement>        ::= <var_decl>
//                     | <return_stmt>
//                     | <if_stmt>
//                     | <while_stmt>
//                     | <expr_stmt>
//                     | <break_stmt>
//                     | <continue_stmt>
t_node  *parse_statement(t_parser *prs)
{
    t_token *token;

    token = parser_peek(prs);
    if (token == NULL)
        return (NULL);

    switch(token->type) {
        case TOKEN_KW_VAR: 

    }

}