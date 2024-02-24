#pragma once

#include "common/ast/ast.hpp"
#include "common/ast/mixin/parent_mixin.hpp"
#include "common/ast/mixin/uses_mixin.hpp"

namespace sp {
class MixinTypeChecker {
  public:
    static auto is_uses_mixin_node(const std::shared_ptr<AstNode>& node) -> bool {
        return std::dynamic_pointer_cast<UsesMixin>(node) != nullptr;
    }

    static auto is_parent_mixin_node(const std::shared_ptr<AstNode>& node) -> bool {
        return std::dynamic_pointer_cast<ParentMixin>(node) != nullptr;
    }
};
} // namespace sp
