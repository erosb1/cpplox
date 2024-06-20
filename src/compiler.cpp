#include "compiler.h"

Chunk Compiler::Compile(Program* program) {
    const Chunk new_chunk_;
    cur_chunk_ = new_chunk_;
    program->accept(*this);
    return cur_chunk_;
}

void Compiler::visit(Program &node) {
    for (auto& declaration : node.declarations) {
        declaration->accept(*this);
    }
}

void Compiler::visit(FunDecl &node) {
    // TODO implement functions
    node.body->accept(*this);
}

void Compiler::visit(VarDecl &node) {
    node.expression->accept(*this);
}

void Compiler::visit(ExprStmt &node) {
    node.expression->accept(*this);
}

void Compiler::visit(IfStmt &node) {
    node.condition->accept(*this);
    node.if_body->accept(*this);
    if (node.else_body != nullptr) node.else_body->accept(*this);
}

void Compiler::visit(PrintStmt &node) {
    node.expression->accept(*this);
}

void Compiler::visit(ReturnStmt &node) {
    node.expression->accept(*this);
}

void Compiler::visit(WhileStmt &node) {
    node.condition->accept(*this);
    node.body->accept(*this);
}

void Compiler::visit(Block &node) {
    // PushScope
    for (auto& declaration : node.declarations) {
        declaration->accept(*this);
    }
    // PopScope
}

void Compiler::visit(Assignment &node) {
    node.expression->accept(*this);
}

void Compiler::visit(Binary &node) {
    node.left_expression->accept(*this);
    node.right_expression->accept(*this);
    switch (node.op) {
        case TT::PLUS: Emit(OP::ADD); break;
        case TT::MINUS: Emit(OP::SUBTRACT); break;
        case TT::STAR: Emit(OP::MULTIPLY); break;
        case TT::SLASH: Emit(OP::DIVIDE); break;
        default:
            throw "no";
    }
}

void Compiler::visit(Unary &node) {
    node.expression->accept(*this);
}

void Compiler::visit(Call &node) {
    node.arguments->accept(*this);
}

void Compiler::visit(Identifier &node) {

}

void Compiler::visit(Literal &node) {
    // TODO intern string
    uint8_t index = cur_chunk_.AddConstant(node.value);
    EmitWithOperand(OP::CONSTANT, index);
}

void Compiler::visit(Parameters &node) {
}

void Compiler::visit(Arguments &node) {
    for (auto& expression : node.expressions) {
        expression->accept(*this);
    }
}

void Compiler::Emit(OpCode op_code) {
    auto byte = static_cast<uint8_t>(op_code);
    cur_chunk_.Write(byte);
}

void Compiler::EmitWithOperand(OpCode op_code, uint8_t operand) {
    auto byte = static_cast<uint8_t>(op_code);
    cur_chunk_.Write(byte);
    cur_chunk_.Write(operand);
}
