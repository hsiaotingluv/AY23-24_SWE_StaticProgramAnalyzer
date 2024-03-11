#pragma once

#include "pkb/facades/write_facade.h"

namespace sp {

class DesignEntitiesMixin {
  public:
    virtual auto populate_pkb_entities(const std::shared_ptr<pkb::WriteFacade>& write_facade) const -> void = 0;

    virtual ~DesignEntitiesMixin() = default;
};
} // namespace sp