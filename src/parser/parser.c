
#include "types.h"
#include "utils.h"
#include "parser.h"


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
        type == TOKEN_KW_WHILE ||
        type == TOKEN_IDENTIFIER);
}

// <function_decl>   ::= “func” <identifier> “(“ [ <param_list> ] “)”  “->” <type> <“{“ { <statement_list> } “}” 
t_node     *parse_function_decl(t_parser *prs)
{
    t_node  *node;
    t_node  *params;
    t_node  *body;
    t_node  *body_list;
    t_node *body_head = NULL;
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
    token = parser_advance(prs);
    if (token && token->type != TOKEN_IDENTIFIER) 
        return NULL;

    name = strdup(token->value);

    // func ID ( 
    token = parser_advance(prs); 
    if (token && token->type != TOKEN_L_PAREN)
        return NULL;

    token = parser_peek(prs);
    if (token && token->type == TOKEN_IDENTIFIER)
    {
        params = parse_parameter_list(prs);
        if (params == NULL)
            return NULL;
    }

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
    if (token == NULL)
        return NULL;
    
    if (token->type != TOKEN_TYPE_I64 && token->type != TOKEN_TYPE_CHAR)
        return NULL;

    return_type = strdup(token->value);

    // func ID ( params ) -> DATATYPE {
    token = parser_advance(prs);
    if (token && token->type != TOKEN_L_BRACE)
        return NULL;

    token = parser_peek(prs);
    if (token && is_statement_intro(token->type))
    {
        body_list = NULL;
        body_head = NULL;
        while (parser_peek(prs) && is_statement_intro(parser_peek(prs)->type))
        {       
            body = parse_statement(prs);
            if (token == NULL)
                return NULL;
            
            if (body_list == NULL)
            {
                body_list = body;
                body_head = body_list;
            }
            else 
            {
                body_list->next = body;
                body_list = body_list->next;
            }
        }
        token = parser_peek(prs);
        if (token == NULL)
            return NULL;
    }

    // func ID ( params ) -> DATATYPE {
    //      body
    // }
    if (token && token->type != TOKEN_R_BRACE) 
        return NULL; 


    node = new_func_decl(name, params, return_type, body_head);

    print_ast(node, 1);
    return node;
}

t_node     *parser(t_lexer *lx)
{
    t_parser    *prs;
    t_node      *node;
    t_token     *traverse;

    prs = init_parser(lx->head);
    while (prs->token)
    {
        traverse = prs->token;
        // Global level tokens
        if (traverse->type == TOKEN_KW_FUNC)
        {
            node = parse_function_decl(prs);
            if (node == NULL) 
            {
                printf("error : %s\n", parser_peek(prs)->value);
            }
            break;
        }

    }    

    return NULL;
}
