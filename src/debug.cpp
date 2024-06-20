#include "debug.h"
#include <iomanip>

constexpr size_t AST_INDENT_SPACING = 4;

static std::string GetTokenString(TokenType type) {
    switch (type) {
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::COMMA: return "COMMA";
        case TokenType::DOT: return "DOT";
        case TokenType::MINUS: return "MINUS";
        case TokenType::PLUS: return "PLUS";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::SLASH: return "SLASH";
        case TokenType::STAR: return "STAR";
        case TokenType::BANG: return "BANG";
        case TokenType::BANG_EQUAL: return "BANG_EQUAL";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::EQUAL_EQUAL: return "EQUAL_EQUAL";
        case TokenType::GREATER: return "GREATER";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case TokenType::LESS: return "LESS";
        case TokenType::LESS_EQUAL: return "LESS_EQUAL";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::STRING: return "STRING";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::AND: return "AND";
        case TokenType::ELSE: return "ELSE";
        case TokenType::FALSE: return "FALSE";
        case TokenType::FOR: return "FOR";
        case TokenType::FUN: return "FUN";
        case TokenType::IF: return "IF";
        case TokenType::NIL: return "NIL";
        case TokenType::OR: return "OR";
        case TokenType::PRINT: return "PRINT";
        case TokenType::RETURN: return "RETURN";
        case TokenType::TRUE: return "TRUE";
        case TokenType::VAR: return "VAR";
        case TokenType::WHILE: return "WHILE";
        case TokenType::ERROR: return "ERROR";
        case TokenType::END: return "END";
        default: return "unknown";
    }
}

static std::string VariantToString(const Value& var) {
    return std::visit([&](auto&& arg) -> std::string {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::string_view>) {
            return std::string(arg);
        } else if constexpr (std::is_same_v<T, double>) {
            return std::to_string(arg);
        } else if constexpr (std::is_same_v<T, bool>) {
            if (std::get<bool>(var) == true) return "true";
            return "false";
        } else return "nil";
    }, var);
}

std::ostream& operator<<(std::ostream& os, const TokenType& type) {
    os << GetTokenString(type);
    return os;
}

static void PrintToken(const Token& token, size_t last_line_num) {
    std::string line_num_str = token.line == last_line_num ? "| " : std::to_string(token.line) + " ";
    std::string token_type_str = GetTokenString(token.type);
    std::cout << std::right << std::setw(6)  << line_num_str << std::left << "     " << std::setw(14)
        << token_type_str << "    " << std::setw(8) << token.lexeme << '\n';
}

Debug::ASTStringVisitor::ASTStringVisitor()
    : cur_indent_("") {}

std::string Debug::ASTStringVisitor::GetString() const {
    return oss_.str();
}

void Debug::ASTStringVisitor::visit(Program &node) {
    GrowIndent();
    oss_ << "Program <list> [\n";
    for (auto& declaration : node.declarations) {
        oss_ << cur_indent_;
        declaration->accept(*this);
    }
    ShrinkIndent();
    oss_ << cur_indent_ << "],\n";
}

void Debug::ASTStringVisitor::visit(FunDecl &node) {
    GrowIndent();
    oss_ << "FunDecl {\n"
         << cur_indent_ << "name: ";
    node.name->accept(*this);
    if (node.parameters != nullptr) {
        oss_ << cur_indent_ << "parameters: ";
        node.parameters->accept(*this);
    } else {
        oss_ << cur_indent_ << "parameters: { none },\n";
    }
    oss_ << cur_indent_ << "body: ";
    node.body->accept(*this);
    ShrinkIndent();
    oss_ << cur_indent_ << "},\n";
}

void Debug::ASTStringVisitor::visit(VarDecl &node) {
    GrowIndent();
    oss_ << "VarDecl {\n"
         << cur_indent_ << "variable: ";
    node.variable->accept(*this);
    if (node.expression != nullptr) {
        oss_ << cur_indent_ << "expression: ";
        node.expression->accept(*this);
    } else {
        oss_ << cur_indent_ << "expression: { none },\n";
    }
    ShrinkIndent();
    oss_ << cur_indent_ << "},\n";
}

void Debug::ASTStringVisitor::visit(ExprStmt &node) {
    GrowIndent();
    oss_ << "ExprStmt {\n";
    oss_ << cur_indent_ << "expression: ";
    node.expression->accept(*this);
    ShrinkIndent();
    oss_ << cur_indent_ << "},\n";
}

void Debug::ASTStringVisitor::visit(IfStmt &node) {
    GrowIndent();
    oss_ << "IfStmt {\n"
         << cur_indent_ << "condition: ";
    node.condition->accept(*this);
    oss_ << cur_indent_ << "if-body: ";
    node.if_body->accept(*this);
    if (node.else_body != nullptr) {
        oss_ << cur_indent_ << "else-body: ";
        node.else_body->accept(*this);
    } else {
        oss_ << cur_indent_ << "else-body: { none },\n";
    }
    ShrinkIndent();
    oss_ << cur_indent_ << "},\n";
}

void Debug::ASTStringVisitor::visit(PrintStmt &node) {
    GrowIndent();
    oss_ << "PrintStmt {\n"
         << cur_indent_ << "expression: ";
    node.expression->accept(*this);
    ShrinkIndent();
    oss_ << cur_indent_ << "},\n";
}

void Debug::ASTStringVisitor::visit(ReturnStmt &node) {
    GrowIndent();
    oss_ << "ReturnStmt {\n";
    if (node.expression != nullptr) {
        oss_ << cur_indent_ << "expression: ";
        node.expression->accept(*this);
    } else {
        oss_ << cur_indent_ << "expression: { none },\n";
    }
    ShrinkIndent();
    oss_ << cur_indent_ << "},\n";
}

void Debug::ASTStringVisitor::visit(WhileStmt &node) {
    GrowIndent();
    oss_ << "WhileStmt {\n"
         << cur_indent_ << "condition: ";
    node.condition->accept(*this);
    oss_ << cur_indent_ << "body: ";
    node.body->accept(*this);
    ShrinkIndent();
    oss_ << cur_indent_ << "},\n";
}

void Debug::ASTStringVisitor::visit(Block &node) {
    GrowIndent();
    oss_ << "Block <list> [\n";
    for (auto& declaration : node.declarations) {
        oss_ << cur_indent_;
        declaration->accept(*this);
    }
    ShrinkIndent();
    oss_ << cur_indent_ << "],\n";
}

void Debug::ASTStringVisitor::visit(Assignment &node) {
    GrowIndent();
    oss_ << "Assignment {\n"
         << cur_indent_ << "variable: ";
    node.variable->accept(*this);
    oss_ << cur_indent_ << "expression: ";
    node.expression->accept(*this);
    ShrinkIndent();
    oss_ << cur_indent_ << "},\n";
}

void Debug::ASTStringVisitor::visit(Binary &node) {
    GrowIndent();
    oss_ << "Binary {\n"
         << cur_indent_ << "op: " << GetTokenString(node.op) << ",\n"
         << cur_indent_ << "left_expression: ";
    node.left_expression->accept(*this);
    oss_ << cur_indent_ << "right_expression: ";
    node.right_expression->accept(*this);
    ShrinkIndent();
    oss_ << cur_indent_ << "},\n";
}

void Debug::ASTStringVisitor::visit(Unary &node) {
    GrowIndent();
    oss_ << "Unary {\n"
         << cur_indent_ << "op: " << GetTokenString(node.op) << ",\n"
         << cur_indent_ << "expression: ";
    node.expression->accept(*this);
    ShrinkIndent();
    oss_ << cur_indent_ << "},\n";
}

void Debug::ASTStringVisitor::visit(Call &node) {
    GrowIndent();
    oss_ << "Call {\n";
    oss_ << cur_indent_<< "callee: ";
    node.callee->accept(*this);
    if (node.arguments != nullptr) {
        oss_ << cur_indent_ << "arguments: ";
        node.arguments->accept(*this);
    } else {
        oss_ << cur_indent_ << "arguments: { none },\n";
    }
    ShrinkIndent();
    oss_ << cur_indent_ << "},\n";
}

void Debug::ASTStringVisitor::visit(Identifier &node) {
    oss_ << "Identifier { name: " << node.name << " },\n";
}

void Debug::ASTStringVisitor::visit(Literal &node) {
    oss_ << "Literal { value: " << VariantToString(node.value) << " },\n";
}

void Debug::ASTStringVisitor::visit(Parameters &node) {
    GrowIndent();
    oss_ << "Parameters <list> [\n";
    for (auto& identifier : node.identifiers) {
        oss_ << cur_indent_;
        identifier->accept(*this);
    }
    ShrinkIndent();
    oss_ << cur_indent_ << "],\n";
}

void Debug::ASTStringVisitor::visit(Arguments &node) {
    GrowIndent();
    oss_ << "Arguments <list> [\n";
    for (auto& expression : node.expressions) {
        oss_ << cur_indent_;
        expression->accept(*this);
    }
    ShrinkIndent();
    oss_ << cur_indent_ << "],\n";
}

void Debug::ASTStringVisitor::GrowIndent() {
    cur_indent_ += std::string(indent_size_, ' ');
}

void Debug::ASTStringVisitor::ShrinkIndent() {
    int new_indent_size = std::max(0, static_cast<int>(cur_indent_.size()) - indent_size_);
    cur_indent_ = std::string(new_indent_size, ' ');
}

void Debug::PrintTokens(const std::vector<Token> &tokens) {
    std::cout << "[line]    [TokenType]       [lexeme]\n";
    size_t last_line_num = -1;
    for (auto& token : tokens) {
        PrintToken(token, last_line_num);
        last_line_num = token.line;
    }
    std::cout << std::endl;
}

std::string Debug::GetChunkStr(const Chunk &chunk) {
    std::ostringstream oss;
    auto code = chunk.GetCode();
    for (int i = 0; i < code.size(); i++) {
        int col_width = 12;
        oss << std::left << std::setw(col_width);
        switch (auto op_code = static_cast<OP>(code[i])) {
            case OP::CONSTANT:
                oss << "[CONSTANT]";
                i++;
                oss  << static_cast<int>(code[i]) << "\n";
                break;
            case OP::ADD:
                oss << "[ADD]\n";
                break;
            case OP::SUBTRACT:
                oss << "[SUBTRACT]\n";
                break;
            case OP::MULTIPLY:
                oss << "[MULTIPLY]\n";
                break;
            case OP::DIVIDE:
                oss << "[DIVIDE]\n";
                break;
            default:
                oss << "[UNKNOWN OPCODE]\n";
        }
    }
    return oss.str();
}

std::string Debug::GetASTString(ASTNode* const node) {
    ASTStringVisitor visitor;
    if (auto* program = dynamic_cast<Program*>(node)) {
        program->accept(visitor);
    } else if (auto* fun_decl = dynamic_cast<FunDecl*>(node)) {
        fun_decl->accept(visitor);
    } else if (auto* var_decl = dynamic_cast<VarDecl*>(node)) {
        var_decl->accept(visitor);
    } else if (auto* expr_stmt = dynamic_cast<ExprStmt*>(node)) {
        expr_stmt->accept(visitor);
    } else if (auto* if_stmt = dynamic_cast<IfStmt*>(node)) {
        if_stmt->accept(visitor);
    } else if (auto* print_stmt = dynamic_cast<PrintStmt*>(node)) {
        print_stmt->accept(visitor);
    } else if (auto* return_stmt = dynamic_cast<ReturnStmt*>(node)) {
        return_stmt->accept(visitor);
    } else if (auto* while_stmt = dynamic_cast<WhileStmt*>(node)) {
        while_stmt->accept(visitor);
    } else if (auto* block = dynamic_cast<Block*>(node)) {
        block->accept(visitor);
    } else if (auto* assignment = dynamic_cast<Assignment*>(node)) {
        assignment->accept(visitor);
    } else if (auto* binary = dynamic_cast<Binary*>(node)) {
        binary->accept(visitor);
    } else if (auto* unary = dynamic_cast<Unary*>(node)) {
        unary->accept(visitor);
    } else if (auto* call = dynamic_cast<Call*>(node)) {
        call->accept(visitor);
    } else if (auto* identifier = dynamic_cast<Identifier*>(node)) {
        identifier->accept(visitor);
    } else if (auto* literal = dynamic_cast<Literal*>(node)) {
        literal->accept(visitor);
    } else if (auto* parameters = dynamic_cast<Parameters*>(node)) {
        parameters->accept(visitor);
    } else if (auto* arguments = dynamic_cast<Arguments*>(node)) {
        arguments->accept(visitor);
    }

    return visitor.GetString();
}

static std::string GetOperator(TokenType type) {
    switch (type) {
        case TokenType::PLUS: return "+";
        case TokenType::MINUS: return "-";
        case TokenType::STAR: return "*";
        case TokenType::SLASH: return "/";
        case TokenType::AND: return "and";
        case TokenType::OR: return "or";
        case TokenType::EQUAL: return "=";
        case TokenType::EQUAL_EQUAL: return "==";
        case TokenType::BANG: return "!";
        case TokenType::BANG_EQUAL: return "!=";
        case TokenType::GREATER: return ">";
        case TokenType::GREATER_EQUAL: return ">=";
        case TokenType::LESS: return "<";
        case TokenType::LESS_EQUAL: return "<=";
        default: return "?";
    }
}

static std::string WrapWithParen(std::string str) {
    return "(" + str + ")";
}

static std::string CreateExprString(const Expression* const expression) {
    if (const auto* literal = dynamic_cast<const Literal*>(expression)) {
        return VariantToString(literal->value);
    }
    if (const auto* identifier = dynamic_cast<const Identifier*>(expression)) {
        return std::string(identifier->name);
    }
    if (const auto* binary = dynamic_cast<const Binary*>(expression)) {
        return WrapWithParen(CreateExprString(binary->left_expression.get()) +
            " " + GetOperator(binary->op) + " " +
                CreateExprString(binary->right_expression.get()));
    }
    if (const auto* unary = dynamic_cast<const Unary*>(expression)) {
        return WrapWithParen(GetOperator(unary->op) + CreateExprString(unary->expression.get()));
    }
    return "(Unknown Expression)";
}

std::string Debug::GetExpressionStr(const Expression* const expression) {
    std::string expr = CreateExprString(expression);
    if (expr.size() > 2 && expr[0] == '(') {
        expr = expr.substr(1, expr.size() - 2);
    }
    return expr;
}

