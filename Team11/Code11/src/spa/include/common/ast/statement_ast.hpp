#pragma once

#include "common/ast/ast.hpp"

#include <optional>

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

    [[nodiscard]] auto get_statement_number() const -> int {
        return statement_number.value_or(-1);
    }

    [[nodiscard]] auto has_statement_number() const -> bool {
        return statement_number.has_value();
    }
};

class ReadNode : public StatementNode {
  public:
    std::string variable;

    explicit ReadNode(std::string variable) : StatementNode(sp::NodeType::Read), variable(std::move(variable)) {
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

    explicit PrintNode(std::string variable) : StatementNode(sp::NodeType::Print), variable(std::move(variable)) {
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

    explicit CallNode(std::string variable) : StatementNode(sp::NodeType::Call), variable(std::move(variable)) {
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
        : StatementNode(sp::NodeType::If), cond_expr(cond_expr), then_stmt_list(then_stmt_list),
          else_stmt_list(else_stmt_list) {
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
        : StatementNode(sp::NodeType::While), cond_expr(cond_expr), stmt_list(stmt_list) {
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
        : StatementNode(sp::NodeType::Assign), variable(std::move(variable)), expr(std::move(expr)) {
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
