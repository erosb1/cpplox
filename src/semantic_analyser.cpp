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
    size_t parameter_count = node.parameters == nullptr ? 0 : node.parameters->identifiers.size();
    FunctionInfo function_info = {std::string(node.name->name), parameter_count};
    Symbol sym = { SymbolType::FUNCTION, function_info };
    bool result = scopes_.back().AddSymbol(std::string(node.name->name), sym);
    if (!result) Error(std::string(node.name->name) + " is already defined");
    PushScope();
    if (parameter_count > 0) node.parameters->accept(*this);
    node.body->accept(*this);
    PopScope();
}

void SemanticAnalyser::visit(VarDecl &node) {
    VariableInfo variable_info = { std::string(node.variable->name) };
    Symbol sym = { SymbolType::VARIABLE, variable_info };
    bool result = scopes_.back().AddSymbol(std::string(node.variable->name), sym);
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
    auto symbol = GetSymbol(std::string(node.callee->name));
    if (symbol == nullptr) {
        Error("Call to undefined function " + std::string(node.callee->name));
        return;
    }
    if (!std::holds_alternative<FunctionInfo>(symbol->object)) {
        Error(std::string(node.callee->name) + " is not a function");
    }
    auto function_info = std::get<FunctionInfo>(symbol->object);
    size_t call_argument_count = node.arguments == nullptr ? 0 : node.arguments->expressions.size();
    if (call_argument_count != function_info.parameter_count) {
        Error("Invalid argument count when calling function: " + std::string(node.callee->name) +
                    ",\n\tExpected: " + std::to_string(function_info.parameter_count) + ", Actual: " + std::to_string(call_argument_count));
    }
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
    for (auto& identifier : node.identifiers) {
        VariableInfo variable_info = { std::string(identifier->name) };
        Symbol symbol = { SymbolType::VARIABLE, variable_info };
        scopes_.back().AddSymbol(std::string(identifier->name), symbol);
    }
}

void SemanticAnalyser::visit(Arguments &node) {
    return;
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

const Symbol* SemanticAnalyser::GetSymbol(std::string symbol_name) {
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
        if (it->Contains(symbol_name)) {
            return it->GetSymbol(symbol_name);
        }
    }
    return nullptr;
}
