

#include "utils.h"


t_node *init_node(void)
{
    t_node *node;

    node = (t_node *)malloc(sizeof(t_node));
    if (node == NULL)
        return NULL;

    memset(node, 0, sizeof(t_node));
    return (node);
}


t_token  *parser_lookahead(t_parser *prs)
{
    if (prs && prs->token && prs->token->next) 
        return prs->token->next;
    return NULL;
}

t_token *parser_advance(t_parser *prs) 
{
    if (prs && prs->token && prs->token->next)
    {
        t_token *token;
        token = prs->token;
        prs->token = prs->token->next;
        return token;
    }
    return NULL;
}

t_token *parser_peek(t_parser *prs)
{
    if (prs && prs->token)
        return prs->token;
    return NULL;
}

t_node  *init_program(void)
{
    t_node  *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_PROGRAM;
    return node;
}


t_node  *new_struct_decl(char *name, t_node *members) 
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_STRUCT_DECL;
    node->data.struct_decl.name = strdup(name);
    node->data.struct_decl.members = members;
    return node;
}

t_node *new_struct_member(char *name, t_node *type_node)
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_STRUCT_MEMBER;
    node->data.struct_member.name = strdup(name);
    node->data.struct_member.type = type_node;
    return node;
}

t_node  *new_func_decl(char *name, t_node *params, char *return_type, t_node *body)
{
    t_node *node; 

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_FUNCTION_DECL;
    node->data.func_decl.name = strdup(name);
    node->data.func_decl.params = params;
    node->data.func_decl.return_type = strdup(return_type);
    node->data.func_decl.body = body;
    return node;
}

t_node *new_param(char *name, t_node *type)
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_PARAM;
    node->data.param.name = strdup(name);
    node->data.param.type = type;
    return node;
}

t_node  *new_var_decl(char *name, t_node *type, t_node *initializer)
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_VAR_DECL;
    node->data.var_decl.name = strdup(name);
    node->data.var_decl.type = type;
    node->data.var_decl.initializer = initializer;
    return node;
}

t_node *new_return_stmt(t_node *expression)
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_RETURN_STMT;
    node->data.expr_stmt.expression = expression;
    return node;
}

t_node *new_break_stmt(void)
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_BREAK_STMT;
    node->data.break_stmt.name = strdup("break");
    return (node);
}

t_node *new_continue_stmt(void)
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_CONTINUE_STMT;
    node->data.continue_stmt.name = strdup("continue");
    return (node);
}

t_node *new_if_stmt(t_node *condition, t_node *then_branch, t_node *else_branch)
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_IF_STMT;
    node->data.if_stmt.condition = condition;
    node->data.if_stmt.then_branch = then_branch;
    node->data.if_stmt.else_branch = else_branch;
    return node;
}

t_node *new_while_stmt(t_node *condition, t_node *body)
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL;
    
    node->type = NODE_WHILE_STMT;
    node->data.while_stmt.condition = condition;
    node->data.while_stmt.body = body;
    return node;
}

t_node  *new_expr_stmt(t_node *expression) 
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL; 

    node->type = NODE_EXPR_STMT;
    node->data.expr_stmt.expression = expression;
    return node;
}

t_node *new_binary_expr(t_node *left, char *op, t_node *right)
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_BINARY_EXPR;
    node->data.binary_expr.left = left;
    node->data.binary_expr.op = op;
    node->data.binary_expr.right = right;
    return node;
}

t_node *new_assignment(t_node *target, t_node *value)
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_ASSIGNMENT;
    node->data.assignment.target = target;
    node->data.assignment.value = value;
    return node;
}

t_node *new_member_access(t_node *struct_expr, char *member_name, bool is_arrow)
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_MEMEBER_ACCESS;
    node->data.member_access.struct_expr = struct_expr;
    node->data.member_access.member_name = strdup(member_name);
    node->data.member_access.is_arrow = is_arrow;
    return node;
}

t_node  *new_call(t_node *callee, t_node *args)
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_CALL;
    node->data.call.callee = callee;
    node->data.call.args = args;
    return node;
}

t_node  *new_struct_initializer(char *struct_name, t_node *fields)
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_STRUCT_INIT;
    node->data.struct_initializer.struct_name = strdup(struct_name);
    node->data.struct_initializer.fields = fields;
    return node;
}

t_node *new_struct_field(char *name ,t_node *value)
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_STRUCT_FIELD;
    node->data.init_field.name = strdup(name);
    node->data.init_field.value = value;
    return node;
}

t_node *new_identifier(char *name)
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_IDENTIFIER;
    node->data.identifier.name = strdup(name);
    return node;
}

t_node *new_type_spec(char *base_type, int pointer_level)
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_TYPE_SPEC;
    node->data.type_spec.base_type = strdup(base_type);
    node->data.type_spec.pointer_level = pointer_level;
    return node;
}

t_node  *new_int_literal(long i)
{
    t_node  *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_LITERAL_INT;
    node->data.int_literal.int_value = i;
    return node;
}

t_node  *new_string_literal(char *str)
{
    t_node  *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_LITERAL_STR;
    node->data.string_literal.str = strdup(str);
    return node;
}

t_node  *new_char_literal(char c)
{
    t_node  *node;

    node = init_node();
    if (node == NULL)
        return NULL; 

    node->type = NODE_LITERAL_CHAR;
    node->data.char_literal.character = c;
    return node;
}