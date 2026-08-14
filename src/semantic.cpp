#include "semantic.h"

#include <iostream>
#include <sstream>

// ============================================================
// HELPER FUNCTIONS
// ============================================================

static std::string dataTypeToString(BhashaDataType type)
{
    switch (type)
    {
    case BhashaDataType::NUMBER:
        return "NUMBER";

    case BhashaDataType::DECIMAL:
        return "DECIMAL";

    case BhashaDataType::STRING:
        return "STRING";

    case BhashaDataType::CHARACTER:
        return "CHARACTER";

    case BhashaDataType::BOOLEAN:
        return "BOOLEAN";

    case BhashaDataType::LIST:
        return "LIST";

    case BhashaDataType::LONG:
        return "LONG";

    case BhashaDataType::POSITIVE:
        return "POSITIVE";

    case BhashaDataType::VOID:
        return "VOID";

    case BhashaDataType::UNKNOWN:
    default:
        return "UNKNOWN";
    }
}

// ============================================================
// NUMERIC TYPE CHECK
// ============================================================

static bool isNumericType(BhashaDataType type)
{
    return type == BhashaDataType::NUMBER ||
           type == BhashaDataType::DECIMAL ||
           type == BhashaDataType::LONG ||
           type == BhashaDataType::POSITIVE;
}

// ============================================================
// TYPE COMPATIBILITY
// ============================================================

static bool areTypesCompatible(
    BhashaDataType expected,
    BhashaDataType actual)
{
    // Exact same type
    if (expected == actual)
    {
        return true;
    }

    // NUMBER can be assigned to DECIMAL
    if (expected == BhashaDataType::DECIMAL &&
        actual == BhashaDataType::NUMBER)
    {
        return true;
    }

    // NUMBER can be assigned to LONG
    if (expected == BhashaDataType::LONG &&
        actual == BhashaDataType::NUMBER)
    {
        return true;
    }

    // Numeric values can be assigned to POSITIVE.
    // The actual positive-value check is intentionally
    // kept simple because LiteralExpression stores values
    // as strings.
    if (expected == BhashaDataType::POSITIVE &&
        isNumericType(actual))
    {
        return true;
    }

    return false;
}

// ============================================================
// CONSTRUCTOR
// ============================================================
// Constructor is defaulted in semantic.h.
// No separate implementation is required.

// ============================================================
// ADD ERROR
// ============================================================

void SemanticAnalyzer::addError(
    const std::string &message)
{
    errors.push_back(message);
}

// ============================================================
// ANALYZE COMPLETE PROGRAM
// ============================================================

void SemanticAnalyzer::analyze(
    const std::shared_ptr<Program> &program)
{
    // Clear previous analysis results
    symbolTable.clear();
    errors.clear();

    if (!program)
    {
        addError("Program is null.");
        return;
    }

    for (const auto &statement :
         program->statements)
    {
        analyzeStatement(statement);
    }
}

// ============================================================
// ANALYZE STATEMENT
// ============================================================

void SemanticAnalyzer::analyzeStatement(
    const std::shared_ptr<Statement> &statement)
{
    if (!statement)
    {
        return;
    }

    // ========================================================
    // VARIABLE DECLARATION
    // ========================================================

    auto declaration =
        std::dynamic_pointer_cast<DeclarationStatement>(
            statement);

    if (declaration)
    {
        const std::string &name =
            declaration->variableName;

        // Check duplicate declaration
        if (symbolTable.find(name) != symbolTable.end())
        {
            addError(
                "Variable '" +
                name +
                "' is already declared.");

            // Still analyze initializer
            if (declaration->initializer)
            {
                analyzeExpression(
                    declaration->initializer);
            }

            return;
        }

        // Analyze initializer
        BhashaDataType initializerType =
            analyzeExpression(
                declaration->initializer);

        // Check type compatibility
        if (initializerType != BhashaDataType::UNKNOWN &&
            !areTypesCompatible(
                declaration->dataType,
                initializerType))
        {
            addError(
                "Type mismatch: variable '" +
                name +
                "' is declared as " +
                dataTypeToString(
                    declaration->dataType) +
                " but initializer is " +
                dataTypeToString(
                    initializerType) +
                ".");
        }

        // Add variable to symbol table
        symbolTable[name] =
            declaration->dataType;

        return;
    }

    // ========================================================
    // ASSIGNMENT
    // ========================================================

    auto assignment =
        std::dynamic_pointer_cast<AssignmentStatement>(
            statement);

    if (assignment)
    {
        const std::string &name =
            assignment->variableName;

        // Check variable exists
        auto it =
            symbolTable.find(name);

        if (it == symbolTable.end())
        {
            addError(
                "Variable '" +
                name +
                "' is not declared.");

            // Still analyze RHS
            analyzeExpression(
                assignment->value);

            return;
        }

        // Analyze assigned value
        BhashaDataType valueType =
            analyzeExpression(
                assignment->value);

        // Check assignment type
        if (valueType != BhashaDataType::UNKNOWN &&
            !areTypesCompatible(
                it->second,
                valueType))
        {
            addError(
                "Type mismatch: cannot assign " +
                dataTypeToString(valueType) +
                " to variable '" +
                name +
                "' of type " +
                dataTypeToString(it->second) +
                ".");
        }

        return;
    }

    // ========================================================
    // PRINT STATEMENT
    // ========================================================

    auto print =
        std::dynamic_pointer_cast<PrintStatement>(
            statement);

    if (print)
    {
        analyzeExpression(
            print->expression);

        return;
    }

    // ========================================================
    // BLOCK STATEMENT
    // ========================================================

    auto block =
        std::dynamic_pointer_cast<BlockStatement>(
            statement);

    if (block)
    {
        analyzeBlock(block);
        return;
    }

    // ========================================================
    // IF STATEMENT
    // ========================================================

    auto ifStatement =
        std::dynamic_pointer_cast<IfStatement>(
            statement);

    if (ifStatement)
    {
        BhashaDataType conditionType =
            analyzeExpression(
                ifStatement->condition);

        // IF condition must be BOOLEAN
        if (conditionType != BhashaDataType::UNKNOWN &&
            conditionType != BhashaDataType::BOOLEAN)
        {
            addError(
                "If condition must be BOOLEAN, but got " +
                dataTypeToString(conditionType) +
                ".");
        }

        // Analyze THEN block
        if (ifStatement->thenBranch)
        {
            analyzeBlock(
                ifStatement->thenBranch);
        }

        // Analyze ELSE block
        if (ifStatement->elseBranch)
        {
            analyzeBlock(
                ifStatement->elseBranch);
        }

        return;
    }

    // ========================================================
    // WHILE STATEMENT
    // ========================================================

    auto whileStatement =
        std::dynamic_pointer_cast<WhileStatement>(
            statement);

    if (whileStatement)
    {
        BhashaDataType conditionType =
            analyzeExpression(
                whileStatement->condition);

        // WHILE condition must be BOOLEAN
        if (conditionType != BhashaDataType::UNKNOWN &&
            conditionType != BhashaDataType::BOOLEAN)
        {
            addError(
                "While condition must be BOOLEAN, but got " +
                dataTypeToString(conditionType) +
                ".");
        }

        // Analyze WHILE body
        if (whileStatement->body)
        {
            analyzeBlock(
                whileStatement->body);
        }

        return;
    }
}

// ============================================================
// ANALYZE BLOCK
// ============================================================

void SemanticAnalyzer::analyzeBlock(
    const std::shared_ptr<BlockStatement> &block)
{
    if (!block)
    {
        return;
    }

    for (const auto &statement :
         block->statements)
    {
        analyzeStatement(statement);
    }
}

// ============================================================
// ANALYZE EXPRESSION
// ============================================================

BhashaDataType SemanticAnalyzer::analyzeExpression(
    const std::shared_ptr<Expression> &expression)
{
    if (!expression)
    {
        return BhashaDataType::UNKNOWN;
    }

    // ========================================================
    // LITERAL
    // ========================================================

    auto literal =
        std::dynamic_pointer_cast<LiteralExpression>(
            expression);

    if (literal)
    {
        return literal->dataType;
    }

    // ========================================================
    // VARIABLE
    // ========================================================

    auto variable =
        std::dynamic_pointer_cast<VariableExpression>(
            expression);

    if (variable)
    {
        auto it =
            symbolTable.find(variable->name);

        if (it == symbolTable.end())
        {
            addError(
                "Variable '" +
                variable->name +
                "' is not declared.");

            return BhashaDataType::UNKNOWN;
        }

        return it->second;
    }

    // ========================================================
    // BINARY EXPRESSION
    // ========================================================

    auto binary =
        std::dynamic_pointer_cast<BinaryExpression>(
            expression);

    if (binary)
    {
        BhashaDataType leftType =
            analyzeExpression(binary->left);

        BhashaDataType rightType =
            analyzeExpression(binary->right);

        const std::string &op =
            binary->operatorSymbol;

        // ----------------------------------------------------
        // ARITHMETIC OPERATORS
        // ----------------------------------------------------

        if (op == "+" ||
            op == "-" ||
            op == "*" ||
            op == "/" ||
            op == "%")
        {
            // String + String
            if (op == "+" &&
                leftType == BhashaDataType::STRING &&
                rightType == BhashaDataType::STRING)
            {
                return BhashaDataType::STRING;
            }

            // Both operands must be numeric
            if (!isNumericType(leftType) ||
                !isNumericType(rightType))
            {
                addError(
                    "Operator '" +
                    op +
                    "' requires numeric operands.");

                return BhashaDataType::UNKNOWN;
            }

            // Modulo with decimal is not allowed
            if (op == "%" &&
                (leftType == BhashaDataType::DECIMAL ||
                 rightType == BhashaDataType::DECIMAL))
            {
                addError(
                    "Modulo operator '%' cannot be used with DECIMAL values.");

                return BhashaDataType::UNKNOWN;
            }

            // Division produces DECIMAL
            if (op == "/")
            {
                return BhashaDataType::DECIMAL;
            }

            // If either operand is decimal,
            // result is decimal
            if (leftType == BhashaDataType::DECIMAL ||
                rightType == BhashaDataType::DECIMAL)
            {
                return BhashaDataType::DECIMAL;
            }

            return BhashaDataType::NUMBER;
        }

        // ----------------------------------------------------
        // COMPARISON OPERATORS
        // ----------------------------------------------------

        if (op == ">" ||
            op == "<" ||
            op == ">=" ||
            op == "<=")
        {
            if (!isNumericType(leftType) ||
                !isNumericType(rightType))
            {
                addError(
                    "Comparison operator '" +
                    op +
                    "' requires numeric operands.");

                return BhashaDataType::UNKNOWN;
            }

            return BhashaDataType::BOOLEAN;
        }

        // ----------------------------------------------------
        // EQUALITY OPERATORS
        // ----------------------------------------------------

        if (op == "==" ||
            op == "!=")
        {
            if (leftType != BhashaDataType::UNKNOWN &&
                rightType != BhashaDataType::UNKNOWN)
            {
                bool compatible =
                    areTypesCompatible(
                        leftType,
                        rightType) ||
                    areTypesCompatible(
                        rightType,
                        leftType);

                if (!compatible)
                {
                    addError(
                        "Cannot compare " +
                        dataTypeToString(leftType) +
                        " with " +
                        dataTypeToString(rightType) +
                        ".");
                }
            }

            return BhashaDataType::BOOLEAN;
        }

        // ----------------------------------------------------
        // LOGICAL OPERATORS
        // ----------------------------------------------------

        if (op == "&&" ||
            op == "||")
        {
            if (leftType != BhashaDataType::BOOLEAN ||
                rightType != BhashaDataType::BOOLEAN)
            {
                addError(
                    "Logical operator '" +
                    op +
                    "' requires BOOLEAN operands.");

                return BhashaDataType::UNKNOWN;
            }

            return BhashaDataType::BOOLEAN;
        }

        // Unknown operator
        addError(
            "Unknown binary operator '" +
            op +
            "'.");

        return BhashaDataType::UNKNOWN;
    }

    // ========================================================
    // UNARY EXPRESSION
    // ========================================================

    auto unary =
        std::dynamic_pointer_cast<UnaryExpression>(
            expression);

    if (unary)
    {
        BhashaDataType operandType =
            analyzeExpression(
                unary->operand);

        const std::string &op =
            unary->operatorSymbol;

        // ----------------------------------------------------
        // NOT
        // ----------------------------------------------------

        if (op == "!")
        {
            if (operandType != BhashaDataType::UNKNOWN &&
                operandType != BhashaDataType::BOOLEAN)
            {
                addError(
                    "Unary operator '!' requires BOOLEAN operand.");

                return BhashaDataType::UNKNOWN;
            }

            return BhashaDataType::BOOLEAN;
        }

        // ----------------------------------------------------
        // UNARY PLUS / MINUS
        // ----------------------------------------------------

        if (op == "+" ||
            op == "-")
        {
            if (!isNumericType(operandType))
            {
                addError(
                    "Unary operator '" +
                    op +
                    "' requires numeric operand.");

                return BhashaDataType::UNKNOWN;
            }

            return operandType;
        }

        // Unknown unary operator
        addError(
            "Unknown unary operator '" +
            op +
            "'.");

        return BhashaDataType::UNKNOWN;
    }

    return BhashaDataType::UNKNOWN;
}

// ============================================================
// HAS ERRORS
// ============================================================

bool SemanticAnalyzer::hasErrors() const
{
    return !errors.empty();
}

// ============================================================
// PRINT ERRORS
// ============================================================

void SemanticAnalyzer::printErrors() const
{
    if (errors.empty())
    {
        std::cout
            << "No semantic errors found."
            << std::endl;

        return;
    }

    std::cout
        << "Semantic Errors:"
        << std::endl;

    for (const auto &error :
         errors)
    {
        std::cout
            << "  - "
            << error
            << std::endl;
    }
}