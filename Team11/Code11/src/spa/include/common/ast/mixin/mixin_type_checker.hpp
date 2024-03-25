#pragma once

#include "common/ast/ast.hpp"
#include "common/ast/mixin/parent_mixin.hpp"
#include "common/ast/mixin/uses_mixin.hpp"

namespace sp {
/**
 * @brief Mixin for methods to check the type of Mixin Nodes. (To refactor out)
 */
class MixinTypeChecker {
  public:
    /**
     * @brief Check if the given node is a UsesMixin Node.
     */
    static auto is_uses_mixin_node(const std::shared_ptr<AstNode>& node) -> bool {
        return std::dynamic_pointer_cast<UsesMixin>(node) != nullptr;
    }

    /**
     * @brief Check if the given node is a ParentMixin Node.
     */
    static auto is_parent_mixin_node(const std::shared_ptr<AstNode>& node) -> bool {
        return std::dynamic_pointer_cast<ParentMixin>(node) != nullptr;
    }
};
} // namespace sp
