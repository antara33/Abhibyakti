#include "lexer.h"

Lexer::Lexer(const std::string& source)
    : source(source), start(0), current(0), line(1) {

    // Data type keywords
    keywords["সংখ্যা"] = AbhibyaktiTokenType::NUMBER_TYPE;
    keywords["দশমিক"] = AbhibyaktiTokenType::DECIMAL_TYPE;
    keywords["লেখা"] = AbhibyaktiTokenType::STRING_TYPE;
    keywords["চরিত্র"] = AbhibyaktiTokenType::CHAR_TYPE;
    keywords["সত্যমিথ্যা"] = AbhibyaktiTokenType::BOOLEAN_TYPE;
    keywords["তালিকা"] = AbhibyaktiTokenType::LIST_TYPE;
    keywords["দীর্ঘসংখ্যা"] = AbhibyaktiTokenType::LONG_TYPE;
    keywords["ধনাত্মক"] = AbhibyaktiTokenType::POSITIVE_TYPE;
    keywords["শূন্য"] = AbhibyaktiTokenType::VOID_TYPE;

    // Control keywords
    keywords["যদি"] = AbhibyaktiTokenType::IF;
    keywords["নাহলে"] = AbhibyaktiTokenType::ELSE;
    keywords["যতক্ষণ"] = AbhibyaktiTokenType::WHILE;
    keywords["দেখাও"] = AbhibyaktiTokenType::PRINT;

    // Boolean keywords
    keywords["সত্য"] = AbhibyaktiTokenType::BOOLEAN_TRUE;
    keywords["মিথ্যা"] = AbhibyaktiTokenType::BOOLEAN_FALSE;
}


// Main tokenization function
std::vector<Token> Lexer::tokenize() {

    while (current < source.length()) {

        start = current;
        scanToken();
    }

    // End of file token
    tokens.push_back(
        Token(AbhibyaktiTokenType::END_OF_FILE, "", line)
    );

    return tokens;
}


// Number and decimal number scanner
void Lexer::number() {

    bool isDecimal = false;

    // Read integer part
    while (current < source.length() &&
           source[current] >= '0' &&
           source[current] <= '9') {

        current++;
    }

    // Check decimal point
    if (current < source.length() &&
        source[current] == '.' &&
        current + 1 < source.length() &&
        source[current + 1] >= '0' &&
        source[current + 1] <= '9') {

        isDecimal = true;
        current++;

        // Read decimal part
        while (current < source.length() &&
               source[current] >= '0' &&
               source[current] <= '9') {

            current++;
        }
    }

    // Extract number
    std::string value =
        source.substr(start, current - start);

    // Create token
    if (isDecimal) {

        tokens.push_back(
            Token(AbhibyaktiTokenType::DECIMAL, value, line)
        );

    } else {

        tokens.push_back(
            Token(AbhibyaktiTokenType::INTEGER, value, line)
        );
    }
}


// Identifier scanner
void Lexer::identifier() {

    while (current < source.length()) {

        unsigned char c = source[current];

        // English letters, digits and underscore
        if ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') ||
            c == '_') {

            current++;
        }

        // Bangla UTF-8 character
        else if (c >= 0xE0 && c <= 0xEF) {

            // Bangla character normally uses 3 bytes
            current += 3;
        }

        else {
            break;
        }
    }

    // Get complete word
    std::string text =
        source.substr(start, current - start);

    // Check keyword
    auto keyword = keywords.find(text);

    if (keyword != keywords.end()) {

        tokens.push_back(
            Token(keyword->second, text, line)
        );

    } else {

        tokens.push_back(
            Token(AbhibyaktiTokenType::IDENTIFIER, text, line)
        );
    }
}


// Scan one token
void Lexer::scanToken() {

    char c = source[current++];

    switch (c) {

        // Arithmetic operators
        case '+':

            tokens.push_back(
                Token(AbhibyaktiTokenType::PLUS, "+", line)
            );

            break;

        case '-':

            tokens.push_back(
                Token(AbhibyaktiTokenType::MINUS, "-", line)
            );

            break;

        case '*':

            tokens.push_back(
                Token(AbhibyaktiTokenType::MULTIPLY, "*", line)
            );

            break;

        case '/':

            tokens.push_back(
                Token(AbhibyaktiTokenType::DIVIDE, "/", line)
            );

            break;

        case '%':

            tokens.push_back(
                Token(AbhibyaktiTokenType::MODULO, "%", line)
            );

            break;


        // Assignment and equality
        case '=':

            if (current < source.length() &&
                source[current] == '=') {

                current++;

                tokens.push_back(
                    Token(AbhibyaktiTokenType::EQUAL_EQUAL, "==", line)
                );

            } else {

                tokens.push_back(
                    Token(AbhibyaktiTokenType::ASSIGN, "=", line)
                );
            }

            break;


        // Greater than
        case '>':

            if (current < source.length() &&
                source[current] == '=') {

                current++;

                tokens.push_back(
                    Token(AbhibyaktiTokenType::GREATER_EQUAL, ">=", line)
                );

            } else {

                tokens.push_back(
                    Token(AbhibyaktiTokenType::GREATER, ">", line)
                );
            }

            break;


        // Less than
        case '<':

            if (current < source.length() &&
                source[current] == '=') {

                current++;

                tokens.push_back(
                    Token(AbhibyaktiTokenType::LESS_EQUAL, "<=", line)
                );

            } else {

                tokens.push_back(
                    Token(AbhibyaktiTokenType::LESS, "<", line)
                );
            }

            break;


        // NOT and NOT EQUAL
        case '!':

            if (current < source.length() &&
                source[current] == '=') {

                current++;

                tokens.push_back(
                    Token(AbhibyaktiTokenType::NOT_EQUAL, "!=", line)
                );

            } else {

                tokens.push_back(
                    Token(AbhibyaktiTokenType::NOT, "!", line)
                );
            }

            break;


        // Parentheses
        case '(':

            tokens.push_back(
                Token(AbhibyaktiTokenType::LEFT_PAREN, "(", line)
            );

            break;

        case ')':

            tokens.push_back(
                Token(AbhibyaktiTokenType::RIGHT_PAREN, ")", line)
            );

            break;


        // Braces
        case '{':

            tokens.push_back(
                Token(AbhibyaktiTokenType::LEFT_BRACE, "{", line)
            );

            break;

        case '}':

            tokens.push_back(
                Token(AbhibyaktiTokenType::RIGHT_BRACE, "}", line)
            );

            break;


        // Brackets
        case '[':

            tokens.push_back(
                Token(AbhibyaktiTokenType::LEFT_BRACKET, "[", line)
            );

            break;

        case ']':

            tokens.push_back(
                Token(AbhibyaktiTokenType::RIGHT_BRACKET, "]", line)
            );

            break;


        // Other symbols
        case ';':

            tokens.push_back(
                Token(AbhibyaktiTokenType::SEMICOLON, ";", line)
            );

            break;

        case ',':

            tokens.push_back(
                Token(AbhibyaktiTokenType::COMMA, ",", line)
            );

            break;


        // Whitespace
        case ' ':
        case '\r':
        case '\t':

            break;


        // New line
        case '\n':

            line++;

            break;


        // Numbers, identifiers and Bangla keywords
        default:

            // Number
            if (c >= '0' && c <= '9') {

                current--;
                number();
            }

            // English identifier
            else if ((c >= 'a' && c <= 'z') ||
                     (c >= 'A' && c <= 'Z') ||
                     c == '_') {

                current--;
                identifier();
            }

            // Bangla keyword / identifier
            else if ((unsigned char)c >= 0xE0 &&
                     (unsigned char)c <= 0xEF) {

                current--;

                identifier();
            }

            // Unknown character
            else {

                tokens.push_back(
                    Token(
                        AbhibyaktiTokenType::UNKNOWN,
                        std::string(1, c),
                        line
                    )
                );
            }

            break;
    }
}