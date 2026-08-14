#include "token.h"

std::string tokenTypeToString(BhashaTokenType type) {

    switch (type) {

        case BhashaTokenType::NUMBER_TYPE:
            return "NUMBER_TYPE";

        case BhashaTokenType::DECIMAL_TYPE:
            return "DECIMAL_TYPE";

        case BhashaTokenType::STRING_TYPE:
            return "STRING_TYPE";

        case BhashaTokenType::CHAR_TYPE:
            return "CHAR_TYPE";

        case BhashaTokenType::BOOLEAN_TYPE:
            return "BOOLEAN_TYPE";

        case BhashaTokenType::LIST_TYPE:
            return "LIST_TYPE";

        case BhashaTokenType::LONG_TYPE:
            return "LONG_TYPE";

        case BhashaTokenType::POSITIVE_TYPE:
            return "POSITIVE_TYPE";

        case BhashaTokenType::VOID_TYPE:
            return "VOID_TYPE";

        case BhashaTokenType::IF:
            return "IF";

        case BhashaTokenType::ELSE:
            return "ELSE";

        case BhashaTokenType::WHILE:
            return "WHILE";

        case BhashaTokenType::PRINT:
            return "PRINT";

        case BhashaTokenType::BOOLEAN_TRUE:
            return "BOOLEAN_TRUE";

        case BhashaTokenType::BOOLEAN_FALSE:
            return "BOOLEAN_FALSE";

        case BhashaTokenType::IDENTIFIER:
            return "IDENTIFIER";

        case BhashaTokenType::INTEGER:
            return "INTEGER";

        case BhashaTokenType::DECIMAL:
            return "DECIMAL";

        case BhashaTokenType::STRING:
            return "STRING";

        case BhashaTokenType::CHARACTER:
            return "CHARACTER";

        case BhashaTokenType::PLUS:
            return "PLUS";

        case BhashaTokenType::MINUS:
            return "MINUS";

        case BhashaTokenType::MULTIPLY:
            return "MULTIPLY";

        case BhashaTokenType::DIVIDE:
            return "DIVIDE";

        case BhashaTokenType::MODULO:
            return "MODULO";

        case BhashaTokenType::GREATER:
            return "GREATER";

        case BhashaTokenType::LESS:
            return "LESS";

        case BhashaTokenType::GREATER_EQUAL:
            return "GREATER_EQUAL";

        case BhashaTokenType::LESS_EQUAL:
            return "LESS_EQUAL";

        case BhashaTokenType::EQUAL_EQUAL:
            return "EQUAL_EQUAL";

        case BhashaTokenType::NOT_EQUAL:
            return "NOT_EQUAL";

        case BhashaTokenType::AND:
            return "AND";

        case BhashaTokenType::OR:
            return "OR";

        case BhashaTokenType::NOT:
            return "NOT";

        case BhashaTokenType::ASSIGN:
            return "ASSIGN";

        case BhashaTokenType::LEFT_PAREN:
            return "LEFT_PAREN";

        case BhashaTokenType::RIGHT_PAREN:
            return "RIGHT_PAREN";

        case BhashaTokenType::LEFT_BRACE:
            return "LEFT_BRACE";

        case BhashaTokenType::RIGHT_BRACE:
            return "RIGHT_BRACE";

        case BhashaTokenType::LEFT_BRACKET:
            return "LEFT_BRACKET";

        case BhashaTokenType::RIGHT_BRACKET:
            return "RIGHT_BRACKET";

        case BhashaTokenType::SEMICOLON:
            return "SEMICOLON";

        case BhashaTokenType::COMMA:
            return "COMMA";

        case BhashaTokenType::END_OF_FILE:
            return "EOF";

        case BhashaTokenType::UNKNOWN:
            return "UNKNOWN";

        default:
            return "UNKNOWN";
    }
}