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
class Expression;
class Function;
class Identifier;

using ProgramPtr = std::unique_ptr<Program>;
using DeclarationPtr = std::unique_ptr<Declaration>;
using FunDeclPtr = std::unique_ptr<FunDecl>;
using VarDeclPtr = std::unique_ptr<VarDecl>;
using ExpressionPtr = std::unique_ptr<Expression>;
using FunctionPtr = std::unique_ptr<Function>;
using IdentifierPtr = std::unique_ptr<Identifier>;

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
    virtual void visit(Expression &node) = 0;
    virtual void visit(Function &node) = 0;
    virtual void visit(Identifier &node) = 0;
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
    IdentifierPtr identifier;
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

class Identifier : public Expression {
public:
    std::string name;
    explicit Identifier(std::string name) : name(std::move(name)) {}
    void accept(ASTVisitor &visitor) override;
};

inline void Program::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void FunDecl::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void VarDecl::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void Expression::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void Function::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void Identifier::accept(ASTVisitor &visitor) { visitor.visit(*this); }

#endif //AST_H
