#ifndef CHUNK_H
#define CHUNK_H

#include <vector>

#include "ast.h"

enum class OpCode {
    CONSTANT,
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    POP,
    NEGATE,
    NOT,
    EQUAL,
    GREATER,
    LESS,
    JUMP,
    JUMP_IF_FALSE,
};

using OP = OpCode;

struct OpDefinition {
    std::string name;
    size_t operand_count;
};

const inline std::unordered_map<OP, OpDefinition> OP_DEFINITIONS = {
    {OP::CONSTANT, {"CONSTANT", 1}},
    {OP::ADD, {"ADD", 0}},
    {OP::SUBTRACT, {"SUBTRACT", 0}},
    {OP::MULTIPLY, {"MULTIPLY", 0}},
    {OP::DIVIDE, {"DIVIDE", 0}},
    {OP::POP, {"POP", 0}},
    {OP::NEGATE, {"NEGATE", 0}},
    {OP::NOT, {"NOT", 0}},
    {OP::EQUAL, {"EQUAL", 0}},
    {OP::GREATER, {"GREATER", 0}},
    {OP::LESS, {"LESS", 0}},
    {OP::JUMP, {"JUMP", 2}},
    {OP::JUMP_IF_FALSE, {"JUMP_IF_FALSE", 2}},
};

class Chunk {
public:
    void Write(uint8_t byte);
    uint8_t AddConstant(Value constant);
    [[nodiscard]] const std::vector<uint8_t>& GetCode() const; // for debug printing
    [[nodiscard]] const std::vector<Value>& GetConstants() const;
    [[nodiscard]] size_t Size() const;
private:
    std::vector<uint8_t> code_;
    std::vector<Value> constants_;
};

#endif //CHUNK_H
