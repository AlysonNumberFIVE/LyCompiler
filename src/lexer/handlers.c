
#include "utils.h"

bool   isidentifier(char c) 
{
    return (c == '_' || isalpha(c));
}

bool    ismididentifier(char c)
{
    return (c == '_' || isalnum(c));
}

void handle_double(t_lexer *lx, 
        char *buffer, size_t length, char expected, 
        token_type double_type, char *double_lit,
        token_type single_type, char *single_lit)
{
    int start_col = lx->column;

    if (peek(lx, buffer, length) == expected) 
    {
        advance(lx, buffer, length); 
        push_token(lx, double_type, double_lit, lx->line, start_col);
    } 
    else 
        push_token(lx, single_type, single_lit, lx->line, start_col);
}



void    push_identifier(t_lexer *lx, char *token)
{
    if (strcmp(token, "func") == 0) 
        push_token(lx, TOKEN_KW_FUNC, token, lx->line, lx->column);
    else if (strcmp(token, "struct") == 0)
        push_token(lx, TOKEN_KW_STRUCT, token, lx->line, lx->column);
    else if (strcmp(token, "var") == 0)
        push_token(lx, TOKEN_KW_VAR, token, lx->line, lx->column);
    else if (strcmp(token, "return") == 0)
        push_token(lx, TOKEN_KW_RETURN, token, lx->line, lx->column);
    else if (strcmp(token, "if") == 0)
        push_token(lx, TOKEN_KW_IF, token, lx->line, lx->column);
    else if (strcmp(token, "else") == 0)
        push_token(lx, TOKEN_KW_ELSE, token, lx->line, lx->column);
    else if (strcmp(token, "while") == 0)
        push_token(lx, TOKEN_KW_WHILE, token, lx->line, lx->column);
    else if (strcmp(token, "i64") == 0)
        push_token(lx, TOKEN_TYPE_I64, token, lx->line, lx->column);
    else if (strcmp(token, "char") == 0)
        push_token(lx, TOKEN_TYPE_CHAR, token, lx->line, lx->column);   
    else
        push_token(lx, TOKEN_IDENTIFIER, token, lx->line, lx->column);
}


void handle_number(char first, t_lexer *lx, char *buffer, size_t length)
{
    char c;
    char *tmp;
    char *res;

    res = malloc(2);
    res[0] = first;
    res[1] = '\0';

    while (isdigit(peek(lx, buffer, length)))
    {        
        c = advance(lx, buffer, length);
        
        tmp = charpush(res, c);
        res = tmp;
    }
    push_token(lx, TOKEN_INT_LITERAL, res, lx->line, lx->column);
    free(res);
}


void handle_identifier(char first, t_lexer *lx, char *buffer, size_t length)
{
    char c;
    char *tmp;
    char *res;

    res = malloc(2);
    res[0] = first;
    res[1] = '\0';
    while (ismididentifier(peek(lx, buffer, length)))
    {        
        c = advance(lx, buffer, length);
        
        tmp = charpush(res, c);
        res = tmp;
    }
    push_identifier(lx, res);
    free(res);
}

void handle_chars(char first, t_lexer *lx, char *buffer, size_t length)
{
    char c;
    char *tmp;
    char *res;

    res = malloc(2);
    res[0] = first;
    res[1] = '\0';

    while (peek(lx, buffer, length) != '\0' && peek(lx, buffer, length) != '\'')
    {        
        c = advance(lx, buffer, length);
        
        if (c == '\\' && peek(lx, buffer, length) != '\0')
        {
            c = advance(lx, buffer, length);
        }

        tmp = charpush(res, c);
        res = tmp;
    }

    if (peek(lx, buffer, length) == '\'')
    {
        c = advance(lx, buffer, length);
        tmp = charpush(res, c);
        res = tmp;
    }
    push_token(lx, TOKEN_CHAR_LITERAL, res, lx->line, lx->column);
    free(res);
}


void handle_string(char first, t_lexer *lx, char *buffer, size_t length)
{
    char c;
    char *tmp;
    char *res;

    res = malloc(2);
    res[0] = first;
    res[1] = '\0';

    while (peek(lx, buffer, length) != '\0' && peek(lx, buffer, length) != '"')
    {        
        c = advance(lx, buffer, length);
        
        if (c == '\\' && peek(lx, buffer, length) != '\0')
        {
            c = advance(lx, buffer, length);
        }

        tmp = charpush(res, c);
        res = tmp;
    }

    if (peek(lx, buffer, length) == '"')
    {
        c = advance(lx, buffer, length);
        tmp = charpush(res, c);
        res = tmp;
    }
    push_token(lx, TOKEN_STRING_LITERAL, res, lx->line, lx->column);
    free(res);
}

void handle_errors(t_lexer *lx, char c)
{
    char str[4096];

    bzero((char*)str, 4096);
    sprintf(str, "%s:%d:%d error: use of invalid token: %c\n", lx->file_path, lx->line, lx->column, c);
    push_error(lx->errors, str, NULL, lx->line, lx->column);
    printf("%s\n", str);
}

void scan_token(t_lexer *lx, char *buffer, size_t length) 
{
    char c;

    c = advance(lx, buffer, length);
    switch(c) 
    {
        case '=': handle_double(lx, buffer, length, '=', TOKEN_OP_EQ, "==", TOKEN_OP_ASSIGN, "="); break;
        case '!': handle_double(lx, buffer, length, '=', TOKEN_OP_NEQ, "!=", TOKEN_OP_BANG, "!"); break;
        case '<': handle_double(lx, buffer, length, '=', TOKEN_OP_LTEQ, "<=", TOKEN_OP_LT, "<"); break;
        case '>': handle_double(lx, buffer, length, '=', TOKEN_OP_GTEQ, ">=", TOKEN_OP_GT, ">"); break;
        case '-': handle_double(lx, buffer, length, '>', TOKEN_OP_ARROW, "->", TOKEN_OP_MINUS, "-"); break;
        case '&': handle_double(lx, buffer, length, '&', TOKEN_OP_AMP, "&&", TOKEN_OP_AND, "&"); break;
        case '{': push_token(lx, TOKEN_L_BRACE, "{", lx->line, lx->column); break;
        case '}': push_token(lx, TOKEN_R_BRACE, "}", lx->line, lx->column); break;
        case '(': push_token(lx, TOKEN_L_PAREN, "(", lx->line, lx->column); break;
        case ')': push_token(lx, TOKEN_R_PAREN, ")", lx->line, lx->column); break;  
        case '[': push_token(lx, TOKEN_L_BLOCK, "[", lx->line, lx->column); break;
        case ']': push_token(lx, TOKEN_R_BLOCK, "]", lx->line, lx->column); break;
        case ',': push_token(lx, TOKEN_COMMA, ",", lx->line, lx->column); break;
        case ';': push_token(lx, TOKEN_SEMICOLON, ";", lx->line, lx->column); break;
        case '+': push_token(lx, TOKEN_OP_PLUS, "+", lx->line, lx->column); break;
        case '*': push_token(lx, TOKEN_OP_STAR, "*", lx->line, lx->column); break;
        case '/': push_token(lx, TOKEN_OP_SLASH, "/", lx->line, lx->column); break;
        case ':': push_token(lx, TOKEN_COLON, ":", lx->line, lx->column); break;
        case '.': push_token(lx, TOKEN_OP_DOT, ".", lx->line, lx->column); break;
        case '\'': handle_chars('\'', lx, buffer, length); break;
        case '"': handle_string('"', lx, buffer, length); break;
        default:
            if (isidentifier(c))
                handle_identifier(c, lx, buffer, length);
            else if (isdigit(c))
                handle_number(c, lx, buffer, length);
            else
                handle_errors(lx, c);
    }
}