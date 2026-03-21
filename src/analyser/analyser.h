#ifndef _ANALYSER_H
#define _ANALYSER_H

#include "parser.h"
#include "utils.h"

typedef struct s_type {
    char            *type_name;          
    int             pointer_level;   
    bool            is_const;
    struct s_node   *struct_def; 
} t_datatype;

typedef struct s_symbol {
    char            *name;
    t_datatype      *type;
    int             scope_level;
    int             memory_offset; // For code generation, if needed
    bool            initialized;    // For variables, track if they've been initialized
    struct s_symbol *next; 
}   t_symbol;

bool    push_symtab(t_symbol **head, char *name, t_datatype *type,
            int scope_level, int memory_offset, bool initialized);
void    free_symtab(t_symbol *head);

#endif