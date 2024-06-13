#include <iostream>

#include "lexer.h"
#include "debug.h"

int main() {
    std::string src = "\"this is a string\" while (true) { hello 23 } \" this is a \n multiline string \" \" this is an unterminated string";

    Lexer lexer{std::string_view(src)};
    auto tokens = lexer.Tokenize();

    PrintTokens(tokens);
    return 0;
}