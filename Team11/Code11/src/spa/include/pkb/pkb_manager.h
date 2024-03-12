#pragma once

#include "common/hashable_tuple.h"
#include "pkb/stores/calls_store/calls_star_store.h"
#include "pkb/stores/calls_store/direct_calls_store.h"
#include "pkb/stores/entity_store.h"
#include "pkb/stores/follows_store/direct_follows_store.h"
#include "pkb/stores/follows_store/follows_star_store.h"
#include "pkb/stores/modifies_store/procedure_modifies_store.h"
#include "pkb/stores/modifies_store/statement_modifies_store.h"
#include "pkb/stores/next_store.h"
#include "pkb/stores/parent_store/direct_parent_store.h"
#include "pkb/stores/parent_store/parent_star_store.h"
#include "pkb/stores/pattern_matching_store/assignment_store.h"
#include "pkb/stores/pattern_matching_store/if_var_store.h"
#include "pkb/stores/pattern_matching_store/while_var_store.h"
#include "pkb/stores/statement_store.h"
#include "pkb/stores/uses_store/procedure_uses_store.h"
#include "pkb/stores/uses_store/statement_uses_store.h"
#include <memory>
#include <tuple>

// Forward declaration of facades
class ReadFacade;
class WriteFacade;

static constexpr auto identity_fun = [](const auto& s) {
    return s;
};

static constexpr auto tuple_stmt_no_extractor = [](const auto& p) {
    return std::get<0>(p);
};

class PkbManager {
  public:
    static std::tuple<std::shared_ptr<ReadFacade>, std::shared_ptr<WriteFacade>> create_facades();

    template <class T, class Extractor>
    std::unordered_set<T> filter_by_statement_type(const std::unordered_set<T>& set, StatementType statement_type,
                                                   Extractor extractor) const {
        std::unordered_set<T> filtered;
        for (const auto& elem : set) {
            if (statement_store->get_val_by_key(extractor(elem)) == statement_type) {
                filtered.insert(elem);
            }
        }
        return filtered;
    }

    template <class T>
    std::unordered_set<T> filter_by_statement_type(const std::unordered_set<T>& set,
                                                   StatementType statement_type) const {
        return filter_by_statement_type(set, statement_type, identity_fun);
    }

    template <class T>
    std::unordered_set<std::string> get_name_list(const std::unordered_set<T>& set) const {
        std::unordered_set<std::string> temp;
        for (const T& entity : set) {
            temp.insert(entity.getName());
        }

        return temp;
    }

    template <class T>
    std::unordered_set<std::tuple<std::string, std::string>>
    get_tuple_list_from_string_entity_pairs(const std::unordered_set<T>& pairs) {
        std::unordered_set<std::tuple<std::string, std::string>> temp;
        for (const auto& [s, v] : pairs) {
            temp.insert(std::make_tuple(s, v.getName()));
        }

        return temp;
    }

    template <class T>
    std::unordered_set<std::tuple<std::string, std::string>>
    get_tuple_list_from_entity_string_pairs(const std::unordered_set<T>& pairs) {
        std::unordered_set<std::tuple<std::string, std::string>> temp;
        for (const auto& [v, s] : pairs) {
            temp.insert(std::make_tuple(s, v.getName()));
        }

        return temp;
    }

    template <class T>
    std::unordered_set<std::tuple<std::string, std::string>>
    get_tuple_list_from_entity_entity_pairs(const std::unordered_set<T>& pairs) {
        std::unordered_set<std::tuple<std::string, std::string>> temp;
        for (const auto& [p, v] : pairs) {
            temp.insert(std::make_tuple(p.getName(), v.getName()));
        }

        return temp;
    }

    PkbManager();

    // Entity-related Read Operations
    std::unordered_set<std::string> get_entities();

    std::unordered_set<std::string> get_procedures();

    std::unordered_set<std::string> get_variables();

    std::unordered_set<std::string> get_constants();

    // Statement-related Read Operations
    std::unordered_set<std::string> get_all_statements();

    std::unordered_set<std::string> get_assign_statements();

    std::unordered_set<std::string> get_if_statements();

    std::unordered_set<std::string> get_while_statements();

    std::unordered_set<std::string> get_read_statements();

    std::unordered_set<std::string> get_print_statements();

    std::unordered_set<std::string> get_call_statements();

    // Modifies-related Read Operations
    std::unordered_set<std::string> get_vars_modified_by_statement(const std::string& s);

    std::unordered_set<std::string> get_statements_that_modify_var(const std::string& variable);

    std::unordered_set<std::string> get_statements_that_modify_var(const std::string& variable,
                                                                   const StatementType& statement_type);

    bool does_statement_modify_var(const std::string& statement_number, const std::string& variable);

    std::unordered_set<std::string> get_vars_modified_by_procedure(const std::string& procedure);

    std::unordered_set<std::string> get_procedures_that_modify_var(const std::string& variable);

    bool does_procedure_modify_var(const std::string& procedure, const std::string& variable);

    std::unordered_set<std::string> get_all_statements_that_modify();

    std::unordered_set<std::string> get_all_statements_that_modify(const StatementType& statement_type);

    bool does_statement_modify_any_var(const std::string& statement_number);

    std::unordered_set<std::tuple<std::string, std::string>> get_all_statements_and_var_modify_pairs();

    std::unordered_set<std::tuple<std::string, std::string>>
    get_all_statements_and_var_modify_pairs(const StatementType& statement_type);

    std::unordered_set<std::string> get_all_procedures_that_modify();

    bool does_procedure_modify_any_var(const std::string& procedure);

    std::unordered_set<std::tuple<std::string, std::string>> get_all_procedures_and_var_modify_pairs();

    // Uses-related Read Operations
    std::unordered_set<std::string> get_vars_used_by_statement(const std::string& s);

    std::unordered_set<std::string> get_statements_that_use_var(const std::string& variable);

    std::unordered_set<std::string> get_statements_that_use_var(const std::string& variable,
                                                                const StatementType& statement_type);

    bool does_statement_use_var(const std::string& statement_number, const std::string& variable);

    std::unordered_set<std::string> get_vars_used_by_procedure(const std::string& procedure);

    std::unordered_set<std::string> get_procedures_that_use_var(const std::string& variable);

    bool does_procedure_use_var(const std::string& procedure, const std::string& variable);

    std::unordered_set<std::string> get_all_statements_that_use();

    std::unordered_set<std::string> get_all_statements_that_use(const StatementType& statement_type);

    bool does_statement_use_any_var(const std::string& statement_number);

    std::unordered_set<std::tuple<std::string, std::string>> get_all_statements_and_var_use_pairs();

    std::unordered_set<std::tuple<std::string, std::string>>
    get_all_statements_and_var_use_pairs(const StatementType& statement_type);

    std::unordered_set<std::string> get_all_procedures_that_use();

    bool does_procedure_use_any_var(const std::string& procedure);

    std::unordered_set<std::tuple<std::string, std::string>> get_all_procedures_and_var_use_pairs();

    // Follows-related Read Operations
    bool has_follows_relation(const std::string& stmt1, const std::string& stmt2) const;

    std::unordered_map<std::string, std::string> get_all_follows() const;

    std::unordered_set<std::string> get_all_follows_keys() const;

    std::unordered_set<std::string> get_all_follows_keys(const StatementType& statement_type) const;

    std::unordered_set<std::string> get_all_follows_values() const;

    std::unordered_set<std::string> get_all_follows_values(const StatementType& statement_type) const;

    std::string get_statement_following(const std::string& s) const;

    std::string get_statement_following(const std::string& s, const StatementType& statement_type) const;

    std::string get_statement_followed_by(const std::string& s) const;

    std::string get_statement_followed_by(const std::string& s, const StatementType& statement_type) const;

    bool has_follows_star_relation(const std::string& stmt1, const std::string& stmt2) const;

    std::unordered_map<std::string, std::unordered_set<std::string>> get_all_follows_star() const;

    std::unordered_set<std::string> get_all_follows_star_keys() const;

    std::unordered_set<std::string> get_all_follows_star_keys(const StatementType& statement_type) const;

    std::unordered_set<std::string> get_all_follows_star_values() const;

    std::unordered_set<std::string> get_all_follows_star_values(const StatementType& statement_type) const;

    std::unordered_set<std::string> get_follows_stars_following(const std::string& stmt) const;

    std::unordered_set<std::string> get_follows_stars_following(const std::string& stmt,
                                                                const StatementType& statement_type) const;

    std::unordered_set<std::string> get_follows_stars_by(const std::string& stmt) const;

    std::unordered_set<std::string> get_follows_stars_by(const std::string& stmt,
                                                         const StatementType& statement_type) const;

    // Parent-related Read Operations
    bool has_parent_relation(const std::string& parent, const std::string& child) const;

    std::unordered_map<std::string, std::unordered_set<std::string>> get_all_parent() const;

    std::unordered_set<std::string> get_all_parent_keys() const;

    std::unordered_set<std::string> get_all_parent_keys(const StatementType& statement_type) const;

    std::unordered_set<std::string> get_all_parent_values() const;

    std::unordered_set<std::string> get_all_parent_values(const StatementType& statement_type) const;

    std::unordered_set<std::string> get_children_of(const std::string& parent) const;

    std::unordered_set<std::string> get_children_of(const std::string& parent,
                                                    const StatementType& statement_type) const;

    std::string get_parent_of(const std::string& child) const;

    std::string get_parent_of(const std::string& child, const StatementType& statement_type) const;

    bool has_parent_star_relation(const std::string& parent, const std::string& child) const;

    std::unordered_map<std::string, std::unordered_set<std::string>> get_all_parent_star() const;

    std::unordered_set<std::string> get_all_parent_star_keys() const;

    std::unordered_set<std::string> get_all_parent_star_keys(const StatementType& statement_type) const;

    std::unordered_set<std::string> get_all_parent_star_values() const;

    std::unordered_set<std::string> get_all_parent_star_values(const StatementType& statement_type) const;

    std::unordered_set<std::string> get_children_star_of(const std::string& parent) const;

    std::unordered_set<std::string> get_children_star_of(const std::string& parent,
                                                         const StatementType& statement_type) const;

    std::unordered_set<std::string> get_parent_star_of(const std::string& child) const;

    std::unordered_set<std::string> get_parent_star_of(const std::string& child,
                                                       const StatementType& statement_type) const;

    // Next-related Read Operations
    bool has_next_relation(const std::string& before, const std::string& after) const;

    std::unordered_set<std::string> get_all_next_keys() const;

    std::unordered_set<std::string> get_all_next_values() const;

    std::unordered_set<std::string> get_next_of(const std::string& before) const;

    std::unordered_set<std::string> get_previous_of(const std::string& after) const;

    // Calls-related Read Operations
    bool has_calls_relation(const std::string& caller, const std::string& callee) const;

    std::unordered_set<std::string> get_all_calls_values() const;

    std::unordered_set<std::string> get_all_calls_keys() const;

    std::unordered_set<std::string> get_callees(const std::string& caller) const;

    std::unordered_set<std::string> get_callers(const std::string& callee) const;

    bool has_calls_star_relation(const std::string& caller, const std::string& callee) const;

    std::unordered_set<std::string> get_all_calls_star_values() const;

    std::unordered_set<std::string> get_all_calls_star_keys() const;

    std::unordered_set<std::string> get_star_callees(const std::string& caller) const;

    std::unordered_set<std::string> get_star_callers(const std::string& callee) const;

    // Assignment pattern-related Read Operations
    std::unordered_set<std::string> get_all_assignments_rhs(const std::string& rhs);

    std::unordered_set<std::string> get_all_assignments_rhs_partial(const std::string& rhs);

    std::unordered_set<std::string> get_all_assignments_lhs(const std::string& lhs);

    std::unordered_set<std::string> get_all_assignments_lhs_rhs(const std::string& lhs, const std::string& rhs);

    std::unordered_set<std::string> get_all_assignments_lhs_rhs_partial(const std::string& lhs, const std::string& rhs);

    // If pattern-related Read Operations
    std::unordered_set<std::string> get_if_stmts_with_var();

    std::unordered_set<std::string> get_if_stmts_with_var(const std::string& variable);

    std::unordered_set<std::string> get_vars_in_any_if();

    std::unordered_set<std::string> get_vars_in_if(const std::string& if_stmt);

    std::unordered_set<std::tuple<std::string, std::string>> get_all_if_stmt_var_pairs();

    // While pattern-related Read Operations
    std::unordered_set<std::string> get_while_stmts_with_var();

    std::unordered_set<std::string> get_while_stmts_with_var(const std::string& variable);

    std::unordered_set<std::string> get_vars_in_any_while();

    std::unordered_set<std::string> get_vars_in_while(const std::string& while_stmt);

    std::unordered_set<std::tuple<std::string, std::string>> get_all_while_stmt_var_pairs();

    void add_procedure(std::string procedure);

    void add_variable(std::string variable);

    void add_constant(std::string constant);

    void add_statement(const std::string& statement_number, StatementType type);

    void add_statement_modifies_var(const std::string& statement_number, std::string variable);

    void add_procedure_modifies_var(std::string procedure, std::string variable);

    void add_statement_uses_var(const std::string& statement_number, std::string variable);

    void add_procedure_uses_var(std::string procedure, std::string variable);

    void add_follows(const std::string& stmt1, const std::string& stmt2);

    void add_parent(const std::string& parent, const std::string& child);

    void add_assignment(const std::string& statement_number, const std::string& lhs, const std::string& rhs);

    void add_next(const std::string& stmt1, const std::string& stmt2);

    void add_calls(const std::string& caller, const std::string& callee);

    void add_if_var(const std::string& statement_number, const std::string& variable);

    void add_while_var(const std::string& statement_number, const std::string& variable);

    void finalise_pkb();

  private:
    std::shared_ptr<EntityStore> entity_store;
    std::shared_ptr<StatementStore> statement_store;
    std::shared_ptr<DirectFollowsStore> direct_follows_store;
    std::shared_ptr<FollowsStarStore> follows_star_store;
    std::shared_ptr<DirectParentStore> direct_parent_store;
    std::shared_ptr<ParentStarStore> parent_star_store;
    std::shared_ptr<ProcedureModifiesStore> procedure_modifies_store;
    std::shared_ptr<StatementModifiesStore> statement_modifies_store;
    std::shared_ptr<ProcedureUsesStore> procedure_uses_store;
    std::shared_ptr<StatementUsesStore> statement_uses_store;
    std::shared_ptr<AssignmentStore> assignment_store;
    std::shared_ptr<NextStore> next_store;
    std::shared_ptr<DirectCallsStore> direct_calls_store;
    std::shared_ptr<CallsStarStore> calls_star_store;
    std::shared_ptr<IfVarStore> if_var_store;
    std::shared_ptr<WhileVarStore> while_var_store;

    template <class DirectStore, class StarStore>
    void populate_star_from_direct(DirectStore direct_store, StarStore star_store);

    template <class DirectStore, class StarStore>
    void populate_call_star_from_direct(DirectStore direct_store, StarStore star_store);

    friend class ReadFacade;

    friend class WriteFacade;
};
