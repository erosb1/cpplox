#include "debug.h"
#include <iomanip>

static std::string GetTokenString(TokenType type) {
    switch (type) {
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::COMMA: return "COMMA";
        case TokenType::DOT: return "DOT";
        case TokenType::MINUS: return "MINUS";
        case TokenType::PLUS: return "PLUS";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::SLASH: return "SLASH";
        case TokenType::STAR: return "STAR";
        case TokenType::BANG: return "BANG";
        case TokenType::BANG_EQUAL: return "BANG_EQUAL";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::EQUAL_EQUAL: return "EQUAL_EQUAL";
        case TokenType::GREATER: return "GREATER";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case TokenType::LESS: return "LESS";
        case TokenType::LESS_EQUAL: return "LESS_EQUAL";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::STRING: return "STRING";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::AND: return "AND";
        case TokenType::ELSE: return "ELSE";
        case TokenType::FALSE: return "FALSE";
        case TokenType::FOR: return "FOR";
        case TokenType::FUN: return "FUN";
        case TokenType::IF: return "IF";
        case TokenType::NIL: return "NIL";
        case TokenType::OR: return "OR";
        case TokenType::PRINT: return "PRINT";
        case TokenType::RETURN: return "RETURN";
        case TokenType::TRUE: return "TRUE";
        case TokenType::VAR: return "VAR";
        case TokenType::WHILE: return "WHILE";
        case TokenType::ERROR: return "ERROR";
        case TokenType::END: return "END";
        default: return "unknown";
    }
}

std::ostream& operator<<(std::ostream& os, const TokenType& type) {
    os << GetTokenString(type);
    return os;
}

static void PrintToken(const Token& token, size_t last_line_num) {
    std::string line_num_str = token.line == last_line_num ? "| " : std::to_string(token.line) + " ";
    std::string token_type_str = GetTokenString(token.type);
    std::cout << std::right << std::setw(6)  << line_num_str << std::left << "     " << std::setw(11)
        << token_type_str << "    " << std::setw(8) << token.lexeme << '\n';
}

void PrintTokens(const std::vector<Token> &tokens) {
    std::cout << "[line]    [TokenType]    [lexeme]\n";
    size_t last_line_num = -1;
    for (auto& token : tokens) {
        PrintToken(token, last_line_num);
        last_line_num = token.line;
    }
    std::cout << std::endl;
}
