#include "lexer.h"

Lexer::Lexer(const std::string& source)
    : source(source), start(0), current(0), line(1) {

    // Data type keywords
    keywords["সংখ্যা"] = BhashaTokenType::NUMBER_TYPE;
    keywords["দশমিক"] = BhashaTokenType::DECIMAL_TYPE;
    keywords["লেখা"] = BhashaTokenType::STRING_TYPE;
    keywords["চরিত্র"] = BhashaTokenType::CHAR_TYPE;
    keywords["সত্যমিথ্যা"] = BhashaTokenType::BOOLEAN_TYPE;
    keywords["তালিকা"] = BhashaTokenType::LIST_TYPE;
    keywords["দীর্ঘসংখ্যা"] = BhashaTokenType::LONG_TYPE;
    keywords["ধনাত্মক"] = BhashaTokenType::POSITIVE_TYPE;
    keywords["শূন্য"] = BhashaTokenType::VOID_TYPE;

    // Control keywords
    keywords["যদি"] = BhashaTokenType::IF;
    keywords["নাহলে"] = BhashaTokenType::ELSE;
    keywords["যতক্ষণ"] = BhashaTokenType::WHILE;
    keywords["দেখাও"] = BhashaTokenType::PRINT;

    // Boolean keywords
    keywords["সত্য"] = BhashaTokenType::BOOLEAN_TRUE;
    keywords["মিথ্যা"] = BhashaTokenType::BOOLEAN_FALSE;
}


// Main tokenization function
std::vector<Token> Lexer::tokenize() {

    while (current < source.length()) {

        start = current;
        scanToken();
    }

    // End of file token
    tokens.push_back(
        Token(BhashaTokenType::END_OF_FILE, "", line)
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
            Token(BhashaTokenType::DECIMAL, value, line)
        );

    } else {

        tokens.push_back(
            Token(BhashaTokenType::INTEGER, value, line)
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
            Token(BhashaTokenType::IDENTIFIER, text, line)
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
                Token(BhashaTokenType::PLUS, "+", line)
            );

            break;

        case '-':

            tokens.push_back(
                Token(BhashaTokenType::MINUS, "-", line)
            );

            break;

        case '*':

            tokens.push_back(
                Token(BhashaTokenType::MULTIPLY, "*", line)
            );

            break;

        case '/':

            tokens.push_back(
                Token(BhashaTokenType::DIVIDE, "/", line)
            );

            break;

        case '%':

            tokens.push_back(
                Token(BhashaTokenType::MODULO, "%", line)
            );

            break;


        // Assignment and equality
        case '=':

            if (current < source.length() &&
                source[current] == '=') {

                current++;

                tokens.push_back(
                    Token(BhashaTokenType::EQUAL_EQUAL, "==", line)
                );

            } else {

                tokens.push_back(
                    Token(BhashaTokenType::ASSIGN, "=", line)
                );
            }

            break;


        // Greater than
        case '>':

            if (current < source.length() &&
                source[current] == '=') {

                current++;

                tokens.push_back(
                    Token(BhashaTokenType::GREATER_EQUAL, ">=", line)
                );

            } else {

                tokens.push_back(
                    Token(BhashaTokenType::GREATER, ">", line)
                );
            }

            break;


        // Less than
        case '<':

            if (current < source.length() &&
                source[current] == '=') {

                current++;

                tokens.push_back(
                    Token(BhashaTokenType::LESS_EQUAL, "<=", line)
                );

            } else {

                tokens.push_back(
                    Token(BhashaTokenType::LESS, "<", line)
                );
            }

            break;


        // NOT and NOT EQUAL
        case '!':

            if (current < source.length() &&
                source[current] == '=') {

                current++;

                tokens.push_back(
                    Token(BhashaTokenType::NOT_EQUAL, "!=", line)
                );

            } else {

                tokens.push_back(
                    Token(BhashaTokenType::NOT, "!", line)
                );
            }

            break;


        // Parentheses
        case '(':

            tokens.push_back(
                Token(BhashaTokenType::LEFT_PAREN, "(", line)
            );

            break;

        case ')':

            tokens.push_back(
                Token(BhashaTokenType::RIGHT_PAREN, ")", line)
            );

            break;


        // Braces
        case '{':

            tokens.push_back(
                Token(BhashaTokenType::LEFT_BRACE, "{", line)
            );

            break;

        case '}':

            tokens.push_back(
                Token(BhashaTokenType::RIGHT_BRACE, "}", line)
            );

            break;


        // Brackets
        case '[':

            tokens.push_back(
                Token(BhashaTokenType::LEFT_BRACKET, "[", line)
            );

            break;

        case ']':

            tokens.push_back(
                Token(BhashaTokenType::RIGHT_BRACKET, "]", line)
            );

            break;


        // Other symbols
        case ';':

            tokens.push_back(
                Token(BhashaTokenType::SEMICOLON, ";", line)
            );

            break;

        case ',':

            tokens.push_back(
                Token(BhashaTokenType::COMMA, ",", line)
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
                        BhashaTokenType::UNKNOWN,
                        std::string(1, c),
                        line
                    )
                );
            }

            break;
    }
}