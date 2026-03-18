# Ly Compiler

A compiler for an educational system programming language called **L** (not to be confused with the other L).

It was initially called LanguageLy (because a lot of derivative garbage has -ly at the end of a noun). This language is _purely_ for educational purposes and isn't to be treated as a production programming language — but having said that, it's written as if it were, because the goal is to learn real-world practical compiler engineering.



---

## Table of Contents
- [What is L](#what-is-l)
- [Language Syntax](#the-l-syntax)
  - [Types](#types)
  - [Functions](#function-definitions)
  - [Variables](#variable-declarations)
  - [Structs](#struct-declarations)
  - [Control Flow](#control-flow)
  - [Expressions](#expressions)
  - [Arrays](#arrays)
  - [Struct Initialisers](#struct-initialisers)
- [Parser Architecture](#parser-architecture)
  - [Expression Precedence Chain](#expression-precedence-chain)
  - [Error Recovery](#error-recovery)
- [Project Status](#project-status)

---

## What is L

L is a C-like language with a lot of Go's syntactic sugar to make parsing it as unambiguous as possible. It is like C in the sense that it uses pointers, has no classes, relies on linked lists, and the high-level representation of the language in source code has a very close **1:1 mapping** to the x86-64 Intel Assembly it outputs. This makes it a good compiler learning language — the 1:1 mapping makes debugging and the translation of individual logic blocks predictable and easy to communicate in a tutorial style.

Concepts like classes, interfaces, and multiple return values don't exist at the architecture level and are not supported in L.

### Why the syntax looks the way it does

In C, parsing a function signature like:

```c
unsigned char *function(unsigned int i, unsigned long long j)
```

requires consuming `unsigned`, then looking ahead to determine if the type is `char`, `int`, or `long` (and if it is `long`, looking ahead again to check for `long long`), then consuming the `*`, finding the function name, and then checking whether this is a declaration or an assignment. That is a lot of lookahead and branching just to figure out the return type.

In L, the exact same function is written:

```
func function(i ui32, j ui64) -> uchar* {
    ...
}
```

The `func` keyword makes parsing unambiguous from the first token. `var` does the same for variable declarations. Single-token types (`i64`, `char`) remove multi-token lookahead entirely. Like C and unlike Go, L only returns a single value — this maps directly to a function epilogue in x86-64.

---

## The L Syntax

### Types

L currently has two primitive types with pointer variants formed by appending `*`:

| Type    | Description                   |
|---------|-------------------------------|
| `i64`   | 64-bit signed integer         |
| `char`  | Single byte character         |
| `char*` | Pointer to character (string) |
| `Type*` | Pointer to any named struct   |

Pointer depth is expressed by stacking `*` characters — `Item**` is a pointer to a pointer to `Item`.

---

### Function Definitions

```
func function_name(param1 Type, param2 Type*) -> ReturnType {
    ...
}
```

- `func` is the declaring keyword.
- Parameters follow the pattern `name type` (no `var`, no comma-separated types).
- `->` precedes the return type, which may be a primitive, a named struct, or a pointer to either.
- The body is delimited by `{ }`.

**Example:**
```
func add(a i64, b i64) -> i64 {
    return a + b;
}
```

**BNF:**
```
<function_decl> ::= "func" IDENTIFIER "(" [ <param_list> ] ")" "->" <type> "{" { <statement> } "}"
<param_list>    ::= <param> { IDENTIFIER <type> }
<param>         ::= IDENTIFIER <type>
<type>          ::= ("i64" | "char" | IDENTIFIER) { "*" }
```

---

### Variable Declarations

```
var name Type;
var name Type = expression;
```

- `var` is required — it makes the declaration unambiguous from an expression statement.
- The type follows the name.
- The initialiser is optional.

**Examples:**
```
var count i64;
var count i64 = 0;
var message char* = "hello";
var node Item* = malloc(8);
```

**BNF:**
```
<var_decl> ::= "var" IDENTIFIER <type> [ "=" <expression> ] ";"
```

---

### Struct Declarations

```
struct Name {
    field1 Type;
    field2 Type*;
}
```

Struct members follow the same `name type` pattern as parameters. Each member ends with `;`.

**Example:**
```
struct Node {
    value i64;
    next Node*;
}
```

**BNF:**
```
<struct_decl> ::= "struct" IDENTIFIER "{" { IDENTIFIER <type> ";" } "}"
```

---

### Control Flow

#### if / else if / else

```
if (condition) {
    ...
} else if (other) {
    ...
} else {
    ...
}
```

The condition is wrapped in `( )`. Both `else if` chaining and a bare `else` block are supported.

#### while

```
while (condition) {
    ...
}
```

#### break and continue

```
break;
continue;
```

Both are valid inside any `while` body and must end with `;`.

#### return

```
return expression;
return;            // void return
```

---

### Expressions

Expressions follow standard C-like precedence, lowest to highest:

| Level          | Operators                          |
|----------------|------------------------------------|
| Assignment     | `=`                                |
| Logical OR     | `\|\|`                             |
| Logical AND    | `&&`                               |
| Equality       | `==`  `!=`                         |
| Additive       | `+`  `-`                           |
| Multiplicative | `*`  `/`                           |
| Postfix        | `->`  `.`  `[i]`                   |
| Primary        | literals, identifiers, calls, `()` |

**Member access** uses `->` for pointer members and `.` for value members, consistent with C:

```
node->value
pair.left
```

**Array subscript:**

```
arr[i]
arr[i + 1]
```

**Function calls:**

```
result = add(a, b);
free(ptr);
```

---

### Arrays

Array literals are declared with `[` `]` and can appear as initialisers:

```
var nums i64 = [1, 2, 3, 4];
```

Empty arrays are allowed:

```
var empty i64 = [];
```

**BNF:**
```
<array> ::= "[" [ <expression> { "," <expression> } ] "]"
```

---

### Struct Initialisers

A struct literal uses the `@` sigil followed by the struct name and a `{ field: value }` body:

```
var n Node = @Node{ value: 42, next: 0 };
```

- Fields are separated by `,`.
- Each field name is followed by `:` and then an expression.
- The `@` is required to distinguish a struct literal from a block statement — the parser dispatches on a single token with no lookahead.

**BNF:**
```
<struct_init> ::= "@" IDENTIFIER "{" [ <field> { "," <field> } ] "}"
<field>       ::= IDENTIFIER ":" <expression>
```

---

## Parser Architecture

The parser is a hand-written **recursive descent** parser in C. The entry point is `parser()` in `src/parser/parser.c`.

### Top-level pass

`parser()` walks the token list and dispatches on the first token of each declaration:

| Token    | Calls                  |
|----------|------------------------|
| `func`   | `parse_function_decl`  |
| `struct` | `parse_struct`         |
| `var`    | `parse_var_decl`       |

All successfully parsed top-level nodes are accumulated into linked lists and attached to a `NODE_PROGRAM` root, which is returned as the AST.

### Statement dispatch

Inside a function body, `parse_statement()` dispatches on the peeked token:

| Token      | Calls                                                  |
|------------|--------------------------------------------------------|
| `var`      | `parse_var_decl`                                       |
| `return`   | `parse_return_stmt`                                    |
| `break`    | `parse_break_stmt`                                     |
| `continue` | `parse_continue_stmt`                                  |
| `if`       | `parse_if_statement`                                   |
| `while`    | `parse_while_stmt`                                     |
| identifier | `parse_assignment` wrapped in `NODE_EXPR_STMT`         |

### Expression Precedence Chain

The expression parser is a classic precedence-climbing chain. Each level calls the level above it and only consumes its own operators. From lowest to highest precedence:

```
parse_assignment
    └─ parse_logical_or
           └─ parse_logical_and
                  └─ parse_equality
                         └─ parse_additive
                                └─ parse_multiplicative
                                       └─ parse_postfix
                                              └─ parse_primary
```

`parse_postfix` handles left-associative suffix operators (`->`, `.`, `[i]`) in a loop after `parse_primary` returns. This means `a->b->c` and `arr[i][j]` are handled correctly without recursion.

`parse_assignment` is right-associative: it calls `parse_logical_or` for the left side, and only if `=` follows does it recurse into `parse_assignment` for the right side. This means `a = b = 0` chains correctly.

### AST Nodes

Every node is a `t_node` tagged union defined in `src/utils/types.h`. Sibling nodes are chained via `node->next`. The root is always `NODE_PROGRAM`, which holds two sibling chains: `struct_decl` and `function_decl`.

Key node types:

| Node type            | Represents                         |
|----------------------|------------------------------------|
| `NODE_PROGRAM`       | Root of the entire AST             |
| `NODE_FUNCTION_DECL` | Function definition                |
| `NODE_STRUCT_DECL`   | Struct definition                  |
| `NODE_VAR_DECL`      | Variable declaration               |
| `NODE_IF_STMT`       | if / else if / else                |
| `NODE_WHILE_STMT`    | while loop                         |
| `NODE_RETURN_STMT`   | return statement                   |
| `NODE_BINARY_EXPR`   | Binary operation (`+`, `==`, etc.) |
| `NODE_ASSIGNMENT`    | `target = value`                   |
| `NODE_CALL`          | Function call                      |
| `NODE_MEMBER_ACCESS` | `->` or `.` access                 |
| `NODE_ARRAY_ACCESS`  | `array[index]`                     |
| `NODE_ARRAY`         | `[a, b, c]` literal                |
| `NODE_STRUCT_INIT`   | `@Name{ field: val }` literal      |
| `NODE_TYPE_SPEC`     | Type with pointer level            |

### Error Recovery

When a parse error occurs inside a function, `parser_error()` (in `src/parser/errors.c`) records the error — line number, column, and a plain-English message — into a linked error chain on `prs->errors`, then returns `NULL`. The caller propagates `NULL` back up to the top-level `parser()` loop, which calls `search_for_recovery()`.

`search_for_recovery()` advances through tokens until it sees a `func` or `struct` keyword, so parsing resumes at the next top-level declaration. Statement-level keywords (`return`, `if`, `while`) are deliberately excluded from the recovery set — stopping there would leave the parser mid-function and produce cascading noise errors.

Up to **20 errors** are collected before the run is aborted. At end-of-parse, if any errors were recorded they are printed to stderr:

```
error [1/3] (line 5, col 12): expected '->' before return type
error [2/3] (line 14, col 3): expected '{' to open function body
error [3/3] (line 22, col 9): expected identifier after 'func'

parse failed: 3 error(s).
```

The process exits with code 1. A clean parse returns the `NODE_PROGRAM` root and exits with code 0.

---

## Project Status

| Stage             | Status      |
|-------------------|-------------|
| Lexer             | Complete    |
| Parser            | Complete    |
| Error recovery    | Complete    |
| Semantic analyser | Not started |
| Code generation   | Not started |

---

### Written by Alyson Ngonyama
