#ifndef AST_H
#define AST_H

#include <utility>
#include <vector>
#include <memory>
#include <string>
#include <variant>

#include "ast.h"
#include "lexer.h"

class ASTNode;
class ASTVisitor;
class Program;
class Declaration;
class FunDecl;
class VarDecl;
class Statement;
class ExprStmt;
class IfStmt;
class PrintStmt;
class ReturnStmt;
class WhileStmt;
class Block;
class Expression;
class Assignment;
class Binary;
class Unary;
class Call;
class Identifier;
class Literal;
class Parameters;
class Arguments;

using ASTNodePtr = std::unique_ptr<ASTNode>;
using ProgramPtr = std::unique_ptr<Program>;
using DeclarationPtr = std::unique_ptr<Declaration>; // Abstract class
using FunDeclPtr = std::unique_ptr<FunDecl>;
using VarDeclPtr = std::unique_ptr<VarDecl>;
using StatementPtr = std::unique_ptr<Statement>; // Abstract class
using ExprStmtPtr = std::unique_ptr<ExprStmt>;
using IfStmtPtr = std::unique_ptr<IfStmt>;
using PrintStmtPtr = std::unique_ptr<PrintStmt>;
using ReturnStmtPtr = std::unique_ptr<ReturnStmt>;
using WhileStmtPtr = std::unique_ptr<WhileStmt>;
using BlockPtr = std::unique_ptr<Block>;
using ExpressionPtr = std::unique_ptr<Expression>; // Abstract class
using AssignmentPtr = std::unique_ptr<Assignment>;
using BinaryPtr = std::unique_ptr<Binary>;
using UnaryPtr = std::unique_ptr<Unary>;
using CallPtr = std::unique_ptr<Call>;
using IdentifierPtr = std::unique_ptr<Identifier>;
using LiteralPtr = std::unique_ptr<Literal>;
using ParametersPtr = std::unique_ptr<Parameters>;
using ArgumentsPtr = std::unique_ptr<Arguments>;

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void accept(ASTVisitor &visitor) = 0;
};

class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    virtual void visit(Program &node) = 0;
    virtual void visit(FunDecl &node) = 0;
    virtual void visit(VarDecl &node) = 0;
    virtual void visit(ExprStmt &node) = 0;
    virtual void visit(IfStmt &node) = 0;
    virtual void visit(PrintStmt &node) = 0;
    virtual void visit(ReturnStmt &node) = 0;
    virtual void visit(WhileStmt &node) = 0;
    virtual void visit(Block &node) = 0;
    virtual void visit(Assignment &node) = 0;
    virtual void visit(Binary &node) = 0;
    virtual void visit(Unary &node) = 0;
    virtual void visit(Call &node) = 0;
    virtual void visit(Identifier &node) = 0;
    virtual void visit(Literal &node) = 0;
    virtual void visit(Parameters &node) = 0;
    virtual void visit(Arguments &node) = 0;
};

class Program : public ASTNode {
public:
    std::vector<DeclarationPtr> declarations;
    void accept(ASTVisitor &visitor) override;
};

class Declaration : public ASTNode {
public:
    virtual ~Declaration() = default;
};

class FunDecl : public Declaration {
public:
    IdentifierPtr name;
    ParametersPtr parameters;
    BlockPtr body;
    void accept(ASTVisitor &visitor) override;
};

class VarDecl : public Declaration {
public:
    IdentifierPtr variable;
    ExpressionPtr expression;
    void accept(ASTVisitor &visitor) override;
};

class Statement : public Declaration {
public:
    virtual ~Statement() = default;
};

class ExprStmt : public Statement {
public:
    ExpressionPtr expression;
    void accept(ASTVisitor &visitor) override;
};

class IfStmt : public Statement {
public:
    ExpressionPtr condition;
    StatementPtr if_body;
    StatementPtr else_body;
    void accept(ASTVisitor &visitor) override;
};

class PrintStmt : public Statement {
public:
    ExpressionPtr expression;
    void accept(ASTVisitor &visitor) override;
};

class ReturnStmt : public Statement {
public:
    ExpressionPtr expression;
    void accept(ASTVisitor &visitor) override;
};

class WhileStmt : public Statement {
public:
    ExpressionPtr condition;
    StatementPtr body;
    void accept(ASTVisitor &visitor) override;
};

class Block : public Statement {
public:
    std::vector<DeclarationPtr> declarations;
    void accept(ASTVisitor &visitor) override;
};

class Expression : public ASTNode {
public:
    virtual ~Expression() = default;
};

class Assignment : public Expression {
public:
    IdentifierPtr variable;
    ExpressionPtr expression;
    void accept(ASTVisitor &visitor) override;
};

class Binary : public Expression {
public:
    TokenType op{};
    ExpressionPtr left_expression;
    ExpressionPtr right_expression;
    void accept(ASTVisitor &visitor) override;
};

class Unary : public Expression {
public:
    TokenType op{};
    ExpressionPtr expression;
    void accept(ASTVisitor &visitor) override;
};

class Call : public Expression {
public:
    IdentifierPtr callee;
    ArgumentsPtr arguments;
    void accept(ASTVisitor &visitor) override;
};

class Identifier : public Expression {
public:
    std::string_view name;
    void accept(ASTVisitor &visitor) override;
};

using Value = std::variant<double, bool, std::string, std::monostate>;
class Literal : public Expression {
public:
    Value value{};
    void accept(ASTVisitor &visitor) override;
};

class Parameters : public ASTNode {
public:
    std::vector<IdentifierPtr> identifiers;
    void accept(ASTVisitor &visitor) override;
};

class Arguments : public ASTNode {
public:
    std::vector<ExpressionPtr> expressions;
    void accept(ASTVisitor &visitor) override;
};

inline void Program::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void FunDecl::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void VarDecl::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void ExprStmt::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void IfStmt::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void PrintStmt::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void ReturnStmt::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void WhileStmt::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void Block::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void Assignment::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void Binary::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void Unary::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void Call::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void Identifier::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void Literal::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void Parameters::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void Arguments::accept(ASTVisitor &visitor) { visitor.visit(*this); }

#endif //AST_H
