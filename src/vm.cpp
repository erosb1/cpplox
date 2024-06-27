#include <iostream>
#include <iomanip>

#include "vm.h"
#include "debug.h"

#include <cassert>
#include <boost/test/tools/assertion.hpp>

VM::VM(const Chunk& chunk)
    : chunk_(chunk)
    , pc_(0)
    , sp_(0)
    , stack_has_changed_(false) {
    Logger error_logger(LogLevel::ERROR);
    error_logger_ = std::move(error_logger_);
}

void VM::Interpret() {
    if (HasDebugLogger()) PrintChunkDebugInfo();
    while (pc_ < chunk_.Size()) {
        if (HasDebugLogger()) PrintStatus();
        bool had_error = InterpretNext();
        if (HasDebugLogger() && stack_has_changed_) PrintStack();
        if (had_error) break;
    }
}

void VM::SetDebug(Logger logger) {
    debug_logger_ = std::move(logger);
}

bool VM::InterpretNext() {
    auto op_code = NextInstruction();
    switch (op_code) {
        case OP::CONSTANT: {
            auto index = ConsumeOperand();
            Value constant = chunk_.GetConstants().at(index);
            PushStack(constant);
        } break;
        case OP::ADD: {
            // TODO implement string concatenation
            Value right = PopStack();
            Value left = PopStack();
            if (left.IsDouble() && right.IsDouble()) {
                Value sum = left.AsDouble() + right.AsDouble();
                PushStack(sum);
            } else {
                Error("Cannot perform addition. Invalid types: " + left.GetTypeDebugString() + " and " + right.GetTypeDebugString());
                return true;
            }
        } break;
        case OP::SUBTRACT: {
            Value right = PopStack();
            Value left = PopStack();
            if (left.IsDouble() && right.IsDouble()) {
                Value difference = left.AsDouble() - right.AsDouble();
                PushStack(difference);
            } else {
                Error("Cannot perform subtraction. Invalid types: " + left.GetTypeDebugString() + " and " + right.GetTypeDebugString());
                return true;
            }
        } break;
        case OP::MULTIPLY: {
            Value right = PopStack();
            Value left = PopStack();
            if (left.IsDouble() && right.IsDouble()) {
                Value product = left.AsDouble() * right.AsDouble();
                PushStack(product);
            } else {
                Error("Cannot perform multiplication. Invalid types: " + left.GetTypeDebugString() + " and " + right.GetTypeDebugString());
                return true;
            }
        } break;
        case OP::DIVIDE: {
            Value right = PopStack();
            Value left = PopStack();
            if (left.IsDouble() && right.IsDouble()) {
                if (right.AsDouble() == 0.0) Error("Tried to divide by 0");
                else {
                    Value quotient = left.AsDouble() / right.AsDouble();
                    PushStack(quotient);
                }
            } else {
                Error("Cannot perform division. Invalid types: " + left.GetTypeDebugString() + " and " + right.GetTypeDebugString());
                return true;
            }
        } break;
        case OP::POP: {
            PopStack();
        } break;
        case OP::NEGATE: {
            Value val = PopStack();
            if (val.IsDouble()) {
                Value negated = -(val.AsDouble());
                PushStack(negated);
            } else {
                Error("Cannot perform negation. Invalid type: " + val.GetTypeDebugString());
                return true;
            }
        } break;
        case OP::NOT: {
            Value val = PopStack();
            if (val.IsFalsey()) PushStack(true);
            else PushStack(false);
        } break;
        case OP::EQUAL: {
            Value right = PopStack();
            Value left = PopStack();
            PushStack(right == left);
        } break;
        case OP::GREATER: {
            Value right = PopStack();
            Value left = PopStack();
            if (left.IsDouble() && right.IsDouble()) {
                PushStack(left.AsDouble() > right.AsDouble());
            }
            else {
                Error("Cannot perform comparison. Invalid types: " + left.GetTypeDebugString() + " and " + right.GetTypeDebugString());
                return true;
            }
        } break;
        case OP::LESS: {
            Value right = PopStack();
            Value left = PopStack();
            if (left.IsDouble() && right.IsDouble()) {
                PushStack(left.AsDouble() < right.AsDouble());
            }
            else {
                Error("Cannot perform comparison. Invalid types: " + left.GetTypeDebugString() + " and " + right.GetTypeDebugString());
                return true;
            }
        } break;
        default:
            Error("Invalid OPCODE");
            return true;
    }
    return false;
}

void VM::PushStack(Value val) {
    if (sp_ >= MAX_STACK_SIZE_) {
        Error("Stack Overflow");
        return;
    }
    stack_[sp_++] = val;
    stack_has_changed_ = true;
}

Value VM::PopStack() {
    if (stack_.empty()) {
        Error("Stack is empty");
        return {};
    }
    stack_has_changed_ = true;
    return stack_[--sp_];
}

Value VM::StackTop() const {
    if (stack_.empty()) {
        Error("Stack is empty");
        return {};
    }
    return stack_[sp_];
}

uint64_t VM::ConsumeOperand(int operand_count) {
    uint64_t combined = 0;
    for (int i = 0; i < operand_count; i++) {
        auto operand = NextInstruction();
        combined |= static_cast<uint64_t>(operand) << (8 * i);
    }
    return combined;
}

void VM::Error(std::string msg) const {
    error_logger_.Log("[RUNTIME ERROR]" + msg);
}


OP VM::NextInstruction() {
    return static_cast<OP>(chunk_.GetCode()[pc_++]);
}

// Each PrintStatus call corresponds to one row in the printed debug info (exluding stack content)
void VM::PrintStatus() const {
    assert(debug_logger_.has_value());
    auto& code = chunk_.GetCode();
    auto cur_instruction = static_cast<OP>(code.at(pc_));

    // Print Offset
    auto offset_str = std::to_string(pc_);
    auto padded_offset_str = std::string(6 - std::min(6, (int)offset_str.length()), '0') + offset_str;
    *debug_logger_ << padded_offset_str << "     ";

    // Print Op Code
    auto& op_name = OP_DEFINITIONS.at(cur_instruction).name;
    *debug_logger_ << std::left << std::setw(14) << op_name;

    // Print operands (convert all operands into one uint64_t)
    auto operand_count = OP_DEFINITIONS.at(cur_instruction).operand_count;
    uint64_t combined = 0;
    for (int i = 0; i < operand_count; i++) {
        uint8_t operand = code.at(pc_ + i + 1);
        combined |= static_cast<uint64_t>(operand) << (8 * i);
    }
    *debug_logger_ << std::left << std::setw(12) << combined;
}

void VM::PrintStack() const {
    assert(debug_logger_.has_value());
    if (stack_has_changed_) {
        *debug_logger_ << "[";
        for (int i = 0; i < sp_; i++) {
            *debug_logger_ << stack_[i].GetValueDebugString();
            if (i+1 != sp_) *debug_logger_ << ", ";
        }
        *debug_logger_ << "]\n";
    } else {
        *debug_logger_ << " |\n";
    }
    stack_has_changed_ = false;
}

void VM::PrintChunkDebugInfo() const {
    assert(debug_logger_.has_value());
    *debug_logger_ << "VM DEBUG INFO\nConstants: [";
    auto& constants = chunk_.GetConstants();
    for (int i = 0; i < constants.size(); i++) {
        *debug_logger_ << constants[i].GetValueDebugString();
        if (i+1 != constants.size()) *debug_logger_ << ", ";
    }
    *debug_logger_ << "]\n[OFFSET]   [OP CODE]     [OPERAND]   [STACK]\n";
}

bool VM::HasDebugLogger() const {
    return debug_logger_.has_value();
}