#include <iostream>
#include <windows.h>
#include <vector>
#include <memory>
#include <filesystem>
#include <fstream>
#include <sstream>

#ifdef VOID
#undef VOID
#endif

#include "src/lexer.h"
#include "src/parser.h"
#include "src/semantic.h"
#include "src/codegen.h"




void printExpression(
    const std::shared_ptr<Expression>& expr,
    const std::string& indent = "")
{
    if (auto literal =
            std::dynamic_pointer_cast<LiteralExpression>(expr))
    {
        std::cout << indent << "Literal: "
                  << literal->value << "\n";
    }
    else if (auto variable =
                 std::dynamic_pointer_cast<VariableExpression>(expr))
    {
        std::cout << indent << "Variable: "
                  << variable->name << "\n";
    }
    else if (auto binary =
                 std::dynamic_pointer_cast<BinaryExpression>(expr))
    {
        std::cout << indent << "Binary: "
                  << binary->operatorSymbol << "\n";

        printExpression(binary->left, indent + "  ");
        printExpression(binary->right, indent + "  ");
    }
    else if (auto unary =
                 std::dynamic_pointer_cast<UnaryExpression>(expr))
    {
        std::cout << indent << "Unary: "
                  << unary->operatorSymbol << "\n";

        printExpression(unary->operand, indent + "  ");
    }
}

void printAST(
    const std::shared_ptr<Program>& program)
{
    std::cout << "\n========== AST TREE ==========\n";

    std::cout << "Program\n";

    for (auto& statement : program->statements)
    {
        if (auto declaration =
                std::dynamic_pointer_cast<DeclarationStatement>(statement))
        {
            std::cout << "  Declaration: "
                      << declaration->variableName << "\n";

            printExpression(
                declaration->initializer,
                "    ");
        }

        else if (auto assignment =
                     std::dynamic_pointer_cast<AssignmentStatement>(statement))
        {
            std::cout << "  Assignment: "
                      << assignment->variableName << "\n";

            printExpression(
                assignment->value,
                "    ");
        }

        else if (auto print =
                     std::dynamic_pointer_cast<PrintStatement>(statement))
        {
            std::cout << "  Print\n";

            printExpression(
                print->expression,
                "    ");
        }

        else if (auto ifStatement =
                     std::dynamic_pointer_cast<IfStatement>(statement))
        {
            std::cout << "  If\n";

            std::cout << "    Condition\n";
            printExpression(
                ifStatement->condition,
                "      ");

            std::cout << "    Then\n";

            for (auto& s : ifStatement->thenBranch->statements)
            {
                if (auto p =
                        std::dynamic_pointer_cast<PrintStatement>(s))
                {
                    std::cout << "      Print\n";
                    printExpression(
                        p->expression,
                        "        ");
                }
            }

            if (ifStatement->elseBranch)
            {
                std::cout << "    Else\n";

                for (auto& s : ifStatement->elseBranch->statements)
                {
                    if (auto p =
                            std::dynamic_pointer_cast<PrintStatement>(s))
                    {
                        std::cout << "      Print\n";
                        printExpression(
                            p->expression,
                            "        ");
                    }
                }
            }
        }
    }

    std::cout << "==============================\n\n";
}

int main(int argc, char *argv[])
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::cout << "========================================\n";
    std::cout << "        Abhibyakti COMPILER\n";
    std::cout << "========================================\n\n";

    std::string source;

    if (argc > 1)
    {
        std::ifstream file(argv[1]);

        if (!file)
        {
            std::cout << "Error: Could not open file: "
                      << argv[1] << "\n";
            return 1;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        source = buffer.str();

        std::cout << "Source file: " << argv[1] << "\n\n";
    }
    else
    {
        source = R"(
সংখ্যা জমি = 5;
সংখ্যা ধান = 100;

দশমিক দাম = 35.50;
দশমিক খরচ = 2500.00;

দশমিক বিক্রয় = ধান * দাম;
দশমিক লাভ = বিক্রয় - খরচ;

যদি (লাভ > 1000) {
    দেখাও লাভ;
}
নাহলে {
    দেখাও খরচ;
}

যদি (লাভ < 0) {
    দেখাও খরচ;
}

সংখ্যা বস্তা = 10;
সংখ্যা দামবস্তা = 50;

সংখ্যা মোট = বস্তা * দামবস্তা;

যদি (মোট >= 500) {
    দেখাও মোট;
}
)";

        std::cout << "Using default Farmer's Crop Profit Calculator.\n\n";
    }

    std::cout << "========== SOURCE CODE ==========\n\n";
    std::cout << source << "\n";

    std::cout << "========== LEXICAL ANALYSIS ==========\n\n";

    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    std::cout << "Total tokens generated: "
              << tokens.size() << "\n\n";

    for (const Token &token : tokens)
    {
        std::cout
            << "Lexeme: " << token.lexeme
            << " | Line: " << token.line
            << " | Token Type: "
            << tokenTypeToString(token.type)
            << '\n';
    }

    std::cout << "\nLEXER: SUCCESS\n\n";

    std::cout << "========== SYNTAX ANALYSIS ==========\n\n";

    try
    {
        Parser parser(tokens);
        std::shared_ptr<Program> program = parser.parse();

        if (!program)
        {
            std::cout << "PARSER: FAILED\n";
            return 1;
        }

        std::cout << "AST generated successfully.\n";
        std::cout << "Total statements in AST: "
                  << program->statements.size() << "\n";
        std::cout << "PARSER: SUCCESS\n\n";

        printAST(program);

        std::cout << "========== SEMANTIC ANALYSIS ==========\n\n";

        SemanticAnalyzer semanticAnalyzer;
        semanticAnalyzer.analyze(program);

        if (semanticAnalyzer.hasErrors())
        {
            std::cout << "SEMANTIC ANALYSIS: FAILED\n\n";
            semanticAnalyzer.printErrors();
            return 1;
        }

        std::cout << "No semantic errors found.\n";
        std::cout << "SEMANTIC ANALYSIS: SUCCESS\n\n";

        std::cout << "========== CODE GENERATION ==========\n\n";

        std::filesystem::create_directories("output");

        CodeGenerator codeGenerator;

        std::string generatedCode =
            codeGenerator.generate(program);

        std::cout << "Generated Python code:\n";
        std::cout << "----------------------------------------\n";
        std::cout << generatedCode;
        std::cout << "----------------------------------------\n\n";

        bool saved =
            codeGenerator.generateToFile(
                program,
                "output/output.py");

        if (!saved)
        {
            std::cout << "CODE GENERATION: FAILED\n";
            return 1;
        }

        std::cout
            << "Python code saved to: output/output.py\n";

        std::cout
            << "CODE GENERATION: SUCCESS\n\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "COMPILER ERROR: "
                  << e.what() << "\n";

        return 1;
    }

    std::cout << "========================================\n";
    std::cout << "       COMPILATION SUCCESSFUL\n";
    std::cout << "========================================\n";

    return 0;
}
