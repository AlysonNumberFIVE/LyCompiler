

#include "utils.h"

t_node *init_node(void)
{
    t_node *node;

    node = (t_node *)malloc(sizeof(t_node));
    if (node == NULL)
        return NULL;

    memset(node, 0, sizeof(node));
    return (node);
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

t_node  *new_func_decl(char *name, t_node *params, t_node *return_type, t_node *body)
{
    t_node *node; 

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_FUNCTION_DECL;
    node->data.func_decl.name = strdup(name);
    node->data.func_decl.params = params;
    node->data.func_decl.return_type = params;
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
    node->data.expression = expression;
    return node;
}

t_node *new_break_stmt(void)
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_BREAK_STMT;
    node->name = strdup("break");
    return (node);
}

t_node *new_continue_stmt(void)
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_CONTINUE_STMT;
    node->name = strdup("continue");
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
    node->while_stmt.condition = condition;
    node->while_stmt.body = body;
    return node;
}

t_node  *new_expr_stmt(t_node *expression) 
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL; 

    node->type = NODE_EXPR_STMT;
    node->expr_stmt.expression = expression;
    return node;
}

t_node *new_binary_expr(char *left, char *op, char *right)
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_BINARY_EXPR;
    node->binary_expr.left = left;
    node->binary_expr.op = op;
    node->binary_expr.right = right;
    return node;
}

t_node *new_assignment(t_node *target, t_node *value)
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_ASSIGNMENT;
    node->assignment.target = target;
    node->assignment.value = value;
    return node;
}

t_node *new_member_access(t_node *struct_expr, char *member_name, bool is_arrow)
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_MEMEBER_ACCESS;
    node->member_access.struct_expr = struct_expr;
    node->member_access.member_name = strdup(member_name);
    node->member_access.is_arrow = is_arrow;
}

t_node  *new_call(t_node *callee, t_node *args)
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_CALL;
    node->call.callee = callee;
    node->call.args = args;
    return node;
}

t_node  *new_struct_initializer(char *struct_name, t_node *fields)
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_STRUCT_INIT;
    node->struct_initializer.struct_name = strdup(struct_name);
    node->struct_initializer.fields = fields;
    return node;
}

t_node *new_struct_field(char *name ,t_node *value)
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_STRUCT_FIELD;
    node->init_field.name = strdup(name);
    node->init_field.value = value;
    return node;
}

t_node *new_identifier(char *name)
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node.type = NODE_IDENTIFIER;
    node.identifier.name = strdup(name);
    return node;
}

t_node *new_type_spec(char *base_type, int pointer_level)
{
    t_node *node;

    node = init_node();
    if (node == NULL)
        return NULL;

    node->type = NODE_TYPE_SPEC;
    node->type_spec.base_type = strdup(base_type);
    node->type_spec.pointer_level = pointer_level;
    return node;
}