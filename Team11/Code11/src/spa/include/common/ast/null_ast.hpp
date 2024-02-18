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

    [[nodiscard]] auto get_node_name() const -> std::string override {
        return "NullNode";
    }

    [[nodiscard]] auto identifier() const -> std::stringstream override {
        auto ss = std::stringstream();
        ss << get_node_name() << "()";
        return ss;
    }

    [[nodiscard]] auto to_xml() const -> std::string override {
        return "<" + get_node_name() + " />";
    }
};
} // namespace sp
