#pragma once

#include "common/ast/node_type.hpp"

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace sp {

class AstNode {
  public:
    NodeType T;

    explicit AstNode(NodeType T) : T(T) {
    }

    virtual ~AstNode() = default;

    [[nodiscard]] virtual auto get_node_name() const -> std::string = 0;

    [[nodiscard]] virtual auto identifier() const -> std::stringstream = 0;
    virtual auto get_children() -> std::vector<std::shared_ptr<AstNode>> = 0;
    [[nodiscard]] virtual auto to_xml() const -> std::string = 0;

    friend auto operator<<(std::ostream& os, const AstNode& node) -> std::ostream& {
        return os << node.identifier().str();
    };
};

} // namespace sp
