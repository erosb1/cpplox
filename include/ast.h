#ifndef AST_H
#define AST_H

#include <utility>
#include <vector>
#include <memory>
#include <string>

#include "ast.h"
#include "ast.h"
#include "ast.h"

class ASTNode;
class ASTVisitor;
class Program;
class Declaration;
class FunDecl;
class VarDecl;
class Statement;
class ExprStmt;
class Expression;
class Function;

using ASTNodePtr = std::unique_ptr<ASTNode>;
using ProgramPtr = std::unique_ptr<Program>;
using DeclarationPtr = std::unique_ptr<Declaration>; // Abstract class
using FunDeclPtr = std::unique_ptr<FunDecl>;
using VarDeclPtr = std::unique_ptr<VarDecl>;
using StatementPtr = std::unique_ptr<Statement>; // Abstract class
using ExprStmtPtr = std::unique_ptr<ExprStmt>;
using ExpressionPtr = std::unique_ptr<Expression>;
using FunctionPtr = std::unique_ptr<Function>;

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
    virtual void visit(Expression &node) = 0;
    virtual void visit(Function &node) = 0;
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
    FunctionPtr function;
    ~FunDecl() override = default;
    void accept(ASTVisitor &visitor) override;
};

class VarDecl : public Declaration {
public:
    std::string variable_name;
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

class Expression : public ASTNode {
public:
    void accept(ASTVisitor &visitor) override;
};

class Function : public ASTNode {
public:
    void accept(ASTVisitor &visitor) override;
};

inline void Program::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void FunDecl::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void VarDecl::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void ExprStmt::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void Expression::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void Function::accept(ASTVisitor &visitor) { visitor.visit(*this); }

#endif //AST_H
