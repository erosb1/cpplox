#include "parser.h"
#include <iostream>

Parser::Parser(std::string_view source_code)
    : lexer_(source_code)
    , prev_token_(TT::ERROR, "", -1)
    , cur_token_(TT::ERROR, "", -1) {
    Advance(); Advance(); // Generate the first two tokens;
}

Program Parser::GenerateAST() {
    Program program;


    return program;
}

void Parser::Advance() {
    prev_token_ = cur_token_;

    // Get next token and until TokenType != ERROR
    while (true) {
        cur_token_ = lexer_.ReadNextToken();
        if (cur_token_.type != TT::ERROR) break;
        // ErrorAtCur(token_.lexeme);
    }
}

void Parser::ErrorAt(Token &token, std::string_view msg) {
    if (panic_mode_) return;
    panic_mode_ = true;
    had_error_ = true;
    std::string error_type = token.type == TT::ERROR ? "[LEXING ERROR]" : "[PARSING ERROR]";
    std::cerr << error_type << " at line: " << token.line << "\n\t" << msg << "\n";
}

void Parser::ErrorAtCur(std::string_view msg) {
    ErrorAt(cur_token_, msg);
}

void Parser::Synchronize() {
    //TODO implement
}
