#pragma once

#include "common/ast/node_type.hpp"

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace sp {

/**
 * @brief Class to represent Abstract Syntax Node.
 */
class AstNode {
  public:
    NodeType T;

    explicit AstNode(NodeType T) : T(T) {
    }

    virtual ~AstNode() = default;

    /**
     * @brief Retrieves the children of th ASTNode.
     * @return A vector of shared pointers to the children of the ASTNode.
     */
    virtual auto get_children() -> std::vector<std::shared_ptr<AstNode>> = 0;
    /**
     * @brief Retrieves the name of the ASTNode.
     * @return A string of the ASTNode's name.
     */
    [[nodiscard]] virtual auto get_node_name() const -> std::string = 0;
    /**
     * @brief Retrieves the identifier of the ASTNode.
     * @return A stringstream containing the identifier of the ASTNode.
     */
    [[nodiscard]] virtual auto identifier() const -> std::stringstream = 0;
    /**
     * @brief Retrieves the identifier of the ASTNode.
     * @return The XML representation of the ASTNode.
     */
    [[nodiscard]] virtual auto to_xml() const -> std::string = 0;

    /**
     * @brief Overloaded << operator to print the identifier of the ASTNode.
     * @param os The output stream.
     * @param node the ASTNode object.
     * @return The output stream with the identifier of the ASTNode printed.
     */
    friend auto operator<<(std::ostream& os, const AstNode& node) -> std::ostream& {
        return os << node.identifier().str();
    }
};

} // namespace sp
