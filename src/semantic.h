#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class SemanticAnalyzer
{
private:
    // ========================================================
    // SYMBOL TABLE
    // Variable name -> Data type
    // ========================================================

    std::unordered_map<std::string, BhashaDataType> symbolTable;

    // ========================================================
    // SEMANTIC ERRORS
    // ========================================================

    std::vector<std::string> errors;

    // ========================================================
    // EXPRESSION ANALYSIS
    // ========================================================

    BhashaDataType analyzeExpression(
        const std::shared_ptr<Expression>& expression);

    // ========================================================
    // STATEMENT ANALYSIS
    // ========================================================

    void analyzeStatement(
        const std::shared_ptr<Statement>& statement);

    // ========================================================
    // BLOCK ANALYSIS
    // ========================================================

    void analyzeBlock(
        const std::shared_ptr<BlockStatement>& block);

    // ========================================================
    // ERROR HANDLING
    // ========================================================

    void addError(
        const std::string& message);

public:
    // Constructor
    SemanticAnalyzer() = default;

    // Analyze complete AST
    void analyze(
        const std::shared_ptr<Program>& program);

    // Check whether semantic errors exist
    bool hasErrors() const;

    // Print semantic errors
    void printErrors() const;
};

#endif // SEMANTIC_H