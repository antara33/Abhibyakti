#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "ast.h"

#include <vector>
#include <memory>
#include <stdexcept>
#include <string>

// ============================================================
// BHASHA PARSER
//
// Parser-এর কাজ:
//
// Tokens
//    ↓
// Syntax Analysis
//    ↓
// Abstract Syntax Tree (AST)
//
// Example:
//
// সংখ্যা x = 10;
//
// Tokens:
// NUMBER_TYPE IDENTIFIER ASSIGN INTEGER SEMICOLON
//
//        ↓ Parser
//
// DeclarationStatement
// ============================================================

class Parser
{
private:
    // ========================================================
    // TOKEN STREAM
    // ========================================================

    std::vector<Token> tokens;

    // Current position in token list
    size_t current;

    // ========================================================
    // BASIC TOKEN OPERATIONS
    // ========================================================

    // Return the current token without consuming it
    const Token &peek() const;

    // Return the previous token
    const Token &previous() const;

    // Check whether we reached END_OF_FILE
    bool isAtEnd() const;

    // Move to the next token
    const Token &advance();

    // Check current token type
    bool check(BhashaTokenType type) const;

    // If current token matches type,
    // consume it and return true
    bool match(BhashaTokenType type);

    // ========================================================
    // ERROR HANDLING
    // ========================================================

    // Consume a token of expected type.
    //
    // Example:
    //
    // consume(SEMICOLON, "Expected ';' after declaration.");
    //
    const Token &consume(
        BhashaTokenType type,
        const std::string &message);

    // Report parser error
    void error(
        const std::string &message);

    // Skip tokens until a safe point
    // for syntax error recovery
    void synchronize();

    // ========================================================
    // STATEMENT PARSING
    // ========================================================

    // Parse one statement
    std::shared_ptr<Statement> parseStatement();

    // Parse variable declaration
    //
    // Example:
    //
    // সংখ্যা x = 10;
    //
    std::shared_ptr<Statement> parseDeclaration();

    // Parse assignment
    //
    // Example:
    //
    // x = 20;
    //
    std::shared_ptr<Statement> parseAssignment();

    // Parse print statement
    //
    // Example:
    //
    // দেখাও(x);
    //
    std::shared_ptr<Statement> parsePrint();

    // Parse if statement
    //
    // Example:
    //
    // যদি (x > 10) {
    //     দেখাও(x);
    // }
    //
    std::shared_ptr<Statement> parseIf();

    // Parse while statement
    //
    // Example:
    //
    // যতক্ষণ (x < 10) {
    //     x = x + 1;
    // }
    //
    std::shared_ptr<Statement> parseWhile();

    // Parse a block
    //
    // Example:
    //
    // {
    //     x = x + 1;
    //     দেখাও(x);
    // }
    //
    std::shared_ptr<BlockStatement> parseBlock();

    // ========================================================
    // EXPRESSION PARSING
    //
    // These functions implement operator precedence.
    //
    // Lowest precedence
    //
    // expression
    //     ↓
    // logical OR
    //     ↓
    // logical AND
    //     ↓
    // equality
    //     ↓
    // comparison
    //     ↓
    // term
    //     ↓
    // factor
    //     ↓
    // unary
    //     ↓
    // primary
    //
    // Highest precedence
    // ========================================================

    // General expression
    std::shared_ptr<Expression> parseExpression();

    // Logical OR
    //
    // Example:
    //
    // x || y
    //
    std::shared_ptr<Expression> parseOr();

    // Logical AND
    //
    // Example:
    //
    // x && y
    //
    std::shared_ptr<Expression> parseAnd();

    // Equality
    //
    // == and !=
    //
    // Example:
    //
    // x == y
    // x != y
    //
    std::shared_ptr<Expression> parseEquality();

    // Comparison
    //
    // > < >= <=
    //
    // Example:
    //
    // x > 10
    //
    std::shared_ptr<Expression> parseComparison();

    // Addition and subtraction
    //
    // + -
    //
    // Example:
    //
    // x + 5 - 2
    //
    std::shared_ptr<Expression> parseTerm();

    // Multiplication, division and modulo
    //
    // * / %
    //
    // Example:
    //
    // x * 5 / 2
    //
    std::shared_ptr<Expression> parseFactor();

    // Unary operators
    //
    // ! and -
    //
    // Example:
    //
    // -x
    // !flag
    //
    std::shared_ptr<Expression> parseUnary();

    // Primary expressions
    //
    // Examples:
    //
    // 10
    // 3.5
    // x
    // (x + 5)
    //
    std::shared_ptr<Expression> parsePrimary();

    // ========================================================
    // DATA TYPE HANDLING
    // ========================================================

    // Convert token type to AST data type
    //
    // NUMBER_TYPE  -> NUMBER
    // DECIMAL_TYPE -> DECIMAL
    // STRING_TYPE  -> STRING
    // etc.
    //
    BhashaDataType tokenToDataType(
        BhashaTokenType type) const;

public:
    // ========================================================
    // CONSTRUCTOR
    // ========================================================

    explicit Parser(
        const std::vector<Token> &tokens);

    // ========================================================
    // MAIN PARSE FUNCTION
    //
    // Takes the complete token list and
    // returns the root Program AST node.
    // ========================================================

    std::shared_ptr<Program> parse();
};

#endif // PARSER_H