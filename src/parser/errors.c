
#include "types.h"
#include "utils.h"

bool        recovery_tokens(token_type type)
{
    return (type == TOKEN_KW_VAR ||
        type == TOKEN_KW_FUNC ||
        type == TOKEN_KW_IF ||
        type == TOKEN_KW_WHILE || 
        type == TOKEN_KW_RETURN);
}    

void        traverse_to_recovery_token(t_parser *prs)
{
    t_token *traverse;

    traverse = parser_peek(prs);
    while (traverse && !recovery_tokens(traverse->type))
        traverse = parser_advance(prs);
}

void        error_mode(t_parser *prs)
{
    
}
