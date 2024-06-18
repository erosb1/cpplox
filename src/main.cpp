#include <iostream>

#include "lexer.h"
#include "ast.h"
#include "parser.h"
#include "debug.h"

int main() {
    std::string source_code = R"(
    functino();
    )";
    Parser parser(source_code);
    auto ast = parser.GenerateAST();
    Debug::PrintAST(ast.get());
    //std::cout << Debug::GetExpressionStr(expression.get()) << std::endl;
    return 0;
}