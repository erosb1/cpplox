#ifndef AST_H
#define AST_H

#include <vector>
#include <memory>
#include <string>

// Forward Declarations
class ASTNode;
class ASTVisitor;
class Program;
class Declaration;
class FunDecl;
class VarDecl;
class Expression;
class Function;

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
};

// Node Types
class Program : public ASTNode {
public:
    std::vector<std::unique_ptr<Declaration>> declarations;
    void accept(ASTVisitor &visitor) override;
};

class Declaration : public ASTNode {
    virtual ~Declaration() = default;
};

class FunDecl : public Declaration {
    std::unique_ptr<Function> function;
    void accept(ASTVisitor &visitor) override;
};

class VarDecl : public Declaration {
    std::string identifier;
    std::unique_ptr<Expression> expression;
    void accept(ASTVisitor &visitor) override;
};

inline void Program::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void FunDecl::accept(ASTVisitor &visitor) { visitor.visit(*this); }
inline void VarDecl::accept(ASTVisitor &visitor) { visitor.visit(*this); }

#endif //AST_H
