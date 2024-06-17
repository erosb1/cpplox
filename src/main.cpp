#include <iostream>

#include "lexer.h"
#include "ast.h"
#include "parser.h"
#include "debug.h"

int main() {
    std::string source_code = "bitch = dog + food";
    Parser parser(source_code);
    auto expression = parser.ParseExpression();
    Debug::PrintAST(expression.get());
    //std::cout << Debug::GetExpressionStr(expression.get()) << std::endl;
    return 0;
}