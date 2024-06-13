#include "lexer.h"

Lexer::Lexer(std::string_view source_code)
    : source_code_(source_code)
    , cur_index_(0)
    , start_index_(0)
    , cur_line_(0)
{}

std::vector<Token> Lexer::Tokenize() {
    std::vector<Token> tokens;
    while (true) {
        tokens.push_back(ScanNext());
        if (IsAtEnd()) {
            tokens.push_back(CreateToken(TT::END));
            break;
        }
    }
    return tokens;
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

Token Lexer::CreateToken(TT type) {
    auto sv = std::string_view(source_code_.begin() + start_index_, source_code_.begin() + cur_index_);
    return {type, sv, cur_line_};
}

Token Lexer::ScanNext() {
    SkipWhitespace();
    start_index_ = cur_index_;
    char c = Advance();

    switch (c) {
        case '(': return CreateToken(TT::LEFT_PAREN);
        case ')': return CreateToken(TT::RIGHT_PAREN);
        case '{': return CreateToken(TT::LEFT_BRACE);
        case '}': return CreateToken(TT::RIGHT_BRACE);
        case ',': return CreateToken(TT::COMMA);
        case '.': return CreateToken(TT::DOT);
        case '-': return CreateToken(TT::MINUS);
        case '+': return CreateToken(TT::PLUS);
        case ';': return CreateToken(TT::SEMICOLON);
        case '/': return CreateToken(TT::SLASH);
        case '*': return CreateToken(TT::STAR);
        case '!': return CreateToken(TT::BANG);
        default: return CreateToken(TT::ERROR);
    }
}

void Lexer::SkipWhitespace() {
    while (true) {
        char c = Peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                Advance();
            break;
            case '\n':
                Advance();
                cur_line_++;
            break;
            case '/':
                if (PeekNext() == '/') {
                    while (Peek() != '\n' && !IsAtEnd()) Advance();
                } else {
                    return;
                }
            break;
            default:
                return;
        }
    }
}
