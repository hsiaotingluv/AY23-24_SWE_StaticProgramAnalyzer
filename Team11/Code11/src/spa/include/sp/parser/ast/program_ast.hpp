#pragma once

#include "sp/parser/ast/ast.hpp"

namespace sp {
class ProgramNode : public sp::AstNode {

  public:
    std::vector<std::shared_ptr<AstNode>> procedures;

    explicit ProgramNode(std::vector<std::shared_ptr<AstNode>>& procedures)
        : procedures(std::move(procedures)), AstNode(sp::NodeType::Program) {
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