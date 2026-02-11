
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



typedef enum {
    NODE_PROGRAM,
    NODE_STRUCT_DECL, 
    
    NODE_FUNCTION_DECL,
    NODE_STRUCT_MEMBER,
    NODE_PARAM,

    NODE_VAR_DECL, 
    NODE_RETURN_STMT, 
    NODE_IF_STMT, 
    NODE_WHILE_STMT,
    NODE_BREAK_STMT, 
    NODE_CONTINUE_STMT, 
    NODE_EXPR_STMT,

    NODE_BINARY_EXPR,
    NODE_ASSIGNMENT, 
    NODE_MEMEBER_ACCESS,
    NODE_CALL, 
    NODE_STRUCT_INIT,
    NODE_STRUCT_FIELD,
    NODE_TYPE_SPEC,

    NODE_IDENTIFIER,
    NODE_LITERAL_INT,
    NODE_LITERAL_CHAR,
    NODE_LITERAL_STR
}   node_type;

typedef struct s_node {
    node_type type;
    union {
        struct {
            struct s_node   *struct_decl;
            struct s_node   *var_decl;
            struct s_node   *function_decl;
        }   program;

        struct {
            char            *name;
            struct s_node     *members;
        }   struct_decl; 

        struct {
            char            *name;
            struct s_node     *type;
        }   struct_member;

        struct {
            char              *name;
            struct s_node     *params;
            char              *return_type;
            struct s_node     *body;
        }   func_decl;

        struct {
            char            *name;
            struct s_node     *type;
        }   param;

        struct {
            char            *name;
            struct s_node     *type;
            struct s_node     *initializer;
        }   var_decl;

        struct {
            struct s_node *condition;
            struct s_node *then_branch;
            struct s_node *else_branch;
        }   if_stmt;

        struct {
            struct s_node *condition;
            struct s_node *body;
        }   while_stmt;

        struct {
            struct s_node *expression; // Opional (return ;)
        }   return_stmt;

        struct {
            char *name;  // required by C to have at least 1 member.
        }   continue_stmt;

        struct {
            char *name;
        }   break_stmt;

        struct {
            struct s_node *expression;
        }   expr_stmt;

        struct {
            struct s_node *left;
            char *op;
            struct s_node *right;
        }   binary_expr;

        struct {
            struct s_node *target;
            struct s_node *value;
        }   assignment;

        struct {
            struct s_node *callee;
            struct s_node *args;
        }   call;

        struct {
            struct s_node *struct_expr;
            char        *member_name;
            bool        is_arrow;
        }   member_access;

        struct {
            char            *struct_name;
            struct s_node     *fields;
        }   struct_initializer;

        struct {
            char            *name;
            struct s_node     *value;
        }   init_field;

        struct {
            long int_value;
            char *str_value;
            char char_value;
        }   literal;

        struct {
            char *name;
        }   identifier;

        struct {
            char *base_type;
            int pointer_level;
        }   type_spec;
    }   data;

    struct s_node *next; 
}   t_node;

typedef struct s_parser {
    t_token             *token;
    t_error_control     *errors;
    t_node              *ast;
}   t_parser;


#endif 