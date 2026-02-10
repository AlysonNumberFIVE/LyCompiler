
#include "types.h"
#include "utils.h"

// BNF Definition: 
// <var_decl>               ::= “var” IDENTIFIER  <type> [ “=” <expr> ] “;”
// <type>                   ::= <type_spec> { “*” }
// <type_spec>              ::= “i64” 
//                          | “char” 
//                          | “struct” IDENTIFIER
// <expr>                   ::= IDENTIFIER 
//                          | <literal> 
// <literal>                ::= CHAR_LITERAL 
//                          | INT_LITERAL 
//                          | STRING_LITERAL


t_parser    *init_parser(t_token *tokens)
{
    t_parser *psr;

    psr = (t_parser *)malloc(sizeof(t_parser));
    if (parser == NULL)
        return NULL;

    psr->token = tokens;
    psr->errors = NULL;
    psr->ast = NULL;
    return pr;
}


t_node     *parser(t_lexer *lx)
{
    t_parser    *psr;

    psr = init_parser(lx->tokens);
    

    return NULL;
}
