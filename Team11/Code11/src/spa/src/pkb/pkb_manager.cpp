#include "pkb/pkb_manager.h"
#include "common/utils/algo.h"
#include "pkb/facades/read_facade.h"
#include "pkb/facades/write_facade.h"

#include <algorithm>
#include <tuple>
#include <vector>

namespace pkb {
PkbManager::PkbManager()
    : entity_store(std::make_shared<EntityStore>()), statement_store(std::make_shared<StatementStore>()),
      direct_follows_store(std::make_shared<DirectFollowsStore>()),
      follows_star_store(std::make_shared<FollowsStarStore>()),
      direct_parent_store(std::make_shared<DirectParentStore>()),
      parent_star_store(std::make_shared<ParentStarStore>()),
      procedure_modifies_store(std::make_shared<ProcedureModifiesStore>()),
      statement_modifies_store(std::make_shared<StatementModifiesStore>()),
      procedure_uses_store(std::make_shared<ProcedureUsesStore>()),
      statement_uses_store(std::make_shared<StatementUsesStore>()),
      assignment_store(std::make_shared<AssignmentStore>()), next_store(std::make_shared<NextStore>()),
      direct_calls_store(std::make_shared<DirectCallsStore>()), calls_star_store(std::make_shared<CallsStarStore>()),
      if_var_store(std::make_shared<IfVarStore>()), while_var_store(std::make_shared<WhileVarStore>()),
      stmt_no_to_proc_called_store(std::make_shared<StmtNoToProcCalledStore>()),
      proc_to_stmt_nos_store(std::make_shared<ProcToStmtNosStore>()) {
}

auto PkbManager::create_facades() -> std::tuple<std::shared_ptr<ReadFacade>, std::shared_ptr<WriteFacade>> {
    auto pkb = std::make_shared<PkbManager>();
    auto read_facade = std::make_shared<ReadFacade>(pkb);
    auto write_facade = std::make_shared<WriteFacade>(pkb);

    return {std::move(read_facade), std::move(write_facade)};
}

// ReadFacade APIs
std::unordered_set<std::string> PkbManager::get_entities() const {
    std::unordered_set<std::string> entities;

    auto procedures = entity_store->get_procedures();
    auto vars = entity_store->get_variables();
    auto consts = entity_store->get_constants();

    for (const Procedure& p : procedures) {
        entities.insert(p.get_name());
    }
    for (const Variable& v : vars) {
        entities.insert(v.get_name());
    }
    for (const Constant& c : consts) {
        entities.insert(c.get_name());
    }

    return entities;
}

std::unordered_set<std::string> PkbManager::get_procedures() const {
    auto procedures = entity_store->get_procedures();
    return get_name_list(procedures);
}

std::unordered_set<std::string> PkbManager::get_variables() const {
    auto vars = entity_store->get_variables();
    return get_name_list(vars);
}

std::unordered_set<std::string> PkbManager::get_constants() const {
    auto consts = entity_store->get_constants();
    return get_name_list(consts);
}

bool PkbManager::has_entity(const std::string& entity) const {
    std::unordered_set<std::string> entities;

    auto procedures = entity_store->get_procedures();
    auto vars = entity_store->get_variables();
    auto consts = entity_store->get_constants();

    for (const Procedure& p : procedures) {
        if (p.get_name() == entity) {
            return true;
        }
    }
    for (const Variable& v : vars) {
        if (v.get_name() == entity) {
            return true;
        }
    }
    for (const Constant& c : consts) {
        if (c.get_name() == entity) {
            return true;
        }
    }

    return false; // Entity not found in any category
}

bool PkbManager::has_procedure(const std::string& procedure) const {
    return entity_store->has_procedure(procedure);
}

bool PkbManager::has_variable(const std::string& variable) const {
    return entity_store->has_variable(variable);
}

bool PkbManager::has_constant(const std::string& constant) const {
    return entity_store->has_constant(constant);
}

std::unordered_set<std::string> PkbManager::get_all_statements() const {
    return statement_store->get_all_keys();
}

std::unordered_set<std::string> PkbManager::get_assign_statements() const {
    return statement_store->get_keys_by_val(StatementType::Assign);
}

std::unordered_set<std::string> PkbManager::get_if_statements() const {
    return statement_store->get_keys_by_val(StatementType::If);
}

std::unordered_set<std::string> PkbManager::get_while_statements() const {
    return statement_store->get_keys_by_val(StatementType::While);
}

std::unordered_set<std::string> PkbManager::get_read_statements() const {
    return statement_store->get_keys_by_val(StatementType::Read);
}

std::unordered_set<std::string> PkbManager::get_print_statements() const {
    return statement_store->get_keys_by_val(StatementType::Print);
}

std::unordered_set<std::string> PkbManager::get_call_statements() const {
    return statement_store->get_keys_by_val(StatementType::Call);
}

bool PkbManager::has_statement(const std::string& s) const {
    auto stmts = statement_store->get_all_keys();
    return stmts.find(s) != stmts.end();
}

bool PkbManager::has_assign_statement(const std::string& s) const {
    auto stmts = statement_store->get_keys_by_val(StatementType::Assign);
    return stmts.find(s) != stmts.end();
}

bool PkbManager::has_if_statement(const std::string& s) const {
    auto stmts = statement_store->get_keys_by_val(StatementType::If);
    return stmts.find(s) != stmts.end();
}

bool PkbManager::has_while_statement(const std::string& s) const {
    auto stmts = statement_store->get_keys_by_val(StatementType::While);
    return stmts.find(s) != stmts.end();
}

bool PkbManager::has_read_statement(const std::string& s) const {
    auto stmts = statement_store->get_keys_by_val(StatementType::Read);
    return stmts.find(s) != stmts.end();
}

bool PkbManager::has_print_statement(const std::string& s) const {
    auto stmts = statement_store->get_keys_by_val(StatementType::Print);
    return stmts.find(s) != stmts.end();
}

bool PkbManager::has_call_statement(const std::string& s) const {
    auto stmts = statement_store->get_keys_by_val(StatementType::Call);
    return stmts.find(s) != stmts.end();
}

std::unordered_set<std::string> PkbManager::get_vars_modified_by_statement(const std::string& s) const {
    auto variables = statement_modifies_store->get_vals_by_key(s);
    return get_name_list(variables);
}

std::unordered_set<std::string> PkbManager::get_statements_that_modify_var(const std::string& variable) const {
    auto v = Variable(variable);
    return statement_modifies_store->get_keys_by_val(v);
}

std::unordered_set<std::string> PkbManager::get_statements_that_modify_var(const std::string& variable,
                                                                           const StatementType& statement_type) const {
    auto stmts_pool = get_statements_that_modify_var(variable);
    return filter_by_statement_type(stmts_pool, statement_type);
}

bool PkbManager::has_statement_modify_var() const {
    return statement_modifies_store->has_relationship();
}

bool PkbManager::contains_statement_modify_var(const std::string& statement, const std::string& variable) const {
    auto v = Variable(variable);
    return statement_modifies_store->contains_key_val_pair(statement, v);
}

bool PkbManager::contains_statement_modify_var_key(const std::string& statement_number) const {
    return statement_modifies_store->contains_key(statement_number);
}

bool PkbManager::contains_statement_modify_var_value(const std::string& variable) const {
    auto v = Variable(variable);
    return statement_modifies_store->contains_val(v);
}

std::unordered_set<std::string> PkbManager::get_all_statements_that_modify() const {
    return statement_modifies_store->get_all_keys();
}

std::unordered_set<std::string> PkbManager::get_all_statements_that_modify(const StatementType& statement_type) const {
    auto stmts_pool = get_all_statements_that_modify();
    return filter_by_statement_type(stmts_pool, statement_type);
}

std::unordered_set<std::tuple<std::string, std::string>> PkbManager::get_all_statements_and_var_modify_pairs() const {
    auto pairs = statement_modifies_store->get_all_pairs();
    return get_tuple_list_from_string_entity_pairs(pairs);
}

std::unordered_set<std::tuple<std::string, std::string>>
PkbManager::get_all_statements_and_var_modify_pairs(const StatementType& statement_type) const {
    auto pairs = get_all_statements_and_var_modify_pairs();
    return filter_by_statement_type(pairs, statement_type, tuple_stmt_no_extractor);
}

std::unordered_set<std::string> PkbManager::get_all_procedures_that_modify() const {
    auto procedures = procedure_modifies_store->get_all_keys();
    return get_name_list(procedures);
}

bool PkbManager::has_procedure_modify_var() const {
    return procedure_modifies_store->has_relationship();
}

bool PkbManager::contains_procedure_modify_var(const std::string& procedure, const std::string& variable) const {
    auto v = Variable(variable);
    auto p = Procedure(procedure);
    return procedure_modifies_store->contains_key_val_pair(p, v);
}

bool PkbManager::contains_procedure_modify_var_key(const std::string& procedure) const {
    auto p = Procedure(procedure);
    return procedure_modifies_store->contains_key(p);
}

bool PkbManager::contains_procedure_modify_var_value(const std::string& variable) const {
    auto v = Variable(variable);
    return procedure_modifies_store->contains_val(v);
}

std::unordered_set<std::tuple<std::string, std::string>> PkbManager::get_all_procedures_and_var_modify_pairs() const {
    auto pairs = procedure_modifies_store->get_all_pairs();
    return get_tuple_list_from_entity_entity_pairs(pairs);
}

std::unordered_set<std::string> PkbManager::get_vars_modified_by_procedure(const std::string& procedure) const {
    auto p = Procedure(procedure);
    auto variables = procedure_modifies_store->get_vals_by_key(p);
    return get_name_list(variables);
}

std::unordered_set<std::string> PkbManager::get_procedures_that_modify_var(const std::string& variable) const {
    auto v = Variable(variable);
    auto procedures = procedure_modifies_store->get_keys_by_val(v);
    return get_name_list(procedures);
}

std::unordered_set<std::string> PkbManager::get_vars_used_by_statement(const std::string& s) const {
    auto variables = statement_uses_store->get_vals_by_key(s);
    return get_name_list(variables);
}

std::unordered_set<std::string> PkbManager::get_statements_that_use_var(const std::string& variable) const {
    auto v = Variable(variable);
    return statement_uses_store->get_keys_by_val(v);
}

std::unordered_set<std::string> PkbManager::get_statements_that_use_var(const std::string& variable,
                                                                        const StatementType& statement_type) const {
    auto stmts_pool = get_statements_that_use_var(variable);
    return filter_by_statement_type(stmts_pool, statement_type);
}

bool PkbManager::has_statement_use_var() const {
    return statement_uses_store->has_relationship();
}

bool PkbManager::contains_statement_use_var(const std::string& statement, const std::string& variable) const {
    auto v = Variable(variable);
    return statement_uses_store->contains_key_val_pair(statement, v);
}

bool PkbManager::contains_statement_use_var_key(const std::string& statement_number) const {
    return statement_uses_store->contains_key(statement_number);
}

bool PkbManager::contains_statement_use_var_value(const std::string& variable) const {
    auto v = Variable(variable);
    return statement_uses_store->contains_val(v);
}

std::unordered_set<std::string> PkbManager::get_vars_used_by_procedure(const std::string& procedure) const {
    auto p = Procedure(procedure);
    auto variables = procedure_uses_store->get_vals_by_key(p);
    return get_name_list(variables);
}

std::unordered_set<std::string> PkbManager::get_procedures_that_use_var(const std::string& variable) const {
    auto v = Variable(variable);
    auto procedures = procedure_uses_store->get_keys_by_val(v);
    return get_name_list(procedures);
}

bool PkbManager::has_procedure_use_var() const {
    return procedure_uses_store->has_relationship();
}

bool PkbManager::contains_procedure_use_var(const std::string& procedure, const std::string& variable) const {
    auto v = Variable(variable);
    auto p = Procedure(procedure);
    return procedure_uses_store->contains_key_val_pair(p, v);
}

bool PkbManager::contains_procedure_use_var_key(const std::string& procedure) const {
    auto p = Procedure(procedure);
    return procedure_uses_store->contains_key(p);
}

bool PkbManager::contains_procedure_use_var_value(const std::string& variable) const {
    auto v = Variable(variable);
    return procedure_uses_store->contains_val(v);
}

std::unordered_set<std::string> PkbManager::get_all_statements_that_use() const {
    return statement_uses_store->get_all_keys();
}

std::unordered_set<std::string> PkbManager::get_all_statements_that_use(const StatementType& statement_type) const {
    auto stmts_pool = get_all_statements_that_use();
    return filter_by_statement_type(stmts_pool, statement_type);
}

std::unordered_set<std::tuple<std::string, std::string>> PkbManager::get_all_statements_and_var_use_pairs() const {
    auto pairs = statement_uses_store->get_all_pairs();
    return get_tuple_list_from_string_entity_pairs(pairs);
}

std::unordered_set<std::tuple<std::string, std::string>>
PkbManager::get_all_statements_and_var_use_pairs(const StatementType& statement_type) const {
    auto pairs = get_all_statements_and_var_use_pairs();
    return filter_by_statement_type(pairs, statement_type, tuple_stmt_no_extractor);
}

std::unordered_set<std::string> PkbManager::get_all_procedures_that_use() const {
    auto procedures = procedure_uses_store->get_all_keys();
    return get_name_list(procedures);
}

std::unordered_set<std::tuple<std::string, std::string>> PkbManager::get_all_procedures_and_var_use_pairs() const {
    auto pairs = procedure_uses_store->get_all_pairs();
    return get_tuple_list_from_entity_entity_pairs(pairs);
}

std::unordered_map<std::string, std::string> PkbManager::get_all_follows() const {
    return direct_follows_store->get_all();
}

std::unordered_set<std::string> PkbManager::get_all_follows_keys() const {
    return direct_follows_store->get_all_keys();
}

std::unordered_set<std::string> PkbManager::get_all_follows_keys(const StatementType& statement_type) const {
    auto stmts_pool = direct_follows_store->get_all_keys();
    return filter_by_statement_type(stmts_pool, statement_type);
}

std::unordered_set<std::string> PkbManager::get_all_follows_values() const {
    return direct_follows_store->get_all_vals();
}

std::unordered_set<std::string> PkbManager::get_all_follows_values(const StatementType& statement_type) const {
    auto stmts_pool = direct_follows_store->get_all_vals();
    return filter_by_statement_type(stmts_pool, statement_type);
}

bool PkbManager::has_follows_relation() const {
    return direct_follows_store->has_relationship();
}

bool PkbManager::has_follows_relation(const std::string& stmt1, const std::string& stmt2) const {
    return direct_follows_store->contains_key_val_pair(stmt1, stmt2);
}

bool PkbManager::contains_follows_key(const std::string& stmt) const {
    return direct_follows_store->contains_key(stmt);
}

bool PkbManager::contains_follows_value(const std::string& stmt) const {
    return direct_follows_store->contains_val(stmt);
}

std::string PkbManager::get_statement_following(const std::string& s) const {
    return direct_follows_store->get_val_by_key(s);
}

std::string PkbManager::get_statement_following(const std::string& s, const StatementType& statement_type) const {
    auto stmt = direct_follows_store->get_val_by_key(s);

    if (statement_store->get_val_by_key(stmt) == statement_type) {
        return stmt;
    }

    return "";
}

std::string PkbManager::get_statement_followed_by(const std::string& s) const {
    return direct_follows_store->get_key_by_val(s);
}

std::string PkbManager::get_statement_followed_by(const std::string& s, const StatementType& statement_type) const {
    auto stmt = direct_follows_store->get_key_by_val(s);

    if (statement_store->get_val_by_key(stmt) == statement_type) {
        return stmt;
    }

    return "";
}

bool PkbManager::has_follows_star_relation() const {
    return follows_star_store->has_relationship();
}

bool PkbManager::has_follows_star_relation(const std::string& stmt1, const std::string& stmt2) const {
    return follows_star_store->contains_key_val_pair(stmt1, stmt2);
}

bool PkbManager::contains_follows_star_key(const std::string& stmt) const {
    return follows_star_store->contains_key(stmt);
}

bool PkbManager::contains_follows_star_value(const std::string& stmt) const {
    return follows_star_store->contains_val(stmt);
}

std::unordered_map<std::string, std::unordered_set<std::string>> PkbManager::get_all_follows_star() const {
    return follows_star_store->get_all();
}

std::unordered_set<std::string> PkbManager::get_all_follows_star_keys() const {
    return follows_star_store->get_all_keys();
}

std::unordered_set<std::string> PkbManager::get_all_follows_star_keys(const StatementType& statement_type) const {
    auto stmts_pool = get_all_follows_star_keys();
    return filter_by_statement_type(stmts_pool, statement_type);
}

std::unordered_set<std::string> PkbManager::get_all_follows_star_values() const {
    return follows_star_store->get_all_vals();
}

std::unordered_set<std::string> PkbManager::get_all_follows_star_values(const StatementType& statement_type) const {
    auto stmts_pool = get_all_follows_star_values();
    return filter_by_statement_type(stmts_pool, statement_type);
}

std::unordered_set<std::string> PkbManager::get_follows_stars_following(const std::string& stmt) const {
    return follows_star_store->get_vals_by_key(stmt);
}

std::unordered_set<std::string> PkbManager::get_follows_stars_following(const std::string& stmt,
                                                                        const StatementType& statement_type) const {
    auto stmts_pool = get_follows_stars_following(stmt);
    return filter_by_statement_type(stmts_pool, statement_type);
}

std::unordered_set<std::string> PkbManager::get_follows_stars_by(const std::string& stmt) const {
    return follows_star_store->get_keys_by_val(stmt);
}

std::unordered_set<std::string> PkbManager::get_follows_stars_by(const std::string& stmt,
                                                                 const StatementType& statement_type) const {
    auto stmts_pool = get_follows_stars_by(stmt);
    return filter_by_statement_type(stmts_pool, statement_type);
}

bool PkbManager::has_parent_relation() const {
    return direct_parent_store->has_relationship();
}

bool PkbManager::has_parent_relation(const std::string& parent, const std::string& child) const {
    return direct_parent_store->contains_key_val_pair(parent, child);
}

bool PkbManager::contains_parent_key(const std::string& stmt) const {
    return direct_parent_store->contains_key(stmt);
}

bool PkbManager::contains_parent_value(const std::string& stmt) const {
    return direct_parent_store->contains_val(stmt);
}

std::unordered_map<std::string, std::unordered_set<std::string>> PkbManager::get_all_parent() const {
    return direct_parent_store->get_all();
}

std::unordered_set<std::string> PkbManager::get_all_parent_keys() const {
    return direct_parent_store->get_all_keys();
}

std::unordered_set<std::string> PkbManager::get_all_parent_keys(const StatementType& statement_type) const {
    auto stmts_pool = get_all_parent_keys();
    return filter_by_statement_type(stmts_pool, statement_type);
}

std::unordered_set<std::string> PkbManager::get_all_parent_values() const {
    return direct_parent_store->get_all_vals();
}

std::unordered_set<std::string> PkbManager::get_all_parent_values(const StatementType& statement_type) const {
    auto stmts_pool = get_all_parent_values();
    return filter_by_statement_type(stmts_pool, statement_type);
}

std::unordered_set<std::string> PkbManager::get_children_of(const std::string& parent) const {
    return direct_parent_store->get_vals_by_key(parent);
}

std::unordered_set<std::string> PkbManager::get_children_of(const std::string& parent,
                                                            const StatementType& statement_type) const {
    auto stmts_pool = get_children_of(parent);
    return filter_by_statement_type(stmts_pool, statement_type);
}

std::string PkbManager::get_parent_of(const std::string& child) const {
    return direct_parent_store->get_key_by_val(child);
}

std::string PkbManager::get_parent_of(const std::string& child, const StatementType& statement_type) const {
    auto stmt = get_parent_of(child);

    if (statement_store->get_val_by_key(stmt) == statement_type) {
        return stmt;
    }

    return "";
}

bool PkbManager::has_parent_star_relation() const {
    return parent_star_store->has_relationship();
}

bool PkbManager::has_parent_star_relation(const std::string& parent, const std::string& child) const {
    return parent_star_store->contains_key_val_pair(parent, child);
}

bool PkbManager::contains_parent_star_key(const std::string& parent) const {
    return parent_star_store->contains_key(parent);
}

bool PkbManager::contains_parent_star_value(const std::string& child) const {
    return parent_star_store->contains_val(child);
}

std::unordered_map<std::string, std::unordered_set<std::string>> PkbManager::get_all_parent_star() const {
    return parent_star_store->get_all();
}

std::unordered_set<std::string> PkbManager::get_all_parent_star_keys() const {
    return parent_star_store->get_all_keys();
}

std::unordered_set<std::string> PkbManager::get_all_parent_star_keys(const StatementType& statement_type) const {
    auto stmts_pool = get_all_parent_star_keys();
    return filter_by_statement_type(stmts_pool, statement_type);
}

std::unordered_set<std::string> PkbManager::get_all_parent_star_values() const {
    return parent_star_store->get_all_vals();
}

std::unordered_set<std::string> PkbManager::get_all_parent_star_values(const StatementType& statement_type) const {
    auto stmts_pool = get_all_parent_star_values();
    return filter_by_statement_type(stmts_pool, statement_type);
}

std::unordered_set<std::string> PkbManager::get_children_star_of(const std::string& parent) const {
    return parent_star_store->get_vals_by_key(parent);
}

std::unordered_set<std::string> PkbManager::get_children_star_of(const std::string& parent,
                                                                 const StatementType& statement_type) const {
    auto stmts_pool = get_children_star_of(parent);
    return filter_by_statement_type(stmts_pool, statement_type);
}

std::unordered_set<std::string> PkbManager::get_parent_star_of(const std::string& child) const {
    return parent_star_store->get_keys_by_val(child);
}

std::unordered_set<std::string> PkbManager::get_parent_star_of(const std::string& child,
                                                               const StatementType& statement_type) const {
    auto stmts_pool = get_parent_star_of(child);
    return filter_by_statement_type(stmts_pool, statement_type);
}

bool PkbManager::has_next_relation() const {
    return next_store->has_relationship();
}

bool PkbManager::has_next_relation(const std::string& before, const std::string& after) const {
    return next_store->contains_key_val_pair(before, after);
}

bool PkbManager::contains_next_key(const std::string& before) const {
    return next_store->contains_key(before);
}

bool PkbManager::contains_next_value(const std::string& after) const {
    return next_store->contains_val(after);
}

std::unordered_set<std::string> PkbManager::get_all_next_keys() const {
    return next_store->get_all_keys();
}

std::unordered_set<std::string> PkbManager::get_all_next_values() const {
    return next_store->get_all_vals();
}

std::unordered_set<std::string> PkbManager::get_next_of(const std::string& before) const {
    return next_store->get_vals_by_key(before);
}

std::unordered_set<std::string> PkbManager::get_previous_of(const std::string& after) const {
    return next_store->get_keys_by_val(after);
}

std::unordered_map<std::string, std::unordered_set<std::string>> PkbManager::get_all_next() const {
    return next_store->get_all();
}

std::unordered_map<std::string, std::unordered_set<std::string>> PkbManager::get_all_next_reverse() const {
    return next_store->get_all_reverse();
}

bool PkbManager::has_calls_relation() const {
    return direct_calls_store->has_relationship();
}

bool PkbManager::has_calls_relation(const std::string& caller, const std::string& callee) const {
    return direct_calls_store->contains_key_val_pair(Procedure(caller), Procedure(callee));
}

bool PkbManager::contains_calls_key(const std::string& caller) const {
    return direct_calls_store->contains_key(Procedure(caller));
}

bool PkbManager::contains_calls_value(const std::string& callee) const {
    return direct_calls_store->contains_val(Procedure(callee));
}

std::unordered_set<std::string> PkbManager::get_all_calls_values() const {
    auto procedures = direct_calls_store->get_all_vals();
    return get_name_list(procedures);
}

std::unordered_set<std::string> PkbManager::get_all_calls_keys() const {
    auto procedures = direct_calls_store->get_all_keys();
    return get_name_list(procedures);
}

std::unordered_set<std::string> PkbManager::get_callees(const std::string& caller) const {
    auto procedures = direct_calls_store->get_vals_by_key(Procedure(caller));
    return get_name_list(procedures);
}

std::unordered_set<std::string> PkbManager::get_callers(const std::string& callee) const {
    auto procedures = direct_calls_store->get_keys_by_val(Procedure(callee));
    return get_name_list(procedures);
}

std::string PkbManager::get_procedure_name_called_by(const std::string& stmt_number) const {
    Procedure p = stmt_no_to_proc_called_store->get_val_by_key(stmt_number);
    return p.get_name();
}

bool PkbManager::has_calls_star_relation() const {
    return calls_star_store->has_relationship();
}

bool PkbManager::has_calls_star_relation(const std::string& caller, const std::string& callee) const {
    return calls_star_store->contains_key_val_pair(Procedure(caller), Procedure(callee));
}

bool PkbManager::contains_calls_star_key(const std::string& caller) const {
    return calls_star_store->contains_key(Procedure(caller));
}

bool PkbManager::contains_calls_star_value(const std::string& callee) const {
    return calls_star_store->contains_val(Procedure(callee));
}

std::unordered_set<std::string> PkbManager::get_all_calls_star_values() const {
    auto procedures = calls_star_store->get_all_vals();
    return get_name_list(procedures);
}

std::unordered_set<std::string> PkbManager::get_all_calls_star_keys() const {
    auto procedures = calls_star_store->get_all_keys();
    return get_name_list(procedures);
}

std::unordered_set<std::string> PkbManager::get_star_callees(const std::string& caller) const {
    auto procedures = calls_star_store->get_vals_by_key(Procedure(caller));
    return get_name_list(procedures);
}

std::unordered_set<std::string> PkbManager::get_star_callers(const std::string& callee) const {
    auto procedures = calls_star_store->get_keys_by_val(Procedure(callee));
    return get_name_list(procedures);
}

std::unordered_set<std::string> PkbManager::get_all_assignments_rhs(const std::string& rhs) const {
    return assignment_store->get_all_assignments_rhs(rhs);
}

std::unordered_set<std::string> PkbManager::get_all_assignments_rhs_partial(const std::string& rhs) const {
    return assignment_store->get_all_assignments_rhs_partial(rhs);
}

std::unordered_set<std::string> PkbManager::get_all_assignments_lhs(const std::string& lhs) const {
    auto v = Variable(lhs);
    return assignment_store->get_all_assignments_lhs(v);
}

std::unordered_set<std::string> PkbManager::get_all_assignments_lhs_rhs(const std::string& lhs,
                                                                        const std::string& rhs) const {
    auto v = Variable(lhs);
    return assignment_store->get_all_assignments_lhs_rhs(v, rhs);
}

std::unordered_set<std::string> PkbManager::get_all_assignments_lhs_rhs_partial(const std::string& lhs,
                                                                                const std::string& rhs) const {
    auto v = Variable(lhs);
    return assignment_store->get_all_assignments_lhs_rhs_partial(v, rhs);
}

std::unordered_set<std::string> PkbManager::get_if_stmts_with_var() const {
    return if_var_store->get_all_vals();
}

std::unordered_set<std::string> PkbManager::get_if_stmts_with_var(const std::string& variable) const {
    auto v = Variable(variable);
    return if_var_store->get_vals_by_key(v);
}

std::unordered_set<std::string> PkbManager::get_vars_in_any_if() const {
    auto temp = if_var_store->get_all_keys();
    return get_name_list(temp);
}

std::unordered_set<std::string> PkbManager::get_vars_in_if(const std::string& if_stmt) const {
    auto temp = if_var_store->get_keys_by_val(if_stmt);
    return get_name_list(temp);
}

std::unordered_set<std::tuple<std::string, std::string>> PkbManager::get_all_if_stmt_var_pairs() const {
    auto pairs = if_var_store->get_all_pairs();
    return get_tuple_list_from_entity_string_pairs(pairs);
}

std::unordered_set<std::string> PkbManager::get_while_stmts_with_var() const {
    return while_var_store->get_all_vals();
}

std::unordered_set<std::string> PkbManager::get_while_stmts_with_var(const std::string& variable) const {
    auto v = Variable(variable);
    return while_var_store->get_vals_by_key(v);
}

std::unordered_set<std::string> PkbManager::get_vars_in_any_while() const {
    auto temp = while_var_store->get_all_keys();
    return get_name_list(temp);
}

std::unordered_set<std::string> PkbManager::get_vars_in_while(const std::string& while_stmt) const {
    auto temp = while_var_store->get_keys_by_val(while_stmt);
    return get_name_list(temp);
}

std::unordered_set<std::tuple<std::string, std::string>> PkbManager::get_all_while_stmt_var_pairs() const {
    auto pairs = while_var_store->get_all_pairs();
    return get_tuple_list_from_entity_string_pairs(pairs);
}

std::unordered_map<std::string, std::unordered_set<std::string>> PkbManager::get_all_proc_to_stmts_nos_map() const {
    auto temp = proc_to_stmt_nos_store->get_all();

    std::unordered_map<std::string, std::unordered_set<std::string>> result;

    for (const auto& [p, stmt_nos] : temp) {
        result[p.get_name()] = stmt_nos;
    }

    return result;
}

std::unordered_set<std::string> PkbManager::get_all_stmts_nos_by_proc(const std::string& proc_name) const {
    auto p = Procedure(proc_name);
    return proc_to_stmt_nos_store->get_vals_by_key(p);
}

std::string PkbManager::get_proc_name_by_stmt_no(const std::string& stmt_no) const {
    auto p = proc_to_stmt_nos_store->get_key_by_val(stmt_no);
    return p.get_name();
}

bool PkbManager::are_stmt_nos_in_same_proc(const std::string& stmt_no_1, const std::string& stmt_no_2) const {
    auto p1 = proc_to_stmt_nos_store->get_key_by_val(stmt_no_1);
    auto p2 = proc_to_stmt_nos_store->get_key_by_val(stmt_no_2);
    return p1 == p2;
}

// WriteFacade APIs
void PkbManager::add_procedure(std::string procedure) {
    Procedure p = Procedure(std::move(procedure));
    entity_store->add_procedure(p);
}

void PkbManager::add_variable(std::string variable) {
    Variable v = Variable(std::move(variable));
    entity_store->add_variable(v);
}

void PkbManager::add_constant(std::string constant) {
    Constant c = Constant(std::move(constant));
    entity_store->add_constant(c);
}

void PkbManager::add_statement(const std::string& statement_number, StatementType statement_type) {
    statement_store->add(statement_number, statement_type);
}

void PkbManager::add_statement_modify_var(const std::string& statement_number, std::string variable) {
    auto v = Variable(std::move(variable));
    statement_modifies_store->add(statement_number, v);
}

void PkbManager::add_procedure_modify_var(std::string procedure, std::string variable) {
    auto p = Procedure(std::move(procedure));
    auto v = Variable(std::move(variable));
    procedure_modifies_store->add(p, v);
}

void PkbManager::add_statement_use_var(const std::string& statement_number, std::string variable) {
    auto v = Variable(std::move(variable));
    statement_uses_store->add(statement_number, v);
}

void PkbManager::add_procedure_use_var(std::string procedure, std::string variable) {
    auto p = Procedure(std::move(procedure));
    auto v = Variable(std::move(variable));
    procedure_uses_store->add(p, v);
}

void PkbManager::add_follows(const std::string& stmt1, const std::string& stmt2) {
    direct_follows_store->add(stmt1, stmt2);
}

void PkbManager::add_parent(const std::string& parent, const std::string& child) {
    direct_parent_store->add(parent, child);
}

void PkbManager::add_assignment(const std::string& statement_number, const std::string& lhs, const std::string& rhs) {
    auto v = Variable(lhs);
    assignment_store->add_assignment(statement_number, v, rhs);
}

void PkbManager::add_if_var(const std::string& statement_number, const std::string& variable) {
    auto v = Variable(variable);
    if_var_store->add(v, statement_number);
}

void PkbManager::add_while_var(const std::string& statement_number, const std::string& variable) {
    auto v = Variable(variable);
    while_var_store->add(v, statement_number);
}

void PkbManager::add_next(const std::string& stmt1, const std::string& stmt2) {
    next_store->add(stmt1, stmt2);
}

void PkbManager::add_calls(const std::string& caller, const std::string& callee) {
    auto caller_procedure = Procedure(caller);
    auto callee_procedure = Procedure(callee);
    direct_calls_store->add(caller_procedure, callee_procedure);
}

void PkbManager::add_stmt_no_proc_called_mapping(const std::string& stmt_no, const std::string& proc_called) {
    auto p = Procedure(proc_called);
    stmt_no_to_proc_called_store->add(stmt_no, p);
}

template <class Key, class Value>
auto insert(std::vector<std::tuple<Key, Value>>& rs, const Key& s1, const Value& s2) -> void {
    rs.emplace_back(s1, s2);
}

template <class Key, class Value>
auto insert(std::vector<std::tuple<Key, Value>>& rs, const Key& s1, const std::unordered_set<Value>& s2) -> void {
    for (const auto& s : s2) {
        insert(rs, s1, s);
    }
}

template <class DirectStore, class StarStore, class OrderingStrategy>
void PkbManager::populate_star_from_direct(std::shared_ptr<DirectStore> direct_store,
                                           std::shared_ptr<StarStore> star_store, OrderingStrategy ordering_strategy) {
    // extract all the direct relationships into a vector of tuples
    static_assert(std::is_same_v<typename DirectStore::Key, typename StarStore::Key>, "Key types must be the same");
    static_assert(std::is_same_v<typename DirectStore::Value, typename StarStore::Value>,
                  "Value types must be the same");

    std::vector<std::tuple<typename DirectStore::Key, typename DirectStore::Value>> relationships;
    std::vector<std::string> procedure_order{};

    for (const auto& pair : direct_store->get_all()) {
        insert(relationships, pair.first, pair.second);
    }

    std::sort(relationships.begin(), relationships.end(), ordering_strategy);

    // populate star_store, starting from the last element of relationships
    for (auto it = relationships.rbegin(); it != relationships.rend(); ++it) {
        const auto& [s1, s2] = *it;
        star_store->add(s1, s2);

        // add all the star relationships from s2 to s1
        for (const auto& s3 : star_store->get_vals_by_key(s2)) {
            star_store->add(s1, s3);
        }
    }
}

void PkbManager::add_proc_to_stmt_no_mapping(const std::string& procedure, const std::string& stmt_no) {
    auto p = Procedure(procedure);
    proc_to_stmt_nos_store->add(p, stmt_no);
}

void PkbManager::finalise_pkb(const std::vector<std::string>& procedure_string_order) {
    std::vector<Procedure> procedure_order;
    std::transform(procedure_string_order.begin(), procedure_string_order.end(), std::back_inserter(procedure_order),
                   [](const std::string& s) {
                       return Procedure(s);
                   });

    populate_star_from_direct(direct_follows_store, follows_star_store, OrderingBySecondElement{});
    populate_star_from_direct(direct_parent_store, parent_star_store, OrderingBySecondElement{});
    populate_star_from_direct(direct_calls_store, calls_star_store, OrderingByIndexMap{procedure_order});
}
} // namespace pkb