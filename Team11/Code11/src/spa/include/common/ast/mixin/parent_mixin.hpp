#pragma once

#include "common/ast/ast.hpp"
#include "common/ast/statement_list_ast.hpp" // No circular imports
#include "pkb/facades/write_facade.h"

namespace sp {

/**
 * @brief Mixin for methods to populate PKB's Parent Store.
 */
class ParentMixin {
  public:
    virtual ~ParentMixin() = default;
    /**
     * @brief Populate PKB's Parent Store according to the AST Node type.
     */
    virtual auto populate_pkb_parent(const std::shared_ptr<pkb::WriteFacade>& write_facade) const -> void = 0;
};
} // namespace sp