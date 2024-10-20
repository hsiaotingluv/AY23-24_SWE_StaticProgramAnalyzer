#pragma once

#include <string>
#include <utility>

#include "pkb/facades/read_facade.h"

class AffectsConditions {
    std::string start_node;
    std::shared_ptr<pkb::ReadFacade> read_facade;

    std::string modified_var;

    std::function<bool(const std::string&)> start_node_cond;
    std::function<bool(const std::string&)> end_node_cond;
    std::function<bool(const std::string&)> intermediate_node_cond;

  public:
    explicit AffectsConditions(std::string start_node, std::shared_ptr<pkb::ReadFacade> read_facade)
        : start_node(std::move(start_node)), read_facade(std::move(read_facade)) {

        this->start_node_cond = [this](const std::string& start_node) {
            auto is_assign = this->read_facade->has_assign_statement(start_node);

            if (is_assign) {
                this->modified_var = *this->read_facade->get_vars_modified_by_statement(start_node).begin();
            }

            return is_assign;
        };

        this->end_node_cond = [this](const std::string& end_node) {
            auto is_assign = this->read_facade->has_assign_statement(end_node);

            if (is_assign) {
                auto used_vars = this->read_facade->get_vars_used_by_statement(end_node);
                auto is_same_var_used = used_vars.find(this->modified_var) != used_vars.end();
                auto is_in_same_proc = this->read_facade->are_stmt_nos_in_same_proc(this->start_node, end_node);
                return is_same_var_used && is_in_same_proc;
            }

            return false;
        };

        this->intermediate_node_cond = [this](const std::string& intermediate_node) {
            auto is_modified = this->read_facade->contains_statement_modify_var(intermediate_node, this->modified_var);
            auto is_not_if = !this->read_facade->has_if_statement(intermediate_node);
            auto is_not_while = !this->read_facade->has_while_statement(intermediate_node);
            auto is_same_proc = this->read_facade->are_stmt_nos_in_same_proc(this->start_node, intermediate_node);

            return !(is_modified && is_not_if && is_not_while) && is_same_proc; // somehow return true
        };
    }

    [[nodiscard]] std::function<bool(const std::string&)> get_start_node_cond() const {
        return this->start_node_cond;
    }

    [[nodiscard]] std::function<bool(const std::string&)> get_end_node_cond() const {
        return this->end_node_cond;
    }

    [[nodiscard]] std::function<bool(const std::string&)> get_intermediate_node_cond() const {
        return this->intermediate_node_cond;
    }
};
