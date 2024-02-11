#pragma once

#include "sp/parser/ast/node_type.hpp"
#include <memory>
#include <string>
#include <utility>
#include <variant>

namespace sp {

class AstNode {
  public:
    NodeType T;

    explicit AstNode(NodeType T) : T(T) {
    }

    virtual ~AstNode() = default;
};

/*
 * Dummy token to return empty string parsing
 */
class NullNode : public AstNode {
  public:
    explicit NullNode() : AstNode(NodeType::EmptyString) {
    }
};

class ConstantNode : public AstNode {
  public:
    int integer;

    explicit ConstantNode(int integer) : integer(integer), AstNode(NodeType::Constant) {
    }
};

class NameNode : public AstNode {
  public:
    std::string name;

    explicit NameNode(std::string name) : name(std::move(name)), AstNode(NodeType::Variable) {
    }
};

/* These nodes follow the specs on
 * https://nus-cs3203.github.io/course-website/contents/basic-spa-requirements/abstract-syntax-tree.html
 * It means that variable is not stored as a new name node, but as the content of read node itself
 */
class ReadNode : public AstNode {
  public:
    std::string variable;

    explicit ReadNode(std::string variable) : variable(std::move(variable)), AstNode(NodeType::Read) {
    }
};

class PrintNode : public AstNode {
  public:
    std::string variable;

    explicit PrintNode(std::string variable) : variable(std::move(variable)), AstNode(NodeType::Print) {
    }
};

class CallNode : public AstNode {
  public:
    std::string variable;

    explicit CallNode(std::string variable) : variable(std::move(variable)), AstNode(NodeType::Call) {
    }
};

class IfNode : public AstNode {
  public:
    std::shared_ptr<AstNode> cond_expr;
    std::shared_ptr<AstNode> then_stmt_list;
    std::shared_ptr<AstNode> else_stmt_list;

    IfNode(std::shared_ptr<AstNode>& cond_expr, std::shared_ptr<AstNode>& then_stmt_list,
           std::shared_ptr<AstNode>& else_stmt_list)
        : cond_expr(cond_expr), then_stmt_list(then_stmt_list), else_stmt_list(else_stmt_list), AstNode(NodeType::If) {
    }
};

class WhileNode : public AstNode {
  public:
    std::shared_ptr<AstNode> cond_expr;
    std::shared_ptr<AstNode> stmt_list;

    WhileNode(std::shared_ptr<AstNode>& cond_expr, std::shared_ptr<AstNode>& stmt_list)
        : cond_expr(cond_expr), stmt_list(stmt_list), AstNode(NodeType::While) {
    }
};

class StatementListNode : public AstNode {

  public:
    std::vector<std::shared_ptr<AstNode>> statements;

    explicit StatementListNode(std::vector<std::shared_ptr<AstNode>>& statements)
        : statements(std::move(statements)), AstNode(NodeType::StmtList) {
    }
};

class ProcedureNode : public AstNode {

  public:
    std::string proc_name;
    std::shared_ptr<AstNode> stmt_list;

    explicit ProcedureNode(std::string proc_name, std::shared_ptr<AstNode>& stmt_list)
        : proc_name(std::move(proc_name)), stmt_list(stmt_list), AstNode(NodeType::Procedure) {
    }
};

// Generic AstNode with 2 operands
class BinaryNode : public AstNode {
  public:
    std::shared_ptr<AstNode> left{}, right{};

    explicit BinaryNode(NodeType T) : AstNode(T) {
    }
};

class LogicalNotNode : public AstNode {
  public:
    std::shared_ptr<AstNode> cond_expr;

    explicit LogicalNotNode(std::shared_ptr<AstNode>& cond_expr) : cond_expr(cond_expr), AstNode(NodeType::LNot) {
    }
};

class ProgramNode : public AstNode {

  public:
    std::vector<std::shared_ptr<AstNode>> procedures;

    explicit ProgramNode(std::vector<std::shared_ptr<AstNode>>& procedures)
        : procedures(std::move(procedures)), AstNode(NodeType::Program) {
    }
};
} // namespace sp
