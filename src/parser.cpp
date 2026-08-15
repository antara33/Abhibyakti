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
    return peek().type == BhashaTokenType::END_OF_FILE;
}

const Token &Parser::advance()
{
    if (!isAtEnd())
    {
        current++;
    }

    return previous();
}

bool Parser::check(BhashaTokenType type) const
{
    if (isAtEnd())
    {
        return type == BhashaTokenType::END_OF_FILE;
    }

    return peek().type == type;
}

bool Parser::match(BhashaTokenType type)
{
    if (!check(type))
    {
        return false;
    }

    advance();
    return true;
}


const Token &Parser::consume(
    BhashaTokenType type,
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
        if (previous().type == BhashaTokenType::SEMICOLON)
        {
            return;
        }

        switch (peek().type)
        {
        case BhashaTokenType::NUMBER_TYPE:
        case BhashaTokenType::DECIMAL_TYPE:
        case BhashaTokenType::STRING_TYPE:
        case BhashaTokenType::CHAR_TYPE:
        case BhashaTokenType::BOOLEAN_TYPE:
        case BhashaTokenType::IF:
        case BhashaTokenType::WHILE:
        case BhashaTokenType::PRINT:
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
        check(BhashaTokenType::NUMBER_TYPE) ||
        check(BhashaTokenType::DECIMAL_TYPE) ||
        check(BhashaTokenType::STRING_TYPE) ||
        check(BhashaTokenType::CHAR_TYPE) ||
        check(BhashaTokenType::BOOLEAN_TYPE) ||
        check(BhashaTokenType::LIST_TYPE) ||
        check(BhashaTokenType::LONG_TYPE) ||
        check(BhashaTokenType::POSITIVE_TYPE) ||
        check(BhashaTokenType::VOID_TYPE))
    {
        return parseDeclaration();
    }

    // If
    if (match(BhashaTokenType::IF))
    {
        return parseIf();
    }

    // While
    if (match(BhashaTokenType::WHILE))
    {
        return parseWhile();
    }

    // Print
    if (match(BhashaTokenType::PRINT))
    {
        return parsePrint();
    }

    // Block
    if (match(BhashaTokenType::LEFT_BRACE))
    {
        return parseBlock();
    }

    // Assignment
    if (check(BhashaTokenType::IDENTIFIER))
    {
        return parseAssignment();
    }

    error("Unexpected token.");

    synchronize();

    return nullptr;
}



std::shared_ptr<Statement> Parser::parseDeclaration()
{
    BhashaTokenType typeToken = advance().type;

    BhashaDataType dataType =
        tokenToDataType(typeToken);

    const Token &name =
        consume(
            BhashaTokenType::IDENTIFIER,
            "Expected variable name after data type.");

    consume(
        BhashaTokenType::ASSIGN,
        "Expected '=' after variable name.");

    auto initializer = parseExpression();

    consume(
        BhashaTokenType::SEMICOLON,
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
        BhashaTokenType::ASSIGN,
        "Expected '=' after variable name.");

    auto value = parseExpression();

    consume(
        BhashaTokenType::SEMICOLON,
        "Expected ';' after assignment.");

    return std::make_shared<AssignmentStatement>(
        name.lexeme,
        value);
}



std::shared_ptr<Statement> Parser::parsePrint()
{
   

    bool hasParentheses =
        match(BhashaTokenType::LEFT_PAREN);

    auto expression = parseExpression();

    if (hasParentheses)
    {
        consume(
            BhashaTokenType::RIGHT_PAREN,
            "Expected ')' after print expression.");
    }

    consume(
        BhashaTokenType::SEMICOLON,
        "Expected ';' after print statement.");

    return std::make_shared<PrintStatement>(
        expression);
}



std::shared_ptr<Statement> Parser::parseIf()
{
    consume(
        BhashaTokenType::LEFT_PAREN,
        "Expected '(' after যদি.");

    auto condition = parseExpression();

    consume(
        BhashaTokenType::RIGHT_PAREN,
        "Expected ')' after condition.");

    consume(
        BhashaTokenType::LEFT_BRACE,
        "Expected '{' before if body.");

    auto thenBranch = parseBlock();

    std::shared_ptr<BlockStatement> elseBranch = nullptr;

    if (match(BhashaTokenType::ELSE))
    {
        consume(
            BhashaTokenType::LEFT_BRACE,
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
        BhashaTokenType::LEFT_PAREN,
        "Expected '(' after যতক্ষণ.");

    auto condition = parseExpression();

    consume(
        BhashaTokenType::RIGHT_PAREN,
        "Expected ')' after condition.");

    consume(
        BhashaTokenType::LEFT_BRACE,
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
        !check(BhashaTokenType::RIGHT_BRACE) &&
        !isAtEnd())
    {
        auto statement = parseStatement();

        if (statement)
        {
            statements.push_back(statement);
        }
    }

    consume(
        BhashaTokenType::RIGHT_BRACE,
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

    while (match(BhashaTokenType::OR))
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

    while (match(BhashaTokenType::AND))
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
        check(BhashaTokenType::EQUAL_EQUAL) ||
        check(BhashaTokenType::NOT_EQUAL))
    {
        std::string op;

        if (match(BhashaTokenType::EQUAL_EQUAL))
        {
            op = "==";
        }
        else
        {
            match(BhashaTokenType::NOT_EQUAL);
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
        check(BhashaTokenType::GREATER) ||
        check(BhashaTokenType::LESS) ||
        check(BhashaTokenType::GREATER_EQUAL) ||
        check(BhashaTokenType::LESS_EQUAL))
    {
        std::string op;

        if (match(BhashaTokenType::GREATER))
        {
            op = ">";
        }
        else if (match(BhashaTokenType::LESS))
        {
            op = "<";
        }
        else if (match(BhashaTokenType::GREATER_EQUAL))
        {
            op = ">=";
        }
        else
        {
            match(BhashaTokenType::LESS_EQUAL);
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
        check(BhashaTokenType::PLUS) ||
        check(BhashaTokenType::MINUS))
    {
        std::string op;

        if (match(BhashaTokenType::PLUS))
        {
            op = "+";
        }
        else
        {
            match(BhashaTokenType::MINUS);
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
        check(BhashaTokenType::MULTIPLY) ||
        check(BhashaTokenType::DIVIDE) ||
        check(BhashaTokenType::MODULO))
    {
        std::string op;

        if (match(BhashaTokenType::MULTIPLY))
        {
            op = "*";
        }
        else if (match(BhashaTokenType::DIVIDE))
        {
            op = "/";
        }
        else
        {
            match(BhashaTokenType::MODULO);
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
    if (match(BhashaTokenType::NOT))
    {
        auto operand = parseUnary();

        return std::make_shared<UnaryExpression>(
            "!",
            operand);
    }

    if (match(BhashaTokenType::MINUS))
    {
        auto operand = parseUnary();

        return std::make_shared<UnaryExpression>(
            "-",
            operand);
    }

    if (match(BhashaTokenType::PLUS))
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
    if (match(BhashaTokenType::INTEGER))
    {
        return std::make_shared<LiteralExpression>(
            previous().lexeme,
            BhashaDataType::NUMBER);
    }

    // Decimal
    if (match(BhashaTokenType::DECIMAL))
    {
        return std::make_shared<LiteralExpression>(
            previous().lexeme,
            BhashaDataType::DECIMAL);
    }

    // String
    if (match(BhashaTokenType::STRING))
    {
        return std::make_shared<LiteralExpression>(
            previous().lexeme,
            BhashaDataType::STRING);
    }

    // Character
    if (match(BhashaTokenType::CHARACTER))
    {
        return std::make_shared<LiteralExpression>(
            previous().lexeme,
            BhashaDataType::CHARACTER);
    }

    // Boolean true
    if (match(BhashaTokenType::BOOLEAN_TRUE))
    {
        return std::make_shared<LiteralExpression>(
            previous().lexeme,
            BhashaDataType::BOOLEAN);
    }

    // Boolean false
    if (match(BhashaTokenType::BOOLEAN_FALSE))
    {
        return std::make_shared<LiteralExpression>(
            previous().lexeme,
            BhashaDataType::BOOLEAN);
    }

    // Identifier
    if (match(BhashaTokenType::IDENTIFIER))
    {
        return std::make_shared<VariableExpression>(
            previous().lexeme);
    }

    // Parenthesized expression
    if (match(BhashaTokenType::LEFT_PAREN))
    {
        auto expression = parseExpression();

        consume(
            BhashaTokenType::RIGHT_PAREN,
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
        BhashaDataType::NUMBER);
}



BhashaDataType Parser::tokenToDataType(
    BhashaTokenType type) const
{
    switch (type)
    {
    case BhashaTokenType::NUMBER_TYPE:
        return BhashaDataType::NUMBER;

    case BhashaTokenType::DECIMAL_TYPE:
        return BhashaDataType::DECIMAL;

    case BhashaTokenType::STRING_TYPE:
        return BhashaDataType::STRING;

    case BhashaTokenType::CHAR_TYPE:
        return BhashaDataType::CHARACTER;

    case BhashaTokenType::BOOLEAN_TYPE:
        return BhashaDataType::BOOLEAN;

    case BhashaTokenType::LIST_TYPE:
        return BhashaDataType::LIST;

    case BhashaTokenType::LONG_TYPE:
        return BhashaDataType::LONG;

    case BhashaTokenType::POSITIVE_TYPE:
        return BhashaDataType::POSITIVE;

    case BhashaTokenType::VOID_TYPE:
        return BhashaDataType::VOID;

    default:
        return BhashaDataType::UNKNOWN;
    }
}