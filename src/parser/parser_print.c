
#include "types.h"
#include "utils.h"
#include "parser.h"


void print_ast(t_node *node, int depth) {
    if (!node) return;

        // 1. Print indentation
        for (int i = 0; i < depth; i++) {
            printf("  │ ");
        }
    printf("  ├── ");

    // 2. Handle each node type
    switch (node->type) {
        case NODE_PROGRAM:
            printf("PROGRAM\n");
            if (node->data.program.struct_decl)
                print_ast(node->data.program.struct_decl, depth + 1);
            if (node->data.program.var_decl)
                print_ast(node->data.program.var_decl, depth + 1);
            if (node->data.program.function_decl)
                print_ast(node->data.program.function_decl, depth + 1);
            return; // NODE_PROGRAM is the root — do not follow ->next

        case NODE_BINARY_EXPR:
            printf("BINARY_EXPR (%s)\n", node->data.binary_expr.op);
            print_ast(node->data.binary_expr.left, depth + 1);
            print_ast(node->data.binary_expr.right, depth + 1);
            break;

        case NODE_LITERAL_INT:
            printf("INT_LITERAL: %ld\n", node->data.int_literal.int_value);
            break;

        case NODE_LITERAL_STRING:
            printf("STR_LITERAL: \"%s\"\n", node->data.string_literal.str);
            break;

        case NODE_LITERAL_CHAR:
            printf("CHAR_LITERAL: '%c'\n", node->data.char_literal.character);
            break;

        case NODE_IDENTIFIER:
            printf("IDENTIFIER: %s\n", node->data.identifier.name);
            break;

        case NODE_VAR_DECL:
            printf("VAR_DECL: %s\n", node->data.var_decl.name);
            if (node->data.var_decl.type)
                print_ast(node->data.var_decl.type, depth + 1);
            if (node->data.var_decl.initializer)
                print_ast(node->data.var_decl.initializer, depth + 1);
            break;

        case NODE_ASSIGNMENT:
            printf("ASSIGNMENT\n");
            print_ast(node->data.assignment.target, depth + 1);
            print_ast(node->data.assignment.value, depth + 1);
            break;

        case NODE_IF_STMT:
            printf("IF_STMT\n");
            print_ast(node->data.if_stmt.condition, depth + 1);
            print_ast(node->data.if_stmt.then_branch, depth + 1);
            if (node->data.if_stmt.else_branch)
            {
                print_ast(node->data.if_stmt.else_branch, depth + 1);
            }
        
            break;

        case NODE_FUNCTION_DECL:
            printf("FUNC_DECL: %s\n", node->data.func_decl.name);
            if (node->data.func_decl.return_type)
            print_ast(node->data.func_decl.return_type, depth + 1);
            print_ast(node->data.func_decl.params, depth + 1);
            print_ast(node->data.func_decl.body, depth + 1);
            break;

        case NODE_ARRAY_ACCESS:
            printf("ARRAY_ACCESS\n");
            print_ast(node->data.array_access.array, depth + 1);
            print_ast(node->data.array_access.index, depth + 1);
            break;

        case NODE_MEMEBER_ACCESS:
            printf("MEMBER_ACCESS (%s) .%s\n",
                    node->data.member_access.is_arrow ? "->" : ".",
                    node->data.member_access.member_name);
            print_ast(node->data.member_access.struct_expr, depth + 1);
            break;
        case NODE_PARAM:
            printf("PARAM: %s\n", node->data.param.name);
            if (node->data.param.type)
                print_ast(node->data.param.type, depth + 1);
            break;
        case NODE_TYPE_SPEC:
            printf("TYPE_SPEC: %s", node->data.type_spec.base_type);
            
            // Print a '*' for every level of pointer indirection
            for (int i = 0; i < node->data.type_spec.pointer_level; i++)
            {
                printf("*");
            }
            printf("\n");
            break;

        case NODE_CALL:
        {
            if (node->data.call.callee &&
                    node->data.call.callee->type == NODE_IDENTIFIER)
                printf("CALL: %s\n", node->data.call.callee->data.identifier.name);
            else
            {
                printf("CALL\n");
                print_ast(node->data.call.callee, depth + 1);
            }
            if (node->data.call.args)
                print_ast(node->data.call.args, depth + 1);
            break;
        }
        case NODE_WHILE_STMT:
                printf("WHILE_STMT\n");
                print_ast(node->data.while_stmt.condition, depth + 1);
                if (node->data.while_stmt.body)
                {
                    print_ast(node->data.while_stmt.body, depth + 1);
                }
                break;
        case NODE_BREAK_STMT:
                printf("BREAK_STMT\n");
                break;
        case NODE_CONTINUE_STMT:
                printf("CONTINUE_STMT\n");
                break;
        case NODE_EXPR_STMT:
                printf("EXPR_STMT\n");
                print_ast(node->data.expr_stmt.expression, depth + 1);
                break;
        case NODE_RETURN_STMT:
            printf("RETURN_STMT\n");
            print_ast(node->data.expr_stmt.expression, depth + 1);
            break;
        case NODE_ARRAY:
        {
            t_node *array = node->data.array.items;
            printf("NODE_ARRAY\n");
            print_ast(array, depth + 1);
            break;
        }
        case NODE_STRUCT_INIT:
        {
            t_node *field = node->data.struct_initializer.fields;
            printf("STRUCT_INIT: %s\n", node->data.struct_initializer.struct_name);
            while (field)
            {
                for (int i = 0; i < depth + 1; i++) printf("  │ ");
                printf("  ├── FIELD: %s\n", field->data.init_field.name);
                print_ast(field->data.init_field.value, depth + 2);
                field = field->next;
            }
            break;
        }
        case NODE_STRUCT_FIELD:
            printf("STRUCT_FIELD: %s\n", node->data.init_field.name);
            print_ast(node->data.init_field.value, depth + 1);
            break;
        case NODE_STRUCT_DECL:
        {
            t_node *parameter = node->data.struct_decl.members;
            printf("STRUCT_DECL: %s\n", node->data.struct_decl.name);
            while (parameter)
            {
                t_node *type_node = parameter->data.param.type;
                for (int i = 0; i < depth + 1; i++) printf("  │ ");
                printf("  ├── MEMBER: %s ", parameter->data.param.name);
                printf("%s", type_node->data.type_spec.base_type);
                for (int j = 0; j < type_node->data.type_spec.pointer_level; j++)
                    printf("*");
                printf("\n");
                parameter = parameter->next;
            }
            break;
        }
        default:
            printf("UNKNOWN_NODE_TYPE (%d)\n", node->type);
            break;
    }

    // 3. Handle Sibling Nodes (The linked list logic)
    // If your parser linked multiple statements via node->next, 
    // we print them at the SAME depth level.
    if (node->next) {
        print_ast(node->next, depth);
    }
}
