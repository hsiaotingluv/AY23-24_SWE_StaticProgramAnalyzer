#pragma once

#include "common/ast/ast.hpp"
#include <type_traits>

namespace sp {

// CRTP only works with derived classes call, not from parent class
class ExprMixin {
  public:
    [[nodiscard]] virtual auto get_postfix() const -> std::string = 0;

    virtual ~ExprMixin() = default;
};

class ExprNode : public AstNode, public ExprMixin {
    using AstNode::AstNode;
};
} // namespace sp