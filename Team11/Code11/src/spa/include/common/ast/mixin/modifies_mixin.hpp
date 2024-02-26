#pragma once

#include "common/ast/ast.hpp"
#include "pkb/facades/write_facade.h"

namespace sp {

using ModifyMap = std::unordered_map<std::string, std::unordered_set<std::string>>;

class ModifiesMixin {
  public:
    virtual auto populate_pkb_modifies(const std::shared_ptr<WriteFacade>& write_facade,
                                       const std::shared_ptr<ModifyMap>& modify_map)
        -> std::unordered_set<std::string> = 0;

    virtual ~ModifiesMixin() = default;
};
} // namespace sp