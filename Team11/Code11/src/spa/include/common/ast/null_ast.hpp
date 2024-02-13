#pragma once

#include "common/ast/ast.hpp"
#include "common/ast/node_type.hpp"

/*
 * Dummy token to return empty string parsing
 */
namespace sp {
class NullNode : public sp::AstNode {
  public:
    explicit NullNode() : AstNode(sp::NodeType::EmptyString) {
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
} // namespace sp
