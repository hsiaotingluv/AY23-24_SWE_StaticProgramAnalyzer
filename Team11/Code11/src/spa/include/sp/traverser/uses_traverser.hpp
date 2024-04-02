#pragma once

#include "common/ast/procedure_ast.hpp"
#include "pkb/facades/write_facade.h"
#include "sp/traverser/traverser.hpp"

#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace sp {

class UsesTraverser : public Traverser {
    using UsesMap =
        std::unordered_map<std::string, std::unordered_set<std::string>>; // Map Statement Number to Name of Modified
                                                                          // Variables (i.e. Variables found on the RHS)
    using ProcMap = std::unordered_map<std::string, std::shared_ptr<ProcedureNode>>;

    UsesMap uses_map{};
    ProcMap proc_map{};

    std::shared_ptr<pkb::WriteFacade> write_facade;

  public:
    explicit UsesTraverser(std::shared_ptr<pkb::WriteFacade> write_facade) : write_facade(std::move(write_facade)) {
    }

    auto traverse(std::shared_ptr<AstNode> node, const std::vector<std::string>& proc_topo_sort)
        -> std::shared_ptr<AstNode> override;
};
} // namespace sp