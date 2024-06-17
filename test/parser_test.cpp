#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>
#include "parser.h"
#include "debug.h"

std::string ParseExpression(std::string input) {
    Parser parser(input);
    auto expression = parser.ParseExpression();
    return Debug::GetExpressionStr(expression.get());
}

BOOST_AUTO_TEST_CASE(ParserExpression1) {
    std::string input = "a + b * c";
    std::string expected = "a + (b * c)";

    std::string result = ParseExpression(input);
    BOOST_REQUIRE_EQUAL(expected, result);
}

BOOST_AUTO_TEST_CASE(ParserExpression2) {
    std::string input = "a + b > c";
    std::string expected = "(a + b) > c";

    std::string result = ParseExpression(input);
    BOOST_REQUIRE_EQUAL(expected, result);
}

BOOST_AUTO_TEST_CASE(ParserExpression3) {
    std::string input = "a > b and c < d";
    std::string expected = "(a > b) and (c < d)";

    std::string result = ParseExpression(input);
    BOOST_REQUIRE_EQUAL(expected, result);
}

BOOST_AUTO_TEST_CASE(ParserExpression4) {
    std::string input = "a >= b == c <= d";
    std::string expected = "(a >= b) == (c <= d)";

    std::string result = ParseExpression(input);
    BOOST_REQUIRE_EQUAL(expected, result);
}

BOOST_AUTO_TEST_CASE(ParserExpression5) {
    std::string input = "a and b or c";
    std::string expected = "(a and b) or c";

    std::string result = ParseExpression(input);
    BOOST_REQUIRE_EQUAL(expected, result);
}

BOOST_AUTO_TEST_CASE(ParserExpression6) {
    std::string input = "-a * b";
    std::string expected = "(-a) * b";

    std::string result = ParseExpression(input);
    BOOST_REQUIRE_EQUAL(expected, result);
}

BOOST_AUTO_TEST_CASE(ParserExpression7) {
    std::string input = "a + b * c > d and e != f";
    std::string expected = "((a + (b * c)) > d) and (e != f)";

    std::string result = ParseExpression(input);
    BOOST_REQUIRE_EQUAL(expected, result);
}

BOOST_AUTO_TEST_CASE(ParserExpression8) {
    std::string input = "a = b + c * d";
    std::string expected = "a = b + (c * d)";

    std::string result = ParseExpression(input);
    BOOST_REQUIRE_EQUAL(expected, result);
}

BOOST_AUTO_TEST_CASE(ParserExpression9) {
    std::string input = "a * (b + c) / d";
    std::string expected = "(a * (b + c)) / d";

    std::string result = ParseExpression(input);
    BOOST_REQUIRE_EQUAL(expected, result);
}

BOOST_AUTO_TEST_CASE(ParserExpression10) {
    std::string input = "a + b < c or d >= e";
    std::string expected = "((a + b) < c) or (d >= e)";

    std::string result = ParseExpression(input);
    BOOST_REQUIRE_EQUAL(expected, result);
}

BOOST_AUTO_TEST_CASE(ParserExpression11) {
    std::string input = "!a + b > -c";
    std::string expected = "(!a + b) > (-c)";

    std::string result = ParseExpression(input);
    BOOST_REQUIRE_EQUAL(expected, result);
}

BOOST_AUTO_TEST_CASE(ParserExpression12) {
    std::string input = "a + b * c - d / e and f == g or h";
    std::string expected = "(((a + (b * c)) - (d / e)) and (f == g)) or h";

    std::string result = ParseExpression(input);
    BOOST_REQUIRE_EQUAL(expected, result);
}
