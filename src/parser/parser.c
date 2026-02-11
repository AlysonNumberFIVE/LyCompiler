
#include "types.h"
#include "utils.h"


t_parser    *init_parser(t_token *token)
{
    t_parser *prs;

    prs = (t_parser *)malloc(sizeof(t_parser));
    if (prs == NULL)
        return NULL;

    prs->token = token;
    prs->errors = NULL;
    prs->ast = NULL;
    return prs;
}

bool        is_statement_intro(token_type type)
{
    return (type == TOKEN_KW_VAR ||
        type == TOKEN_KW_RETURN || 
        type == TOKEN_KW_IF ||
        type == TOKEN_KW_WHILE);
}

// <function_decl>   ::= “func” <identifier> “(“ [ <param_list> ] “)”  “->” <type> <“{“ { <statement_list> } “}” 
t_node     *parse_function_decl(t_parser *prs)
{
    t_node  *node;
    t_node  *params;
    t_node  *body;
    t_token *token;
    char    *name;
    char    *return_type;

    body = NULL;
    params = NULL;

    // func 
    token = parser_advance(prs);
    if (token && token->type != TOKEN_KW_FUNC)
        return NULL;
    
    // func ID
    token = parser_advance(prs)
    if (token && token->type != TOKEN_IDENTIFIER) 
        return NULL;

    name = strdup(token->value);

    // func ID ( 
    token = parser_advance(prs); 
    if (token && token->type != TOKEN_L_PAREN)
        return NULL;

    
    token = parser_lookahead(prs);
    if (token && token->type == TOKEN_IDENTIFIER)
        printf("params = parse_parameter_list()\n");
        // params = parse_parameter_list()

    // func ID ( params ) 
    token = parser_advance(prs);    
    if (token && token->type != TOKEN_R_PAREN) 
        return NULL;

     // func ID ( params ) ->
    token = parser_advance(prs);
    if (token && token->type != TOKEN_OP_ARROW)
        return NULL;

    // func ID ( params ) -> DATATYPE
    token = parser_advance(prs);
    if (token && (token->type != TOKEN_TYPE_I64 && token->type != TOKEN_TYPE_CHAR))
        return NULL;

    return_type = strdup(token->value);

        // func IDENTIFIER ( ... ) -> DATATYPE {  
    token = parser_advance(prs);
    if (token && token->type != TOKEN_L_BRACE)
        return NULL;

    token = parser_advance(prs);
    if (token && is_statement_intro(token->type))
        printf("parse_statement()\n");
        // parse_statement()

    if (token && token->type != TOKEN_R_BRACE) 
        return NULL; 

    node = new_func_decl(name, params, return_type, body);
    return node;
}

t_node     *parser(t_lexer *lx)
{
    t_parser    *prs;
    t_token     *traverse;

    prs = init_parser(lx->head);
    while (prs->token)
    {
        traverse = prs->token;
        // Global level functions
        if (traverse->type == TOKEN_KW_FUNC)
        {
            parse_function_decl(prs);
            break;
        }

    }    

    return NULL;
}
