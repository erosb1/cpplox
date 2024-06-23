#ifndef VM_H
#define VM_H

#include "ast.h"
#include "chunk.h"

class VM {
public:
    VM();
    void Interpret(const Chunk& chunk, bool debug = false);
private:
    void PushStack(Value val);
    Value PopStack();
    Value StackTop();
    void Error(std::string msg);
    void PrintStatus(OP cur_instruction);

    Value Add(Value left, Value right);
    Value Subtract(Value left, Value right);
    Value Multiply(Value left, Value right);
    Value Divide(Value left, Value right);
private:
    static constexpr int stack_size_ = 2048;
    std::array<Value, stack_size_> stack_;
    int sp_;
};

#endif //VM_H
