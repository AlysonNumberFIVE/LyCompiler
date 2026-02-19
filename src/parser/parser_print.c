
#include "types.h"
#include "utils.h"
#include "parser.h"


void print_ast(t_node *node, int depth) {
    if (!node) return;

    if (node->type != NODE_TYPE_SPEC) {
        // 1. Print indentation
        for (int i = 0; i < depth; i++) {
            printf("  │ ");
        }
    }
    printf("  ├── ");

    // 2. Handle each node type
    switch (node->type) {
        case NODE_BINARY_EXPR:
            printf("BINARY_EXPR (%s)\n", node->data.binary_expr.op);
            print_ast(node->data.binary_expr.left, depth + 1);
            print_ast(node->data.binary_expr.right, depth + 1);
            break;

        case NODE_LITERAL_INT:
            printf("INT_LITERAL: %ld\n", node->data.int_literal.int_value);
            break;

        case NODE_LITERAL_STR:
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
            if (node->data.var_decl.type) {
                printf("  │ "); // Extra visual for type
                print_ast(node->data.var_decl.type, depth + 1);
            }
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
                print_ast(node->data.if_stmt.else_branch, depth + 1);
            break;

        case NODE_FUNCTION_DECL:
            printf("FUNC_DECL: %s (Returns: %s)\n", 
                    node->data.func_decl.name, node->data.func_decl.return_type);
            print_ast(node->data.func_decl.params, depth + 1);
            print_ast(node->data.func_decl.body, depth + 1);
            break;

        case NODE_ARRAY_ACCESS:
            printf("ARRAY_ACCESS\n");
            // Print the array base
            for (int i = 0; i < depth + 1; i++) printf("  │ ");
            printf("  ├── Base:\n");
            print_ast(node->data.array_access.array, depth + 2);

            // Print the index expression
            for (int i = 0; i < depth + 1; i++) printf("  │ ");
            printf("  ├── Index:\n");
            print_ast(node->data.array_access.index, depth + 2);
            break;
            // Add more cases here as you implement them...

        case NODE_MEMEBER_ACCESS:
            // 1. Print the type of access
            printf("MEMBER_ACCESS (%s)\n", node->data.member_access.is_arrow ? "->" : ".");

            // 2. Print the struct/pointer (The "Left" side)
            for (int i = 0; i < depth; i++) printf("  │ ");
            printf("  ├── Struct:\n");
            print_ast(node->data.member_access.struct_expr, depth + 2);

            // 3. Print the member name (The "Right" side)
            for (int i = 0; i < depth; i++) printf("  │ ");
            printf("  └── Member: %s\n", node->data.member_access.member_name);
            break;
        case NODE_PARAM:
            printf("PARAM: %s\n", node->data.param.name);
            
            // 1. Print the Type Node
            for (int i = 0; i < depth; i++) printf("  │ ");
            printf("  ├── Type: "); 
            // We pass depth + 1 so the TYPE_SPEC knows how far to indent if needed
            // (Though here we print TYPE_SPEC on the same line or right after)
            print_ast(node->data.param.type, depth + 2);

 
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
