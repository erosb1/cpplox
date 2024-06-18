#ifndef SEMANTIC_ANALYSER_H
#define SEMANTIC_ANALYSER_H

#include <vector>

#include "ast.h"
#include "symbol_table.h"

class SemanticAnalyser : public ASTVisitor {
public:
    SemanticAnalyser();
    void visit(Program &node) override;
    void visit(FunDecl &node) override;
    void visit(VarDecl &node) override;
    void visit(ExprStmt &node) override;
    void visit(IfStmt &node) override;
    void visit(PrintStmt &node) override;
    void visit(ReturnStmt &node) override;
    void visit(WhileStmt &node) override;
    void visit(Block &node) override;
    void visit(Assignment &node) override;
    void visit(Binary &node) override;
    void visit(Unary &node) override;
    void visit(Call &node) override;
    void visit(Identifier &node) override;
    void visit(Literal &node) override;
    void visit(Parameters &node) override;
    void visit(Arguments &node) override;
private:
    void PushScope();
    void PopScope();
    void Error(std::string msg);
    bool CheckSymbol(std::string msg);
private:
    std::vector<SymbolTable> scopes_;
};

#endif //SEMANTIC_ANALYSER_H
