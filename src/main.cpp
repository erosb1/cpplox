#include <iostream>

#include "lexer.h"
#include "debug.h"

int main() {
    std::string src = "56.433 4 54 34 . 45. 54.132 234234";

    Lexer lexer{std::string_view(src)};
    auto tokens = lexer.Tokenize();

    PrintTokens(tokens);
    return 0;
}