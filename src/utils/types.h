
#ifndef _UTILS_TYPES_H
#define _UTILS_TYPES_H

#include "compiler.h" // for token_type


typedef enum {
    TOKEN_KW_FUNC,
    TOKEN_KW_STRUCT,
    TOKEN_KW_VAR, 
    TOKEN_KW_RETURN,
    TOKEN_KW_IF,
    TOKEN_KW_ELSE,
    TOKEN_KW_WHILE,
    
    TOKEN_TYPE_I64,
    TOKEN_TYPE_CHAR,
    
    TOKEN_OP_ASSIGN,
    TOKEN_OP_PLUS,
    TOKEN_OP_MINUS,
    TOKEN_OP_STAR,
    TOKEN_OP_SLASH,
    TOKEN_OP_AMP,
    TOKEN_OP_AND,
    TOKEN_OP_ARROW,
    TOKEN_OP_BANG,

    TOKEN_OP_OR,
    TOKEN_OP_DOT,
    TOKEN_OP_EQ,
    TOKEN_OP_NEQ,
    TOKEN_OP_LT,
    TOKEN_OP_LTEQ,
    TOKEN_OP_GT,
    TOKEN_OP_GTEQ,

    TOKEN_L_PAREN,
    TOKEN_R_PAREN,
    TOKEN_L_BRACE,
    TOKEN_R_BRACE,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    TOKEN_COLON,

    TOKEN_IDENTIFIER,
    TOKEN_INT_LITERAL,
    TOKEN_STRING_LITERAL,
    TOKEN_CHAR_LITERAL
}           token_type;

typedef struct s_token 
{
    token_type      type;
    char            *value;
    int             line;
    int             column;
    struct s_token  *next;
}             t_token;

typedef struct s_sourcefile 
{
    char                *path;
    char                *buffer;
    size_t              buffer_size;
    struct s_sourcefile *next;
}   t_sourcefile;


typedef struct s_error_chain 
{
    char                    *line_message;
    char                    *code_block;
    int                     line;
    int                     column;
    int                     chain_index;
    struct s_error_chain *next;  
}   t_error_chain;

typedef struct {
    t_error_chain *head;
    t_error_chain *tail;
}   t_error_control;

typedef struct {
    t_token     *head;
    t_token     *tail;

    char            *file_path;
    int             line;
    int             column;
    size_t          current_position;
    t_error_control   *errors;
    t_token         *token;
}   t_lexer;

#endif 