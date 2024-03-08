#pragma once

#include "pkb/facades/write_facade.h"
#include "sp/cfg/cfg_builder.hpp"
#include "sp/traverser/traverser.hpp"

namespace sp {

using OutNeighbours = std::pair<std::shared_ptr<CfgNode>, std::shared_ptr<CfgNode>>;

class NextTraverser {
    std::shared_ptr<WriteFacade> write_facade;

  private:
    auto traverse_node(std::shared_ptr<CfgNode> node) -> void;
    auto traverse_edge(std::shared_ptr<CfgNode> node, OutNeighbours outneighbours) -> void;
    auto traverse_procedure(std::shared_ptr<Cfg> cfg) -> void;

  public:
    explicit NextTraverser(std::shared_ptr<WriteFacade> write_facade) : write_facade(std::move(write_facade)){};
    auto traverse(ProcMap cfgs) -> void;
};
} // namespace sp