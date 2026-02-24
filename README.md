# Ly Compiler

A compiler for an educational system programming language L (not to be confused with the other L). 
It was initially called LanguageLy (coz a lot of derivative garbage has -ly at the end of a noun).
This langauge is _purely_ for educational purposes and isn't to be treated as a literal programming laguage, but having said that, I'm writing it as if it were a production grade programming language because I'm aiming to learn real world practical compiler engineering that can be applicable to the real world.

![DEATH_NOTE_L_wallpaper](https://github.com/user-attachments/assets/358c9e10-232f-4280-b703-08ed3f746294)


## Table of Content 
- [TBD]


## What is L
L is a C-like language with a lot of Golang's syntactic sugar to make parsing it a lot easier and as unambiguous as possible. It is like C in the sense that it uses pointers, has no classes, relies on linked lists, and the high level representation of the language (in source code) will have a very close 1:1 mapping to the x86 Intel Assembly it outputs. I believe this makes it the perfect compiler learning language as the 1:1 mapping makes things like debugging and the translation of individual logic blocks predictable and easier to communicate in a tutorial style for any reader interested in using it that way. (i.e concepts like Classes and Interfaces don't exist at the archtecture level so won't be supported in L).

### Example:
In C, declaring a function goes like:
```
unsigned char *function(unsigned int i, unsigned long long j)
{
...
}
```
and in L, the exact above value will be 
```
var function(i ui32 ,j ui64) -> uchar* {
...
}
```

In C, parsing the above requires consuming `unsigned` and then looking ahead to determine if the datatype is a `char`, or `int` or `long` (and if its a `long`, we need to look ahead to make sure its not an `unsigned long long`), then once we've consumed the `*`, we look for the function name `function` and then we need to look ahead to determine if this is a variable declaration like `unsigned char *function;` or an assignment `unsigned char *function =`. This latter example isn't too bad but we repeat the same datatype checks for `unsgined` and then a `long` after the `unsigned`, you get the idea. A lot of `if/switch`ing in the parsing process to determine our datatype.

Where is in L, just like Go, the `var` token sends us into parsing whether our datatype is a variable (and fail if it isn't). Single tokens for datatypes allow us to check a single list without multipel lookaheads. 

Lastly like C (and unlike Go), L only returns a single variable on return. This translates cleanly to a **functoin epilogue** in x86 but more on this later.


## The L Syntax

The following is a basic, non-BNF style look at L's syntax with examples.

#### Function Definition
A function definition will be denoted by the `func` keyword with the return datatye declared after the parameter list
```
func function_name(value1 i64, value2 char *) -> i64 {
    ...
}
```
This was done for unambiguity in function parsing. Once the parser sees a `func`, we know we'll be parsing a function. Function parameters keep the same structure as local variable declaration with the `var` token ommitted (so `ID DATATYPE` whereas local variables are `var ID DATATYPE`)/
.

#### Variable declaration 
Variables will be denoted with a `var` for ease and unambiguity of parsing.
```
var variable i64;
```



### Written by Alyson Ngonyama




