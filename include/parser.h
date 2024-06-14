#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "lexer.h"

class Parser {
public:
    explicit Parser(std::string_view source_code);
    Program GenerateAST();
private:
    void Advance();

    // Error Handling
    void ErrorAt(Token& token, std::string_view msg);
    void ErrorAtCur(std::string_view msg);
    void Synchronize();
private:
    Lexer lexer_;
    Token prev_token_;
    Token cur_token_;
    bool panic_mode_;  // switches between true/false when encountering errors and synchronizing
    bool had_error_;   // remains true throughout entire parsing if had one error
};


#endif //PARSER_H
