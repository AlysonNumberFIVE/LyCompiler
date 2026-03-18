#!/bin/bash

# Run from the LyCompiler root: bash tests/run_tests.sh

COMPILER="./compiler"
PASS=0
FAIL=0

run_test() {
    local file="$1"
    local name
    name=$(basename "$file")
    shift
    local expected=("$@")

    local output
    output=$("$COMPILER" "$file" 2>&1)
    local exit_code=$?

    if [ $exit_code -ne 0 ]; then
        printf "FAIL [%-40s]  compiler exited with code %d\n" "$name" "$exit_code"
        FAIL=$((FAIL + 1))
        return
    fi

    for pattern in "${expected[@]}"; do
        if ! echo "$output" | grep -q "$pattern"; then
            printf "FAIL [%-40s]  expected '%s' not found in output\n" "$name" "$pattern"
            FAIL=$((FAIL + 1))
            return
        fi
    done

    printf "PASS [%s]\n" "$name"
    PASS=$((PASS + 1))
}

echo "Running parser tests..."
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

run_test "tests/parser/01_var_decl.ly" \
    "FUNC_DECL: test_vars" \
    "VAR_DECL: a" \
    "VAR_DECL: b" \
    "RETURN_STMT"

run_test "tests/parser/02_arithmetic.ly" \
    "FUNC_DECL: test_expr" \
    "BINARY_EXPR" \
    "RETURN_STMT"

run_test "tests/parser/03_if_else.ly" \
    "FUNC_DECL: test_if" \
    "IF_STMT" \
    "RETURN_STMT"

run_test "tests/parser/04_while_break_continue.ly" \
    "FUNC_DECL: test_while" \
    "WHILE_STMT" \
    "BREAK_STMT" \
    "CONTINUE_STMT"

run_test "tests/parser/05_function_call.ly" \
    "FUNC_DECL: test_calls" \
    "CALL: foo" \
    "CALL: bar"

run_test "tests/parser/06_member_access.ly" \
    "FUNC_DECL: test_member" \
    "MEMBER_ACCESS (->)" \
    "RETURN_STMT"

# --- Integration tests ---

run_test "tests/parser/07_fibonacci.ly" \
    "FUNC_DECL: fibonacci" \
    "VAR_DECL: a" \
    "VAR_DECL: b" \
    "VAR_DECL: tmp" \
    "WHILE_STMT" \
    "ASSIGNMENT" \
    "BINARY_EXPR" \
    "RETURN_STMT"

run_test "tests/parser/08_search_loop.ly" \
    "FUNC_DECL: find" \
    "VAR_DECL: i" \
    "WHILE_STMT" \
    "IF_STMT" \
    "MEMBER_ACCESS (->)" \
    "BREAK_STMT" \
    "ASSIGNMENT" \
    "RETURN_STMT"

run_test "tests/parser/09_multi_function.ly" \
    "FUNC_DECL: process" \
    "VAR_DECL: sum" \
    "WHILE_STMT" \
    "IF_STMT" \
    "BREAK_STMT" \
    "CALL: clamp" \
    "RETURN_STMT"

run_test "tests/parser/10_struct_and_init.ly" \
    "FUNC_DECL: make_point" \
    "STRUCT_DECL: Point" \
    "STRUCT_INIT: Point" \
    "FIELD: x" \
    "FIELD: y" \
    "RETURN_STMT"

run_test "tests/parser/11_array.ly" \
    "FUNC_DECL: use_arrays" \
    "VAR_DECL: nums" \
    "NODE_ARRAY" \
    "INT_LITERAL: 1" \
    "INT_LITERAL: 2" \
    "INT_LITERAL: 3" \
    "VAR_DECL: mixed" \
    "BINARY_EXPR" \
    "RETURN_STMT"

run_test "tests/parser/12_linked_list.ly" \
    "STRUCT_DECL: Item" \
    "MEMBER: name char*" \
    "MEMBER: next Item*" \
    "FUNC_DECL: new_item" \
    "CALL: malloc" \
    "FUNC_DECL: append" \
    "FUNC_DECL: length" \
    "FUNC_DECL: print_list" \
    "FUNC_DECL: free_list" \
    "WHILE_STMT" \
    "BINARY_EXPR (!=)" \
    "ASSIGNMENT" \
    "MEMBER_ACCESS (->)" \
    "CALL: free" \
    "RETURN_STMT"

echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "Results: ${PASS} passed, ${FAIL} failed"
[ "$FAIL" -eq 0 ] && exit 0 || exit 1
