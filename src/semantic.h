#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include <string>
#include <unordered_map>
#include <vector>

class SemanticAnalyzer
{
private:
    // Variable name -> data type
    std::unordered_map<std::string, BhashaDataType> symbolTable;

    // Store semantic errors
    std::vector<std::string> errors;

    // Analyze expressions
    BhashaDataType analyzeExpression(
        const std::shared_ptr<Expression> &expression);

    // Analyze statements
    void analyzeStatement(
        const std::shared_ptr<Statement> &statement);

    // Analyze blocks
    void analyzeBlock(
        const std::shared_ptr<BlockStatement> &block);

    // Add an error message
    void addError(const std::string &message);

public:
    SemanticAnalyzer() = default;

    // Analyze complete AST
    void analyze(const std::shared_ptr<Program> &program);

    // Check whether semantic errors exist
    bool hasErrors() const;

    // Print all semantic errors
    void printErrors() const;
};

#endif