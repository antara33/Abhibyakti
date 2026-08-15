#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"

#include <string>
#include <memory>
#include <sstream>

class CodeGenerator
{
private:
    std::stringstream output;
    int indentLevel;

    // ========================================================
    // INDENTATION
    // ========================================================

    std::string indent() const;

    // Add one line to generated Python code
    void emit(const std::string &line);

    // ========================================================
    // EXPRESSION GENERATION
    // ========================================================

    std::string generateExpression(
        const std::shared_ptr<Expression> &expression);

    std::string generateLiteral(
        const std::shared_ptr<LiteralExpression> &expression);

    std::string generateVariable(
        const std::shared_ptr<VariableExpression> &expression);

    std::string generateBinary(
        const std::shared_ptr<BinaryExpression> &expression);

    std::string generateUnary(
        const std::shared_ptr<UnaryExpression> &expression);

    // ========================================================
    // STATEMENT GENERATION
    // ========================================================

    void generateStatement(
        const std::shared_ptr<Statement> &statement);

    void generateDeclaration(
        const std::shared_ptr<DeclarationStatement> &statement);

    void generateAssignment(
        const std::shared_ptr<AssignmentStatement> &statement);

    void generatePrint(
        const std::shared_ptr<PrintStatement> &statement);

    void generateBlock(
        const std::shared_ptr<BlockStatement> &statement);

    void generateIf(
        const std::shared_ptr<IfStatement> &statement);

    void generateWhile(
        const std::shared_ptr<WhileStatement> &statement);

public:
    CodeGenerator();

    // Generate complete Python source code
    std::string generate(
        const std::shared_ptr<Program> &program);

    // Generate Python code and save it to a file
    bool generateToFile(
        const std::shared_ptr<Program> &program,
        const std::string &filename);
};

#endif