#include "debug.h"
#include <iomanip>

constexpr size_t AST_INDENT_SPACING = 4;

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

static std::string VariantToString(const Value& var) {
    return std::visit([&](auto&& arg) -> std::string {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::string>) {
            return arg;
        } else if constexpr (std::is_same_v<T, double>) {
            return std::to_string(arg);
        } else if constexpr (std::is_same_v<T, bool>) {
            if (std::get<bool>(var) == true) return "true";
            return "false";
        } else return "nil";
    }, var);
}

std::ostream& operator<<(std::ostream& os, const TokenType& type) {
    os << GetTokenString(type);
    return os;
}

static void PrintToken(const Token& token, size_t last_line_num) {
    std::string line_num_str = token.line == last_line_num ? "| " : std::to_string(token.line) + " ";
    std::string token_type_str = GetTokenString(token.type);
    std::cout << std::right << std::setw(6)  << line_num_str << std::left << "     " << std::setw(14)
        << token_type_str << "    " << std::setw(8) << token.lexeme << '\n';
}

void Debug::PrintTokens(const std::vector<Token> &tokens) {
    std::cout << "[line]    [TokenType]       [lexeme]\n";
    size_t last_line_num = -1;
    for (auto& token : tokens) {
        PrintToken(token, last_line_num);
        last_line_num = token.line;
    }
    std::cout << std::endl;
}

void Debug::PrintAST(const ASTNode* const node, size_t indent_level) {
    auto spacing = std::string(indent_level * AST_INDENT_SPACING, ' ');
    auto spacing2 = spacing + std::string(AST_INDENT_SPACING, ' ');
    if (const auto* program = dynamic_cast<const Program*>(node)) {
        std::cout << spacing << "Program {\n";
        for (auto& declaration : program->declarations) {
            PrintAST(declaration.get(), indent_level + 1);
        }
        std::cout << spacing << "},\n";
    } else if (const auto* varDecl = dynamic_cast<const VarDecl*>(node)) {
        std::cout << spacing << "VarDecl {\n"
                  << spacing2 << "variable_name: \"" << varDecl->variable_name << "\",\n";
        PrintAST(varDecl->expression.get(), indent_level + 1);
        std::cout << spacing << "},\n";
    } else if (const auto* binary = dynamic_cast<const Binary*>(node)) {
        std::cout << spacing << "Binary {\n"
                  << spacing2 << "op: " << GetTokenString(binary->op) << ",\n"
                  << spacing2 << "left_expression: \n";
        PrintAST(binary->left_expression.get(), indent_level + 1);
        std::cout << spacing2 << "right_expression: \n";
        PrintAST(binary->right_expression.get(), indent_level + 1);
        std::cout << spacing << "},\n";
    } else if (const auto* unary = dynamic_cast<const Unary*>(node)) {
        std::cout << spacing << "Unary {\n"
                 << spacing2 << "op: " << GetTokenString(unary->op) << ",\n"
                 << spacing2 << "expression: \n";
        PrintAST(unary->expression.get(), indent_level + 1);
        std::cout << spacing << "},\n";
    } else if (const auto* literal = dynamic_cast<const Literal*>(node)) {
        std::cout << spacing << "Literal {\n"
                 << spacing2 << "value: " << VariantToString(literal->value) << ",\n"
                 << spacing << "},\n";
    } else if (const auto* identifier = dynamic_cast<const Identifier*>(node)) {
        std::cout << spacing << "Identifier {\n"
                 << spacing2 << "name; " << identifier->name << ",\n"
                 << spacing << "},\n";
    } else {
        std::cout << spacing << "Unknown ASTNode type {}, \n";
    }
}

static std::string GetOperator(TokenType type) {
    switch (type) {
        case TokenType::PLUS: return "+";
        case TokenType::MINUS: return "-";
        case TokenType::STAR: return "*";
        case TokenType::SLASH: return "/";
        case TokenType::AND: return "and";
        case TokenType::OR: return "or";
        case TokenType::EQUAL: return "=";
        case TokenType::EQUAL_EQUAL: return "==";
        case TokenType::BANG: return "!";
        case TokenType::BANG_EQUAL: return "!=";
        case TokenType::GREATER: return ">";
        case TokenType::GREATER_EQUAL: return ">=";
        case TokenType::LESS: return "<";
        case TokenType::LESS_EQUAL: return "<=";
        default: return "?";
    }
}

static std::string WrapWithParen(std::string str) {
    return "(" + str + ")";
}

static std::string CreateExprString(const Expression* const expression) {
    if (const auto* literal = dynamic_cast<const Literal*>(expression)) {
        return VariantToString(literal->value);
    }
    if (const auto* identifier = dynamic_cast<const Identifier*>(expression)) {
        return std::string(identifier->name);
    }
    if (const auto* binary = dynamic_cast<const Binary*>(expression)) {
        return WrapWithParen(CreateExprString(binary->left_expression.get()) +
            " " + GetOperator(binary->op) + " " +
                CreateExprString(binary->right_expression.get()));
    }
    if (const auto* unary = dynamic_cast<const Unary*>(expression)) {
        return WrapWithParen(GetOperator(unary->op) + CreateExprString(unary->expression.get()));
    }
    return "(Unknown Expression)";
}

std::string Debug::GetExpressionStr(const Expression* const expression) {
    std::string expr = CreateExprString(expression);
    if (expr.size() > 2 && expr[0] == '(') {
        expr = expr.substr(1, expr.size() - 2);
    }
    return expr;
}

