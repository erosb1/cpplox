#ifndef COMPILER_H
#define COMPILER_H

#include "parser.h"
#include "semantic_analyser.h"
#include "chunk.h"

class Compiler : public ASTVisitor {
public:
    Chunk Compile(Program* program);
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
    void Emit(OpCode op_code);
    void EmitWithOperand(OpCode op_code, uint8_t operand);
private:
    Chunk cur_chunk_;
};

#endif //COMPILER_H
