#pragma once

#include "common/ast/ast.hpp"
#include "common/ast/statement_list_ast.hpp" // No circular imports
#include "pkb/facades/write_facade.h"

namespace sp {

class ParentMixin {
  public:
    virtual auto populate_pkb_parent(const std::shared_ptr<pkb::WriteFacade>& write_facade) const -> void = 0;
    virtual ~ParentMixin() = default;
};
} // namespace sp