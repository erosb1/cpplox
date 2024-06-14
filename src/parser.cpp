#include "parser.h"

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
