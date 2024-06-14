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
private:
    Lexer lexer_;
    Token prev_token_;
    Token cur_token_;
};


#endif //PARSER_H
