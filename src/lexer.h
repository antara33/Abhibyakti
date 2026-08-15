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

public:
    Lexer(const std::string& source);

    std::vector<Token> tokenize();
};

#endif