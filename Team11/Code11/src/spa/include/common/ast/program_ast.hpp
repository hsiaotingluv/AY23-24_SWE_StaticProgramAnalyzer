#pragma once

#include "common/ast/ast.hpp"

namespace sp {
class ProgramNode : public sp::AstNode {

  public:
    std::vector<std::shared_ptr<AstNode>> procedures;

    explicit ProgramNode(std::vector<std::shared_ptr<AstNode>>& procedures)
        : AstNode(sp::NodeType::Program), procedures(std::move(procedures)) {
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