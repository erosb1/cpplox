#ifndef LEXER_H
#define LEXER_H

#include <string_view>
#include <vector>
#include <unordered_map>


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
    ERROR, NONE,
    END,
};

using TT = TokenType;

/*
 *  KEYWORDS
 */
const std::unordered_map<std::string, TokenType> KEYWORDS = {
    {"fun",    TT::FUN},
    {"or",     TT::OR},
    {"and",    TT::AND},
    {"while",  TT::WHILE},
    {"if",     TT::IF},
    {"return", TT::RETURN},
    {"var",    TT::VAR},
    {"print",  TT::PRINT},
    {"true",   TT::TRUE},
    {"false",  TT::FALSE},
    {"nil",    TT::NIL},
    {"else",   TT::ELSE},
};

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
    std::vector<Token> TokenizeAll(); // Tokenizes entire source file, only used for testing and debugging
    Token ReadNextToken();
private:
    // Lexer Control Functions
    char Advance();
    bool Match(char expected);
    [[nodiscard]] char Peek() const;
    [[nodiscard]] char PeekNext() const;
    [[nodiscard]] bool IsAtEnd() const;

    // Create Tokens
    Token CreateToken(TT type);
    template <std::size_t N> // Is templated to only allow string literals as arguments (since its unclear who owns the msg)
    Token CreateErrorToken(const char(&msg)[N]);

    // Read Source Code
    Token ReadNumber();
    Token ReadString();
    Token ReadIdentifier(); // Also used for reading keywords
    void SkipWhitespace();
private:
    std::string_view source_code_;
    size_t cur_index_;
    size_t start_index_;
    size_t cur_line_;
};

#endif //LEXER_H
