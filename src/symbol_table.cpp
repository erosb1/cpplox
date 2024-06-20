#include "symbol_table.h"

bool SymbolTable::AddSymbol(std::string symbol_name, Symbol symbol) {
    if (Contains(symbol_name)) return false;
    table_.emplace(symbol_name, std::move(symbol));
    return true;
}

bool SymbolTable::Contains(std::string symbol_name) const {
    return table_.contains(symbol_name);
}

const Symbol* SymbolTable::GetSymbol(std::string symbol_name) const {
    if (Contains(symbol_name)) {
        return &table_.find(symbol_name)->second;
    }
    return nullptr;
}
