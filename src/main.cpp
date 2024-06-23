#include <iostream>

#include "lexer.h"
#include "ast.h"
#include "compiler.h"
#include "parser.h"
#include "debug.h"
#include "semantic_analyser.h"

int main() {
    std::string source_code = R"(
        4 - 3;
    )";
    Parser parser(source_code);
    auto ast = parser.GenerateAST();
    SemanticAnalyser analyser;
    ast->accept(analyser);
    Compiler compiler;
    Chunk chunk = compiler.Compile(ast.get());
    std::cout << Debug::GetChunkStr(chunk) << std::endl;

    //Debug::ASTStringVisitor debug;
    //ast->accept(debug);
    //std::cout << debug.GetString() << std::endl;

    return 0;
}