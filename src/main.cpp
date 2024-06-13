#include <iostream>

#include "lexer.h"
#include "debug.h"

int main() {
    std::string src = "far boo \n far23 23 bar. yolo_is_100";

    Lexer lexer{std::string_view(src)};
    auto tokens = lexer.Tokenize();

    PrintTokens(tokens);
    return 0;
}