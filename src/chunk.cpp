#include "chunk.h"

void Chunk::Write(uint8_t byte) {
    code_.push_back(byte);
}

uint8_t Chunk::AddConstant(Value constant) {
    constants_.push_back(constant);
    return constants_.size() - 1;
}

const std::vector<uint8_t>& Chunk::GetCode() const {
    return code_;
}

const std::vector<Value> & Chunk::GetConstants() const {
    return constants_;
}

size_t Chunk::Size() const {
    return code_.size();
}
