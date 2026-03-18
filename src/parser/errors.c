
#include "types.h"
#include "utils.h"

bool        recovery_tokens(token_type type)
{
    return (type == TOKEN_KW_VAR ||
        type == TOKEN_KW_FUNC ||
        type == TOKEN_KW_IF ||
        type == TOKEN_KW_WHILE || 
        type == TOKEN_KW_RETURN ||
        type == TOKEN_KW_FUNC);
}    

t_token    *search_for_recovery(t_parser *prs)
{
    t_token *token;

    while (parser_peek(prs) && !recovery_tokens(parser_peek(prs)->type))
    {
        token = parser_advance(prs);
        if (token == NULL)
            return NULL;
    }

 //   token = parser_advance(prs);
    return token;
}
