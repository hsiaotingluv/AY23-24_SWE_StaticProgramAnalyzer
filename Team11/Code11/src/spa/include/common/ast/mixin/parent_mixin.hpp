#pragma once

#include "common/ast/ast.hpp"
#include "pkb/facades/write_facade.h"

namespace sp {

class ParentMixin {
  public:
    virtual auto populate_pkb_parent(const std::shared_ptr<WriteFacade>& write_facade) const -> void = 0;

    virtual ~ParentMixin() = default;
};
} // namespace sp