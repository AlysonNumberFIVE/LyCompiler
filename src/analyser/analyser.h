#ifndef _ANALYSER_H
#define _ANALYSER_H

#include "parser.h"
#include "utils.h"

typedef struct s_type {
    char            *type_name;          
    int             pointer_level;   
    bool            is_const;
    struct s_node   *struct_def; 
    struct s_type   *next;
} t_datatype;

typedef struct s_symbol {
    char            *name;
    t_datatype      *type;
    int             scope_level;
    int             memory_offset; // For code generation, if needed
    bool            initialized;    // For variables, track if they've been initialized
    struct s_symbol *next; 
}   t_symbol;

typedef struct s_function_register {
    char            *name;
    t_datatype      *return_type;
    struct s_parameter_register *parameters;
    int             parameter_count;
    struct s_node   *declaration;
    struct s_function_register *next;
}   t_function_register;

typedef struct s_parameter_register {
    char                        *name;
    t_datatype                  *type;
    struct s_parameter_register *next;
}   t_parameter_register;

typedef struct s_struct_register {
    char                    *name;
    struct s_node           *declaration;
    struct s_struct_register *next;
}   t_struct_register;

typedef struct s_scope {
    int             scope_level;
    t_symbol        *symbols; // Linked list of symbols in this scope
    struct s_scope  *parent; // For nested scopes
    struct s_scope  *next; // For nested scopes
} t_scope;

typedef struct s_analysis_context {
    t_function_register *functions;
    t_struct_register   *structs;
    t_scope             *global_scope;
}   t_analysis_context;

t_function_register *push_function_register(
    t_function_register **head,
    char *name,
    t_datatype *return_type,
    t_parameter_register *parameters,
    t_node *declaration);

t_struct_register   *push_struct_register(
    t_struct_register **head,
    char *name,
    t_node *declaration);
    
t_datatype          *datatype_from_type_node(t_node *type_node);
bool                datatype_equals(t_datatype *left, t_datatype *right);
t_scope             *init_scope(int scope_level, t_scope *parent);
t_symbol            *push_symbol(
    t_symbol **head,
    char *name,
    t_datatype *type,
    int scope_level,
    bool initialized);
t_symbol            *find_symbol(t_symbol *head, char *name);
t_function_register *find_function_register(
    t_function_register *head,
    char *name);
t_struct_register   *find_struct_register(
    t_struct_register *head,
    char *name);
t_parameter_register *parameter_registers_from_ast(t_node *parameters);
bool                register_builtin_functions(
    t_function_register **functions,
    t_struct_register *structs,
    t_scope *global_scope);
bool                register_global_declarations(
    t_node *program,
    t_scope *global_scope,
    t_function_register *functions,
    t_struct_register *structs);
bool                register_top_level_declarations(
    t_node *program,
    t_function_register **functions,
    t_struct_register **structs,
    t_scope *global_scope);
t_node              *semantic_analyser(t_node *program);
bool                pre_register(
    t_node *program,
    t_function_register **functions,
    t_struct_register **structs,
    t_scope *global_scope);



#endif