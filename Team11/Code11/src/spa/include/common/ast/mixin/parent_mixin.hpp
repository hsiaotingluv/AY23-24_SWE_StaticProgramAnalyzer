#pragma once

#include "common/ast/ast.hpp"
#include "common/ast/statement_list_ast.hpp" // No circular imports
#include "pkb/facades/write_facade.h"

namespace sp {

class ParentMixin {
  private:
    virtual auto get_stmt_nums(const std::shared_ptr<StatementListNode>& node) const
        -> std::unordered_set<std::string> = 0;

  public:
    virtual auto populate_pkb_parent(const std::shared_ptr<WriteFacade>& write_facade) const -> void = 0;
    virtual ~ParentMixin() = default;
};
} // namespace sp