#ifndef VM_H
#define VM_H

#include <optional>

#include "ast.h"
#include "chunk.h"
#include "logger.h"

class VM {
public:
    VM();
    void Interpret(const Chunk& chunk);
    void SetDebug(Logger logger);
private:
    void PushStack(Value val);
    Value PopStack();
    Value StackTop();
    void Error(std::string msg);

    // Used for debugging, prints an opcode and potential operand to debug logger
    void PrintStatus(OP cur_instruction, std::optional<uint8_t> operand = std::nullopt);
    void PrintStack();
    void PrintChunkDebugInfo(const Chunk& chunk);
    [[nodiscard]] bool HasDebugLogger() const;

    Value Add(Value left, Value right);
    Value Subtract(Value left, Value right);
    Value Multiply(Value left, Value right);
    Value Divide(Value left, Value right);
private:
    static constexpr int MAX_STACK_SIZE_ = 2048;
    std::array<Value, MAX_STACK_SIZE_> stack_;
    int sp_;
    bool stack_has_changed_;
    Logger error_logger_;
    std::optional<Logger> debug_logger_;
};

#endif //VM_H
