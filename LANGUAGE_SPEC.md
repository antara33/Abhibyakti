# Bhasha Programming Language
## Language Specification

Project: Bangla Programming Language Compiler  
Course: Compiler Design  
Implementation Language: C++  
Target Language: Python  
Version: 1.0  
Status: Initial Specification




# 1. Introduction

Bhasha is a simple Bangla-based programming language developed as
part of a Compiler Design project.

The main purpose of Bhasha is to provide a programming language
whose syntax is easy to understand for Bangla-speaking beginners
while demonstrating the major phases of compiler construction.

The compiler will process Bhasha source code through the following
major stages:

1. Lexical Analysis
2. Syntax Analysis
3. Semantic Analysis
4. Intermediate Representation / Internal Processing
5. Target Code Generation

The compiler will be implemented using C++ and will generate
executable Python source code as the target output.




# 2. Design Goals

The main goals of Bhasha are:

- Provide a simple Bangla-based programming syntax.
- Demonstrate the major phases of compiler construction.
- Support variables and multiple data types.
- Support arithmetic and relational expressions.
- Implement operator precedence.
- Detect lexical, syntax, and semantic errors.
- Perform basic type checking.
- Support conditional statements.
- Support looping constructs.
- Generate target Python code from valid Bhasha programs.
- Provide understandable error messages for users.



# 3. Character Set

Bhasha supports:

- Bangla Unicode characters
- English alphabet characters where required
- Decimal digits: 0-9
- Whitespace characters
- Common programming symbols and operators

The language source code is expected to be stored using UTF-8
encoding.



# 4. Data Types

| Bangla Type | Meaning | Example |
|---|---|---|
| সংখ্যা | Integer | `সংখ্যা x = 10;` |
| দশমিক | Decimal / Floating Point | `দশমিক x = 3.14;` |
| লেখা | String | `লেখা name = "Bhasha";` |
| চরিত্র | Character | `চরিত্র grade = 'A';` |
| সত্যমিথ্যা | Boolean | `সত্যমিথ্যা pass = সত্য;` |
| তালিকা | List / Array | `তালিকা nums = [1, 2, 3];` |
| দীর্ঘসংখ্যা | Long Integer | `দীর্ঘসংখ্যা population = 1000000;` |
| ধনাত্মক | Non-negative Integer | `ধনাত্মক count = 50;` |
| শূন্য | Void / No Value | Used for functions that return no value |

### Implementation Note

The first implementation will prioritize the core data types:

- সংখ্যা
- দশমিক
- লেখা
- চরিত্র
- সত্যমিথ্যা

Advanced types may be implemented after the core compiler
pipeline is functional.



# 5. Boolean Values

Bhasha uses Bangla keywords for Boolean values.

```text
সত্য
মিথ্যা