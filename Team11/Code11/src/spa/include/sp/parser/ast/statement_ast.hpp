#pragma once

#include "sp/parser/ast/ast.hpp"

/* These nodes follow the specs on
 * https://nus-cs3203.github.io/course-website/contents/basic-spa-requirements/abstract-syntax-tree.html
 * It means that variable is not stored as a new name node, but as the content of read node itself
 */
namespace sp {

class StatementNode : public sp::AstNode {
    std::optional<uint32_t> statement_number;

  public:
    explicit StatementNode(sp::NodeType T) : AstNode(T) {
    }

    auto set_statement_number(uint32_t statement_number) -> void {
        this->statement_number = statement_number;
    }

    auto get_statement_number() -> uint32_t {
        return statement_number.value();
    }

    auto has_statement_number() -> bool {
        return statement_number.has_value();
    }
};

class ReadNode : public StatementNode {
  public:
    std::string variable;

    explicit ReadNode(std::string variable) : variable(std::move(variable)), StatementNode(sp::NodeType::Read) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return {};
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "ReadNode(" << variable << ")";
        return ss;
    }
};

class PrintNode : public StatementNode {
  public:
    std::string variable;

    explicit PrintNode(std::string variable) : variable(std::move(variable)), StatementNode(sp::NodeType::Print) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return {};
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "PrintNode(" << variable << ")";
        return ss;
    }
};

class CallNode : public StatementNode {
  public:
    std::string variable;

    explicit CallNode(std::string variable) : variable(std::move(variable)), StatementNode(sp::NodeType::Call) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return {};
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "CallNode(" << variable << ")";
        return ss;
    }
};

class IfNode : public StatementNode {
  public:
    std::shared_ptr<AstNode> cond_expr;
    std::shared_ptr<AstNode> then_stmt_list;
    std::shared_ptr<AstNode> else_stmt_list;

    IfNode(std::shared_ptr<AstNode>& cond_expr, std::shared_ptr<AstNode>& then_stmt_list,
           std::shared_ptr<AstNode>& else_stmt_list)
        : cond_expr(cond_expr), then_stmt_list(then_stmt_list), else_stmt_list(else_stmt_list),
          StatementNode(sp::NodeType::If) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return {cond_expr, then_stmt_list, else_stmt_list};
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "IfNode(" << *cond_expr << ", " << *then_stmt_list << ", " << *else_stmt_list << ")";
        return ss;
    }
};

class WhileNode : public StatementNode {
  public:
    std::shared_ptr<AstNode> cond_expr;
    std::shared_ptr<AstNode> stmt_list;

    WhileNode(std::shared_ptr<AstNode>& cond_expr, std::shared_ptr<AstNode>& stmt_list)
        : cond_expr(cond_expr), stmt_list(stmt_list), StatementNode(sp::NodeType::While) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return {cond_expr, stmt_list};
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "WhileNode(" << *cond_expr << ", " << *stmt_list << ")";
        return ss;
    }
};

class AssignmentNode : public StatementNode {
  public:
    std::shared_ptr<AstNode> variable;
    std::shared_ptr<AstNode> expr;

    AssignmentNode(std::shared_ptr<AstNode> variable, std::shared_ptr<AstNode> expr)
        : variable(std::move(variable)), expr(std::move(expr)), StatementNode(sp::NodeType::Assign) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return {variable, expr};
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "AssignmentNode(" << *variable << ", " << *expr << ")";
        return ss;
    }
};

} // namespace sp
