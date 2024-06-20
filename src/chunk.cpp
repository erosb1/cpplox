#include "chunk.h"

void Chunk::Write(uint8_t byte) {
    code_.push_back(byte);
}

uint8_t Chunk::AddConstant(Value constant) {
    constants_.push_back(constant);
    return constants_.size() - 1;
}

std::vector<uint8_t> Chunk::GetCode() const {
    return code_;
}
