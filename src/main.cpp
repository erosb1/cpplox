#include <iostream>

#include "lexer.h"
#include "ast.h"
#include "parser.h"
#include "debug.h"

int main() {
    std::string source_code = "fibe +";
    Parser parser(source_code);
    auto expression = parser.ParseExpression();
    std::cout << Debug::GetExpressionStr(expression.get()) << std::endl;
    return 0;
}