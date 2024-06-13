#include "lexer.h"
#include <cassert>
#include <boost/test/unit_test_log.hpp>

Lexer::Lexer(std::string_view source_code)
    : source_code_(source_code)
    , cur_index_(0)
    , start_index_(0)
    , cur_line_(0)
{}

std::vector<Token> Lexer::Tokenize() {
    std::vector<Token> tokens;
    while (true) {
        SkipWhitespace();
        if (IsAtEnd()) {
            start_index_ = cur_index_;
            tokens.push_back(CreateToken(TT::END));
            break;
        }
        tokens.push_back(ReadNextToken());
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

template<std::size_t N>
Token Lexer::CreateErrorToken(const char(&msg)[N]) {
    static_assert(N > 0);
    return {TokenType::ERROR, std::string_view(msg, N - 1), cur_line_};
}

// Explicit instantiations for known messages
template Token Lexer::CreateErrorToken<17>(const char(&msg)[17]); // "Invalid Character"
template Token Lexer::CreateErrorToken<19>(const char(&msg)[19]); // "Unterminated String"

Token Lexer::ReadNextToken() {
    start_index_ = cur_index_;
    char c = Advance();

    if (std::isdigit(c)) return ReadNumber();
    if (std::isalpha(c) || c == '_') return ReadIdentifier();

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
        case '!': {
            if (Peek() == '=') {
                Advance();
                return CreateToken(TT::BANG_EQUAL);
            }
            return CreateToken(TT::BANG);
        }
        case '=': {
            if (Peek() == '=') {
                Advance();
                return CreateToken(TT::EQUAL_EQUAL);
            }
            return CreateToken(TT::EQUAL);
        }
        case '>': {
            if (Peek() == '=') {
                Advance();
                return CreateToken(TT::GREATER_EQUAL);
            }
            return CreateToken(TT::GREATER);
        }
        case '<': {
            if (Peek() == '=') {
                Advance();
                return CreateToken(TT::LESS_EQUAL);
            }
            return CreateToken(TT::LESS);
        }
        case '"': return ReadString();
        default: return CreateErrorToken("Invalid Character");
    }
}

Token Lexer::ReadNumber() {
    // read digits until it finds non-digit
    while (std::isdigit(Peek())) Advance();

    if (Peek() == '.' && std::isdigit(PeekNext())) {
        Advance(); // consume '.'
        while (std::isdigit(Peek())) Advance();
    }

    return CreateToken(TT::NUMBER);
}

Token Lexer::ReadString() {
    size_t ending_line = cur_line_; // only used for multiline strings
    while (!IsAtEnd() && Peek() != '"') {
        if (Peek() == '\n') ending_line++;
        Advance();
    }

    if (IsAtEnd()) { // If the string never terminated
        auto token = CreateErrorToken("Unterminated String");
        cur_line_ = ending_line;
        return token;
    }

    Advance(); // consume the ending "
    auto token = CreateToken(TT::STRING);
    cur_line_ = ending_line;
    return token;
}

Token Lexer::ReadIdentifier() {
    while (std::isalpha(Peek()) || std::isdigit(Peek()) || Peek() == '_') Advance();
    auto token = CreateToken(TT::IDENTIFIER);
    auto lexeme_str = std::string(token.lexeme);
    if (KEYWORDS.contains(lexeme_str)) {
        token.type = KEYWORDS.at(lexeme_str);
    }
    return token;
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
