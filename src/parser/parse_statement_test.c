#include "types.h"
#include "utils.h"
#include "parser.h"

// var variable i64;
t_lexer *create_valid_variable_semicolon(void)
{
    t_lexer *lexer;

    lexer = init_lexer();
    push_token(lexer, TOKEN_KW_VAR, "var", 0, 0);
    push_token(lexer, TOKEN_IDENTIFIER, "variable", 0, 4);
    push_token(lexer, TOKEN_TYPE_I64, "i64", 0, 0);
    push_token(lexer, TOKEN_SEMICOLON, ";", 0, 0);
    return lexer;
}

t_lexer *create_valid_expression(void)
{
    t_lexer *lexer;

    lexer = init_lexer();
    push_token(lexer, TOKEN_OP_ASSIGN, "=", 0, 0);
    push_token(lexer, TOKEN_INT_LITERAL, "42", 1, 0); 
    push_token(lexer, TOKEN_OP_PLUS, "+", 2, 0);
    push_token(lexer, TOKEN_INT_LITERAL, "11", 3, 0);
    push_token(lexer, TOKEN_OP_STAR, "*", 4, 0); 
    push_token(lexer, TOKEN_INT_LITERAL, "121", 5, 0);
    push_token(lexer, TOKEN_OP_STAR, "*", 6, 0); 
    // push_token(lexer, TOKEN_L_PAREN, "(", 7, 0);
    push_token(lexer, TOKEN_INT_LITERAL, "4444", 0 ,0);
    push_token(lexer, TOKEN_OP_STAR, "*", 0, 0);
    push_token(lexer, TOKEN_INT_LITERAL, "1231", 0, 0);
    push_token(lexer, TOKEN_OP_MINUS, "-", 0, 0); 
    // push_token(lexer, TOKEN_R_PAREN, ")", 0, 0);
    push_token(lexer, TOKEN_INT_LITERAL, "10", 0, 0);
    push_token(lexer, TOKEN_SEMICOLON, ";", 0, 0); 
    return lexer;
}

int main(void)
{
    t_lexer *lexer;
    t_parser *parser;
    t_token *token;

    lexer = create_valid_variable_semicolon();

    parser = init_parser(lexer->head);
    parse_statement(parser);

    token = parser_peek(parser);
    if (!token)
        printf("token empty");
    else 
        printf("token value after parse_statement : %s\n", token->value);

    
    lexer = create_valid_expression();

    parser = init_parser(lexer->head);
    parse_assignment(parser);


    token = parser_peek(parser);
    if (!token)
        printf("parse_assignment:token empty");
    else 
        printf("token value after parse_assignment : %s\n", token->value);


    return 0;
}

