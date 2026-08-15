#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "ast.h"

#include <vector>
#include <memory>
#include <stdexcept>
#include <string>

// Parser converts tokens into an Abstract Syntax Tree (AST).
class Parser
{
private:
    std::vector<Token> tokens;
    size_t current;

    // Basic token operations
    const Token &peek() const;
    const Token &previous() const;
    bool isAtEnd() const;
    const Token &advance();
    bool check(BhashaTokenType type) const;
    bool match(BhashaTokenType type);

    // Error handling
    const Token &consume(
        BhashaTokenType type,
        const std::string &message);

    void error(const std::string &message);
    void synchronize();

    // Statement parsing
    std::shared_ptr<Statement> parseStatement();
    std::shared_ptr<Statement> parseDeclaration();
    std::shared_ptr<Statement> parseAssignment();
    std::shared_ptr<Statement> parsePrint();
    std::shared_ptr<Statement> parseIf();
    std::shared_ptr<Statement> parseWhile();
    std::shared_ptr<BlockStatement> parseBlock();

    // Expression parsing with operator precedence
    std::shared_ptr<Expression> parseExpression();
    std::shared_ptr<Expression> parseOr();
    std::shared_ptr<Expression> parseAnd();
    std::shared_ptr<Expression> parseEquality();
    std::shared_ptr<Expression> parseComparison();
    std::shared_ptr<Expression> parseTerm();
    std::shared_ptr<Expression> parseFactor();
    std::shared_ptr<Expression> parseUnary();
    std::shared_ptr<Expression> parsePrimary();

    // Convert token type to AST data type
    BhashaDataType tokenToDataType(
        BhashaTokenType type) const;

public:
    explicit Parser(
        const std::vector<Token> &tokens);

    // Start parsing and return the AST.
    std::shared_ptr<Program> parse();
};

#endif // PARSER_H