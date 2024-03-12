#pragma once

#include "common/ast/ast.hpp"

namespace sp {

/**
 * @brief ProgramNode is an AST Node represents the whole SIMPLE source code.
 */
class ProgramNode : public AstNode {
  public:
    std::vector<std::shared_ptr<AstNode>> procedures;

    explicit ProgramNode(std::vector<std::shared_ptr<AstNode>>& procedures)
        : AstNode(sp::NodeType::Program), procedures(std::move(procedures)) {
    }

    auto get_children() -> std::vector<std::shared_ptr<AstNode>> override;
    [[nodiscard]] auto get_node_name() const -> std::string override;
    [[nodiscard]] auto identifier() const -> std::stringstream override;
    [[nodiscard]] auto to_xml() const -> std::string override;
};
} // namespace sp