#include <iostream>

#include "lexer.h"
#include "debug.h"

int main() {
    std::string src = "var x = 54;";

    Lexer lexer{std::string_view(src)};
    auto tokens = lexer.Tokenize();

    PrintTokens(tokens);
    return 0;
}