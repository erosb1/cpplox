#include <iostream>

#include "lexer.h"
#include "ast.h"
#include "parser.h"
#include "debug.h"

int main() {
    std::string source_code = "-a + 453.34 * termilonglong - 43";
    Parser parser(source_code);
    auto expression = parser.ParseExpression();
    Debug::PrintExpressionParen(expression.get());
    return 0;
}