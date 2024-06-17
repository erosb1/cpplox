#include "parser.h"

#include <cassert>
#include <iostream>
#include <unordered_map>
#include <functional>

Parser::Parser(std::string_view source_code)
    : lexer_(source_code)
    , prev_token_(TT::NONE, "", -1)
    , cur_token_(TT::NONE, "", -1)
    , panic_mode_(false)
    , had_error_(false) {

    auto parseLiteral = std::bind(&Parser::ParseLiteral, this);
    auto parseIdentifier = std::bind(&Parser::ParseIdentifier, this);
    auto parseAssignment = std::bind(&Parser::ParseAssignment, this, std::placeholders::_1);
    auto parseBinary = std::bind(&Parser::ParseBinary, this, std::placeholders::_1);
    auto parseUnary = std::bind(&Parser::ParseUnary, this);
    auto parseGrouping = std::bind(&Parser::ParseGrouping, this);
    auto parseCall = std::bind(&Parser::ParseCall, this, std::placeholders::_1);

    pratt_table_[TT::TRUE]          = {parseLiteral, nullptr, Precedence::NONE};
    pratt_table_[TT::FALSE]         = {parseLiteral, nullptr, Precedence::NONE};
    pratt_table_[TT::NIL]           = {parseLiteral, nullptr, Precedence::NONE};
    pratt_table_[TT::STRING]        = {parseLiteral, nullptr, Precedence::NONE};
    pratt_table_[TT::NUMBER]        = {parseLiteral, nullptr, Precedence::NONE};
    pratt_table_[TT::IDENTIFIER]    = {parseIdentifier, nullptr, Precedence::NONE};
    pratt_table_[TT::EQUAL]         = {nullptr, parseAssignment, Precedence::ASSIGNMENT};
    pratt_table_[TT::OR]            = {nullptr, parseBinary, Precedence::OR};
    pratt_table_[TT::AND]           = {nullptr, parseBinary, Precedence::AND};
    pratt_table_[TT::EQUAL_EQUAL]   = {nullptr, parseBinary, Precedence::EQUALITY};
    pratt_table_[TT::BANG_EQUAL]    = {nullptr, parseBinary, Precedence::EQUALITY};
    pratt_table_[TT::GREATER]       = {nullptr, parseBinary, Precedence::COMPARISON};
    pratt_table_[TT::GREATER_EQUAL] = {nullptr, parseBinary, Precedence::COMPARISON};
    pratt_table_[TT::LESS]          = {nullptr, parseBinary, Precedence::COMPARISON};
    pratt_table_[TT::LESS_EQUAL]    = {nullptr, parseBinary, Precedence::COMPARISON};
    pratt_table_[TT::PLUS]          = {nullptr, parseBinary, Precedence::TERM};
    pratt_table_[TT::MINUS]         = {parseUnary, parseBinary, Precedence::TERM};
    pratt_table_[TT::STAR]          = {nullptr, parseBinary, Precedence::FACTOR};
    pratt_table_[TT::SLASH]         = {nullptr, parseBinary, Precedence::FACTOR};
    pratt_table_[TT::LEFT_PAREN]    = {parseGrouping, parseCall, Precedence::CALL};
}

ProgramPtr Parser::GenerateAST() {
    auto ast = std::make_unique<Program>();

    Advance();
    while (cur_token_.type != TokenType::END) {
        ast->declarations.push_back(std::move(ParseDeclaration()));
    }

    return std::move(ast);
}

ExpressionPtr Parser::ParseExpression() {
    Advance();
    return std::move(ParsePrecedence(Precedence::ASSIGNMENT));
}

void Parser::Advance() {
    prev_token_ = cur_token_;

    // Get next token and until TokenType != ERROR
    while (true) {
        cur_token_ = lexer_.ReadNextToken();
        if (cur_token_.type != TT::ERROR) break;
        ErrorAtCur(std::string(cur_token_.lexeme));
    }
}

void Parser::Consume(TT type, std::string_view error_msg) {
    if (Check(type)) {
        Advance();
        return;
    }
    ErrorAtCur(std::string(error_msg));
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

Parser::ParseRule Parser::GetRule(TokenType type) {
    assert(pratt_table_.contains(type));
    return pratt_table_.at(type);
}

DeclarationPtr Parser::ParseDeclaration() {
    if (Match(TT::VAR)) {
        return std::move(ParseVarDecl());
    } else {
        return std::move(ParseStatement());
    }
}

VarDeclPtr Parser::ParseVarDecl() {
    auto varDecl = std::make_unique<VarDecl>();
    Consume(TT::IDENTIFIER, "Expected identifier after 'var'");
    varDecl->variable = ParseIdentifier();
    if (Match(TT::EQUAL)) {
        varDecl-> expression = ParsePrecedence(Precedence::ASSIGNMENT);
    } else {
        varDecl->expression = nullptr;
    }
    Consume(TT::SEMICOLON, "Expected ; after variable declaration.");
    return std::move(varDecl);
}

IfStmtPtr Parser::ParseIfStmt() {
    auto if_stmt = std::make_unique<IfStmt>();

    // Parse condition
    Consume(TT::LEFT_PAREN, "Expected ( after if");
    if_stmt->condition = std::move(ParsePrecedence(Precedence::ASSIGNMENT));
    Consume(TT::RIGHT_PAREN, "Expected ) after if condition");

    // Parse body
    if_stmt->if_body = std::move(ParseStatement());

    // Parse else body
    if (Match(TT::ELSE)) {
        if_stmt->else_body = std::move(ParseStatement());
    }

    return std::move(if_stmt);
}

PrintStmtPtr Parser::ParsePrintStmt() {
    auto print_stmt = std::make_unique<PrintStmt>();
    print_stmt->expression = std::move(ParsePrecedence(Precedence::ASSIGNMENT));
    Consume(TT::SEMICOLON, "Expected ; after print expression");
    return std::move(print_stmt);
}

ReturnStmtPtr Parser::ParseReturnStmt() {
    auto return_stmt = std::make_unique<ReturnStmt>();
    if (!Check(TT::SEMICOLON)) {
        return_stmt->expression = std::move(ParsePrecedence(Precedence::ASSIGNMENT));
    }
    Consume(TT::SEMICOLON, "Expected ; after return expression");
    return std::move(return_stmt);
}

BlockPtr Parser::ParseBlock() {
    auto block = std::make_unique<Block>();
    while (!Check(TT::RIGHT_BRACE) && !Check(TT::END)) {
        block->declarations.push_back(std::move(ParseDeclaration()));
    }
    Consume(TT::RIGHT_BRACE, "Expected ending }");
    return std::move(block);
}

StatementPtr Parser::ParseStatement() {
    if (Match(TT::IF)) {
        return std::move(ParseIfStmt());
    } else if (Match(TT::PRINT)) {
        return std::move(ParsePrintStmt());
    } else if (Match(TT::RETURN)) {
        return std::move(ParseReturnStmt());
    } else if (Match(TT::LEFT_BRACE)) {
        return std::move(ParseBlock());
    } else {
        return std::move(ParseExprStmt());
    }
}

ExprStmtPtr Parser::ParseExprStmt() {
    auto expr_stmt = std::make_unique<ExprStmt>();
    expr_stmt->expression = std::move(ParsePrecedence(Precedence::ASSIGNMENT));
    Consume(TT::SEMICOLON, "Expected ; after expression.");
    return std::move(expr_stmt);
}

ExpressionPtr Parser::ParsePrecedence(Precedence precedence) {
    Advance();
    if (!pratt_table_.contains(prev_token_.type)) {
        ErrorAt(prev_token_, "Expect expression");
        return nullptr;
    }

    auto prefixFn = GetRule(prev_token_.type).prefix;

    if (prefixFn == nullptr) {
        ErrorAt(prev_token_, "Cannot start expression with: " + std::string(prev_token_.lexeme));
        return nullptr;
    }

    auto left = prefixFn();

    while (pratt_table_.contains(cur_token_.type) && precedence <= GetRule(cur_token_.type).precedence) {
        Advance();
        auto infixFn = GetRule(prev_token_.type).infix;
        left = infixFn(std::move(left));
    }

    return std::move(left);
}

AssignmentPtr Parser::ParseAssignment(ExpressionPtr left) {
    auto assign = std::make_unique<Assignment>();
    if (const auto* _identifier = dynamic_cast<const Identifier*>(left.get())) {
        assign->variable = std::unique_ptr<Identifier>(static_cast<Identifier*>(left.release()));
        assign->expression = std::move(ParsePrecedence(Precedence::ASSIGNMENT));
    } else {
        ErrorAtCur("Can only assign values to identifiers");
    }
    return std::move(assign);
}

BinaryPtr Parser::ParseBinary(ExpressionPtr left) {
    auto binary = std::make_unique<Binary>();
    binary->op = prev_token_.type;
    auto operand_precedence = static_cast<int>(GetRule(binary->op).precedence);
    binary->left_expression = std::move(left);
    binary->right_expression = std::move(ParsePrecedence(static_cast<Precedence>(operand_precedence + 1)));
    return std::move(binary);
}

UnaryPtr Parser::ParseUnary() {
    auto unary = std::make_unique<Unary>();
    unary->op = prev_token_.type;
    unary->expression = std::move(ParsePrecedence(Precedence::UNARY));
    return std::move(unary);
}

LiteralPtr Parser::ParseLiteral() {
    auto literal = std::make_unique<Literal>();
    switch (prev_token_.type) {
        case TT::TRUE:
            literal->value = true;
            break;
        case TT::FALSE:
            literal->value = false;
            break;
        case TT::NIL:
            literal->value = std::monostate{};
            break;
        case TT::STRING:
            literal->value = std::string(prev_token_.lexeme);
            break;
        case TT::NUMBER:
            literal->value = std::stod(std::string(prev_token_.lexeme));
            break;
        default:
            ErrorAt(prev_token_, "Invalid literal");
    }
    return std::move(literal);
}

ExpressionPtr Parser::ParseGrouping() {
    auto expression = std::move(ParsePrecedence(Precedence::ASSIGNMENT));
    Consume(TT::RIGHT_PAREN, "Expected ending ')' after expression");
    return std::move(expression);
}

CallPtr Parser::ParseCall(ExpressionPtr left) {

}

IdentifierPtr Parser::ParseIdentifier() {
    auto identifier = std::make_unique<Identifier>();
    identifier->name = prev_token_.lexeme;
    return std::move(identifier);
}

void Parser::ErrorAt(Token &token, std::string msg) {
    if (panic_mode_) return;
    panic_mode_ = true;
    had_error_ = true;
    std::string error_type = token.type == TT::ERROR ? "[LEXING ERROR]" : "[PARSING ERROR]";
    std::cerr << error_type << " at line: " << token.line << "\n\t" << msg << "\n";
}

void Parser::ErrorAtCur(std::string msg) {
    ErrorAt(cur_token_, msg);
}

void Parser::Synchronize() {
    //TODO implement
}
