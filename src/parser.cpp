#include "parser.h"
#include <iostream>

Parser::Parser(std::string_view source_code)
    : lexer_(source_code)
    , prev_token_(TT::ERROR, "", -1)
    , cur_token_(TT::ERROR, "", -1)
{}

ProgramPtr Parser::GenerateAST() {
    auto ast = std::make_unique<Program>();

    Advance();
    while (cur_token_.type != TokenType::ERROR) {
        ast->declarations.push_back(ParseDeclaration());
    }

    return std::move(ast);
}

void Parser::Advance() {
    prev_token_ = cur_token_;

    // Get next token and until TokenType != ERROR
    while (true) {
        cur_token_ = lexer_.ReadNextToken();
        if (cur_token_.type != TT::ERROR) break;
        // ErrorAtCur(token_.lexeme);
    }
}

void Parser::Consume(TT type, std::string_view error_msg) {
    if (Check(type)) {
        Advance();
        return;
    }
    ErrorAtCur(error_msg);
}

bool Parser::Check(TT type) const {
    return cur_token_.type == type;
}

bool Parser::Match(TT type) {
    if (Check(type)) {
        Advance();
        return true;
    }
    return false;
}

DeclarationPtr Parser::ParseDeclaration() {
    if (Match(TT::VAR)) {
        return std::move(ParseVarDecl());
    } else {
        //return ParseStatement();
    }
}

VarDeclPtr Parser::ParseVarDecl() {
    auto varDecl = std::make_unique<VarDecl>();
    varDecl->variable_name = ParseIdentifier("Expected a variable name");
    if (Match(TT::EQUAL)) {
        varDecl-> expression = ParseExpression();
    } else {
        varDecl->expression = nullptr;
    }
    Consume(TT::SEMICOLON, "Expected ; after variable declaration.");
    return std::move(varDecl);
}

StatementPtr Parser::ParseStatement() {
    if (Match(TT::IF)) {
        // return std::move(ParseIfStmt());
    } else {
        return std::move(ParseExprStmt());
    }
}

ExprStmtPtr Parser::ParseExprStmt() {
    auto expr_stmt = std::make_unique<ExprStmt>();
    expr_stmt->expression = ParseExpression();
    Consume(TT::SEMICOLON, "Expected ; after expression.");
    return std::move(expr_stmt);
}

ExpressionPtr Parser::ParseExpression() {
    // TODO Finish this function
    auto expression = std::make_unique<Expression>();
    return std::move(expression);
}

std::string_view Parser::ParseIdentifier(std::string_view error_msg) {
    Consume(TT::IDENTIFIER, error_msg);
    return prev_token_.lexeme;
}

void Parser::ErrorAt(Token &token, std::string_view msg) {
    if (panic_mode_) return;
    panic_mode_ = true;
    had_error_ = true;
    std::string error_type = token.type == TT::ERROR ? "[LEXING ERROR]" : "[PARSING ERROR]";
    std::cerr << error_type << " at line: " << token.line << "\n\t" << msg << "\n";
}

void Parser::ErrorAtCur(std::string_view msg) {
    ErrorAt(cur_token_, msg);
}

void Parser::Synchronize() {
    //TODO implement
}
