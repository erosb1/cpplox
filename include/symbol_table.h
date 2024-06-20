#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include <utility>
#include <variant>

enum class SymbolType {
    VARIABLE,
    FUNCTION,
};

struct VariableInfo {
    std::string name;
    VariableInfo(std::string name)
        : name(std::move(name)) {}
};

struct FunctionInfo {
    std::string name;
    size_t parameter_count;
    FunctionInfo(std::string name, size_t parameter_count)
        : name(std::move(name)), parameter_count(parameter_count) {}
};

struct Symbol {
    SymbolType type;
    std::variant<VariableInfo, FunctionInfo> object;
    Symbol(SymbolType type, std::variant<VariableInfo, FunctionInfo> object)
        : type(type), object(std::move(object)) {}
};

class SymbolTable {
public:
    bool AddSymbol(std::string symbol_name, Symbol symbol);
    [[nodiscard]] bool Contains(std::string symbol_name) const;
    [[nodiscard]] const Symbol* GetSymbol(std::string symbol_name) const;
private:
    std::unordered_map<std::string, Symbol> table_;
};


#endif //SYMBOL_TABLE_H
