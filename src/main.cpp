#include <iostream>

#include "lexer.h"
#include "ast.h"
#include "parser.h"
#include "debug.h"
#include "semantic_analyser.h"

int main() {
    std::string source_code = R"(
        var a = 32;
        a;

        { a; }
        { var b = 32; b;}
    )";
    Parser parser(source_code);
    auto ast = parser.GenerateAST();
    SemanticAnalyser analyser;
    ast->accept(analyser);
    Debug::ASTStringVisitor visitor;
    ast->accept(visitor);
    std::cout << visitor.GetString() << std::endl;

    return 0;
}