#pragma once

#include "common/ast/ast.hpp"
#include "common/ast/node_type.hpp"

namespace sp {
/**
 * @brief NullNode is an AST Node represents a null node.
 * @note NullNode is a dummy node to return empty string parsing.
 */
class NullNode : public AstNode {
  public:
    explicit NullNode() : AstNode(NodeType::EmptyString) {}

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override;
    [[nodiscard]] auto get_node_name() const -> std::string override;
    [[nodiscard]] auto identifier() const -> std::stringstream override;
    [[nodiscard]] auto to_xml() const -> std::string override;
};
} // namespace sp
