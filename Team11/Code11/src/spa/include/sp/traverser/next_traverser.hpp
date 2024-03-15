#pragma once

#include "common/cfg/cfg.hpp"
#include "pkb/facades/write_facade.h"
#include "sp/cfg/cfg_builder.hpp"

namespace sp {

class NextTraverser {
    std::shared_ptr<pkb::WriteFacade> write_facade;

  private:
    auto traverse_node(std::shared_ptr<CfgNode> node) -> void;
    auto traverse_edge(std::shared_ptr<CfgNode> node, std::shared_ptr<CfgNode> outneighbour) -> void;
    auto traverse_edges(std::shared_ptr<CfgNode> node, OutNeighbours outneighbours) -> void;
    auto traverse_procedure(std::shared_ptr<Cfg> cfg) -> void;

  public:
    explicit NextTraverser(std::shared_ptr<pkb::WriteFacade> write_facade) : write_facade(std::move(write_facade)){};
    auto traverse(ProcMap cfgs) -> void;
};
} // namespace sp