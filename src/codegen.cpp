#include "codegen.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

CodeGenerator::CodeGenerator()
    : indentLevel(0)
{
}


// INDENT


std::string CodeGenerator::indent() const
{
    return std::string(indentLevel * 4, ' ');
}


// EMIT
// Add one line to generated Python code


void CodeGenerator::emit(const std::string &line)
{
    output << indent();
    output << line;
    output << '\n';
}


// LITERAL


std::string CodeGenerator::generateLiteral(
    const std::shared_ptr<LiteralExpression> &expression)
{
    if (!expression)
    {
        return "";
    }

    // Boolean
    if (expression->dataType == AbhibyaktiDataType::BOOLEAN)
    {
        if (expression->value == "সত্য" ||
            expression->value == "true")
        {
            return "True";
        }

        if (expression->value == "মিথ্যা" ||
            expression->value == "false")
        {
            return "False";
        }
    }

    return expression->value;
}



std::string CodeGenerator::generateVariable(
    const std::shared_ptr<VariableExpression> &expression)
{
    if (!expression)
    {
        return "";
    }

    return expression->name;
}



std::string CodeGenerator::generateBinary(
    const std::shared_ptr<BinaryExpression> &expression)
{
    if (!expression)
    {
        return "";
    }

    std::string left =
        generateExpression(expression->left);

    std::string right =
        generateExpression(expression->right);

    std::string op =
        expression->operatorSymbol;

    // Abhibyakti → Python logical operators
    if (op == "&&")
    {
        op = "and";
    }
    else if (op == "||")
    {
        op = "or";
    }

    return "(" + left + " " + op + " " + right + ")";
}



std::string CodeGenerator::generateUnary(
    const std::shared_ptr<UnaryExpression> &expression)
{
    if (!expression)
    {
        return "";
    }

    std::string operand =
        generateExpression(expression->operand);

    std::string op =
        expression->operatorSymbol;

    // Abhibyakti ! → Python not
    if (op == "!")
    {
        return "(not " + operand + ")";
    }

    return "(" + op + operand + ")";
}


std::string CodeGenerator::generateExpression(
    const std::shared_ptr<Expression> &expression)
{
    if (!expression)
    {
        return "";
    }

    // Literal
    if (auto literal =
            std::dynamic_pointer_cast<LiteralExpression>(expression))
    {
        return generateLiteral(literal);
    }

    // Variable
    if (auto variable =
            std::dynamic_pointer_cast<VariableExpression>(expression))
    {
        return generateVariable(variable);
    }

    // Binary
    if (auto binary =
            std::dynamic_pointer_cast<BinaryExpression>(expression))
    {
        return generateBinary(binary);
    }

    // Unary
    if (auto unary =
            std::dynamic_pointer_cast<UnaryExpression>(expression))
    {
        return generateUnary(unary);
    }

    throw std::runtime_error(
        "Unknown expression type during code generation.");
}



void CodeGenerator::generateDeclaration(
    const std::shared_ptr<DeclarationStatement> &statement)
{
    if (!statement)
    {
        return;
    }

    std::string value =
        generateExpression(statement->initializer);

    emit(
        statement->variableName +
        " = " +
        value);
}



void CodeGenerator::generateAssignment(
    const std::shared_ptr<AssignmentStatement> &statement)
{
    if (!statement)
    {
        return;
    }

    std::string value =
        generateExpression(statement->value);

    emit(
        statement->variableName +
        " = " +
        value);
}



void CodeGenerator::generatePrint(
    const std::shared_ptr<PrintStatement> &statement)
{
    if (!statement)
    {
        return;
    }

    std::string expression =
        generateExpression(statement->expression);

    emit(
        "print(" +
        expression +
        ")");
}



void CodeGenerator::generateBlock(
    const std::shared_ptr<BlockStatement> &block)
{
    if (!block)
    {
        return;
    }

    indentLevel++;

    for (const auto &statement : block->statements)
    {
        generateStatement(statement);
    }

    indentLevel--;
}



void CodeGenerator::generateIf(
    const std::shared_ptr<IfStatement> &statement)
{
    if (!statement)
    {
        return;
    }

    std::string condition =
        generateExpression(statement->condition);

    emit("if " + condition + ":");

    generateBlock(statement->thenBranch);

    if (statement->elseBranch)
    {
        emit("else:");

        generateBlock(statement->elseBranch);
    }
}


void CodeGenerator::generateWhile(
    const std::shared_ptr<WhileStatement> &statement)
{
    if (!statement)
    {
        return;
    }

    std::string condition =
        generateExpression(statement->condition);

    emit("while " + condition + ":");

    generateBlock(statement->body);
}



void CodeGenerator::generateStatement(
    const std::shared_ptr<Statement> &statement)
{
    if (!statement)
    {
        return;
    }

    // Declaration
    if (auto declaration =
            std::dynamic_pointer_cast<DeclarationStatement>(statement))
    {
        generateDeclaration(declaration);
        return;
    }

    // Assignment
    if (auto assignment =
            std::dynamic_pointer_cast<AssignmentStatement>(statement))
    {
        generateAssignment(assignment);
        return;
    }

    // Print
    if (auto print =
            std::dynamic_pointer_cast<PrintStatement>(statement))
    {
        generatePrint(print);
        return;
    }

    // Block
    if (auto block =
            std::dynamic_pointer_cast<BlockStatement>(statement))
    {
        generateBlock(block);
        return;
    }

    // If
    if (auto ifStatement =
            std::dynamic_pointer_cast<IfStatement>(statement))
    {
        generateIf(ifStatement);
        return;
    }

    // While
    if (auto whileStatement =
            std::dynamic_pointer_cast<WhileStatement>(statement))
    {
        generateWhile(whileStatement);
        return;
    }

    throw std::runtime_error(
        "Unknown statement type during code generation.");
}



std::string CodeGenerator::generate(
    const std::shared_ptr<Program> &program)
{
    output.str("");
    output.clear();

    indentLevel = 0;

    if (!program)
    {
        return "";
    }

    for (const auto &statement : program->statements)
    {
        generateStatement(statement);
    }

    return output.str();
}



bool CodeGenerator::generateToFile(
    const std::shared_ptr<Program> &program,
    const std::string &filename)
{
    std::string generatedCode =
        generate(program);

    std::ofstream file(filename);

    if (!file.is_open())
    {
        return false;
    }

    file << generatedCode;

    file.close();

    return true;
}