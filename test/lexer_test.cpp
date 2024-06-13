#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>
#include "lexer.h"

bool CheckCorrectTokenTypes(std::vector<Token> source, std::vector<TT> expected) {
    if (source.size() != expected.size()) return false;

    for (int i = 0; i < source.size(); i++) {
        if (source[i].type != expected[i]) return false;
    }

    return true;
}

// Ensure that the lexer only generates END token for empty source_code
BOOST_AUTO_TEST_CASE(LexerEmptySourceCode) {
    std::string source_code = "";
    Lexer lexer(source_code);

    const std::vector<TT> expected = {
        TT::END
    };

    const std::vector<Token> tokens = lexer.Tokenize();

    bool is_equal = CheckCorrectTokenTypes(tokens, expected);

    // Verify token properties
    BOOST_REQUIRE_EQUAL(tokens.size(), 1);
    BOOST_REQUIRE_EQUAL(tokens[0].line, 0);
    BOOST_REQUIRE(is_equal);
}

BOOST_AUTO_TEST_CASE(LexerEmptyVarDeclaration) {
    std::string source_code = "var x = 54;";
    Lexer lexer(source_code);
    const std::vector<TT> expected = {
        TT::VAR,
        TT::IDENTIFIER,
        TT::EQUAL,
        TT::NUMBER,
        TT::SEMICOLON,
        TT::END
    };

    const std::vector<Token> tokens = lexer.Tokenize();
    bool is_equal = CheckCorrectTokenTypes(tokens, expected);

    // Verify token properties
    BOOST_REQUIRE_EQUAL(tokens.size(), 6);
    BOOST_REQUIRE(is_equal);
}
