#include <iostream>
#include <cassert>

#include "semantic_analyser.h"

SemanticAnalyser::SemanticAnalyser() {
    PushScope();
}

void SemanticAnalyser::visit(Program &node) {
    for (auto& declaration : node.declarations) {
        declaration->accept(*this);
    }
}

void SemanticAnalyser::visit(FunDecl &node) {
    // TODO implement
}

void SemanticAnalyser::visit(VarDecl &node) {
    Symbol table = { SymbolType::VARIABLE };
    bool result = scopes_.back().AddSymbol(std::string(node.variable->name), table);
    if (!result) Error(std::string(node.variable->name) + " is already defined");
    node.expression->accept(*this);
}

void SemanticAnalyser::visit(ExprStmt &node) {
    node.expression->accept(*this);
}

void SemanticAnalyser::visit(IfStmt &node) {
    node.if_body->accept(*this);
    if (node.else_body != nullptr) {
        node.else_body->accept(*this);
    }
}

void SemanticAnalyser::visit(PrintStmt &node) {
    node.expression->accept(*this);
}

void SemanticAnalyser::visit(ReturnStmt &node) {
    node.expression->accept(*this);
}

void SemanticAnalyser::visit(WhileStmt &node) {
    node.body->accept(*this);
}

void SemanticAnalyser::visit(Block &node) {
    PushScope();
    for (auto& declaration : node.declarations) {
        declaration->accept(*this);
    }
    PopScope();
}

void SemanticAnalyser::visit(Assignment &node) {
    if (!CheckSymbol(std::string(node.variable->name))) {
        Error("undefined variable: " + std::string(node.variable->name));
    }
    node.expression->accept(*this);
}

void SemanticAnalyser::visit(Binary &node) {
    node.left_expression->accept(*this);
    node.right_expression->accept(*this);
}

void SemanticAnalyser::visit(Unary &node) {
    node.expression->accept(*this);
}

void SemanticAnalyser::visit(Call &node) {
    // TODO implement
}

void SemanticAnalyser::visit(Identifier &node) {
    if (!CheckSymbol(std::string(node.name))) {
        Error("Undefined identifier " + std::string(node.name));
    }
}

void SemanticAnalyser::visit(Literal &node) {
    return;
}

void SemanticAnalyser::visit(Parameters &node) {
}

void SemanticAnalyser::visit(Arguments &node) {
}

void SemanticAnalyser::PushScope() {
    scopes_.push_back({});
}

void SemanticAnalyser::PopScope() {
    assert(scopes_.size() >= 2);
    scopes_.pop_back();
}

void SemanticAnalyser::Error(std::string msg) {
    std::cerr << "[SEMANTIC ERROR]: " << msg << std::endl;
}

bool SemanticAnalyser::CheckSymbol(std::string symbol_name) {
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
        if (it->Contains(symbol_name)) return true;
    }
    return false;
}
