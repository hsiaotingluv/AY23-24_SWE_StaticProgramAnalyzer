#pragma once

#include "common/ast/ast.hpp"
#include "common/cfg/cfg.hpp"

namespace sp {
class CfgMixin {
  public:
    virtual ~CfgMixin() = default;
    /**
     * @brief Build onto the given Control Flow Graph according to the AST Node type.
     */
    virtual auto build_cfg(std::shared_ptr<Cfg> cfg) -> void = 0;
};
} // namespace sp