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

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "ReadNode";
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << get_node_name() << "(" << variable << ")";
        return ss;
    }

    [[nodiscard]] auto to_xml() const -> std::string override {
        return "<" + get_node_name() + " stmt_number=\"" + std::to_string(get_statement_number()) + "\" variable=\"" +
               variable + "\" />";
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

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "PrintNode";
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << get_node_name() << "(" << variable << ")";
        return ss;
    }

    [[nodiscard]] auto to_xml() const -> std::string override {
        return "<" + get_node_name() + " stmt_number=\"" + std::to_string(get_statement_number()) + "\" variable=\"" +
               variable + "\" />";
    }
};

class CallNode : public StatementNode {
  public:
    std::string proc_name;

    explicit CallNode(std::string variable) : StatementNode(sp::NodeType::Call), proc_name(std::move(variable)) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return {};
    }

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "CallNode";
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << get_node_name() << "(" << proc_name << ")";
        return ss;
    }

    [[nodiscard]] auto to_xml() const -> std::string override {
        const auto stmt_number = std::to_string(get_statement_number());
        return "<" + get_node_name() + " stmt_number=\"" + stmt_number + "\" proc_name=\"" + proc_name + "\" />";
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

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "IfNode";
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << get_node_name() << "(" << *cond_expr << ", " << *then_stmt_list << ", " << *else_stmt_list << ")";
        return ss;
    }

    [[nodiscard]] auto to_xml() const -> std::string override {
        auto cond_expr_xml = cond_expr->to_xml();
        auto then_xml = then_stmt_list->to_xml();
        auto else_xml = else_stmt_list->to_xml();
        const auto stmt_number = std::to_string(get_statement_number());
        auto opening_xml = "<" + get_node_name() + " stmt_number=\"" + stmt_number + "\">";
        auto ending_xml = "</" + get_node_name() + ">";
        return opening_xml + cond_expr_xml + then_xml + else_xml + ending_xml;
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

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "WhileNode";
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << get_node_name() << "(" << *cond_expr << ", " << *stmt_list << ")";
        return ss;
    }

    [[nodiscard]] auto to_xml() const -> std::string override {
        auto cond_expr_xml = cond_expr->to_xml();
        auto body_xml = stmt_list->to_xml();
        const auto stmt_number = std::to_string(get_statement_number());

        auto opening_xml = "<" + get_node_name() + " stmt_number=\"" + stmt_number + "\">";
        auto ending_xml = "</" + get_node_name() + ">";
        return opening_xml + cond_expr_xml + body_xml + ending_xml;
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

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "AssignmentNode";
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << get_node_name() << "(" << *variable << ", " << *expr << ")";
        return ss;
    }

    [[nodiscard]] auto to_xml() const -> std::string override {
        auto var_xml = variable->to_xml();
        auto expr_xml = expr->to_xml();
        const auto stmt_number = std::to_string(get_statement_number());

        auto opening_xml = "<" + get_node_name() + " stmt_number=\"" + stmt_number + "\">";
        auto ending_xml = "</" + get_node_name() + ">";
        return opening_xml + var_xml + expr_xml + ending_xml;
    }
};

} // namespace sp
