#include <iostream>
#include <iomanip>

#include "vm.h"
#include "debug.h"

#include <cassert>

VM::VM()
    : sp_(0)
    , stack_has_changed_(false)
    , had_error_(false) {
    Logger error_logger(LogLevel::ERROR);
    error_logger_ = std::move(error_logger_);
}

void VM::Interpret(const Chunk &chunk) {
    if (HasDebugLogger()) PrintChunkDebugInfo(chunk);
    auto& code = chunk.GetCode();
    for (int i = 0; i < code.size() && !had_error_; i++) {
        auto op_code = static_cast<OP>(code[i]);
        if (HasDebugLogger()) {
            // Check if op_code has any operands, and then also print them
            if (OP_DEFINITIONS.at(op_code).operand_count == 1) PrintStatus(op_code, code[i+1]);
            // otherwise just print current op_code
            else PrintStatus(op_code);
        }
        switch (op_code) {
            case OP::CONSTANT: {
                uint8_t index = code[++i];
                Value constant = chunk.GetConstants().at(index);
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
                }
            } break;
            case OP::NOT: {
                Value val = PopStack();
                if (val.IsFalsey()) PushStack(true);
                else PushStack(false);
            } break;
            default:
                Error("Invalid OPCODE");
        }
        if (HasDebugLogger() && stack_has_changed_) PrintStack();
    }
}

void VM::SetDebug(Logger logger) {
    debug_logger_ = std::move(logger);
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

void VM::Error(std::string msg) const {
    error_logger_.Log("[RUNTIME ERROR]" + msg);
    had_error_ = true;
}

void VM::PrintStatus(OP op_code, std::optional<uint8_t> operand) const {
    assert(debug_logger_.has_value());
    auto& op_name = OP_DEFINITIONS.at(op_code).name;
    std::string temp = "[" + op_name + "]";
    *debug_logger_ << std::left << std::setw(12) << temp;

    if (operand.has_value()) {
        *debug_logger_ << static_cast<int>(*operand);
    }
    *debug_logger_ << "\n";
}

void VM::PrintStack() const {
    assert(debug_logger_.has_value());
    *debug_logger_ << "Stack: [";
    for (int i = 0; i < sp_; i++) {
        *debug_logger_ << stack_[i].GetValueDebugString();
        if (i+1 != sp_) *debug_logger_ << ", ";
    }
    *debug_logger_ << "]\n";
    stack_has_changed_ = false;
}

void VM::PrintChunkDebugInfo(const Chunk& chunk) const {
    assert(debug_logger_.has_value());
    *debug_logger_ << "VM DEBUG INFO\nConstants: [";
    auto& constants = chunk.GetConstants();
    for (int i = 0; i < constants.size(); i++) {
        *debug_logger_ << constants[i].GetValueDebugString();
        if (i+1 != constants.size()) *debug_logger_ << ", ";
    }
    *debug_logger_ << "]\n\n";
}

bool VM::HasDebugLogger() const {
    return debug_logger_.has_value();
}