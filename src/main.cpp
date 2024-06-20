#include <iostream>

#include "lexer.h"
#include "ast.h"
#include "parser.h"
#include "debug.h"
#include "semantic_analyser.h"

int main() {
    std::string source_code = R"(
        fun add(a, b) {
            return a + b;
            a;
        }
        print add(5,5,4);
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