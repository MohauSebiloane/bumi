# BUMI Language Front End (Flex & Bison)
**Author:** Colin Fairborn  
**Date:** Dec 2025  

This folder implements the **lexical analyzer and parser** for our language, BUMI, using **Flex** and **Bison**. The language syntax is inspired by Dart and C, and supports classes, functions, control flow, and parsing, with good operator precedence.

---

## Project Overview

The Flex/Bison code consists of:
- A **Flex lexer** that tokenizes source code
- A **Bison grammar** that parses programs by structure, supporting unary, binary, and postfix ops

These focus on **syntactic correctness** of the program!

---

## Language Features

### Supported Types
- `int`
- `float`
- `bool`
- `string`
- user-defined types (class identifiers)

### Declarations
- Class declarations with fields and methods
- Standalone function declarations
- Variable declarations with optional initialization

### Control Flow
- `if / else`
- `while`
- `for`
- `return`

### Expressions
- Assignment (`=`, right-associative)
- Logical ops (`&&`, `||`, `!`)
- Equality ops (`==`, `!=`)
- Relational ops (`<`, `>`, `<=`, `>=`)
- Arithmetic ops (`+`, `-`, `*`, `/`, `%`)
- Unary ops (Postfix, `!`, `+/-`)
- Postfix inc and dec (`++`, `--`)
- Object access and method calls (`obj.field`, `obj.method()`)

---

## Operator Precedence
From high to low:
1) Postfix `++ --`
2) Unary `! + -`
3) Multiplicative `* / %`
4) Additive `+ -`
5) Relational `< > <= >=`
6) Equality `== !=`
7) Logical AND `&&`
8) Logical OR `||`
9) Assignment `=`

![alt text](image.png)

I followed C's precedence closely, but left out things like the bitwise, the ternary, and the comma operators, which make the precedence rules simpler and easier to work with.

Assignment is the only **right-associative** operator.