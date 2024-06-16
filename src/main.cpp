#include <iostream>

#include "lexer.h"
#include "ast.h"
#include "parser.h"
#include "debug.h"

int main() {
    std::string source_code = "a + b";
    Parser parser(source_code);
    auto ast = parser.GenerateAST();
    Debug::PrintAST(ast.get());
    return 0;
}