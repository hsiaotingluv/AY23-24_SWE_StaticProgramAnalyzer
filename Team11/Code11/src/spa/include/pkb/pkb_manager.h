#pragma once

#include "common/hashable_tuple.h"
#include "pkb/stores/calls_store/calls_star_store.h"
#include "pkb/stores/calls_store/direct_calls_store.h"
#include "pkb/stores/calls_store/stmt_no_to_proc_called_store.h"
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
#include "pkb/stores/proc_to_stmt_nos_store.h"
#include "pkb/stores/statement_store.h"
#include "pkb/stores/uses_store/procedure_uses_store.h"
#include "pkb/stores/uses_store/statement_uses_store.h"
#include <memory>
#include <tuple>

// Forward declaration of facades
namespace pkb {
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
            temp.insert(entity.get_name());
        }

        return temp;
    }

    template <class T>
    std::unordered_set<std::tuple<std::string, std::string>>
    get_tuple_list_from_string_entity_pairs(const std::unordered_set<T>& pairs) const {
        std::unordered_set<std::tuple<std::string, std::string>> temp;
        for (const auto& [s, v] : pairs) {
            temp.insert(std::make_tuple(s, v.get_name()));
        }

        return temp;
    }

    template <class T>
    std::unordered_set<std::tuple<std::string, std::string>>
    get_tuple_list_from_entity_string_pairs(const std::unordered_set<T>& pairs) const {
        std::unordered_set<std::tuple<std::string, std::string>> temp;
        for (const auto& [v, s] : pairs) {
            temp.insert(std::make_tuple(s, v.get_name()));
        }

        return temp;
    }

    template <class T>
    std::unordered_set<std::tuple<std::string, std::string>>
    get_tuple_list_from_entity_entity_pairs(const std::unordered_set<T>& pairs) const {
        std::unordered_set<std::tuple<std::string, std::string>> temp;
        for (const auto& [p, v] : pairs) {
            temp.insert(std::make_tuple(p.get_name(), v.get_name()));
        }

        return temp;
    }

    PkbManager();

    // Read APIs
    // Entity-related Read Operations
    std::unordered_set<std::string> get_entities() const;

    std::unordered_set<std::string> get_procedures() const;

    std::unordered_set<std::string> get_variables() const;

    std::unordered_set<std::string> get_constants() const;

    bool has_entity(const std::string& entity) const;

    bool has_procedure(const std::string& procedure) const;

    bool has_variable(const std::string& variable) const;

    bool has_constant(const std::string& constant) const;

    // Statement-related Read Operations
    std::unordered_set<std::string> get_all_statements() const;

    std::unordered_set<std::string> get_assign_statements() const;

    std::unordered_set<std::string> get_if_statements() const;

    std::unordered_set<std::string> get_while_statements() const;

    std::unordered_set<std::string> get_read_statements() const;

    std::unordered_set<std::string> get_print_statements() const;

    std::unordered_set<std::string> get_call_statements() const;

    bool has_statement(const std::string& s) const;

    bool has_assign_statement(const std::string& s) const;

    bool has_if_statement(const std::string& s) const;

    bool has_while_statement(const std::string& s) const;

    bool has_read_statement(const std::string& s) const;

    bool has_print_statement(const std::string& s) const;

    bool has_call_statement(const std::string& s) const;

    // Modifies-related Read Operations
    std::unordered_set<std::string> get_vars_modified_by_statement(const std::string& s) const;

    std::unordered_set<std::string> get_statements_that_modify_var(const std::string& variable) const;

    std::unordered_set<std::string> get_statements_that_modify_var(const std::string& variable,
                                                                   const StatementType& statement_type) const;

    bool has_statement_modify_var() const;

    bool contains_statement_modify_var(const std::string& statement_number, const std::string& variable) const;

    bool contains_statement_modify_var_key(const std::string& statement_number) const;

    bool contains_statement_modify_var_value(const std::string& variable) const;

    std::unordered_set<std::string> get_vars_modified_by_procedure(const std::string& procedure) const;

    std::unordered_set<std::string> get_procedures_that_modify_var(const std::string& variable) const;

    bool has_procedure_modify_var() const;

    bool contains_procedure_modify_var(const std::string& procedure, const std::string& variable) const;

    bool contains_procedure_modify_var_key(const std::string& procedure) const;

    bool contains_procedure_modify_var_value(const std::string& variable) const;

    std::unordered_set<std::string> get_all_statements_that_modify() const;

    std::unordered_set<std::string> get_all_statements_that_modify(const StatementType& statement_type) const;

    std::unordered_set<std::tuple<std::string, std::string>> get_all_statements_and_var_modify_pairs() const;

    std::unordered_set<std::tuple<std::string, std::string>>
    get_all_statements_and_var_modify_pairs(const StatementType& statement_type) const;

    std::unordered_set<std::string> get_all_procedures_that_modify() const;

    std::unordered_set<std::tuple<std::string, std::string>> get_all_procedures_and_var_modify_pairs() const;

    // Uses-related Read Operations
    std::unordered_set<std::string> get_vars_used_by_statement(const std::string& s) const;

    std::unordered_set<std::string> get_statements_that_use_var(const std::string& variable) const;

    std::unordered_set<std::string> get_statements_that_use_var(const std::string& variable,
                                                                const StatementType& statement_type) const;

    bool has_statement_use_var() const;

    bool contains_statement_use_var(const std::string& statement_number, const std::string& variable) const;

    bool contains_statement_use_var_key(const std::string& statement_number) const;

    bool contains_statement_use_var_value(const std::string& variable) const;

    std::unordered_set<std::string> get_vars_used_by_procedure(const std::string& procedure) const;

    std::unordered_set<std::string> get_procedures_that_use_var(const std::string& variable) const;

    bool has_procedure_use_var() const;

    bool contains_procedure_use_var(const std::string& procedure, const std::string& variable) const;

    bool contains_procedure_use_var_key(const std::string& statement_number) const;

    bool contains_procedure_use_var_value(const std::string& variable) const;

    std::unordered_set<std::string> get_all_statements_that_use() const;

    std::unordered_set<std::string> get_all_statements_that_use(const StatementType& statement_type) const;

    std::unordered_set<std::tuple<std::string, std::string>> get_all_statements_and_var_use_pairs() const;

    std::unordered_set<std::tuple<std::string, std::string>>
    get_all_statements_and_var_use_pairs(const StatementType& statement_type) const;

    std::unordered_set<std::string> get_all_procedures_that_use() const;

    std::unordered_set<std::tuple<std::string, std::string>> get_all_procedures_and_var_use_pairs() const;

    // Follows-related Read Operations
    bool has_follows_relation() const;

    bool has_follows_relation(const std::string& stmt1, const std::string& stmt2) const;

    bool contains_follows_key(const std::string& stmt) const;

    bool contains_follows_value(const std::string& stmt) const;

    std::unordered_map<std::string, std::string> get_all_follows() const;

    std::unordered_set<std::string> get_all_follows_keys() const;

    std::unordered_set<std::string> get_all_follows_keys(const StatementType& statement_type) const;

    std::unordered_set<std::string> get_all_follows_values() const;

    std::unordered_set<std::string> get_all_follows_values(const StatementType& statement_type) const;

    std::string get_statement_following(const std::string& s) const;

    std::string get_statement_following(const std::string& s, const StatementType& statement_type) const;

    std::string get_statement_followed_by(const std::string& s) const;

    std::string get_statement_followed_by(const std::string& s, const StatementType& statement_type) const;

    bool has_follows_star_relation() const;

    bool has_follows_star_relation(const std::string& stmt1, const std::string& stmt2) const;

    bool contains_follows_star_key(const std::string& stmt) const;

    bool contains_follows_star_value(const std::string& stmt) const;

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
    bool has_parent_relation() const;

    bool has_parent_relation(const std::string& parent, const std::string& child) const;

    bool contains_parent_key(const std::string& parent) const;

    bool contains_parent_value(const std::string& child) const;

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

    bool has_parent_star_relation() const;

    bool has_parent_star_relation(const std::string& parent, const std::string& child) const;

    bool contains_parent_star_key(const std::string& parent) const;

    bool contains_parent_star_value(const std::string& child) const;

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
    bool has_next_relation() const;

    bool has_next_relation(const std::string& before, const std::string& after) const;

    bool contains_next_key(const std::string& before) const;

    bool contains_next_value(const std::string& after) const;

    std::unordered_set<std::string> get_all_next_keys() const;

    std::unordered_set<std::string> get_all_next_values() const;

    std::unordered_set<std::string> get_next_of(const std::string& before) const;

    std::unordered_set<std::string> get_previous_of(const std::string& after) const;

    std::unordered_map<std::string, std::unordered_set<std::string>> get_all_next() const;

    std::unordered_map<std::string, std::unordered_set<std::string>> get_all_next_reverse() const;

    // Calls-related Read Operations
    bool has_calls_relation() const;

    bool has_calls_relation(const std::string& caller, const std::string& callee) const;

    bool contains_calls_key(const std::string& caller) const;

    bool contains_calls_value(const std::string& callee) const;

    std::unordered_set<std::string> get_all_calls_values() const;

    std::unordered_set<std::string> get_all_calls_keys() const;

    std::unordered_set<std::string> get_callees(const std::string& caller) const;

    std::unordered_set<std::string> get_callers(const std::string& callee) const;

    std::string get_procedure_name_called_by(const std::string& stmt_number) const;

    bool has_calls_star_relation() const;

    bool has_calls_star_relation(const std::string& caller, const std::string& callee) const;

    bool contains_calls_star_key(const std::string& caller) const;

    bool contains_calls_star_value(const std::string& callee) const;

    std::unordered_set<std::string> get_all_calls_star_values() const;

    std::unordered_set<std::string> get_all_calls_star_keys() const;

    std::unordered_set<std::string> get_star_callees(const std::string& caller) const;

    std::unordered_set<std::string> get_star_callers(const std::string& callee) const;

    // Assignment pattern-related Read Operations
    std::unordered_set<std::string> get_all_assignments_rhs(const std::string& rhs) const;

    std::unordered_set<std::string> get_all_assignments_rhs_partial(const std::string& rhs) const;

    std::unordered_set<std::string> get_all_assignments_lhs(const std::string& lhs) const;

    std::unordered_set<std::string> get_all_assignments_lhs_rhs(const std::string& lhs, const std::string& rhs) const;

    std::unordered_set<std::string> get_all_assignments_lhs_rhs_partial(const std::string& lhs,
                                                                        const std::string& rhs) const;

    // If pattern-related Read Operations
    std::unordered_set<std::string> get_if_stmts_with_var() const;

    std::unordered_set<std::string> get_if_stmts_with_var(const std::string& variable) const;

    std::unordered_set<std::string> get_vars_in_any_if() const;

    std::unordered_set<std::string> get_vars_in_if(const std::string& if_stmt) const;

    std::unordered_set<std::tuple<std::string, std::string>> get_all_if_stmt_var_pairs() const;

    // While pattern-related Read Operations
    std::unordered_set<std::string> get_while_stmts_with_var() const;

    std::unordered_set<std::string> get_while_stmts_with_var(const std::string& variable) const;

    std::unordered_set<std::string> get_vars_in_any_while() const;

    std::unordered_set<std::string> get_vars_in_while(const std::string& while_stmt) const;

    std::unordered_set<std::tuple<std::string, std::string>> get_all_while_stmt_var_pairs() const;

    // Read operations for procedure to statement number store
    std::unordered_map<std::string, std::unordered_set<std::string>> get_all_proc_to_stmts_nos_map() const;

    std::unordered_set<std::string> get_all_stmts_nos_by_proc(const std::string& proc_name) const;

    std::string get_proc_name_by_stmt_no(const std::string& stmt_no) const;

    bool are_stmt_nos_in_same_proc(const std::string& stmt_no_1, const std::string& stmt_no_2) const;

    // Write APIs
    void add_procedure(std::string procedure);

    void add_variable(std::string variable);

    void add_constant(std::string constant);

    void add_statement(const std::string& statement_number, StatementType type);

    void add_statement_modify_var(const std::string& statement_number, std::string variable);

    void add_procedure_modify_var(std::string procedure, std::string variable);

    void add_statement_use_var(const std::string& statement_number, std::string variable);

    void add_procedure_use_var(std::string procedure, std::string variable);

    void add_follows(const std::string& stmt1, const std::string& stmt2);

    void add_parent(const std::string& parent, const std::string& child);

    void add_assignment(const std::string& statement_number, const std::string& lhs, const std::string& rhs);

    void add_next(const std::string& stmt1, const std::string& stmt2);

    void add_calls(const std::string& caller, const std::string& callee);

    void add_stmt_no_proc_called_mapping(const std::string& stmt_no, const std::string& proc_called);

    void add_if_var(const std::string& statement_number, const std::string& variable);

    void add_while_var(const std::string& statement_number, const std::string& variable);

    void add_proc_to_stmt_no_mapping(const std::string& procedure, const std::string& stmt_no);

    void finalise_pkb(const std::vector<std::string>& procedure_order);

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
    std::shared_ptr<StmtNoToProcCalledStore> stmt_no_to_proc_called_store;
    std::shared_ptr<ProcToStmtNosStore> proc_to_stmt_nos_store;

    template <class DirectStore, class StarStore, class OrderingStrategy>
    void populate_star_from_direct(std::shared_ptr<DirectStore> direct_store, std::shared_ptr<StarStore> star_store,
                                   OrderingStrategy ordering_strategy);

    template <class DirectStore, class StarStore>
    void populate_call_star_from_direct(DirectStore direct_store, StarStore star_store);

    friend class ReadFacade;

    friend class WriteFacade;
};
} // namespace pkb