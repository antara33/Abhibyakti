#include "parser.h"

#include <iostream>


// CONSTRUCTOR


Parser::Parser(const std::vector<Token> &tokens)
    : tokens(tokens), current(0)
{
}



const Token &Parser::peek() const
{
    return tokens[current];
}

const Token &Parser::previous() const
{
    return tokens[current - 1];
}

bool Parser::isAtEnd() const
{
    return peek().type == AbhibyaktiTokenType::END_OF_FILE;
}

const Token &Parser::advance()
{
    if (!isAtEnd())
    {
        current++;
    }

    return previous();
}

bool Parser::check(AbhibyaktiTokenType type) const
{
    if (isAtEnd())
    {
        return type == AbhibyaktiTokenType::END_OF_FILE;
    }

    return peek().type == type;
}

bool Parser::match(AbhibyaktiTokenType type)
{
    if (!check(type))
    {
        return false;
    }

    advance();
    return true;
}


const Token &Parser::consume(
    AbhibyaktiTokenType type,
    const std::string &message)
{
    if (check(type))
    {
        return advance();
    }

    error(message);

    // Return current token so parser can continue
    return peek();
}

void Parser::error(const std::string &message)
{
    std::cerr
        << "Parser Error at line "
        << peek().line
        << ": "
        << message
        << std::endl;
}

void Parser::synchronize()
{
    advance();

    while (!isAtEnd())
    {
        if (previous().type == AbhibyaktiTokenType::SEMICOLON)
        {
            return;
        }

        switch (peek().type)
        {
        case AbhibyaktiTokenType::NUMBER_TYPE:
        case AbhibyaktiTokenType::DECIMAL_TYPE:
        case AbhibyaktiTokenType::STRING_TYPE:
        case AbhibyaktiTokenType::CHAR_TYPE:
        case AbhibyaktiTokenType::BOOLEAN_TYPE:
        case AbhibyaktiTokenType::IF:
        case AbhibyaktiTokenType::WHILE:
        case AbhibyaktiTokenType::PRINT:
            return;

        default:
            advance();
        }
    }
}



std::shared_ptr<Program> Parser::parse()
{
    std::vector<std::shared_ptr<Statement>> statements;

    while (!isAtEnd())
    {
        try
        {
            auto statement = parseStatement();

            if (statement)
            {
                statements.push_back(statement);
            }
        }
        catch (...)
        {
            synchronize();
        }
    }

    return std::make_shared<Program>(
        std::move(statements));
}


std::shared_ptr<Statement> Parser::parseStatement()
{
    // Variable declaration
    if (
        check(AbhibyaktiTokenType::NUMBER_TYPE) ||
        check(AbhibyaktiTokenType::DECIMAL_TYPE) ||
        check(AbhibyaktiTokenType::STRING_TYPE) ||
        check(AbhibyaktiTokenType::CHAR_TYPE) ||
        check(AbhibyaktiTokenType::BOOLEAN_TYPE) ||
        check(AbhibyaktiTokenType::LIST_TYPE) ||
        check(AbhibyaktiTokenType::LONG_TYPE) ||
        check(AbhibyaktiTokenType::POSITIVE_TYPE) ||
        check(AbhibyaktiTokenType::VOID_TYPE))
    {
        return parseDeclaration();
    }

    // If
    if (match(AbhibyaktiTokenType::IF))
    {
        return parseIf();
    }

    // While
    if (match(AbhibyaktiTokenType::WHILE))
    {
        return parseWhile();
    }

    // Print
    if (match(AbhibyaktiTokenType::PRINT))
    {
        return parsePrint();
    }

    // Block
    if (match(AbhibyaktiTokenType::LEFT_BRACE))
    {
        return parseBlock();
    }

    // Assignment
    if (check(AbhibyaktiTokenType::IDENTIFIER))
    {
        return parseAssignment();
    }

    error("Unexpected token.");

    synchronize();

    return nullptr;
}



std::shared_ptr<Statement> Parser::parseDeclaration()
{
    AbhibyaktiTokenType typeToken = advance().type;

    AbhibyaktiDataType dataType =
        tokenToDataType(typeToken);

    const Token &name =
        consume(
            AbhibyaktiTokenType::IDENTIFIER,
            "Expected variable name after data type.");

    consume(
        AbhibyaktiTokenType::ASSIGN,
        "Expected '=' after variable name.");

    auto initializer = parseExpression();

    consume(
        AbhibyaktiTokenType::SEMICOLON,
        "Expected ';' after declaration.");

    return std::make_shared<DeclarationStatement>(
        dataType,
        name.lexeme,
        initializer);
}



std::shared_ptr<Statement> Parser::parseAssignment()
{
    const Token &name = advance();

    consume(
        AbhibyaktiTokenType::ASSIGN,
        "Expected '=' after variable name.");

    auto value = parseExpression();

    consume(
        AbhibyaktiTokenType::SEMICOLON,
        "Expected ';' after assignment.");

    return std::make_shared<AssignmentStatement>(
        name.lexeme,
        value);
}



std::shared_ptr<Statement> Parser::parsePrint()
{
   

    bool hasParentheses =
        match(AbhibyaktiTokenType::LEFT_PAREN);

    auto expression = parseExpression();

    if (hasParentheses)
    {
        consume(
            AbhibyaktiTokenType::RIGHT_PAREN,
            "Expected ')' after print expression.");
    }

    consume(
        AbhibyaktiTokenType::SEMICOLON,
        "Expected ';' after print statement.");

    return std::make_shared<PrintStatement>(
        expression);
}



std::shared_ptr<Statement> Parser::parseIf()
{
    consume(
        AbhibyaktiTokenType::LEFT_PAREN,
        "Expected '(' after যদি.");

    auto condition = parseExpression();

    consume(
        AbhibyaktiTokenType::RIGHT_PAREN,
        "Expected ')' after condition.");

    consume(
        AbhibyaktiTokenType::LEFT_BRACE,
        "Expected '{' before if body.");

    auto thenBranch = parseBlock();

    std::shared_ptr<BlockStatement> elseBranch = nullptr;

    if (match(AbhibyaktiTokenType::ELSE))
    {
        consume(
            AbhibyaktiTokenType::LEFT_BRACE,
            "Expected '{' before else body.");

        elseBranch = parseBlock();
    }

    return std::make_shared<IfStatement>(
        condition,
        thenBranch,
        elseBranch);
}



std::shared_ptr<Statement> Parser::parseWhile()
{
    consume(
        AbhibyaktiTokenType::LEFT_PAREN,
        "Expected '(' after যতক্ষণ.");

    auto condition = parseExpression();

    consume(
        AbhibyaktiTokenType::RIGHT_PAREN,
        "Expected ')' after condition.");

    consume(
        AbhibyaktiTokenType::LEFT_BRACE,
        "Expected '{' before while body.");

    auto body = parseBlock();

    return std::make_shared<WhileStatement>(
        condition,
        body);
}



std::shared_ptr<BlockStatement> Parser::parseBlock()
{
    std::vector<std::shared_ptr<Statement>> statements;

    while (
        !check(AbhibyaktiTokenType::RIGHT_BRACE) &&
        !isAtEnd())
    {
        auto statement = parseStatement();

        if (statement)
        {
            statements.push_back(statement);
        }
    }

    consume(
        AbhibyaktiTokenType::RIGHT_BRACE,
        "Expected '}' after block.");

    return std::make_shared<BlockStatement>(
        std::move(statements));
}


// EXPRESSION
std::shared_ptr<Expression> Parser::parseExpression()
{
    return parseOr();
}



std::shared_ptr<Expression> Parser::parseOr()
{
    auto expression = parseAnd();

    while (match(AbhibyaktiTokenType::OR))
    {
        auto right = parseAnd();

        expression =
            std::make_shared<BinaryExpression>(
                expression,
                "||",
                right);
    }

    return expression;
}


std::shared_ptr<Expression> Parser::parseAnd()
{
    auto expression = parseEquality();

    while (match(AbhibyaktiTokenType::AND))
    {
        auto right = parseEquality();

        expression =
            std::make_shared<BinaryExpression>(
                expression,
                "&&",
                right);
    }

    return expression;
}


std::shared_ptr<Expression> Parser::parseEquality()
{
    auto expression = parseComparison();

    while (
        check(AbhibyaktiTokenType::EQUAL_EQUAL) ||
        check(AbhibyaktiTokenType::NOT_EQUAL))
    {
        std::string op;

        if (match(AbhibyaktiTokenType::EQUAL_EQUAL))
        {
            op = "==";
        }
        else
        {
            match(AbhibyaktiTokenType::NOT_EQUAL);
            op = "!=";
        }

        auto right = parseComparison();

        expression =
            std::make_shared<BinaryExpression>(
                expression,
                op,
                right);
    }

    return expression;
}



std::shared_ptr<Expression> Parser::parseComparison()
{
    auto expression = parseTerm();

    while (
        check(AbhibyaktiTokenType::GREATER) ||
        check(AbhibyaktiTokenType::LESS) ||
        check(AbhibyaktiTokenType::GREATER_EQUAL) ||
        check(AbhibyaktiTokenType::LESS_EQUAL))
    {
        std::string op;

        if (match(AbhibyaktiTokenType::GREATER))
        {
            op = ">";
        }
        else if (match(AbhibyaktiTokenType::LESS))
        {
            op = "<";
        }
        else if (match(AbhibyaktiTokenType::GREATER_EQUAL))
        {
            op = ">=";
        }
        else
        {
            match(AbhibyaktiTokenType::LESS_EQUAL);
            op = "<=";
        }

        auto right = parseTerm();

        expression =
            std::make_shared<BinaryExpression>(
                expression,
                op,
                right);
    }

    return expression;
}



std::shared_ptr<Expression> Parser::parseTerm()
{
    auto expression = parseFactor();

    while (
        check(AbhibyaktiTokenType::PLUS) ||
        check(AbhibyaktiTokenType::MINUS))
    {
        std::string op;

        if (match(AbhibyaktiTokenType::PLUS))
        {
            op = "+";
        }
        else
        {
            match(AbhibyaktiTokenType::MINUS);
            op = "-";
        }

        auto right = parseFactor();

        expression =
            std::make_shared<BinaryExpression>(
                expression,
                op,
                right);
    }

    return expression;
}



std::shared_ptr<Expression> Parser::parseFactor()
{
    auto expression = parseUnary();

    while (
        check(AbhibyaktiTokenType::MULTIPLY) ||
        check(AbhibyaktiTokenType::DIVIDE) ||
        check(AbhibyaktiTokenType::MODULO))
    {
        std::string op;

        if (match(AbhibyaktiTokenType::MULTIPLY))
        {
            op = "*";
        }
        else if (match(AbhibyaktiTokenType::DIVIDE))
        {
            op = "/";
        }
        else
        {
            match(AbhibyaktiTokenType::MODULO);
            op = "%";
        }

        auto right = parseUnary();

        expression =
            std::make_shared<BinaryExpression>(
                expression,
                op,
                right);
    }

    return expression;
}



std::shared_ptr<Expression> Parser::parseUnary()
{
    if (match(AbhibyaktiTokenType::NOT))
    {
        auto operand = parseUnary();

        return std::make_shared<UnaryExpression>(
            "!",
            operand);
    }

    if (match(AbhibyaktiTokenType::MINUS))
    {
        auto operand = parseUnary();

        return std::make_shared<UnaryExpression>(
            "-",
            operand);
    }

    if (match(AbhibyaktiTokenType::PLUS))
    {
        auto operand = parseUnary();

        return std::make_shared<UnaryExpression>(
            "+",
            operand);
    }

    return parsePrimary();
}



std::shared_ptr<Expression> Parser::parsePrimary()
{
    // Integer
    if (match(AbhibyaktiTokenType::INTEGER))
    {
        return std::make_shared<LiteralExpression>(
            previous().lexeme,
            AbhibyaktiDataType::NUMBER);
    }

    // Decimal
    if (match(AbhibyaktiTokenType::DECIMAL))
    {
        return std::make_shared<LiteralExpression>(
            previous().lexeme,
            AbhibyaktiDataType::DECIMAL);
    }

    // String
    if (match(AbhibyaktiTokenType::STRING))
    {
        return std::make_shared<LiteralExpression>(
            previous().lexeme,
            AbhibyaktiDataType::STRING);
    }

    // Character
    if (match(AbhibyaktiTokenType::CHARACTER))
    {
        return std::make_shared<LiteralExpression>(
            previous().lexeme,
            AbhibyaktiDataType::CHARACTER);
    }

    // Boolean true
    if (match(AbhibyaktiTokenType::BOOLEAN_TRUE))
    {
        return std::make_shared<LiteralExpression>(
            previous().lexeme,
            AbhibyaktiDataType::BOOLEAN);
    }

    // Boolean false
    if (match(AbhibyaktiTokenType::BOOLEAN_FALSE))
    {
        return std::make_shared<LiteralExpression>(
            previous().lexeme,
            AbhibyaktiDataType::BOOLEAN);
    }

    // Identifier
    if (match(AbhibyaktiTokenType::IDENTIFIER))
    {
        return std::make_shared<VariableExpression>(
            previous().lexeme);
    }

    // Parenthesized expression
    if (match(AbhibyaktiTokenType::LEFT_PAREN))
    {
        auto expression = parseExpression();

        consume(
            AbhibyaktiTokenType::RIGHT_PAREN,
            "Expected ')' after expression.");

        return expression;
    }

    error("Expected expression.");

    // Prevent infinite loop
    if (!isAtEnd())
    {
        advance();
    }

    return std::make_shared<LiteralExpression>(
        "0",
        AbhibyaktiDataType::NUMBER);
}



AbhibyaktiDataType Parser::tokenToDataType(
    AbhibyaktiTokenType type) const
{
    switch (type)
    {
    case AbhibyaktiTokenType::NUMBER_TYPE:
        return AbhibyaktiDataType::NUMBER;

    case AbhibyaktiTokenType::DECIMAL_TYPE:
        return AbhibyaktiDataType::DECIMAL;

    case AbhibyaktiTokenType::STRING_TYPE:
        return AbhibyaktiDataType::STRING;

    case AbhibyaktiTokenType::CHAR_TYPE:
        return AbhibyaktiDataType::CHARACTER;

    case AbhibyaktiTokenType::BOOLEAN_TYPE:
        return AbhibyaktiDataType::BOOLEAN;

    case AbhibyaktiTokenType::LIST_TYPE:
        return AbhibyaktiDataType::LIST;

    case AbhibyaktiTokenType::LONG_TYPE:
        return AbhibyaktiDataType::LONG;

    case AbhibyaktiTokenType::POSITIVE_TYPE:
        return AbhibyaktiDataType::POSITIVE;

    case AbhibyaktiTokenType::VOID_TYPE:
        return AbhibyaktiDataType::VOID;

    default:
        return AbhibyaktiDataType::UNKNOWN;
    }
}