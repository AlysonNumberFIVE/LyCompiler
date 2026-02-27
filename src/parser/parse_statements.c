
#include "types.h"
#include "utils.h"
#include "parser.h"

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
    t_token *token;

    token = parser_advance(prs);
    if (token == NULL)
        return NULL;

    return NULL;
}


// <var_decl>           ::= “var” IDENTIFIER  <type> [ “[“ [ INT_LITERAL | IDENTIFIER ] “]”  ]  [ “=” <expr> ] “;”
t_node  *parse_var_decl(t_parser *prs)
{
    t_token     *token;
    t_node      *node;
    t_node      *initializer;
    char        *name;
    t_node      *type;

    node = NULL;
    initializer = NULL;
    token = parser_advance(prs);
    if (!token) 
        return NULL;

    printf("var_decl token is %s\n", token->value);
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

    token = parser_peek(prs);
    if (token == NULL)
        return NULL;
    
    if (token->type == TOKEN_OP_ASSIGN) 
    {
        initializer = parse_assignment(prs);
    } 

    token = parser_peek(prs);
    if (token == NULL)
        return NULL;

    node = new_var_decl(name, type, initializer);
    if (token->type == TOKEN_SEMICOLON) 
    {
        token = parser_advance(prs);
        if (token == NULL)
            return node;
    }
    else 
    {
        printf("error\n");
    }

    return node;
}



// <statement>        ::= <var_decl>        DONE
//                     | <return_stmt>      DONE
//                     | <if_stmt>          DONE
//                     | <while_stmt>       DONE 
//                     | <expr_stmt>        
//                     | <break_stmt>       DONE
//                     | <continue_stmt>    DONE
//                     | <function_call>    DONE
t_node  *parse_statement(t_parser *prs)
{
    t_token *token;
    t_node *node;

    node = NULL;
    token = parser_peek(prs);
    if (token == NULL)
        return (NULL);

    printf("token type KW _________ %s\n", token->value);
    if (token->type == TOKEN_KW_VAR) 
    {
        node = parse_var_decl(prs);      
    }
    else if (token->type == TOKEN_KW_IF) 
    {
        node = parse_if_statement(prs);
    }
    else if (token->type == TOKEN_IDENTIFIER) 
    {  
        printf("LOGICAL\n");
        node = parse_logical_or(prs);
        parser_advance(prs);
    }
    printf("BEFORE EXITING PARSE_STATEMENT %s\n", parser_peek(prs)->value);
    return node;
}


