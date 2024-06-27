#ifndef VM_H
#define VM_H

#include <optional>

#include "ast.h"
#include "chunk.h"
#include "logger.h"

class VM {
public:
    VM(const Chunk& chunk);
    void Interpret();
    void SetDebug(Logger logger);
private:
    bool InterpretNext();
    void PushStack(Value val);
    Value PopStack();
    Value StackTop() const;
    uint64_t ConsumeOperand(int operand_size = 1);
    void Error(std::string msg) const;
    OP NextInstruction();

    // Used for debugging, prints an opcode and potential operand to debug logger
    void PrintStatus() const;
    void PrintStack() const;
    void PrintChunkDebugInfo() const;
    [[nodiscard]] bool HasDebugLogger() const;
private:
    const Chunk& chunk_;
    int pc_;
    static constexpr int MAX_STACK_SIZE_ = 2048;
    std::array<Value, MAX_STACK_SIZE_> stack_;
    int sp_;

    // Debug variables, not part of the VM logic
    mutable bool stack_has_changed_;
    mutable Logger error_logger_;
    mutable std::optional<Logger> debug_logger_;
};

#endif //VM_H
