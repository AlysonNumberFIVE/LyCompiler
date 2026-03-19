
#include "types.h"
#include "utils.h"
#include "parser.h"


t_parser    *init_parser(t_token *token)
{
    t_parser *prs;

    prs = (t_parser *)malloc(sizeof(t_parser));
    if (prs == NULL)
        return NULL;

    prs->token = token;
    prs->errors = init_error_control();
    prs->ast = NULL;
    return prs;
}

bool        is_statement_intro(token_type type)
{
    return (type == TOKEN_KW_VAR ||
        type == TOKEN_KW_RETURN || 
        type == TOKEN_KW_IF ||
        type == TOKEN_KW_WHILE ||
        type == TOKEN_KW_BREAK ||
        type == TOKEN_KW_CONTINUE ||
        type == TOKEN_IDENTIFIER);
}

// <function_decl>   ::= “func” <identifier> “(“ [ <param_list> ] “)”  “->” <type> <“{“ { <statement_list> } “}” 
t_node     *parse_function_decl(t_parser *prs)
{
    t_node  *node;
    t_node  *params;
    t_node  *body;
    t_node  *body_list;
    t_node *body_head = NULL;
    t_token *token;
    char    *name;
    char    *return_type;

    body = NULL;
    params = NULL;

    // func 
    token = parser_advance(prs);
    if (token == NULL)
        return parser_error(prs, "expected 'func'", prs->token);
    if (token->type != TOKEN_KW_FUNC)
        return parser_error(prs, "expected 'func'", token);

    // func ID
    token = parser_advance(prs);
    if (token == NULL)
        return parser_error(prs, "expected identifier after 'func'", prs->token);
    if (token->type != TOKEN_IDENTIFIER) 
        return parser_error(prs, "expected identifier after 'func'", token);

    name = strdup(token->value);

    // func ID ( 
    token = parser_advance(prs); 
    if (token == NULL)
        return parser_error(prs, "expected '(' after function name", prs->token);
    if (token->type != TOKEN_L_PAREN)
        return parser_error(prs, "expected '(' after function name", token);

    token = parser_peek(prs);
    if (token && token->type == TOKEN_IDENTIFIER)
    {
        params = parse_parameter_list(prs);
        if (params == NULL)
            return parser_error(prs, "error parsing parameter list", parser_peek(prs));
    }

    // func ID ( params ) 
    token = parser_advance(prs);    
    if (token == NULL)
        return parser_error(prs, "expected ')' after parameter list", prs->token);
    if (token->type != TOKEN_R_PAREN) 
        return parser_error(prs, "expected ')' after parameter list", token);

     // func ID ( params ) ->
    token = parser_advance(prs);
    if (token == NULL)
        return parser_error(prs, "expected '->' before return type", prs->token);
    if (token->type != TOKEN_OP_ARROW)
        return parser_error(prs, "expected '->' before return type", token);

    // func ID ( params ) -> DATATYPE
    token = parser_advance(prs);
    if (token == NULL)
        return parser_error(prs, "expected return type", prs->token);

    if (token->type != TOKEN_TYPE_I64 && token->type != TOKEN_TYPE_CHAR && token->type != TOKEN_IDENTIFIER)
        return parser_error(prs, "expected return type (i64, char, or identifier)", token);

    // Build return_type string, consuming any trailing '*' for pointer types
    {
        char    *base = token->value;
        size_t  base_len = strlen(base);
        int     ptr_level = 0;

        while (parser_peek(prs) && parser_peek(prs)->type == TOKEN_OP_STAR)
        {
            parser_advance(prs);
            ptr_level++;
        }
        return_type = malloc(base_len + ptr_level + 1);
        if (return_type == NULL)
            return NULL;
        strcpy(return_type, base);
        
        for (int i = 0; i < ptr_level; i++)
            return_type[base_len + i] = '*';
        return_type[base_len + ptr_level] = '\0';
    }

    // func ID ( params ) -> DATATYPE {
    token = parser_advance(prs);
    if (token == NULL)
        return parser_error(prs, "expected '{' to open function body", prs->token);
    if (token->type != TOKEN_L_BRACE)
        return parser_error(prs, "expected '{' to open function body", token);

    token = parser_peek(prs);
    if (token && is_statement_intro(token->type))
    {
        body_list = NULL;
        body_head = NULL;
        while (parser_peek(prs) && is_statement_intro(parser_peek(prs)->type))
        {       
            body = parse_statement(prs);
            if (token == NULL)
                return NULL;
            
            if (body_list == NULL)
            {
                body_list = body;
                body_head = body_list;
            }
            else 
            {
                body_list->next = body;
                body_list = body_list->next;
            }
        }
        token = parser_peek(prs);
        if (token == NULL)
            return parser_error(prs, "unexpected end of file in function body", prs->token);
    }

    // func ID ( params ) -> DATATYPE {
    //      body
    // }
    if (token == NULL)
        return parser_error(prs, "expected '}' to close function body", prs->token);
    if (token->type != TOKEN_R_BRACE) 
        return parser_error(prs, "expected '}' to close function body", token);


    node = new_func_decl(name, params, return_type, body_head);

    parser_advance(prs); // consume '}' — already verified above
    return node;
}


t_node     *parser(t_lexer *lx)
{
    t_parser    *prs;
    t_node      *program;
    t_node      *parsed;
    t_node      *func_head;
    t_node      *func_tail;
    t_node      *struct_head;
    t_node      *struct_tail;
    t_token     *traverse;

    func_head   = NULL;
    func_tail   = NULL;
    struct_head = NULL;
    struct_tail = NULL;

    program = init_program();
    if (program == NULL)
        return NULL;

    prs = init_parser(lx->head);
    while (prs->token)
    {
        traverse = parser_peek(prs);
        if (traverse->type == TOKEN_EOF)
            break;
        if (traverse->type == TOKEN_KW_FUNC)
        {
            parsed = parse_function_decl(prs);
            if (parsed == NULL || parsed->type == NODE_ERROR)
                search_for_recovery(prs);
            else
            {
                parsed->next = NULL;
                if (func_head == NULL)
                {
                    func_head = parsed;
                    func_tail = parsed;
                }
                else
                {
                    func_tail->next = parsed;
                    func_tail = parsed;
                }
            }
        }
        else if (traverse->type == TOKEN_KW_STRUCT)
        {
            parsed = parse_struct(prs);
            if (parsed == NULL)
                break;
            parsed->next = NULL;
            if (struct_head == NULL)
            {
                struct_head = parsed;
                struct_tail = parsed;
            }
            else
            {
                struct_tail->next = parsed;
                struct_tail = parsed;
            }
        }
        else if (traverse->type == TOKEN_KW_VAR)
        {
            parse_var_decl(prs);
        }
        else
        {
            break;
        }
    }

    program->data.program.function_decl = func_head;
    program->data.program.struct_decl   = struct_head;

    if (prs->errors && prs->errors->count > 0)
    {
        print_parse_errors(prs);
        return NULL;
    }
    return program;
}
