
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



// <statement>        ::= <var_decl>
//                     | <return_stmt>
//                     | <if_stmt>
//                     | <while_stmt>
//                     | <expr_stmt>
//                     | <break_stmt>
//                     | <continue_stmt>
//                     | <function_call>

t_node  *parse_statement(t_parser *prs)
{
    t_token *token;
    t_node *node;

    node = NULL;
    token = parser_peek(prs);
    if (token == NULL)
        return (NULL);

    printf("here once more %s\n", token->value);
    if (token->type == TOKEN_KW_VAR) 
    {
        node = parse_var_decl(prs);
        if (node != NULL)
            print_ast(node, 1);
    }

    return node;
}


