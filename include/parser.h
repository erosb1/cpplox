#ifndef PARSER_H
#define PARSER_H

#include <functional>

#include "ast.h"
#include "lexer.h"

class Parser {
public:
    explicit Parser(std::string_view source_code);
    ProgramPtr GenerateAST();
    ExpressionPtr ParseExpression();
private:
    // Pratt Parsing
    enum class Precedence {
        NONE,
        ASSIGNMENT,  // =
        OR,          // or
        AND,         // and
        EQUALITY,    // == !=
        COMPARISON,  // < > <= >=
        TERM,        // + -
        FACTOR,      // * /
        UNARY,       // ! -
        CALL,        // . ()
        PRIMARY
    };

    struct ParseRule {
        std::function<ExpressionPtr()> prefix;
        std::function<ExpressionPtr(ExpressionPtr)> infix;
        Precedence precedence;
    };
    std::unordered_map<TokenType, ParseRule> pratt_table_;

private:
    // Control functions
    void Advance(); // Reads tokens until TokenType != ERROR
    void Consume(TT type, std::string_view error_msg); // Checks if cur_token_ == type, advances and creates error if not
    [[nodiscard]] bool Check(TT type) const; // Checks if cur_token_ == type
    bool Match(TT type); // Checks if cur_token_ == type and advances

    // ParseRules
    ParseRule GetRule(TokenType type);

    // Statements
    DeclarationPtr ParseDeclaration();
    VarDeclPtr ParseVarDecl();
    IfStmtPtr ParseIfStmt();
    PrintStmtPtr ParsePrintStmt();
    ReturnStmtPtr ParseReturnStmt();
    WhileStmtPtr ParseWhileStmt();
    BlockPtr ParseBlock();
    StatementPtr ParseStatement();
    ExprStmtPtr ParseExprStmt();

    // Expressions
    ExpressionPtr ParsePrecedence(Precedence precedence);
    AssignmentPtr ParseAssignment(ExpressionPtr left);
    BinaryPtr ParseBinary(ExpressionPtr left);
    UnaryPtr ParseUnary();
    LiteralPtr ParseLiteral();
    ExpressionPtr ParseGrouping();
    CallPtr ParseCall(ExpressionPtr left);
    IdentifierPtr ParseIdentifier();

    // Error Handling
    void ErrorAt(Token& token, std::string msg);
    void ErrorAtCur(std::string msg);
    void Synchronize();
private:
    Lexer lexer_;
    Token prev_token_;
    Token cur_token_;
    bool panic_mode_;  // switches between true/false when encountering errors and synchronizing
    bool had_error_;   // remains true throughout entire parsing if had one error



};


#endif //PARSER_H
