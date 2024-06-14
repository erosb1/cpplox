#ifndef PARSER_H
#define PARSER_H

#include <functional>

#include "ast.h"
#include "lexer.h"

class Parser {
public:
    explicit Parser(std::string_view source_code);
    ProgramPtr GenerateAST();
private:
    // Control functions
    void Advance(); // Reads tokens until TokenType != ERROR
    void Consume(TT type, std::string_view error_msg); // Checks if cur_token_ == type, advances and creates error if not
    [[nodiscard]] bool Check(TT type) const; // Checks if cur_token_ == type
    bool Match(TT type); // Checks if cur_token_ == type and advances

    // Parsing Specific NodeTypes
    DeclarationPtr ParseDeclaration();
    VarDeclPtr ParseVarDecl();
    StatementPtr ParseStatement();
    ExprStmtPtr ParseExprStmt();
    ExpressionPtr ParseExpression();
    AssignmentPtr ParseAssignment();
    BinaryPtr ParseBinary();
    UnaryPtr ParseUnary();
    LiteralPtr ParseLiteral();
    LiteralPtr ParseString();
    GroupingPtr ParseGrouping();
    CallPtr ParseCall();

    // Parse smaller stuff
    std::string_view ParseIdentifier(std::string_view error_msg);

    // Error Handling
    void ErrorAt(Token& token, std::string_view msg);
    void ErrorAtCur(std::string_view msg);
    void Synchronize();
private:
    Lexer lexer_;
    Token prev_token_;
    Token cur_token_;
    bool panic_mode_;  // switches between true/false when encountering errors and synchronizing
    bool had_error_;   // remains true throughout entire parsing if had one error


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

    using ParseFn = std::function<ExpressionPtr(Parser&)>;
    struct ParseRule {
        ParseFn prefix;
        ParseFn infix;
        Precedence precedence;
    };
    std::unordered_map<TokenType, ParseRule> pratt_table_;
};


#endif //PARSER_H
