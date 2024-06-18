#include "symbol_table.h"

void SymbolTable::AddSymbol(std::string symbol_name, Symbol symbol) {
    table_[symbol_name] = symbol;
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
