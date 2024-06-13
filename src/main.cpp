#include <iostream>

#include "lexer.h"
#include "debug.h"

int main() {
    std::string src = R"str(
        while (true) {
            print("annoying message")
        }
    )str";

    Lexer lexer{std::string_view(src)};
    auto tokens = lexer.Tokenize();

    PrintTokens(tokens);
    return 0;
}