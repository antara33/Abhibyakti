#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <string>
#include <vector>
#include <unordered_map>

class Lexer {
private:
    std::string source;
    std::vector<Token> tokens;

    size_t start;
    size_t current;
    int line;

    std::unordered_map<std::string, AbhibyaktiTokenType> keywords;

    void scanToken();
    void number();
    void identifier();

    // Bangla digit helper functions
    bool isBanglaDigitAt(size_t position) const;
    int banglaDigitValueAt(size_t position) const;

public:
    Lexer(const std::string& source);

    std::vector<Token> tokenize();
};

#endif