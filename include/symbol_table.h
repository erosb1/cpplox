#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>

enum class SymbolType {
    VARIABLE,
    FUNCTION,
};

struct Symbol {
    SymbolType type;
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
