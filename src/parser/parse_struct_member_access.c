
#include "types.h"
#include "utils.h"


// <struct_mem_access>        ::= IDENTIFIER  ['.' | '->' ] IDENTIFIER 
t_node  *parse_struct_member(t_parser *prs)
{
    t_token     *token; 
    t_node      *node; 

    // skip over IDENTIFIER checked in function above;
    token = parser_advance(prs);
    if (token == NULL)
        return NULL;

    
}