#pragma once

#include "pkb/facades/write_facade.h"

namespace sp {

using ModifyMap = std::unordered_map<std::string, std::unordered_set<std::string>>;

/**
 * @brief Mixin for methods to populate PKB's Modifies Store.
 */
class ModifiesMixin {
  public:
    virtual ~ModifiesMixin() = default;
    /**
     * @brief Populate PKB's Modifies Store according to the AST Node type.
     */
    virtual auto populate_pkb_modifies(const std::shared_ptr<pkb::WriteFacade>& write_facade,
                                       const std::shared_ptr<ModifyMap>& modify_map)
        -> std::unordered_set<std::string> = 0;
};
} // namespace sp