#pragma once

#include "common/cfg/cfg.hpp"

namespace sp {
/**
 * @brief Mixin for methods to build Control Flow Graphs.
 */
class CfgMixin {
  public:
    virtual ~CfgMixin() = default;
    /**
     * @brief Build onto the given Control Flow Graph according to the AST Node type.
     */
    virtual auto build_cfg(std::shared_ptr<Cfg> cfg) -> void = 0;
};
} // namespace sp