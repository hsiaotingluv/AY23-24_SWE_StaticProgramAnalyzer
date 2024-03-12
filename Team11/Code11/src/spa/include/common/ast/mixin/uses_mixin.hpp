#pragma once

#include "pkb/facades/write_facade.h"

namespace sp {

using UsesMap =
    std::unordered_map<std::string, std::unordered_set<std::string>>; // Procedure Name -> vector of Var Names

/**
 * @brief Mixin for methods to populate PKB's Uses Store.
 */
class UsesMixin {
  public:
    virtual ~UsesMixin() = default;
    /**
     * @brief Populate PKB's Uses Store according to the AST Node type.
     *
     */
    virtual auto populate_pkb_uses(const std::shared_ptr<pkb::WriteFacade>& write_facade,
                                   const std::shared_ptr<UsesMap>& uses_map) const
        -> std::unordered_set<std::string> = 0;

};
} // namespace sp