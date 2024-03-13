#pragma once

#include "common/ast/ast.hpp"
#include <type_traits>

namespace sp {

/**
 * @brief Mixin for methods to represent Expressions.
 * @note CRTP only works with derived classes call, not from parent class
 */
class ExprMixin {
  public:
    virtual ~ExprMixin() = default;
    /**
     * @brief Get the postfix representation of the Expression.
     * @note The postfix representation of a AST branch is always unique. Suitable for pattern matching.
     */
    [[nodiscard]] virtual auto get_postfix() const -> std::string = 0;
};

/**
 * @brief ExprNode represents an AST Node that is an Expression.
 */
class ExprNode : public AstNode, public ExprMixin {
    using AstNode::AstNode;
};
} // namespace sp