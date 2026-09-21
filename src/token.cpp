#include "token.h"

std::string tokenTypeToString(AbhibyaktiTokenType type) {

    switch (type) {

        case AbhibyaktiTokenType::NUMBER_TYPE:
            return "NUMBER_TYPE";

        case AbhibyaktiTokenType::DECIMAL_TYPE:
            return "DECIMAL_TYPE";

        case AbhibyaktiTokenType::STRING_TYPE:
            return "STRING_TYPE";

        case AbhibyaktiTokenType::CHAR_TYPE:
            return "CHAR_TYPE";

        case AbhibyaktiTokenType::BOOLEAN_TYPE:
            return "BOOLEAN_TYPE";

        case AbhibyaktiTokenType::LIST_TYPE:
            return "LIST_TYPE";

        case AbhibyaktiTokenType::LONG_TYPE:
            return "LONG_TYPE";

        case AbhibyaktiTokenType::POSITIVE_TYPE:
            return "POSITIVE_TYPE";

        case AbhibyaktiTokenType::VOID_TYPE:
            return "VOID_TYPE";

        case AbhibyaktiTokenType::IF:
            return "IF";

        case AbhibyaktiTokenType::ELSE:
            return "ELSE";

        case AbhibyaktiTokenType::WHILE:
            return "WHILE";

        case AbhibyaktiTokenType::PRINT:
            return "PRINT";

        case AbhibyaktiTokenType::BOOLEAN_TRUE:
            return "BOOLEAN_TRUE";

        case AbhibyaktiTokenType::BOOLEAN_FALSE:
            return "BOOLEAN_FALSE";

        case AbhibyaktiTokenType::IDENTIFIER:
            return "IDENTIFIER";

        case AbhibyaktiTokenType::INTEGER:
            return "INTEGER";

        case AbhibyaktiTokenType::DECIMAL:
            return "DECIMAL";

        case AbhibyaktiTokenType::STRING:
            return "STRING";

        case AbhibyaktiTokenType::CHARACTER:
            return "CHARACTER";

        case AbhibyaktiTokenType::PLUS:
            return "PLUS";

        case AbhibyaktiTokenType::MINUS:
            return "MINUS";

        case AbhibyaktiTokenType::MULTIPLY:
            return "MULTIPLY";

        case AbhibyaktiTokenType::DIVIDE:
            return "DIVIDE";

        case AbhibyaktiTokenType::MODULO:
            return "MODULO";

        case AbhibyaktiTokenType::GREATER:
            return "GREATER";

        case AbhibyaktiTokenType::LESS:
            return "LESS";

        case AbhibyaktiTokenType::GREATER_EQUAL:
            return "GREATER_EQUAL";

        case AbhibyaktiTokenType::LESS_EQUAL:
            return "LESS_EQUAL";

        case AbhibyaktiTokenType::EQUAL_EQUAL:
            return "EQUAL_EQUAL";

        case AbhibyaktiTokenType::NOT_EQUAL:
            return "NOT_EQUAL";

        case AbhibyaktiTokenType::AND:
            return "AND";

        case AbhibyaktiTokenType::OR:
            return "OR";

        case AbhibyaktiTokenType::NOT:
            return "NOT";

        case AbhibyaktiTokenType::ASSIGN:
            return "ASSIGN";

        case AbhibyaktiTokenType::LEFT_PAREN:
            return "LEFT_PAREN";

        case AbhibyaktiTokenType::RIGHT_PAREN:
            return "RIGHT_PAREN";

        case AbhibyaktiTokenType::LEFT_BRACE:
            return "LEFT_BRACE";

        case AbhibyaktiTokenType::RIGHT_BRACE:
            return "RIGHT_BRACE";

        case AbhibyaktiTokenType::LEFT_BRACKET:
            return "LEFT_BRACKET";

        case AbhibyaktiTokenType::RIGHT_BRACKET:
            return "RIGHT_BRACKET";

        case AbhibyaktiTokenType::SEMICOLON:
            return "SEMICOLON";

        case AbhibyaktiTokenType::COMMA:
            return "COMMA";

        case AbhibyaktiTokenType::END_OF_FILE:
            return "EOF";

        case AbhibyaktiTokenType::UNKNOWN:
            return "UNKNOWN";

        default:
            return "UNKNOWN";
    }
}