#ifndef CHUNK_H
#define CHUNK_H

#include <vector>

#include "ast.h"

enum class OpCode {
    PUSH,
    ADD,
};

class Chunk {
public:
    void Write(uint8_t byte);
    void AddConstant(Value constant);
private:
    std::vector<uint8_t> code_;
    std::vector<Value> constants_;
};

#endif //CHUNK_H
