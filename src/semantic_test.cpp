#include "semantic.h"

#include <iostream>
#include <memory>
#include <vector>


// SEMANTIC ANALYZER TEST


int main()
{
    std::cout << "====================================\n";
    std::cout << "     SEMANTIC ANALYSIS TEST\n";
    std::cout << "====================================\n\n";



    std::cout << "TEST 1: Valid Program\n";
    std::cout << "------------------------------------\n";

    auto validProgram = std::make_shared<Program>();

    // সংখ্যা x = 10;
    validProgram->statements.push_back(
        std::make_shared<DeclarationStatement>(
            BhashaDataType::NUMBER,
            "x",
            std::make_shared<LiteralExpression>(
                "10",
                BhashaDataType::NUMBER)));

    // দশমিক y = 5.5;
    validProgram->statements.push_back(
        std::make_shared<DeclarationStatement>(
            BhashaDataType::DECIMAL,
            "y",
            std::make_shared<LiteralExpression>(
                "5.5",
                BhashaDataType::DECIMAL)));

    // x = x + 5;
    auto xVariable =
        std::make_shared<VariableExpression>("x");

    auto five =
        std::make_shared<LiteralExpression>(
            "5",
            BhashaDataType::NUMBER);

    auto addition =
        std::make_shared<BinaryExpression>(
            xVariable,
            "+",
            five);

    validProgram->statements.push_back(
        std::make_shared<AssignmentStatement>(
            "x",
            addition));

    // দেখাও(x);
    validProgram->statements.push_back(
        std::make_shared<PrintStatement>(
            std::make_shared<VariableExpression>("x")));

    SemanticAnalyzer analyzer1;

    analyzer1.analyze(validProgram);

    if (analyzer1.hasErrors())
    {
        std::cout << "FAILED: Semantic errors found.\n";
        analyzer1.printErrors();
    }
    else
    {
        std::cout << "PASSED: No semantic errors found.\n";
    }

  

    std::cout << "\n";
    std::cout << "TEST 2: Undeclared Variable\n";
    std::cout << "------------------------------------\n";

    auto undeclaredProgram =
        std::make_shared<Program>();

    undeclaredProgram->statements.push_back(
        std::make_shared<AssignmentStatement>(
            "x",
            std::make_shared<LiteralExpression>(
                "10",
                BhashaDataType::NUMBER)));

    SemanticAnalyzer analyzer2;

    analyzer2.analyze(undeclaredProgram);

    if (analyzer2.hasErrors())
    {
        std::cout << "PASSED: Error detected correctly.\n";
        analyzer2.printErrors();
    }
    else
    {
        std::cout << "FAILED: Undeclared variable was not detected.\n";
    }

 

    std::cout << "\n";
    std::cout << "TEST 3: Type Mismatch\n";
    std::cout << "------------------------------------\n";

    auto typeMismatchProgram =
        std::make_shared<Program>();

    typeMismatchProgram->statements.push_back(
        std::make_shared<DeclarationStatement>(
            BhashaDataType::NUMBER,
            "x",
            std::make_shared<LiteralExpression>(
                "5.5",
                BhashaDataType::DECIMAL)));

    SemanticAnalyzer analyzer3;

    analyzer3.analyze(typeMismatchProgram);

    if (analyzer3.hasErrors())
    {
        std::cout << "PASSED: Type mismatch detected correctly.\n";
        analyzer3.printErrors();
    }
    else
    {
        std::cout << "FAILED: Type mismatch was not detected.\n";
    }

    

    std::cout << "\n";
    std::cout << "TEST 4: Duplicate Declaration\n";
    std::cout << "------------------------------------\n";

    auto duplicateProgram =
        std::make_shared<Program>();

    duplicateProgram->statements.push_back(
        std::make_shared<DeclarationStatement>(
            BhashaDataType::NUMBER,
            "x",
            std::make_shared<LiteralExpression>(
                "10",
                BhashaDataType::NUMBER)));

    duplicateProgram->statements.push_back(
        std::make_shared<DeclarationStatement>(
            BhashaDataType::NUMBER,
            "x",
            std::make_shared<LiteralExpression>(
                "20",
                BhashaDataType::NUMBER)));

    SemanticAnalyzer analyzer4;

    analyzer4.analyze(duplicateProgram);

    if (analyzer4.hasErrors())
    {
        std::cout << "PASSED: Duplicate declaration detected.\n";
        analyzer4.printErrors();
    }
    else
    {
        std::cout << "FAILED: Duplicate declaration was not detected.\n";
    }



    std::cout << "\n";
    std::cout << "TEST 5: Invalid IF Condition\n";
    std::cout << "------------------------------------\n";

    auto ifProgram =
        std::make_shared<Program>();

    auto ifBody =
        std::make_shared<BlockStatement>();

    ifBody->statements.push_back(
        std::make_shared<PrintStatement>(
            std::make_shared<LiteralExpression>(
                "10",
                BhashaDataType::NUMBER)));

    ifProgram->statements.push_back(
        std::make_shared<IfStatement>(
            std::make_shared<LiteralExpression>(
                "10",
                BhashaDataType::NUMBER),
            ifBody));

    SemanticAnalyzer analyzer5;

    analyzer5.analyze(ifProgram);

    if (analyzer5.hasErrors())
    {
        std::cout << "PASSED: Invalid IF condition detected.\n";
        analyzer5.printErrors();
    }
    else
    {
        std::cout << "FAILED: Invalid IF condition was not detected.\n";
    }

    

    std::cout << "\n";
    std::cout << "TEST 6: Invalid Logical Operation\n";
    std::cout << "------------------------------------\n";

    auto logicalProgram =
        std::make_shared<Program>();

    auto logicalExpression =
        std::make_shared<BinaryExpression>(
            std::make_shared<LiteralExpression>(
                "10",
                BhashaDataType::NUMBER),
            "&&",
            std::make_shared<LiteralExpression>(
                "20",
                BhashaDataType::NUMBER));

    logicalProgram->statements.push_back(
        std::make_shared<PrintStatement>(
            logicalExpression));

    SemanticAnalyzer analyzer6;

    analyzer6.analyze(logicalProgram);

    if (analyzer6.hasErrors())
    {
        std::cout << "PASSED: Invalid logical operation detected.\n";
        analyzer6.printErrors();
    }
    else
    {
        std::cout << "FAILED: Invalid logical operation was not detected.\n";
    }

   
    // TEST COMPLETE
  

    std::cout << "\n";
    std::cout << "====================================\n";
    std::cout << "       SEMANTIC TEST COMPLETE\n";
    std::cout << "====================================\n";

    return 0;
}