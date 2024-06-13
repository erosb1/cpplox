#ifndef LEXER_H
#define LEXER_H

#include <string_view>
#include <vector>

/**
 *  TOKENS
 */
enum class TokenType {
    // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN,
    LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS,
    SEMICOLON, SLASH, STAR,
    // One or two character tokens.
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
    // Literals.
    IDENTIFIER, STRING, NUMBER,
    // Keywords.
    AND, ELSE, FALSE,
    FOR, FUN, IF, NIL, OR,
    PRINT, RETURN,
    TRUE, VAR, WHILE,
    ERROR,
    END,
};

using TT = TokenType;

struct Token {
    TokenType type;
    std::string_view lexeme;
    size_t line;

    Token(TokenType type, std::string_view lexeme, size_t line)
        : type(type), lexeme(lexeme), line(line) {}
};

class Lexer {
public:
    explicit Lexer(std::string_view source_code);
    std::vector<Token> Tokenize();
private:
    // Lexer Control Functions
    char Advance();
    bool Match(char expected);
    [[nodiscard]] char Peek() const;
    [[nodiscard]] char PeekNext() const;
    [[nodiscard]] bool IsAtEnd() const;

    // More Specific Functions
    Token CreateToken(TT type);
    Token ScanNext();
    Token ReadNumber();
    void SkipWhitespace();
private:
    std::string_view source_code_;
    size_t cur_index_;
    size_t start_index_;
    size_t cur_line_;
};

#endif //LEXER_H
