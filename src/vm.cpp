#include <iostream>
#include <iomanip>

#include "vm.h"
#include "debug.h"

#include <cassert>

VM::VM()
    : sp_(0)
    , stack_has_changed_(false) {
    Logger error_logger(LogLevel::ERROR);
    error_logger_ = std::move(error_logger_);
}

void VM::Interpret(const Chunk &chunk) {
    if (HasDebugLogger()) PrintChunkDebugInfo(chunk);
    auto& code = chunk.GetCode();
    for (int i = 0; i < code.size(); i++) {
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
                Value left = PopStack();
                Value right = PopStack();
                Value sum = Add(left, right);
                PushStack(sum);
            } break;
            case OP::SUBTRACT: {
                Value left = PopStack();
                Value right = PopStack();
                Value difference = Subtract(left, right);
                PushStack(difference);
            } break;
            case OP::MULTIPLY: {
                Value left = PopStack();
                Value right = PopStack();
                Value product = Multiply(left, right);
                PushStack(product);
            } break;
            case OP::DIVIDE: {
                Value left = PopStack();
                Value right = PopStack();
                Value quotient = Divide(left, right);
                PushStack(quotient);
            } break;
            default:
                Error("Invalid OPCODE");
        }
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
    return stack_[sp_--];
}

Value VM::StackTop() {
    if (stack_.empty()) {
        Error("Stack is empty");
        return {};
    }
    return stack_[sp_--];
}

void VM::Error(std::string msg) {
    error_logger_.Log("[RUNTIME ERROR]" + msg);
}

void VM::PrintStatus(OP op_code, std::optional<uint8_t> operand) {
    assert(debug_logger_.has_value());
    if (stack_has_changed_) PrintStack();
    auto& op_name = OP_DEFINITIONS.at(op_code).name;
    std::string temp = "[" + op_name + "]";
    *debug_logger_ << std::left << std::setw(12) << temp;

    if (operand.has_value()) {
        *debug_logger_ << static_cast<int>(*operand);
    }
    *debug_logger_ << "\n";
}

void VM::PrintStack() {
    assert(debug_logger_.has_value());
    *debug_logger_ << "Stack: [";
    for (int i = 0; i < sp_; i++) {
        *debug_logger_ << Debug::VariantToString(stack_[i]);
        if (i+1 != sp_) *debug_logger_ << ", ";
    }
    *debug_logger_ << "]\n";
    stack_has_changed_ = false;
}

void VM::PrintChunkDebugInfo(const Chunk& chunk) {
    assert(debug_logger_.has_value());
    *debug_logger_ << "VM DEBUG INFO\nConstants: [";
    auto constants = chunk.GetConstants();
    for (int i = 0; i < constants.size(); i++) {
        *debug_logger_ << Debug::VariantToString(constants[i]);
        if (i+1 != constants.size()) *debug_logger_ << ", ";
    }
    *debug_logger_ << "]\n\n";
}

bool VM::HasDebugLogger() const {
    return debug_logger_.has_value();
}

Value VM::Add(Value left, Value right) {
    if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
        Value sum = std::get<double>(left) + std::get<double>(right);
        return sum;
    }
    // TODO string concatenation and string intering
    //if (std::holds_alternative<std::string_view>(left) && std::holds_alternative<std::string_view>(right)) {
    //    std::string
    //}
    Error("Mismatched types, cannot perform addition");
    return {};
}

Value VM::Subtract(Value left, Value right) {
    if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
        Value difference = std::get<double>(left) - std::get<double>(right);
        return difference;
    }
    Error("Mismatched types, cannot perform subtraction");
    return {};
}

Value VM::Multiply(Value left, Value right) {
    if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
        Value product = std::get<double>(left) * std::get<double>(right);
        return product;
    }
    Error("Mismatched types, cannot perform multiplication");
    return {};
}

Value VM::Divide(Value left, Value right) {
    if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
        if (std::get<double>(right) == 0.0) {
            Error("Tried to divide by 0");
            return {};
        }
        Value quotient = std::get<double>(left) / std::get<double>(right);
        return quotient;
    }
    Error("Mismatched types, cannot perform division");
    return {};
}
