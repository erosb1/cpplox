#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

#include "lexer.h"
#include "ast.h"

namespace Debug {
    void PrintTokens(const std::vector<Token>& tokens);
    void PrintAST(const ASTNode* head, size_t indent_level = 0);
    std::string GetExpressionStr(const Expression* expression);
}

// Used for printing TokenTypes in unit tests
std::ostream& operator<<(std::ostream& os, const TokenType& type);


#endif //DEBUG_H
