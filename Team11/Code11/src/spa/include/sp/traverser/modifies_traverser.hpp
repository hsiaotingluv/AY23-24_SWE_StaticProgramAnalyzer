#pragma once

#include "common/ast/ast.hpp"
#include "common/ast/procedure_ast.hpp"
#include "pkb/facades/write_facade.h"
#include "sp/traverser/traverser.hpp"

#include <memory>
#include <unordered_map>

namespace sp {

class ModifiesTraverser : public Traverser {
    using ProcMap = std::unordered_map<std::string, std::shared_ptr<ProcedureNode>>;

    ProcMap proc_map{};

    std::shared_ptr<pkb::WriteFacade> write_facade;

  public:
    explicit ModifiesTraverser(std::shared_ptr<pkb::WriteFacade> write_facade) : write_facade(std::move(write_facade)) {
    }

    auto traverse(std::shared_ptr<AstNode> node, const std::vector<std::string>& proc_topo_sort)
        -> std::shared_ptr<AstNode> override;
};
} // namespace sp