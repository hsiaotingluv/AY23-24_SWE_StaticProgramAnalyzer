#pragma once

#include "common/ast/ast.hpp"
#include "common/cfg/cfg.hpp"
#include "sp/traverser/traverser.hpp"

namespace sp {

class CfgTraverser : public Traverser {
  public:
    auto traverse(std::shared_ptr<AstNode> ast, const std::vector<std::string>& proc_topo_sort)
        -> std::shared_ptr<AstNode> override;
};

}