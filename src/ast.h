#ifndef AST_H
#define AST_H

#include <memory>
#include <string>
#include <vector>
#include <utility>

// BHASHA DATA TYPES


enum class BhashaDataType
{
    NUMBER,
    DECIMAL,
    STRING,
    CHARACTER,
    BOOLEAN,
    LIST,
    LONG,
    POSITIVE,
    VOID,
    UNKNOWN
};


// BASE AST NODE


class ASTNode
{
public:
    virtual ~ASTNode() = default;
};


// EXPRESSION BASE CLASS


class Expression : public ASTNode
{
public:
    virtual ~Expression() = default;
};


// STATEMENT BASE CLASS


class Statement : public ASTNode
{
public:
    virtual ~Statement() = default;
};



class LiteralExpression : public Expression
{
public:
    std::string value;
    BhashaDataType dataType;

    LiteralExpression(
        const std::string &value,
        BhashaDataType dataType)
        : value(value),
          dataType(dataType) {}
};



class VariableExpression : public Expression
{
public:
    std::string name;

    explicit VariableExpression(const std::string &name)
        : name(name) {}
};



class BinaryExpression : public Expression
{
public:
    std::shared_ptr<Expression> left;
    std::string operatorSymbol;
    std::shared_ptr<Expression> right;

    BinaryExpression(
        std::shared_ptr<Expression> left,
        const std::string &operatorSymbol,
        std::shared_ptr<Expression> right)
        : left(std::move(left)),
          operatorSymbol(operatorSymbol),
          right(std::move(right)) {}
};



class UnaryExpression : public Expression
{
public:
    std::string operatorSymbol;
    std::shared_ptr<Expression> operand;

    UnaryExpression(
        const std::string &operatorSymbol,
        std::shared_ptr<Expression> operand)
        : operatorSymbol(operatorSymbol),
          operand(std::move(operand)) {}
};



class DeclarationStatement : public Statement
{
public:
    BhashaDataType dataType;
    std::string variableName;
    std::shared_ptr<Expression> initializer;

    DeclarationStatement(
        BhashaDataType dataType,
        const std::string &variableName,
        std::shared_ptr<Expression> initializer)
        : dataType(dataType),
          variableName(variableName),
          initializer(std::move(initializer)) {}
};



class AssignmentStatement : public Statement
{
public:
    std::string variableName;
    std::shared_ptr<Expression> value;

    AssignmentStatement(
        const std::string &variableName,
        std::shared_ptr<Expression> value)
        : variableName(variableName),
          value(std::move(value)) {}
};



class PrintStatement : public Statement
{
public:
    std::shared_ptr<Expression> expression;

    explicit PrintStatement(
        std::shared_ptr<Expression> expression)
        : expression(std::move(expression)) {}
};



class BlockStatement : public Statement
{
public:
    std::vector<std::shared_ptr<Statement>> statements;

    BlockStatement() = default;

    explicit BlockStatement(
        std::vector<std::shared_ptr<Statement>> statements)
        : statements(std::move(statements)) {}
};



class IfStatement : public Statement
{
public:
    std::shared_ptr<Expression> condition;

    std::shared_ptr<BlockStatement> thenBranch;

    std::shared_ptr<BlockStatement> elseBranch;

    IfStatement(
        std::shared_ptr<Expression> condition,
        std::shared_ptr<BlockStatement> thenBranch,
        std::shared_ptr<BlockStatement> elseBranch = nullptr)
        : condition(std::move(condition)),
          thenBranch(std::move(thenBranch)),
          elseBranch(std::move(elseBranch)) {}
};



class WhileStatement : public Statement
{
public:
    std::shared_ptr<Expression> condition;

    std::shared_ptr<BlockStatement> body;

    WhileStatement(
        std::shared_ptr<Expression> condition,
        std::shared_ptr<BlockStatement> body)
        : condition(std::move(condition)),
          body(std::move(body)) {}
};



class Program : public ASTNode
{
public:
    std::vector<std::shared_ptr<Statement>> statements;

    Program() = default;

    explicit Program(
        std::vector<std::shared_ptr<Statement>> statements)
        : statements(std::move(statements)) {}
};

#endif // AST_H