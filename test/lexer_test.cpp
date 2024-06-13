#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>
#include "lexer.h"
#include "debug.h"

// Ensure that the lexer only generates END token for empty source_code
BOOST_AUTO_TEST_CASE(LexerEmptySourceCode) {
    std::string source_code = "";
    Lexer lexer(source_code);

    const std::vector<TT> expected = { TT::END };
    const std::vector<Token> tokens = lexer.Tokenize();

    // Verify token properties
    BOOST_REQUIRE_EQUAL(tokens.size(), 1);
    BOOST_CHECK_EQUAL(tokens[0].type, TT::END);
    BOOST_CHECK_EQUAL(tokens[0].line, 0);
}

// Ensure that the Lexer correctly scans single character tokens
BOOST_AUTO_TEST_CASE(LexerSingleCharTokens) {
    std::string source_code = "(){},.-+;/*";
    Lexer lexer(source_code);

    const std::vector<TT> expected = {
        TT::LEFT_PAREN, TT::RIGHT_PAREN, TT::LEFT_BRACE, TT::RIGHT_BRACE,
        TT::COMMA, TT::DOT, TT::MINUS, TT::PLUS, TT::SEMICOLON,
        TT::SLASH, TT::STAR, TT::END
    };

    const std::vector<Token> tokens = lexer.Tokenize();

    // Verify token properties
    BOOST_REQUIRE_EQUAL(tokens.size(), expected.size());
    for (size_t i = 0; i < expected.size(); ++i) {
        BOOST_CHECK_EQUAL(tokens[i].type, expected[i]);
    }
}

// Check that the lexer correctly ignores whitespace and comments
BOOST_AUTO_TEST_CASE(LexerIgnoreWhitespace) {
    std::string source_code = "  \n  +  \r\t -  //this is a comment\n / *";
    Lexer lexer(source_code);

    const std::vector<TT> expected = {
        TT::PLUS, TT::MINUS, TT::SLASH, TT::STAR, TT::END,
    };

    const std::vector<Token> tokens = lexer.Tokenize();

    // Verify token properties
    BOOST_REQUIRE_EQUAL(tokens.size(), expected.size());
    for (size_t i = 0; i < expected.size(); ++i) {
        BOOST_CHECK_EQUAL(tokens[i].type, expected[i]);
    }
}

// Check that the lexer can read numbers correctly
BOOST_AUTO_TEST_CASE(LexerReadNumbers) {
    std::string source_code = "56.433 4 54 34 . 45. 54.132 234234";
    Lexer lexer(source_code);

    const std::vector<TT> expectedTypes = {
        TT::NUMBER, TT::NUMBER, TT::NUMBER, TT::NUMBER,
        TT::DOT, TT::NUMBER, TT::DOT, TT::NUMBER, TT::NUMBER, TT::END,
    };

    const std::vector<std::string> expectedLexemes = {
        "56.433", "4", "54", "34", ".", "45",
        ".", "54.132", "234234", "",
    };

    const std::vector<Token> tokens = lexer.Tokenize();

    // Verify token properties
    BOOST_REQUIRE_EQUAL(tokens.size(), expectedTypes.size());
    for (size_t i = 0; i < expectedTypes.size(); ++i) {
        BOOST_CHECK_EQUAL(tokens[i].type, expectedTypes[i]);
        BOOST_CHECK_EQUAL(tokens[i].lexeme, expectedLexemes[i]);
        BOOST_CHECK_EQUAL(tokens[i].line, 0);
    }
}


// Check that the lexer can read numbers correctly
BOOST_AUTO_TEST_CASE(LexerReadIdentifiers) {
    std::string source_code = "far boo \n far23 23 bar. yolo_is_100";
    Lexer lexer(source_code);

    const std::vector<TT> expectedTypes = {
        TT::IDENTIFIER, TT::IDENTIFIER, TT::IDENTIFIER, TT::NUMBER, TT::IDENTIFIER,
        TT::DOT, TT::IDENTIFIER, TT::END,
    };

    const std::vector<std::string> expectedLexemes = {
        "far", "boo", "far23", "23", "bar", ".", "yolo_is_100", "",
    };

    const std::vector<Token> tokens = lexer.Tokenize();

    // Verify token properties
    BOOST_REQUIRE_EQUAL(tokens.size(), expectedTypes.size());
    for (size_t i = 0; i < expectedTypes.size(); ++i) {
        BOOST_CHECK_EQUAL(tokens[i].type, expectedTypes[i]);
        BOOST_CHECK_EQUAL(tokens[i].lexeme, expectedLexemes[i]);
    }
}




// Check if the lexer can handle a basic variable declaration statement
//BOOST_AUTO_TEST_CASE(LexerVarDeclaration) {
//    std::string source_code = "var x = 54;";
//    Lexer lexer(source_code);
//
//    const std::vector<TT> expected = {
//        TT::VAR, TT::IDENTIFIER, TT::EQUAL, TT::NUMBER, TT::SEMICOLON, TT::END
//    };
//
//    const std::vector<Token> tokens = lexer.Tokenize();
//
//    // Verify token properties
//    BOOST_REQUIRE_EQUAL(tokens.size(), expected.size());
//    for (size_t i = 0; i < expected.size(); ++i) {
//        BOOST_CHECK_EQUAL(tokens[i].type, expected[i]);
//        BOOST_CHECK_EQUAL(tokens[i].line, 0);
//    }
//}