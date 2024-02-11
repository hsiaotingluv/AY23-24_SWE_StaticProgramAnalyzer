#pragma once

#include "sp/parser/ast/node_type.hpp"
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace sp {

class AstNode {
  public:
    NodeType T;

    explicit AstNode(NodeType T) : T(T) {
    }

    virtual ~AstNode() = default;

    [[nodiscard]] virtual auto identifier() const -> std::stringstream = 0;
    virtual auto get_children() -> std::vector<std::shared_ptr<AstNode>> = 0;

    friend auto operator<<(std::ostream& os, const AstNode& node) -> std::ostream& {
        return os << node.identifier().str();
    };
};

/*
 * Dummy token to return empty string parsing
 */
class NullNode : public AstNode {
  public:
    explicit NullNode() : AstNode(NodeType::EmptyString) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return {};
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "NullNode()";
        return ss;
    }
};

class ConstantNode : public AstNode {
  public:
    int integer;

    explicit ConstantNode(int integer) : integer(integer), AstNode(NodeType::Constant) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return {};
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "ConstantNode(" << integer << ")";
        return ss;
    }
};

class NameNode : public AstNode {
  public:
    std::string name;

    explicit NameNode(std::string name) : name(std::move(name)), AstNode(NodeType::Variable) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return {};
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "NameNode(" << name << ")";
        return ss;
    }
};

/* These nodes follow the specs on
 * https://nus-cs3203.github.io/course-website/contents/basic-spa-requirements/abstract-syntax-tree.html
 * It means that variable is not stored as a new name node, but as the content of read node itself
 */

class StatementNode : public AstNode {
    std::optional<uint32_t> statement_number;

  public:
    explicit StatementNode(NodeType T) : AstNode(T) {
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

    explicit ReadNode(std::string variable) : variable(std::move(variable)), StatementNode(NodeType::Read) {
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

    explicit PrintNode(std::string variable) : variable(std::move(variable)), StatementNode(NodeType::Print) {
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

    explicit CallNode(std::string variable) : variable(std::move(variable)), StatementNode(NodeType::Call) {
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
          StatementNode(NodeType::If) {
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
        : cond_expr(cond_expr), stmt_list(stmt_list), StatementNode(NodeType::While) {
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
        : variable(std::move(variable)), expr(std::move(expr)), StatementNode(NodeType::Assign) {
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

class StatementListNode : public AstNode {

  public:
    std::vector<std::shared_ptr<AstNode>> statements;

    explicit StatementListNode(std::vector<std::shared_ptr<AstNode>>& statements)
        : statements(std::move(statements)), AstNode(NodeType::StmtList) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return statements;
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "StatementListNode(";
        for (const auto& stmt : statements) {
            ss << *stmt << ", ";
        }
        ss << ")";
        return ss;
    }
};

class ProcedureNode : public AstNode {

  public:
    std::string proc_name;
    std::shared_ptr<AstNode> stmt_list;

    explicit ProcedureNode(std::string proc_name, std::shared_ptr<AstNode>& stmt_list)
        : proc_name(std::move(proc_name)), stmt_list(stmt_list), AstNode(NodeType::Procedure) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return {stmt_list};
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "ProcedureNode(" << proc_name << ", " << *stmt_list << ")";
        return ss;
    }
};

// Generic AstNode with 2 operands
class BinaryNode : public AstNode {
  public:
    std::shared_ptr<AstNode> left{}, right{};

    explicit BinaryNode(NodeType T) : AstNode(T) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return {left, right};
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "BinaryNode(" << *left << ", " << *right << ")";
        return ss;
    }
};
class ComparatorNode : public AstNode {
  public:
    std::shared_ptr<AstNode> left{}, right{};

    explicit ComparatorNode(NodeType T, std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : left(std::move(left)), right(std::move(right)), AstNode(T) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return {left, right};
    }
};

class GreaterThanNode : public ComparatorNode {
  public:
    explicit GreaterThanNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : ComparatorNode(NodeType::Gt, left, right) {
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "GreaterThanNode(" << *left << ", " << *right << ")";
        return ss;
    }
};

class GreaterThanEqualNode : public ComparatorNode {
  public:
    explicit GreaterThanEqualNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : ComparatorNode(NodeType::Gte, left, right) {
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "GreaterThanEqualNode(" << *left << ", " << *right << ")";
        return ss;
    }
};

class LessThanNode : public ComparatorNode {
  public:
    explicit LessThanNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : ComparatorNode(NodeType::Lt, left, right) {
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "LessThanNode(" << *left << ", " << *right << ")";
        return ss;
    }
};

class LessThanEqualNode : public ComparatorNode {
  public:
    explicit LessThanEqualNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : ComparatorNode(NodeType::Lte, left, right) {
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "LessThanEqualNode(" << *left << ", " << *right << ")";
        return ss;
    }
};

class EqualNode : public ComparatorNode {
  public:
    explicit EqualNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : ComparatorNode(NodeType::Eq, left, right) {
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "EqualNode(" << *left << ", " << *right << ")";
        return ss;
    }
};

class NotEqualNode : public ComparatorNode {
  public:
    explicit NotEqualNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : ComparatorNode(NodeType::Neq, left, right) {
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "NotEqualNode(" << *left << ", " << *right << ")";
        return ss;
    }
};

class LogicalNode : public AstNode {
    using AstNode::AstNode;
};

class LogicalBinaryNode : public LogicalNode {
  public:
    std::shared_ptr<AstNode> left{}, right{};

    explicit LogicalBinaryNode(NodeType T, std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : left(std::move(left)), right(right), LogicalNode(T) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return {left, right};
    }
};

class LogicalAndNode : public LogicalBinaryNode {
  public:
    explicit LogicalAndNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : LogicalBinaryNode(NodeType::LAnd, left, right) {
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "LogicalAndNode(" << *left << ", " << *right << ")";
        return ss;
    }
};

class LogicalOrNode : public LogicalBinaryNode {
  public:
    explicit LogicalOrNode(std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right)
        : LogicalBinaryNode(NodeType::LOr, left, right) {
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "LogicalOrNode(" << *left << ", " << *right << ")";
        return ss;
    }
};

class LogicalNotNode : public LogicalNode {
  public:
    std::shared_ptr<AstNode> cond_expr;

    explicit LogicalNotNode(std::shared_ptr<AstNode>& cond_expr) : cond_expr(cond_expr), LogicalNode(NodeType::LNot) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return {cond_expr};
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "LogicalNotNode(" << *cond_expr << ")";
        return ss;
    }
};

class ProgramNode : public AstNode {

  public:
    std::vector<std::shared_ptr<AstNode>> procedures;

    explicit ProgramNode(std::vector<std::shared_ptr<AstNode>>& procedures)
        : procedures(std::move(procedures)), AstNode(NodeType::Program) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override {
        return procedures;
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << "ProgramNode(";
        for (const auto& proc : procedures) {
            ss << *proc << ", ";
        }
        ss << ")";
        return ss;
    }
};
} // namespace sp
