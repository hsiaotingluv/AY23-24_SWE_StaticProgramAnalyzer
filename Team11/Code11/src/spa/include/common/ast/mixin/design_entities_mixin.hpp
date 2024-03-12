#pragma once

#include "pkb/facades/write_facade.h"

namespace sp {

/**
 * @brief Mixin for methods to populate PKB entities.
 */
class DesignEntitiesMixin {
  public:
    virtual ~DesignEntitiesMixin() = default;
    /**
     * @brief Populate PKB entities according to the AST Node type.
     */
    virtual auto populate_pkb_entities(const std::shared_ptr<pkb::WriteFacade>& write_facade) const -> void = 0;
};
} // namespace sp