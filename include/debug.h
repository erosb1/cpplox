#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <sstream>

#include "lexer.h"
#include "ast.h"

namespace Debug {
    class ASTStringVisitor : public ASTVisitor {
    public:
        ASTStringVisitor();
        [[nodiscard]] std::string GetString() const;
        void visit(Program &node) override;
        void visit(FunDecl &node) override;
        void visit(VarDecl &node) override;
        void visit(ExprStmt &node) override;
        void visit(IfStmt &node) override;
        void visit(PrintStmt &node) override;
        void visit(ReturnStmt &node) override;
        void visit(WhileStmt &node) override;
        void visit(Block &node) override;
        void visit(Assignment &node) override;
        void visit(Binary &node) override;
        void visit(Unary &node) override;
        void visit(Call &node) override;
        void visit(Identifier &node) override;
        void visit(Literal &node) override;
        void visit(Parameters &node) override;
        void visit(Arguments &node) override;
    private:
        void GrowIndent();
        void ShrinkIndent();
        static constexpr int indent_size_ = 4;
        std::string cur_indent_{};
        std::ostringstream oss_{};
    };

    void PrintTokens(const std::vector<Token>& tokens);
    std::string GetASTString(ASTNode* head);
    std::string GetExpressionStr(const Expression* expression);
}

// Used for printing TokenTypes in unit tests
std::ostream& operator<<(std::ostream& os, const TokenType& type);


#endif //DEBUG_H
