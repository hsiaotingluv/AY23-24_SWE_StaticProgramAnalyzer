#pragma once

#include "common/cfg/cfg.hpp"
#include "pkb/facades/write_facade.h"
#include "sp/cfg/cfg_builder.hpp"

namespace sp {

class NextTraverser {
    std::shared_ptr<pkb::WriteFacade> write_facade;

  private:
    auto traverse_node(const std::shared_ptr<CfgNode>& node) -> void;
    auto traverse_edge(const std::shared_ptr<CfgNode>& node, const std::shared_ptr<CfgNode>& outneighbour) -> void;
    auto traverse_edges(const std::shared_ptr<CfgNode>& node, const OutNeighbours& outneighbours) -> void;
    auto traverse_procedure(const std::shared_ptr<Cfg>& cfg) -> void;

  public:
    explicit NextTraverser(std::shared_ptr<pkb::WriteFacade> write_facade) : write_facade(std::move(write_facade)){};
    auto traverse(const ProcMap& cfgs) -> void;
};
} // namespace sp