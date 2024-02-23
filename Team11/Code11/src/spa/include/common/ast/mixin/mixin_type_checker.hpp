#pragma once

#include "common/ast/ast.hpp"
#include "common/ast/mixin/uses_mixin.hpp"

namespace sp {
class MixinTypeChecker {
  public:
    static auto is_uses_mixin_node(const std::shared_ptr<AstNode>& node) -> bool {
        return std::dynamic_pointer_cast<UsesMixin>(node) != nullptr;
    }
}
} // namespace sp
