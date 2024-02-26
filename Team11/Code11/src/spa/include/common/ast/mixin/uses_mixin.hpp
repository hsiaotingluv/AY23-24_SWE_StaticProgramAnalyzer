#pragma once

#include "pkb/facades/write_facade.h"

namespace sp {

using UsesMap =
    std::unordered_map<std::string, std::unordered_set<std::string>>; // Procedure Name -> vector of Var Names

class UsesMixin {
  public:
    virtual auto populate_pkb_uses(const std::shared_ptr<WriteFacade>& write_facade,
                                   const std::shared_ptr<UsesMap>& uses_map) const
        -> std::unordered_set<std::string> = 0;

    virtual ~UsesMixin() = default;
};
} // namespace sp