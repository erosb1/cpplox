#include <iostream>
#include <iomanip>

#include "vm.h"

#include <boost/test/tools/collection_comparison_op.hpp>

VM::VM()
    : sp_(0)
{}

void VM::Interpret(const Chunk &chunk, bool debug) {
    auto& code = chunk.GetCode();
    for (int i = 0; i < code.size(); i++) {
        auto op_code = static_cast<OP>(code[i]);
        if (debug) PrintStatus(op_code);
        switch (op_code) {
            case OP::CONSTANT: {
                uint8_t index = code[i++];
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

void VM::PushStack(Value val) {
    if (stack_.size() >= stack_size_) {
        Error("Stack Overflow");
        return;
    }
    stack_[sp_++] = val;
}

Value VM::PopStack() {
    if (stack_.empty()) {
        Error("Stack is empty");
        return {};
    }
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
    std::cerr << "[RUNTIME ERROR]: " << msg;
}

void VM::PrintStatus(OP op_code) {
    auto& op_definition = OP_DEFINITIONS.at(op_code);
    std::string temp = "[" + op_definition.name + "]";
    std::cout << std::left << std::setw(12) << temp;

    if (op_definition.operand_count > 0) {
        auto operand = static_cast<int>(code[++i]);
        oss << operand << "\n";
    }
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
