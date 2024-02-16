#pragma once

#include "common/ast/ast.hpp"

namespace sp {

class ProcedureNode : public sp::AstNode {

  public:
    std::string proc_name;
    std::shared_ptr<AstNode> stmt_list;

    explicit ProcedureNode(std::string proc_name, std::shared_ptr<AstNode>& stmt_list)
        : AstNode(sp::NodeType::Procedure), proc_name(std::move(proc_name)), stmt_list(stmt_list) {
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
} // namespace sp
