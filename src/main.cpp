#include <iostream>

#include "lexer.h"
#include "ast.h"
#include "parser.h"
#include "debug.h"

int main() {
    std::string source_code = R"(
        fun add(a, b) {
            return a + b;
        }

        fun main() {
            print("Enter two numbers");
            var a = prompt();
            var b = prompt();
            print("The sum is " + add(a, b) + ".");
        }
    )";
    Parser parser(source_code);
    auto ast = parser.GenerateAST();
    Debug::ASTStringVisitor visitor;
    ast->accept(visitor);
    std::cout << visitor.GetString() << std::endl;

    return 0;
}