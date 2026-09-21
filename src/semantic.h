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
    

    std::unordered_map<std::string, AbhibyaktiDataType> symbolTable;



    std::vector<std::string> errors;

 

    AbhibyaktiDataType analyzeExpression(
        const std::shared_ptr<Expression>& expression);

   

    void analyzeStatement(
        const std::shared_ptr<Statement>& statement);


    void analyzeBlock(
        const std::shared_ptr<BlockStatement>& block);

  

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