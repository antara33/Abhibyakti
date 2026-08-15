#include "semantic.h"

#include <iostream>



static std::string dataTypeToString(AbhibyaktiDataType type)
{
    switch (type)
    {
    case AbhibyaktiDataType::NUMBER:
        return "NUMBER";

    case AbhibyaktiDataType::DECIMAL:
        return "DECIMAL";

    case AbhibyaktiDataType::STRING:
        return "STRING";

    case AbhibyaktiDataType::CHARACTER:
        return "CHARACTER";

    case AbhibyaktiDataType::BOOLEAN:
        return "BOOLEAN";

    case AbhibyaktiDataType::LIST:
        return "LIST";

    case AbhibyaktiDataType::LONG:
        return "LONG";

    case AbhibyaktiDataType::POSITIVE:
        return "POSITIVE";

    case AbhibyaktiDataType::VOID:
        return "VOID";

    case AbhibyaktiDataType::UNKNOWN:
    default:
        return "UNKNOWN";
    }
}



static bool isNumericType(AbhibyaktiDataType type)
{
    return type == AbhibyaktiDataType::NUMBER ||
           type == AbhibyaktiDataType::DECIMAL ||
           type == AbhibyaktiDataType::LONG ||
           type == AbhibyaktiDataType::POSITIVE;
}


// TYPE COMPATIBILITY


static bool areTypesCompatible(
    AbhibyaktiDataType expected,
    AbhibyaktiDataType actual)
{
    // Same type
    if (expected == actual)
    {
        return true;
    }

    // NUMBER -> DECIMAL
    if (expected == AbhibyaktiDataType::DECIMAL &&
        actual == AbhibyaktiDataType::NUMBER)
    {
        return true;
    }

    // NUMBER -> LONG
    if (expected == AbhibyaktiDataType::LONG &&
        actual == AbhibyaktiDataType::NUMBER)
    {
        return true;
    }

    // Numeric -> POSITIVE
    if (expected == AbhibyaktiDataType::POSITIVE &&
        isNumericType(actual))
    {
        return true;
    }

    return false;
}


// ADD ERROR


void SemanticAnalyzer::addError(
    const std::string& message)
{
    errors.push_back(message);
}


// ANALYZE COMPLETE PROGRAM


void SemanticAnalyzer::analyze(
    const std::shared_ptr<Program>& program)
{
    // Clear previous results
    symbolTable.clear();
    errors.clear();

    if (!program)
    {
        addError("Program is null.");
        return;
    }

    for (const auto& statement :
         program->statements)
    {
        analyzeStatement(statement);
    }
}


// ANALYZE STATEMENT


void SemanticAnalyzer::analyzeStatement(
    const std::shared_ptr<Statement>& statement)
{
    if (!statement)
    {
        return;
    }

    
    // VARIABLE DECLARATION
   

    auto declaration =
        std::dynamic_pointer_cast<DeclarationStatement>(
            statement);

    if (declaration)
    {
        const std::string& name =
            declaration->variableName;

        // Duplicate declaration
        if (symbolTable.find(name) != symbolTable.end())
        {
            addError(
                "Variable '" +
                name +
                "' is already declared.");

            if (declaration->initializer)
            {
                analyzeExpression(
                    declaration->initializer);
            }

            return;
        }

        // Analyze initializer
        AbhibyaktiDataType initializerType =
            analyzeExpression(
                declaration->initializer);

        // Check type compatibility
        if (initializerType != AbhibyaktiDataType::UNKNOWN &&
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

        // Add to symbol table
        symbolTable[name] =
            declaration->dataType;

        return;
    }

    
    // ASSIGNMENT
   

    auto assignment =
        std::dynamic_pointer_cast<AssignmentStatement>(
            statement);

    if (assignment)
    {
        const std::string& name =
            assignment->variableName;

        auto it =
            symbolTable.find(name);

        // Variable does not exist
        if (it == symbolTable.end())
        {
            addError(
                "Variable '" +
                name +
                "' is not declared.");

            analyzeExpression(
                assignment->value);

            return;
        }

        // Analyze RHS
        AbhibyaktiDataType valueType =
            analyzeExpression(
                assignment->value);

        // Check assignment type
        if (valueType != AbhibyaktiDataType::UNKNOWN &&
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

    
    // PRINT STATEMENT
    

    auto print =
        std::dynamic_pointer_cast<PrintStatement>(
            statement);

    if (print)
    {
        analyzeExpression(
            print->expression);

        return;
    }

    
    // BLOCK STATEMENT
    

    auto block =
        std::dynamic_pointer_cast<BlockStatement>(
            statement);

    if (block)
    {
        analyzeBlock(block);
        return;
    }



    auto ifStatement =
        std::dynamic_pointer_cast<IfStatement>(
            statement);

    if (ifStatement)
    {
        AbhibyaktiDataType conditionType =
            analyzeExpression(
                ifStatement->condition);

        // IF condition must be BOOLEAN
        if (conditionType != AbhibyaktiDataType::UNKNOWN &&
            conditionType != AbhibyaktiDataType::BOOLEAN)
        {
            addError(
                "If condition must be BOOLEAN, but got " +
                dataTypeToString(conditionType) +
                ".");
        }

        // THEN block
        if (ifStatement->thenBranch)
        {
            analyzeBlock(
                ifStatement->thenBranch);
        }

        // ELSE block
        if (ifStatement->elseBranch)
        {
            analyzeBlock(
                ifStatement->elseBranch);
        }

        return;
    }

   
    // WHILE STATEMENT
    

    auto whileStatement =
        std::dynamic_pointer_cast<WhileStatement>(
            statement);

    if (whileStatement)
    {
        AbhibyaktiDataType conditionType =
            analyzeExpression(
                whileStatement->condition);

        // WHILE condition must be BOOLEAN
        if (conditionType != AbhibyaktiDataType::UNKNOWN &&
            conditionType != AbhibyaktiDataType::BOOLEAN)
        {
            addError(
                "While condition must be BOOLEAN, but got " +
                dataTypeToString(conditionType) +
                ".");
        }

        // WHILE body
        if (whileStatement->body)
        {
            analyzeBlock(
                whileStatement->body);
        }

        return;
    }
}


// ANALYZE BLOCK


void SemanticAnalyzer::analyzeBlock(
    const std::shared_ptr<BlockStatement>& block)
{
    if (!block)
    {
        return;
    }

    for (const auto& statement :
         block->statements)
    {
        analyzeStatement(statement);
    }
}


// ANALYZE EXPRESSION


AbhibyaktiDataType SemanticAnalyzer::analyzeExpression(
    const std::shared_ptr<Expression>& expression)
{
    if (!expression)
    {
        return AbhibyaktiDataType::UNKNOWN;
    }

    
    // LITERAL
   

    auto literal =
        std::dynamic_pointer_cast<LiteralExpression>(
            expression);

    if (literal)
    {
        return literal->dataType;
    }

   
    // VARIABLE
   
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

            return AbhibyaktiDataType::UNKNOWN;
        }

        return it->second;
    }

    
    // BINARY EXPRESSION
    

    auto binary =
        std::dynamic_pointer_cast<BinaryExpression>(
            expression);

    if (binary)
    {
        AbhibyaktiDataType leftType =
            analyzeExpression(binary->left);

        AbhibyaktiDataType rightType =
            analyzeExpression(binary->right);

        const std::string& op =
            binary->operatorSymbol;

        
        // ARITHMETIC OPERATORS

        if (op == "+" ||
            op == "-" ||
            op == "*" ||
            op == "/" ||
            op == "%")
        {
            // String + String
            if (op == "+" &&
                leftType == AbhibyaktiDataType::STRING &&
                rightType == AbhibyaktiDataType::STRING)
            {
                return AbhibyaktiDataType::STRING;
            }

            // Both must be numeric
            if (!isNumericType(leftType) ||
                !isNumericType(rightType))
            {
                addError(
                    "Operator '" +
                    op +
                    "' requires numeric operands.");

                return AbhibyaktiDataType::UNKNOWN;
            }

            // Decimal modulo not allowed
            if (op == "%" &&
                (leftType == AbhibyaktiDataType::DECIMAL ||
                 rightType == AbhibyaktiDataType::DECIMAL))
            {
                addError(
                    "Modulo operator '%' cannot be used with DECIMAL values.");

                return AbhibyaktiDataType::UNKNOWN;
            }

            // Division produces DECIMAL
            if (op == "/")
            {
                return AbhibyaktiDataType::DECIMAL;
            }

            // Decimal result
            if (leftType == AbhibyaktiDataType::DECIMAL ||
                rightType == AbhibyaktiDataType::DECIMAL)
            {
                return AbhibyaktiDataType::DECIMAL;
            }

            return AbhibyaktiDataType::NUMBER;
        }

        

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

                return AbhibyaktiDataType::UNKNOWN;
            }

            return AbhibyaktiDataType::BOOLEAN;
        }

        if (op == "==" ||
            op == "!=")
        {
            if (leftType != AbhibyaktiDataType::UNKNOWN &&
                rightType != AbhibyaktiDataType::UNKNOWN)
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

            return AbhibyaktiDataType::BOOLEAN;
        }

        if (op == "&&" ||
            op == "||")
        {
            if (leftType != AbhibyaktiDataType::BOOLEAN ||
                rightType != AbhibyaktiDataType::BOOLEAN)
            {
                addError(
                    "Logical operator '" +
                    op +
                    "' requires BOOLEAN operands.");

                return AbhibyaktiDataType::UNKNOWN;
            }

            return AbhibyaktiDataType::BOOLEAN;
        }

        // Unknown operator
        addError(
            "Unknown binary operator '" +
            op +
            "'.");

        return AbhibyaktiDataType::UNKNOWN;
    }



    auto unary =
        std::dynamic_pointer_cast<UnaryExpression>(
            expression);

    if (unary)
    {
        AbhibyaktiDataType operandType =
            analyzeExpression(
                unary->operand);

        const std::string& op =
            unary->operatorSymbol;

        // NOT
        if (op == "!")
        {
            if (operandType != AbhibyaktiDataType::UNKNOWN &&
                operandType != AbhibyaktiDataType::BOOLEAN)
            {
                addError(
                    "Unary operator '!' requires BOOLEAN operand.");

                return AbhibyaktiDataType::UNKNOWN;
            }

            return AbhibyaktiDataType::BOOLEAN;
        }

        // Unary + / -
        if (op == "+" ||
            op == "-")
        {
            if (!isNumericType(operandType))
            {
                addError(
                    "Unary operator '" +
                    op +
                    "' requires numeric operand.");

                return AbhibyaktiDataType::UNKNOWN;
            }

            return operandType;
        }

        addError(
            "Unknown unary operator '" +
            op +
            "'.");

        return AbhibyaktiDataType::UNKNOWN;
    }

    return AbhibyaktiDataType::UNKNOWN;
}


// HAS ERRORS


bool SemanticAnalyzer::hasErrors() const
{
    return !errors.empty();
}


// PRINT ERRORS


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

    for (const auto& error : errors)
    {
        std::cout
            << "  - "
            << error
            << std::endl;
    }
}