#include <iostream>
#include <windows.h>
#include <vector>
#include "src/lexer.h"

int main() {

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::cout << "========== BHASHA LEXER TEST ==========\n\n";

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

    Lexer lexer(source);

    std::vector<Token> tokens = lexer.tokenize();

    std::cout << "========== SOURCE CODE ==========\n\n";
    std::cout << source << "\n";

    std::cout << "========== TOKENS ==========\n\n";

    for (const Token& token : tokens) {

        std::cout
            << "Lexeme: " << token.lexeme
            << " | Line: " << token.line
            << " | Token Type: "
            << tokenTypeToString(token.type)
            << std::endl;
    }

    std::cout << "\n========== LEXER TEST COMPLETE ==========\n";

    return 0;
}