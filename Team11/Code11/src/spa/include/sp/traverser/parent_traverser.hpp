#pragma once

#include "sp/traverser/traverser.hpp"

namespace sp {

// Only if and while are parents

class ParentTraverser : public Traverser {
    std::shared_ptr<WriteFacade> write_facade;

    // Functions to extract Parent relationships from different Node types
    auto get_parent_if(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string>;
    auto get_parent_while(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string>;

    // Helper function
    auto traverse_helper(const std::shared_ptr<AstNode>& node) -> void;
    static auto get_stmt_nums_from_stmt_lst(const std::shared_ptr<AstNode>& node) -> std::unordered_set<std::string>;

  public:
    explicit ParentTraverser(std::shared_ptr<WriteFacade> write_facade) : write_facade(std::move(write_facade)) {
    }

    auto traverse(std::shared_ptr<AstNode> node, const std::vector<std::string>& proc_topo_sort)
        -> std::shared_ptr<AstNode> override;
};
} // namespace sp