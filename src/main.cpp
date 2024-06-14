#include <iostream>

#include "lexer.h"
#include "ast.h"
#include "parser.h"
#include "debug.h"

int main() {
    ProgramPtr program = std::make_unique<Program>();
    VarDeclPtr varDecl = std::make_unique<VarDecl>();
    varDecl->variable_name = "Foo";
    ExpressionPtr expression = std::make_unique<Expression>();
    varDecl->expression = std::move(expression);

    VarDeclPtr varDecl2 = std::make_unique<VarDecl>();
    varDecl2->variable_name = "Bar";
    ExpressionPtr expression2 = std::make_unique<Expression>();
    varDecl2->expression = std::move(expression2);

    program->declarations.push_back(std::move(varDecl));
    program->declarations.push_back(std::move(varDecl2));

    Debug::PrintAST(program.get());

    return 0;
}