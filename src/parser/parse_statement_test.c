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
    push_token(lexer, TOKEN_OP_MINUS, "-", 0, 0); 
    push_token(lexer, TOKEN_IDENTIFIER, "function_call", 0, 0);
    push_token(lexer, TOKEN_L_PAREN, "(", 7, 0);  
    push_token(lexer, TOKEN_INT_LITERAL, "1231", 0, 0);
    push_token(lexer, TOKEN_COMMA, ",", 0, 0);  
    push_token(lexer, TOKEN_INT_LITERAL, "1321", 0, 0);
    push_token(lexer, TOKEN_COMMA, ",", 0, 0);  
    push_token(lexer, TOKEN_INT_LITERAL, "11", 0, 0);  
    push_token(lexer, TOKEN_OP_STAR, "*", 0, 0); 
    push_token(lexer, TOKEN_INT_LITERAL, "2", 0, 0);  
    push_token(lexer, TOKEN_R_PAREN, ")", 0, 0);  
    push_token(lexer, TOKEN_SEMICOLON, ";", 0, 0); 
    return lexer;
}

t_lexer *create_valid_expression_with_struct_access(void)
{
    t_lexer *lexer;

    lexer = init_lexer();
    push_token(lexer, TOKEN_OP_ASSIGN, "=", 0, 0);
    push_token(lexer, TOKEN_INT_LITERAL, "42", 1, 0); 
    push_token(lexer, TOKEN_OP_PLUS, "+", 2, 0);
    push_token(lexer, TOKEN_INT_LITERAL, "11", 3, 0);
    push_token(lexer, TOKEN_OP_STAR, "*", 4, 0); 
    push_token(lexer, TOKEN_IDENTIFIER, "value", 5, 0);
    push_token(lexer, TOKEN_OP_ARROW, "->", 0, 0);
    push_token(lexer, TOKEN_IDENTIFIER, "againx", 0, 0);
    push_token(lexer, TOKEN_OP_ARROW, "->", 0, 0);
    push_token(lexer, TOKEN_IDENTIFIER, "againx", 0, 0);
    push_token(lexer, TOKEN_SEMICOLON, ";", 0, 0); 
    return lexer;
}

t_lexer *create_valid_expression_with_array_access(void)
{
    t_lexer *lexer;

    
    // 42 + 11 * value->againx[42]->second * X;
    lexer = init_lexer();
    push_token(lexer, TOKEN_OP_ASSIGN, "=", 0, 0);
    push_token(lexer, TOKEN_INT_LITERAL, "42", 1, 0); 
    push_token(lexer, TOKEN_OP_PLUS, "+", 2, 0);
    push_token(lexer, TOKEN_INT_LITERAL, "11", 3, 0);
    push_token(lexer, TOKEN_OP_STAR, "*", 4, 0); 
    push_token(lexer, TOKEN_IDENTIFIER, "value", 5, 0);
    push_token(lexer, TOKEN_OP_ARROW, "->", 0, 0);
    push_token(lexer, TOKEN_IDENTIFIER, "againx", 0, 0);
    push_token(lexer, TOKEN_L_BLOCK, "[", 0, 0); 
    push_token(lexer, TOKEN_INT_LITERAL, "42", 0, 0); 
    push_token(lexer, TOKEN_R_BLOCK, "]", 0, 0); 
    push_token(lexer, TOKEN_OP_ARROW, "->", 0, 0);
    push_token(lexer, TOKEN_IDENTIFIER, "second", 0, 0);
    push_token(lexer, TOKEN_OP_STAR, "*", 2, 0);
    push_token(lexer, TOKEN_IDENTIFIER, "X", 3, 0);
    push_token(lexer, TOKEN_SEMICOLON, ";", 0, 0); 
    return lexer;
}


t_lexer *create_expression_with_braces_lexer(void)
{
    t_lexer *lexer;

    lexer = init_lexer();
    push_token(lexer, TOKEN_OP_ASSIGN, "=", 0, 0);
    // Expression: 1 + b * (a / 2) + c * 42;
    
    // 1 +
    push_token(lexer, TOKEN_INT_LITERAL, "1", 0, 0);
    push_token(lexer, TOKEN_OP_PLUS, "+", 0, 0);

    // b *
    push_token(lexer, TOKEN_IDENTIFIER, "b", 0, 0);
    push_token(lexer, TOKEN_OP_STAR, "*", 0, 0);

    // (a / 2)
    push_token(lexer, TOKEN_L_PAREN, "(", 0, 0);
    push_token(lexer, TOKEN_IDENTIFIER, "a", 0, 0);
    push_token(lexer, TOKEN_OP_SLASH, "/", 0, 0);
    push_token(lexer, TOKEN_INT_LITERAL, "2", 0, 0);
    push_token(lexer, TOKEN_R_PAREN, ")", 0, 0);

    // + c
    push_token(lexer, TOKEN_OP_PLUS, "+", 0, 0);
    push_token(lexer, TOKEN_IDENTIFIER, "c", 0, 0);

    // * 42;
    push_token(lexer, TOKEN_OP_STAR, "*", 0, 0);
    push_token(lexer, TOKEN_INT_LITERAL, "42", 0, 0);
    push_token(lexer, TOKEN_SEMICOLON, ";", 0, 0);

    return lexer;
}

t_lexer *create_expression_function_call(void)
{
    t_lexer *lexer;

    // printf("Hello %s ", Compiler);
    lexer = init_lexer(); 
    push_token(lexer, TOKEN_IDENTIFIER, "printf", 0, 0);
    push_token(lexer, TOKEN_L_PAREN, "(", 0, 0);
    push_token(lexer, TOKEN_STRING_LITERAL, "\"Hello %s \"", 0 ,0);
    push_token(lexer, TOKEN_COMMA, ",", 0, 0); 
    push_token(lexer, TOKEN_IDENTIFIER, "Compiler", 0, 0);
    push_token(lexer, TOKEN_COMMA, ",", 0, 0); 
    push_token(lexer, TOKEN_INT_LITERAL, "42", 0 , 0);
    push_token(lexer, TOKEN_R_PAREN, ")", 0, 0);
    push_token(lexer, TOKEN_SEMICOLON, ";", 0, 0);
    return lexer;
}


t_lexer *create_expression_function_call_with_empty_body(void)
{
    t_lexer *lexer;

    // function_call();
    lexer = init_lexer(); 
    push_token(lexer, TOKEN_IDENTIFIER, "function_call", 0, 0);
    push_token(lexer, TOKEN_L_PAREN, "(", 0, 0);
    push_token(lexer, TOKEN_R_PAREN, ")", 0, 0);
    push_token(lexer, TOKEN_SEMICOLON, ";", 0, 0);
    return lexer;
}


t_lexer *create_expression_if_stmt(void)
{
    t_lexer *lexer;

    // if ( value == 42) {
    //      var variable i64;
    // } else if (xvalue == 21) {
    //      var variable i64;
    // } else if (zvalue != 11) {
    //      var variable i64;   
    // }
    lexer = init_lexer(); 
    push_token(lexer,  TOKEN_KW_IF, "if", 0, 0);
    push_token(lexer, TOKEN_L_PAREN, "(", 0, 0);
    push_token(lexer, TOKEN_IDENTIFIER, "value", 0, 0);
    push_token(lexer, TOKEN_OP_EQ, "==", 0, 0);
    push_token(lexer, TOKEN_INT_LITERAL, "42", 0, 0);
    push_token(lexer, TOKEN_R_PAREN, ")", 0, 0);
    push_token(lexer, TOKEN_L_BRACE, "{", 0, 0);
 
    push_token(lexer, TOKEN_KW_VAR, "var", 0, 0);
    push_token(lexer, TOKEN_IDENTIFIER, "variable", 0, 4);
    push_token(lexer, TOKEN_TYPE_I64, "i64", 0, 0);
    push_token(lexer, TOKEN_SEMICOLON, ";", 0, 0);
    push_token(lexer, TOKEN_R_BRACE, "}", 0, 0);

    push_token(lexer, TOKEN_KW_ELSE, "else", 0, 0);
    push_token(lexer, TOKEN_KW_IF, "if", 0, 0);
    push_token(lexer, TOKEN_L_PAREN, "(", 0, 0);
    push_token(lexer, TOKEN_IDENTIFIER, "xvalue", 0, 0);
    push_token(lexer, TOKEN_OP_EQ, "==", 0, 0);
    push_token(lexer, TOKEN_INT_LITERAL, "21", 0, 0);
    push_token(lexer, TOKEN_R_PAREN, ")", 0, 0);
    push_token(lexer, TOKEN_L_BRACE, "{", 0, 0);
 
    push_token(lexer, TOKEN_KW_VAR, "var", 0, 0);
    push_token(lexer, TOKEN_IDENTIFIER, "variable", 0, 4);
    push_token(lexer, TOKEN_TYPE_CHAR, "i64", 0, 0);
    push_token(lexer, TOKEN_SEMICOLON, ";", 0, 0);
    push_token(lexer, TOKEN_R_BRACE, "}", 0, 0);

    push_token(lexer, TOKEN_KW_ELSE, "else", 0, 0);
    push_token(lexer, TOKEN_KW_IF, "if", 0, 0);
    push_token(lexer, TOKEN_L_PAREN, "(", 0, 0);
    push_token(lexer, TOKEN_IDENTIFIER, "zvalue", 0, 0);
    push_token(lexer, TOKEN_OP_NEQ, "!=", 0, 0);
    push_token(lexer, TOKEN_INT_LITERAL, "11", 0, 0);
    push_token(lexer, TOKEN_R_PAREN, ")", 0, 0);
    push_token(lexer, TOKEN_L_BRACE, "{", 0, 0);
 
    push_token(lexer, TOKEN_KW_VAR, "var", 0, 0);
    push_token(lexer, TOKEN_IDENTIFIER, "variable", 0, 4);
    push_token(lexer, TOKEN_TYPE_CHAR, "i64", 0, 0);
    push_token(lexer, TOKEN_SEMICOLON, ";", 0, 0);
    push_token(lexer, TOKEN_R_BRACE, "}", 0, 0);
    return lexer;
}

t_lexer     *create_while_statement(void)
{
    t_lexer *lexer;

    // while (xvalue == 21) {
    //      var variable i64;
    //      var vble i64;
    // }
    lexer = init_lexer(); 
    push_token(lexer, TOKEN_KW_WHILE, "while", 0, 0);
    push_token(lexer, TOKEN_L_PAREN, "(", 0, 0);
    push_token(lexer, TOKEN_IDENTIFIER, "xvalue", 0, 0);
    push_token(lexer, TOKEN_OP_EQ, "==", 0, 0);
    push_token(lexer, TOKEN_INT_LITERAL, "21", 0, 0);
    push_token(lexer, TOKEN_R_PAREN, ")", 0, 0);
    push_token(lexer, TOKEN_L_BRACE, "{", 0, 0);
 
    push_token(lexer, TOKEN_KW_VAR, "var", 0, 0);
    push_token(lexer, TOKEN_IDENTIFIER, "variable", 0, 4);
    push_token(lexer, TOKEN_TYPE_CHAR, "i64", 0, 0);
    push_token(lexer, TOKEN_SEMICOLON, ";", 0, 0);

    push_token(lexer, TOKEN_KW_VAR, "var", 0, 0);
    push_token(lexer, TOKEN_IDENTIFIER, "vble", 0, 4);
    push_token(lexer, TOKEN_TYPE_CHAR, "i64", 0, 0);
    push_token(lexer, TOKEN_SEMICOLON, ";", 0, 0);

    push_token(lexer, TOKEN_R_BRACE, "}", 0, 0); 
    return lexer; 
}

t_lexer     *create_return_stmt(void)
{
    t_lexer *lexer;

    // return xvalue == 21;
    lexer = init_lexer();
    push_token(lexer, TOKEN_KW_RETURN, "return", 0, 0);
    push_token(lexer, TOKEN_IDENTIFIER, "xvalue", 0, 0);
    push_token(lexer, TOKEN_OP_EQ, "==", 0, 0);
    push_token(lexer, TOKEN_INT_LITERAL, "21", 0, 0);
    push_token(lexer, TOKEN_SEMICOLON, ";", 0, 0); 
    return lexer;
}

int main(void)
{
    t_lexer     *lexer;
    t_parser    *parser;
    t_token     *token;
    t_node      *top;

    // lexer = create_valid_variable_semicolon();

    // parser = init_parser(lexer->head);
    // parse_statement(parser);

    // token = parser_peek(parser);
    // if (!token)
    //     printf("token empty");
    // else 
    //     printf("token value after parse_statement : %s\n", token->value);

    
    lexer = create_valid_expression();

    parser = init_parser(lexer->head);
    top = parse_assignment(parser);


    token = parser_peek(parser);
    if (!token)
        printf("parse_assignment:token empty");
    else 
        printf("token value after parse_assignment : %s\n", token->value);

    printf("======================\n");
    print_ast(top, 1);


    // lexer = create_valid_expression_with_struct_access();

    // parser = init_parser(lexer->head);
    // parse_assignment(parser);

    // token = parser_peek(parser);
    // if (!token)
    //     printf("parse_assignment:token empty");
    // else 
    //     printf("token value after parse_assignment : %s\n", token->value);


    // lexer = create_valid_expression_with_array_access();

    // parser = init_parser(lexer->head);
    // parse_assignment(parser);

    // token = parser_peek(parser);
    // if (!token)
    //     printf("parse_assignment:token empty");
    // else 
    //     printf("token value after parse_assignment : %s\n", token->value);


    // lexer = create_expression_with_braces_lexer();

    // parser = init_parser(lexer->head);
    // parse_assignment(parser);

    // token = parser_peek(parser);
    // if (!token)
    //     printf("parse_assignment:token empty");
    // else 
    //     printf("token value after parse_assignment : %s\n", token->value);


    lexer = create_expression_function_call();

    parser = init_parser(lexer->head);
    top = parse_func_call(parser);

    token = parser_peek(parser);
    if (!token)
        printf("parse_func_call:token empty");
    else 
        printf("token value after parse_func_call : %s\n", token->value);
    printf("======================\n");
    print_ast(top, 1);


    lexer = create_expression_if_stmt();

    parser = init_parser(lexer->head);
    top = parse_if_statement(parser);

    token = parser_peek(parser);
    if (!token)
        printf("parse_func_call:token empty");
    else 
        printf("token value after parse_if_statement : %s\n", token->value);
    printf("======================\n");
    print_ast(top, 1);


    // lexer = create_while_statement();

    // parser = init_parser(lexer->head);
    // top = parse_while_stmt(parser);

    // token = parser_peek(parser);
    // if (!token)
    //     printf("parse_func_call:token empty");
    // else 
    //     printf("token value after parse_if_statement : %s\n", token->value);
    // printf("======================\n");
    // print_ast(top, 1);


    // lexer = create_return_stmt();

    // parser = init_parser(lexer->head);
    // top = parse_return_stmt(parser);

    // token = parser_peek(parser);
    // if (!token)
    //     printf("parse_func_call:token empty");
    // else 
    //     printf("token value after parse_if_statement : %s\n", token->value);
    // printf("======================\n");
    // print_ast(top, 1);


    // lexer = create_expression_function_call_with_empty_body();

    // parser = init_parser(lexer->head);
    // top = parse_func_call(parser);

    // token = parser_peek(parser);
    // if (!token)
    //     printf("parse_func_call:token empty");
    // else 
    //     printf("token value after parse_func_call : %s\n", token->value);
    // printf("======================\n");   
    // print_ast(top, 1);

    return 0;
}

