#include "lexer.h"
#include "parser.h"

#include <iostream>
#include <memory>
#include <string>



class ASTPrinter
{
private:
    int indentLevel = 0;

    void indent()
    {
        for (int i = 0; i < indentLevel; i++)
        {
            std::cout << "    ";
        }
    }

public:
    void printExpression(
        const std::shared_ptr<Expression> &expression)
    {
        if (!expression)
        {
            indent();
            std::cout << "NULL Expression\n";
            return;
        }

        // Literal
        if (auto literal =
                std::dynamic_pointer_cast<LiteralExpression>(expression))
        {
            indent();

            std::cout << "Literal: "
                      << literal->value
                      << "\n";

            return;
        }

        // Variable
        if (auto variable =
                std::dynamic_pointer_cast<VariableExpression>(expression))
        {
            indent();

            std::cout << "Variable: "
                      << variable->name
                      << "\n";

            return;
        }

        // Binary Expression
        if (auto binary =
                std::dynamic_pointer_cast<BinaryExpression>(expression))
        {
            indent();

            std::cout << "BinaryExpression: "
                      << binary->operatorSymbol
                      << "\n";

            indentLevel++;

            indent();
            std::cout << "Left:\n";

            indentLevel++;
            printExpression(binary->left);
            indentLevel--;

            indent();
            std::cout << "Right:\n";

            indentLevel++;
            printExpression(binary->right);
            indentLevel--;

            indentLevel--;

            return;
        }

        // Unary Expression
        if (auto unary =
                std::dynamic_pointer_cast<UnaryExpression>(expression))
        {
            indent();

            std::cout << "UnaryExpression: "
                      << unary->operatorSymbol
                      << "\n";

            indentLevel++;

            printExpression(unary->operand);

            indentLevel--;

            return;
        }

        indent();
        std::cout << "Unknown Expression\n";
    }

    void printStatement(
        const std::shared_ptr<Statement> &statement)
    {
        if (!statement)
        {
            indent();
            std::cout << "NULL Statement\n";
            return;
        }

        // Declaration
        if (auto declaration =
                std::dynamic_pointer_cast<DeclarationStatement>(statement))
        {
            indent();

            std::cout << "Declaration\n";

            indentLevel++;

            indent();
            std::cout << "Variable: "
                      << declaration->variableName
                      << "\n";

            indent();
            std::cout << "Initializer:\n";

            indentLevel++;
            printExpression(declaration->initializer);
            indentLevel--;

            indentLevel--;

            return;
        }

        // Assignment
        if (auto assignment =
                std::dynamic_pointer_cast<AssignmentStatement>(statement))
        {
            indent();

            std::cout << "Assignment\n";

            indentLevel++;

            indent();
            std::cout << "Variable: "
                      << assignment->variableName
                      << "\n";

            indent();
            std::cout << "Value:\n";

            indentLevel++;
            printExpression(assignment->value);
            indentLevel--;

            indentLevel--;

            return;
        }

        // Print
        if (auto print =
                std::dynamic_pointer_cast<PrintStatement>(statement))
        {
            indent();

            std::cout << "PrintStatement\n";

            indentLevel++;

            printExpression(print->expression);

            indentLevel--;

            return;
        }

        // If
        if (auto ifStatement =
                std::dynamic_pointer_cast<IfStatement>(statement))
        {
            indent();

            std::cout << "IfStatement\n";

            indentLevel++;

            indent();
            std::cout << "Condition:\n";

            indentLevel++;
            printExpression(ifStatement->condition);
            indentLevel--;

            indent();

            std::cout << "Then Branch:\n";

            indentLevel++;

            if (ifStatement->thenBranch)
            {
                for (const auto &stmt :
                     ifStatement->thenBranch->statements)
                {
                    printStatement(stmt);
                }
            }

            indentLevel--;

            if (ifStatement->elseBranch)
            {
                indent();

                std::cout << "Else Branch:\n";

                indentLevel++;

                for (const auto &stmt :
                     ifStatement->elseBranch->statements)
                {
                    printStatement(stmt);
                }

                indentLevel--;
            }

            indentLevel--;

            return;
        }

        // While
        if (auto whileStatement =
                std::dynamic_pointer_cast<WhileStatement>(statement))
        {
            indent();

            std::cout << "WhileStatement\n";

            indentLevel++;

            indent();
            std::cout << "Condition:\n";

            indentLevel++;
            printExpression(whileStatement->condition);
            indentLevel--;

            indent();
            std::cout << "Body:\n";

            indentLevel++;

            if (whileStatement->body)
            {
                for (const auto &stmt :
                     whileStatement->body->statements)
                {
                    printStatement(stmt);
                }
            }

            indentLevel--;

            indentLevel--;

            return;
        }

        indent();
        std::cout << "Unknown Statement\n";
    }

    void printProgram(
        const std::shared_ptr<Program> &program)
    {
        std::cout << "\n";
        std::cout << "====================================\n";
        std::cout << "             AST TREE\n";
        std::cout << "====================================\n";

        if (!program)
        {
            std::cout << "NULL PROGRAM\n";
            return;
        }

        std::cout << "Program\n";

        indentLevel = 1;

        for (const auto &statement :
             program->statements)
        {
            printStatement(statement);
        }

        indentLevel = 0;

        std::cout << "====================================\n";
    }
};

// ============================================================
// MAIN
// ============================================================

int main()
{
    std::cout << "PARSER TEST STARTED\n";

    std::string source = R"(
সংখ্যা x = 10;
দশমিক y = 5.5;

x = x + 5;

যদি (x > 10) {
    দেখাও(x);
}
)";

    // ========================================================
    // LEXER
    // ========================================================

    std::cout << "====================================\n";
    std::cout << "       LEXICAL ANALYSIS\n";
    std::cout << "====================================\n";

    Lexer lexer(source);

    std::vector<Token> tokens = lexer.tokenize();

    std::cout << "Tokens generated: "
              << tokens.size()
              << "\n";

    // ========================================================
    // PARSER
    // ========================================================

    std::cout << "\n";
    std::cout << "====================================\n";
    std::cout << "        SYNTAX ANALYSIS\n";
    std::cout << "====================================\n";

    try
    {
        Parser parser(tokens);

        std::shared_ptr<Program> program =
            parser.parse();

        std::cout << "Parsing successful.\n";
        std::cout << "AST generated successfully.\n";

        // ====================================================
        // PRINT AST
        // ====================================================

        ASTPrinter printer;

        printer.printProgram(program);
    }
    catch (const std::exception &e)
    {
        std::cout << "\nParser Error: "
                  << e.what()
                  << "\n";
    }

    std::cout << "\n";
    std::cout << "====================================\n";
    std::cout << "          TEST COMPLETE\n";
    std::cout << "====================================\n";

    return 0;
}