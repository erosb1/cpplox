#include <iostream>

#include "lexer.h"
#include "debug.h"

// var x =
// 0
// ;

int main() {
    std::string src = "var x =\n0\n;";

    auto sv = [&src](int s, int e){ return std::string_view(src.begin() + s, src.begin() + e); };

    std::vector<Token> tokens = {
        {TT::VAR, sv(0, 3), 0},
        {TT::IDENTIFIER, sv(4, 5), 0},
        {TT::EQUAL, sv(6, 7), 0},
        {TT::NUMBER, sv(8, 9), 1},
        {TT::SEMICOLON, sv(10, 11), 2},
    };

    PrintTokens(tokens);
    return 0;
}