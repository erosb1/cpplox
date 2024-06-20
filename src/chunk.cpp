#include "chunk.h"

void Chunk::Write(uint8_t byte) {
    code_.push_back(byte);
}

void Chunk::AddConstant(Value constant) {
    constants_.push_back(constant);
}
