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
    return 0;
}

