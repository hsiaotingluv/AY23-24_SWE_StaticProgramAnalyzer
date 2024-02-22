#pragma once

#include "common/ast/factor_ast.hpp"
#include "common/ast/procedure_ast.hpp"
#include "common/ast/statement_ast.hpp"
#include "common/ast/statement_list_ast.hpp"
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
    using ProcMap = std::unordered_map<std::string, std::shared_ptr<AstNode>>;

    UsesMap uses_map{};
    ProcMap proc_map{};

    std::shared_ptr<WriteFacade> write_facade;

    // Functions to extract Modify relationships from different Node types
    auto get_use_assignment(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string>;
    auto get_use_print(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string>;
    auto get_use_while(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string>;
    auto get_use_if(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string>;
    auto get_use_proc(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string>;
    auto get_use_call(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string>;

    // Functions to extract Modified variables (i.e. Variables on the RHS)
    static auto is_var_node(const std::shared_ptr<AstNode>& node) -> bool;
    auto traverse_expression(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string>;
    auto traverse_stmt_lst(const std::shared_ptr<StatementListNode>& node) -> std::unordered_set<std::string>;

    // Helper function
    auto traverse_helper(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string>;

  public:
    explicit UsesTraverser(std::shared_ptr<WriteFacade> write_facade) : write_facade(std::move(write_facade)){};
    auto traverse(std::shared_ptr<AstNode> node, const std::vector<std::string>& proc_topo_sort)
        -> std::shared_ptr<AstNode> override;
};
} // namespace sp