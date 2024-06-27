#ifndef VALUE_H
#define VALUE_H

#include <variant>
#include <string_view>
#include <sstream>
#include <cassert>

struct Value {
    using InternalVal = std::variant<double, bool, std::string_view, std::monostate>;

    // Constructors
    Value() : val_(std::monostate{}) {};
    Value(double val) : val_(val) {}
    Value(bool val) : val_(val) {}
    Value(std::string_view val) : val_(val) {}
    Value(std::monostate val) : val_(val) {}

    // Checkers
    [[nodiscard]] bool IsDouble() const { return std::holds_alternative<double>(val_); }
    [[nodiscard]] bool IsBool() const { return std::holds_alternative<bool>(val_); }
    [[nodiscard]] bool IsString() const { return std::holds_alternative<std::string_view>(val_); }
    [[nodiscard]] bool IsNil() const { return std::holds_alternative<std::monostate>(val_); }

    // Getters
    [[nodiscard]] double AsDouble() const { assert(IsDouble()); return std::get<double>(val_); }
    [[nodiscard]] bool AsBool() const { assert(IsBool()); return std::get<bool>(val_); }
    [[nodiscard]] std::string_view AsString() const { assert(IsString()); return std::get<std::string_view>(val_); }
    [[nodiscard]] std::monostate AsNil() const { assert(IsNil()); return std::get<std::monostate>(val_); }

    // Get Underlying Variant
    [[nodiscard]] InternalVal Get() const { return val_; };

    // Assignment
    Value& operator=(double rhs) { val_ = rhs; return *this; }
    Value& operator=(bool rhs) { val_ = rhs; return *this; }
    Value& operator=(std::string_view rhs) { val_ = rhs; return *this; }
    Value& operator=(std::monostate rhs) { val_ = rhs; return *this; }

    // Equality
    bool operator==(const Value& rhs) const { return val_ == rhs.val_; }
    bool operator!=(const Value& rhs) const { return val_ != rhs.val_; }

    [[nodiscard]] std::string GetValueDebugString() const {
        if (IsDouble()) {
            std::ostringstream oss;
            oss.precision(2);
            oss << std::fixed << AsDouble();
            return oss.str();
        }
        if (IsBool()) {
            return AsBool() ? "true" : "false";
        }
        if (IsString()) {
            return std::string(AsString());
        }
        return "nil";
    }

    [[nodiscard]] std::string GetTypeDebugString() const {
        if (IsDouble()) return "double";
        if (IsBool()) return "bool";
        if (IsString()) return "string";
        return "nil";
    }
private:
    InternalVal val_;
};

// Utility function to print Value
inline std::ostream& operator<<(std::ostream& os, const Value& val) {
    os << val.GetValueDebugString();
    return os;
}

#endif //VALUE_H
