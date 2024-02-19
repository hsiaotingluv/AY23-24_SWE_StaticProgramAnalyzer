#pragma once

#include "common/ast/ast.hpp"
#include "pkb/facades/write_facade.h"
#include "sp/traverser/traverser.hpp"
#include <unordered_map>
#include <unordered_set>

namespace sp {

class ModifiesTraverser : public Traverser {
    using ModifyMap = std::unordered_map<std::string, std::unordered_set<std::string>>;
    using ProcMap = std::unordered_map<std::string, std::shared_ptr<AstNode>>;

    ModifyMap modifies_map{};
    ProcMap proc_map{};

    std::shared_ptr<WriteFacade> write_facade;

    static auto is_read_node(const std::shared_ptr<AstNode>& node) -> bool;

    static auto is_assignment_node(const std::shared_ptr<AstNode>& node) -> bool;

    auto traverse_helper(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string>;

    static auto is_while_node(const std::shared_ptr<AstNode>& node) -> bool;
    static auto is_if_node(const std::shared_ptr<AstNode>& node) -> bool;
    static auto is_call_node(const std::shared_ptr<AstNode>& node) -> bool;
    static auto is_proc_node(const std::shared_ptr<AstNode>& node) -> bool;

    auto get_modify_assignment(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string>;
    auto get_modify_read(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string>;
    auto get_modify_call(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string>;
    auto get_modify_while(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string>;
    auto get_modify_if(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string>;
    auto get_modify_proc(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string>;

  public:
    explicit ModifiesTraverser(std::shared_ptr<WriteFacade> write_facade) : write_facade(std::move(write_facade)){};
    auto traverse(std::shared_ptr<AstNode> ast, const std::vector<std::string>& proc_topo_sort)
        -> std::shared_ptr<AstNode> override;
};
} // namespace sp