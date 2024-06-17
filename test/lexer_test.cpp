#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>
#include "lexer.h"
#include "debug.h"

// Ensure that the lexer only generates END token for empty source_code
BOOST_AUTO_TEST_CASE(LexerEmptySourceCode) {
    std::string source_code = "";
    Lexer lexer(source_code);

    const std::vector<TT> expected = { TT::END };
    const std::vector<Token> tokens = lexer.TokenizeAll();

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

    const std::vector<Token> tokens = lexer.TokenizeAll();

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

    const std::vector<Token> tokens = lexer.TokenizeAll();

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

    const std::vector<Token> tokens = lexer.TokenizeAll();

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

    const std::vector<Token> tokens = lexer.TokenizeAll();

    // Verify token properties
    BOOST_REQUIRE_EQUAL(tokens.size(), expectedTypes.size());
    for (size_t i = 0; i < expectedTypes.size(); ++i) {
        BOOST_CHECK_EQUAL(tokens[i].type, expectedTypes[i]);
        BOOST_CHECK_EQUAL(tokens[i].lexeme, expectedLexemes[i]);
    }
}



// Check that the lexer can read numbers correctly
BOOST_AUTO_TEST_CASE(LexerReadKeywords) {
    std::string source_code = "far \n for 45 while \n iff if else () while _and";
    Lexer lexer(source_code);

    const std::vector<TT> expectedTypes = {
        TT::IDENTIFIER, TT::IDENTIFIER, TT::NUMBER, TT::WHILE, TT::IDENTIFIER,
        TT::IF, TT::ELSE, TT::LEFT_PAREN, TT::RIGHT_PAREN, TT::WHILE, TT::IDENTIFIER, TT::END,
    };

    const std::vector<std::string> expectedLexemes = {
        "far", "for", "45", "while", "iff", "if", "else", "(", ")", "while", "_and", ""
    };

    const std::vector<Token> tokens = lexer.TokenizeAll();

    // Verify token properties
    BOOST_REQUIRE_EQUAL(tokens.size(), expectedTypes.size());
    for (size_t i = 0; i < expectedTypes.size(); ++i) {
        BOOST_CHECK_EQUAL(tokens[i].type, expectedTypes[i]);
        BOOST_CHECK_EQUAL(tokens[i].lexeme, expectedLexemes[i]);
    }
}




// Check if the lexer can handle a basic variable declaration statement
BOOST_AUTO_TEST_CASE(LexerVarDeclaration) {
    std::string source_code = "var x = 54;";
    Lexer lexer(source_code);

    const std::vector<TT> expected = {
        TT::VAR, TT::IDENTIFIER, TT::EQUAL, TT::NUMBER, TT::SEMICOLON, TT::END
    };

    const std::vector<Token> tokens = lexer.TokenizeAll();

    // Verify token properties
    BOOST_REQUIRE_EQUAL(tokens.size(), expected.size());
    for (size_t i = 0; i < expected.size(); ++i) {
        BOOST_CHECK_EQUAL(tokens[i].type, expected[i]);
        BOOST_CHECK_EQUAL(tokens[i].line, 0);
    }
}

// Check if the lexer can handle strings: regular, multiline and unterminated
BOOST_AUTO_TEST_CASE(LexerStrings) {
    std::string source_code = "\"this is a string\" while (true) { hello 23 } \" this is a \n multiline string \" \" this is an unterminated string";
    Lexer lexer(source_code);

    const std::vector<TT> expectedTypes = {
        TT::STRING, TT::WHILE, TT::LEFT_PAREN, TT::TRUE, TT::RIGHT_PAREN, TT::LEFT_BRACE,
        TT::IDENTIFIER, TT::NUMBER, TT::RIGHT_BRACE, TT::STRING, TT::ERROR, TT::END,
    };

    const std::vector<std::string> expectedLexemes = {
        "\"this is a string\"", "while", "(", "true", ")", "{", "hello", "23", "}",
        "\" this is a \n multiline string \"", "Unterminated String", ""
    };

    const std::vector<Token> tokens = lexer.TokenizeAll();

    // Verify token properties
    BOOST_REQUIRE_EQUAL(tokens.size(), expectedTypes.size());
    for (size_t i = 0; i < expectedTypes.size(); ++i) {
        BOOST_CHECK_EQUAL(tokens[i].type, expectedTypes[i]);
        BOOST_CHECK_EQUAL(tokens[i].lexeme, expectedLexemes[i]);
    }
}

// Check if the lexer can handle strings: regular, multiline and unterminated
BOOST_AUTO_TEST_CASE(LexerTwoCharTokens) {
    std::string source_code = ">= = => ! != == =";
    Lexer lexer(source_code);

    const std::vector<TT> expected = {
        TT::GREATER_EQUAL, TT::EQUAL, TT::EQUAL, TT::GREATER, TT::BANG, TT::BANG_EQUAL,
        TT::EQUAL_EQUAL, TT::EQUAL, TT::END,
    };

    const std::vector<Token> tokens = lexer.TokenizeAll();

    // Verify token properties
    BOOST_REQUIRE_EQUAL(tokens.size(), expected.size());
    for (size_t i = 0; i < expected.size(); ++i) {
        BOOST_CHECK_EQUAL(tokens[i].type, expected[i]);
    }
}

// Check if the lexer correctly reads a function statement
BOOST_AUTO_TEST_CASE(LexerFunStatement) {
    std::string source_code = R"str(
        fun add(var x, var y) {
            return x + y;
        }
    )str";
    Lexer lexer(source_code);

    const std::vector<TT> expected = {
        TT::FUN, TT::IDENTIFIER, TT::LEFT_PAREN, TT::VAR, TT::IDENTIFIER, TT::COMMA, TT::VAR,
        TT::IDENTIFIER, TT::RIGHT_PAREN, TT::LEFT_BRACE, TT::RETURN, TT::IDENTIFIER, TT::PLUS,
        TT::IDENTIFIER, TT::SEMICOLON, TT::RIGHT_BRACE, TT::END,
    };

    const std::vector<Token> tokens = lexer.TokenizeAll();

    // Verify token properties
    BOOST_REQUIRE_EQUAL(tokens.size(), expected.size());
    for (size_t i = 0; i < expected.size(); ++i) {
        BOOST_CHECK_EQUAL(tokens[i].type, expected[i]);
    }
}

// Check if the lexer can handle a basic variable declaration statement
BOOST_AUTO_TEST_CASE(LexerWhileStatement) {
    std::string source_code = R"str(
        while (true) {
            print("annoying message");
        }
    )str";
    Lexer lexer(source_code);

    const std::vector<TT> expected = {
        TT::WHILE, TT::LEFT_PAREN, TT::TRUE, TT::RIGHT_PAREN, TT::LEFT_BRACE, TT::PRINT, TT::LEFT_PAREN,
        TT::STRING, TT::RIGHT_PAREN, TT::SEMICOLON, TT::RIGHT_BRACE, TT::END,
    };

    const std::vector<Token> tokens = lexer.TokenizeAll();

    // Verify token properties
    BOOST_REQUIRE_EQUAL(tokens.size(), expected.size());
    for (size_t i = 0; i < expected.size(); ++i) {
        BOOST_CHECK_EQUAL(tokens[i].type, expected[i]);
    }
}


// Check if the lexer can handle a basic variable declaration statement
BOOST_AUTO_TEST_CASE(LexerIfElseStatement) {
    std::string source_code = R"str(
        if (age >= 18) {
            print("can drive");
        } else {
            print("too bad");
        }
    )str";
    Lexer lexer(source_code);

    const std::vector<TT> expected = {
        TT::IF, TT::LEFT_PAREN, TT::IDENTIFIER, TT::GREATER_EQUAL, TT::NUMBER, TT::RIGHT_PAREN, TT::LEFT_BRACE,
        TT::PRINT, TT::LEFT_PAREN, TT::STRING, TT::RIGHT_PAREN, TT::SEMICOLON, TT::RIGHT_BRACE, TT::ELSE, TT::LEFT_BRACE,
        TT::PRINT, TT::LEFT_PAREN, TT::STRING, TT::RIGHT_PAREN, TT::SEMICOLON, TT::RIGHT_BRACE, TT::END,
    };

    const std::vector<Token> tokens = lexer.TokenizeAll();

    // Verify token properties
    BOOST_REQUIRE_EQUAL(tokens.size(), expected.size());
    for (size_t i = 0; i < expected.size(); ++i) {
        BOOST_CHECK_EQUAL(tokens[i].type, expected[i]);
    }
}
