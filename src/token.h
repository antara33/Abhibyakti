#ifndef TOKEN_H
#define TOKEN_H

#include <string>

// Avoid Windows macro conflicts
#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

enum class BhashaTokenType {

    // Data Types
    NUMBER_TYPE,
    DECIMAL_TYPE,
    STRING_TYPE,
    CHAR_TYPE,
    BOOLEAN_TYPE,
    LIST_TYPE,
    LONG_TYPE,
    POSITIVE_TYPE,
    VOID_TYPE,

    // Keywords
    IF,
    ELSE,
    WHILE,
    PRINT,

    // Boolean Values
    BOOLEAN_TRUE,
    BOOLEAN_FALSE,

    // Identifiers and Literals
    IDENTIFIER,
    INTEGER,
    DECIMAL,
    STRING,
    CHARACTER,

    // Arithmetic Operators
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    MODULO,

    // Relational Operators
    GREATER,
    LESS,
    GREATER_EQUAL,
    LESS_EQUAL,
    EQUAL_EQUAL,
    NOT_EQUAL,

    // Logical Operators
    AND,
    OR,
    NOT,

    // Assignment
    ASSIGN,

    // Symbols
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    LEFT_BRACKET,
    RIGHT_BRACKET,

    SEMICOLON,
    COMMA,

    // Special
    END_OF_FILE,
    UNKNOWN
};

struct Token {

    BhashaTokenType type;
    std::string lexeme;
    int line;

    Token(BhashaTokenType type, std::string lexeme, int line)
        : type(type), lexeme(lexeme), line(line) {}
};

std::string tokenTypeToString(BhashaTokenType type);

#endif