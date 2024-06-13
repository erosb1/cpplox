#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

#include "lexer.h"

void PrintTokens(const std::vector<Token>& tokens);

// Used for printing TokenTypes in unit tests
std::ostream& operator<<(std::ostream& os, const TokenType& type);


#endif //DEBUG_H
