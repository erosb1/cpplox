#include "lexer.h"

Lexer::Lexer(std::string_view source_code)
    : source_code_(source_code)
    , cur_index_(0)
    , cur_line_(0)
{}

std::vector<Token> Lexer::Tokenize() {
    return {};
}

// Returns the current character than increments the pointer to the next character
char Lexer::Advance() {
    if (IsAtEnd()) return 0;
    return source_code_[cur_index_++];
}

// Checks if the current character is equal to expected, if it is then it returns true and increments pointer
// if not then it just returns false without incrementing
bool Lexer::Match(char expected) {
    if (IsAtEnd()) return false;
    if (source_code_[cur_index_] != expected) return false;
    cur_index_++;
    return true;
}

// Returns the value of the current character without incrementing the pointer
char Lexer::Peek() const {
    return source_code_[cur_index_];
}

// Returns the value of the next character without incrementing the pointer
char Lexer::PeekNext() const {
    return source_code_[cur_index_ + 1];
}

// Checks if the current character is the null terminator
bool Lexer::IsAtEnd() const {
    return source_code_[cur_index_] == '\0';
}
