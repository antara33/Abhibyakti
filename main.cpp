
#include <iostream>
#include <windows.h>
#include <vector>
#include <memory>
#include <filesystem>

#ifdef VOID
#undef VOID
#endif

#include "src/lexer.h"
#include "src/parser.h"
#include "src/semantic.h"
#include "src/codegen.h"

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::cout << "========================================\n";
    std::cout << "        BHASHA COMPILER TEST\n";
    std::cout << "========================================\n\n";

    std::string source = R"(
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
        std::string generatedCode = codeGenerator.generate(program);

        std::cout << "Generated Python code:\n";
        std::cout << "----------------------------------------\n";
        std::cout << generatedCode;
        std::cout << "----------------------------------------\n\n";

        bool saved = codeGenerator.generateToFile(
            program,
            "output/output.py"
        );

        if (!saved)
        {
            std::cout << "CODE GENERATION: FAILED\n";
            return 1;
        }

        std::cout << "Python code saved to: output/output.py\n";
        std::cout << "CODE GENERATION: SUCCESS\n\n";
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
