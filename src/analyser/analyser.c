
#include "analyser.h"

bool    pre_register(t_node *program, t_function_register **functions,
    t_struct_register **structs, t_scope *global_scope)
{
    if (program == NULL || functions == NULL || structs == NULL || global_scope == NULL)
        return false;
    
    return register_top_level_declarations(program, functions, structs, global_scope);
}

t_node      *semantic_analyser(t_node *program)
{
    t_function_register     *functions;
    t_struct_register       *structs;
    t_scope                 *global_scope;

    functions = NULL;
    structs = NULL;
    global_scope = NULL;
    if (program == NULL)
        return NULL;

    global_scope = init_scope(0, NULL);
    if (global_scope == NULL)
        return NULL;

    if (!pre_register(program, &functions, &structs, global_scope))
        return NULL;

    
    return program;
}