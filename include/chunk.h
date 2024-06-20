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
};

using OP = OpCode;

class Chunk {
public:
    void Write(uint8_t byte);
    uint8_t AddConstant(Value constant);
    [[nodiscard]] std::vector<uint8_t> GetCode() const; // for debug printing
private:
    std::vector<uint8_t> code_;
    std::vector<Value> constants_;
};

#endif //CHUNK_H
