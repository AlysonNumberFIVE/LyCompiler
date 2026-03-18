
#include "types.h"
#include "utils.h"
#include "parser.h"
#include <stdlib.h>

// Only stop at top-level declaration boundaries so recovery inside a broken
// function does not halt mid-way at a 'return' or 'if' keyword.
static bool recovery_tokens(token_type type)
{
    return (type == TOKEN_KW_FUNC || type == TOKEN_KW_STRUCT);
}

t_token    *search_for_recovery(t_parser *prs)
{
    t_token *token;

    token = NULL;
    while (parser_peek(prs) && !recovery_tokens(parser_peek(prs)->type))
    {
        token = parser_advance(prs);
        if (token == NULL)
            return NULL;
    }
    return token;
}

// Record a parse error in the error chain.  If 20 errors have accumulated the
// whole chain is printed immediately and the process exits.  Always returns
// NULL so callers can write: return parser_error(prs, "msg", token);
t_node  *parser_error(t_parser *prs, char *message, t_token *token)
{
    int line;
    int col;

    line = token ? token->line   : 0;
    col  = token ? token->column : 0;

    if (!prs->errors)
        return NULL;

    push_error(prs->errors, message, NULL, line, col);

    if (prs->errors->count >= 20)
    {
        print_errors(prs->errors);
        exit(1);
    }
    return NULL;
}

void    print_parse_errors(t_parser *prs)
{
    if (!prs || !prs->errors)
        return;
    print_errors(prs->errors);
}

