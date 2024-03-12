#include "pkb/pkb.h"
#include "pkb/facades/read_facade.h"
#include "pkb/facades/write_facade.h"

#include <tuple>
#include <vector>

PKB::PKB()
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
      if_var_store(std::make_shared<IfVarStore>()), while_var_store(std::make_shared<WhileVarStore>()) {
}

auto PKB::create_facades() -> std::tuple<std::shared_ptr<ReadFacade>, std::shared_ptr<WriteFacade>> {
    auto pkb = std::make_shared<PKB>();
    auto read_facade = std::make_shared<ReadFacade>(pkb);
    auto write_facade = std::make_shared<WriteFacade>(pkb);

    return {std::move(read_facade), std::move(write_facade)};
};

auto insert(std::vector<std::tuple<StatementNumber, StatementNumber>>& rs, const StatementNumber& s1,
            const StatementNumber& s2) -> void {
    rs.emplace_back(s1, s2);
}

auto insert(std::vector<std::tuple<StatementNumber, StatementNumber>>& rs, const StatementNumber& s1,
            const std::unordered_set<StatementNumber>& s2) -> void {
    for (const auto& s : s2) {
        insert(rs, s1, s);
    }
}

auto insert_procedure(std::vector<std::tuple<Procedure, Procedure>>& rs, const Procedure& p1, const Procedure& p2)
    -> void {
    rs.emplace_back(p1, p2);
}

auto insert_procedure(std::vector<std::tuple<Procedure, Procedure>>& rs, const Procedure& p1,
                      const std::unordered_set<Procedure>& p2) -> void {
    for (const auto& p : p2) {
        insert_procedure(rs, p1, p);
    }
}

template <class DirectStore, class StarStore>
void PKB::populate_star_from_direct(DirectStore direct_store, StarStore star_store) {
    // extract all the direct relationships into a vector of tuples
    std::vector<std::tuple<StatementNumber, StatementNumber>> relationships;

    for (const auto& pair : direct_store->get_all()) {
        insert(relationships, pair.first, pair.second);
    }

    // sort relationships by the second element of the tuple
    std::sort(relationships.begin(), relationships.end(), [](const auto& a, const auto& b) {
        return stoi(std::get<1>(a)) < stoi(std::get<1>(b));
    });

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

template <class DirectStore, class StarStore>
void PKB::populate_call_star_from_direct(DirectStore direct_store, StarStore star_store) {
    // extract all the direct relationships into a vector of tuples
    std::vector<std::tuple<Procedure, Procedure>> relationships;

    for (const auto& pair : direct_store->get_all()) {
        insert_procedure(relationships, pair.first, pair.second);
    }

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

// ReadFacade APIs
std::unordered_set<std::string> PKB::get_entities() {
    std::unordered_set<std::string> entities;

    auto procedures = this->entity_store->get_procedures();
    auto vars = this->entity_store->get_variables();
    auto consts = this->entity_store->get_constants();

    for (const Procedure& p : procedures) {
        entities.insert(p.getName());
    }
    for (const Variable& v : vars) {
        entities.insert(v.getName());
    }
    for (const Constant& c : consts) {
        entities.insert(c.getName());
    }

    return entities;
}

std::unordered_set<std::string> PKB::get_procedures() {
    auto procedures = this->entity_store->get_procedures();

    std::unordered_set<std::string> temp;
    for (const Procedure& p : procedures) {
        temp.insert(p.getName());
    }

    return temp;
}

std::unordered_set<std::string> PKB::get_variables() {
    auto vars = this->entity_store->get_variables();

    std::unordered_set<std::string> temp;
    for (const Variable& v : vars) {
        temp.insert(v.getName());
    }

    return temp;
}

std::unordered_set<std::string> PKB::get_constants() {
    auto consts = this->entity_store->get_constants();

    std::unordered_set<std::string> temp;
    for (const Constant& c : consts) {
        temp.insert(c.getName());
    }

    return temp;
}

std::unordered_set<std::string> PKB::get_all_statements() {
    return this->statement_store->get_all_keys();
}

std::unordered_set<std::string> PKB::get_assign_statements() {
    return this->statement_store->get_keys_by_val(StatementType::Assign);
}

std::unordered_set<std::string> PKB::get_if_statements() {
    return this->statement_store->get_keys_by_val(StatementType::If);
}

std::unordered_set<std::string> PKB::get_while_statements() {
    return this->statement_store->get_keys_by_val(StatementType::While);
}

std::unordered_set<std::string> PKB::get_read_statements() {
    return this->statement_store->get_keys_by_val(StatementType::Read);
}

std::unordered_set<std::string> PKB::get_print_statements() {
    return this->statement_store->get_keys_by_val(StatementType::Print);
}

std::unordered_set<std::string> PKB::get_call_statements() {
    return this->statement_store->get_keys_by_val(StatementType::Call);
}

std::unordered_set<std::string> PKB::get_vars_modified_by_statement(const std::string& s) {
    auto variables = this->statement_modifies_store->get_vals_by_key(s);

    std::unordered_set<std::string> temp;
    for (const Variable& v : variables) {
        temp.insert(v.getName());
    }

    return temp;
}

std::unordered_set<std::string> PKB::get_statements_that_modify_var(const std::string& variable) {
    auto v = Variable(variable);

    return this->statement_modifies_store->get_keys_by_val(v);
}

std::unordered_set<std::string> PKB::get_statements_that_modify_var(const std::string& variable,
                                                                    const StatementType& statement_type) {
    auto stmts_pool = get_statements_that_modify_var(variable);
    return this->filter_by_statement_type(stmts_pool, statement_type);
}

bool PKB::does_statement_modify_var(const std::string& statement, const std::string& variable) {
    auto v = Variable(variable);

    return this->statement_modifies_store->contains_key_val_pair(statement, v);
}

std::unordered_set<std::string> PKB::get_all_statements_that_modify() {
    return this->statement_modifies_store->get_all_keys();
};

std::unordered_set<std::string> PKB::get_all_statements_that_modify(const StatementType& statement_type) {
    auto stmts_pool = get_all_statements_that_modify();
    return this->filter_by_statement_type(stmts_pool, statement_type);
};

bool PKB::does_statement_modify_any_var(const std::string& statement_number) {
    return this->statement_modifies_store->contains_key(statement_number);
}

std::unordered_set<std::tuple<std::string, std::string>> PKB::get_all_statements_and_var_modify_pairs() {
    auto pairs = this->statement_modifies_store->get_all_pairs();

    std::unordered_set<std::tuple<std::string, std::string>> temp;
    for (const auto& [s, v] : pairs) {
        temp.insert(std::make_tuple(s, v.getName()));
    }

    return temp;
}

std::unordered_set<std::tuple<std::string, std::string>>
PKB::get_all_statements_and_var_modify_pairs(const StatementType& statement_type) {
    auto pairs = get_all_statements_and_var_modify_pairs();

    return this->filter_by_statement_type(pairs, statement_type, tuple_stmt_no_extractor);
}

std::unordered_set<std::string> PKB::get_all_procedures_that_modify() {
    auto procedures = this->procedure_modifies_store->get_all_keys();

    std::unordered_set<std::string> temp;
    for (const Procedure& p : procedures) {
        temp.insert(p.getName());
    }

    return temp;
}

bool PKB::does_procedure_modify_any_var(const std::string& procedure) {
    auto p = Procedure(procedure);

    return this->procedure_modifies_store->contains_key(p);
}

std::unordered_set<std::tuple<std::string, std::string>> PKB::get_all_procedures_and_var_modify_pairs() {
    auto pairs = this->procedure_modifies_store->get_all_pairs();

    std::unordered_set<std::tuple<std::string, std::string>> temp;
    for (const auto& [p, v] : pairs) {
        temp.insert(std::make_tuple(p.getName(), v.getName()));
    }

    return temp;
}

std::unordered_set<std::string> PKB::get_vars_modified_by_procedure(const std::string& procedure) {
    auto p = Procedure(procedure);

    auto variables = this->procedure_modifies_store->get_vals_by_key(p);

    std::unordered_set<std::string> temp;
    for (const Variable& v : variables) {
        temp.insert(v.getName());
    }

    return temp;
}

std::unordered_set<std::string> PKB::get_procedures_that_modify_var(const std::string& variable) {
    auto v = Variable(variable);

    auto procedures = this->procedure_modifies_store->get_keys_by_val(v);

    std::unordered_set<std::string> temp;
    for (const Procedure& p : procedures) {
        temp.insert(p.getName());
    }

    return temp;
}

bool PKB::does_procedure_modify_var(const std::string& procedure, const std::string& variable) {
    auto v = Variable(variable);
    auto p = Procedure(procedure);

    return this->procedure_modifies_store->contains_key_val_pair(p, v);
}

std::unordered_set<std::string> PKB::get_vars_used_by_statement(const std::string& s) {
    auto variables = this->statement_uses_store->get_vals_by_key(s);

    std::unordered_set<std::string> temp;
    for (const Variable& v : variables) {
        temp.insert(v.getName());
    }

    return temp;
}

std::unordered_set<std::string> PKB::get_statements_that_use_var(const std::string& variable) {
    auto v = Variable(variable);

    return this->statement_uses_store->get_keys_by_val(v);
}

std::unordered_set<std::string> PKB::get_statements_that_use_var(const std::string& variable,
                                                                 const StatementType& statement_type) {
    auto stmts_pool = get_statements_that_use_var(variable);
    return this->filter_by_statement_type(stmts_pool, statement_type);
}

bool PKB::does_statement_use_var(const std::string& statement, const std::string& variable) {
    auto v = Variable(variable);

    return this->statement_uses_store->contains_key_val_pair(statement, v);
}

std::unordered_set<std::string> PKB::get_vars_used_by_procedure(const std::string& procedure) {
    auto p = Procedure(procedure);

    auto variables = this->procedure_uses_store->get_vals_by_key(p);

    std::unordered_set<std::string> temp;
    for (const Variable& v : variables) {
        temp.insert(v.getName());
    }

    return temp;
}

std::unordered_set<std::string> PKB::get_procedures_that_use_var(const std::string& variable) {
    auto v = Variable(variable);

    auto procedures = this->procedure_uses_store->get_keys_by_val(v);

    std::unordered_set<std::string> temp;
    for (const Procedure& p : procedures) {
        temp.insert(p.getName());
    }

    return temp;
}

bool PKB::does_procedure_use_var(const std::string& procedure, const std::string& variable) {
    auto v = Variable(variable);
    auto p = Procedure(procedure);

    return this->procedure_uses_store->contains_key_val_pair(p, v);
}

std::unordered_set<std::string> PKB::get_all_statements_that_use() {
    return this->statement_uses_store->get_all_keys();
};

std::unordered_set<std::string> PKB::get_all_statements_that_use(const StatementType& statement_type) {
    auto stmts_pool = get_all_statements_that_use();
    return this->filter_by_statement_type(stmts_pool, statement_type);
};

bool PKB::does_statement_use_any_var(const std::string& statement_number) {
    return this->statement_uses_store->contains_key(statement_number);
}

std::unordered_set<std::tuple<std::string, std::string>> PKB::get_all_statements_and_var_use_pairs() {
    auto pairs = this->statement_uses_store->get_all_pairs();

    std::unordered_set<std::tuple<std::string, std::string>> temp;
    for (const auto& [s, v] : pairs) {
        temp.insert(std::make_tuple(s, v.getName()));
    }

    return temp;
}

std::unordered_set<std::tuple<std::string, std::string>>
PKB::get_all_statements_and_var_use_pairs(const StatementType& statement_type) {
    auto pairs = get_all_statements_and_var_use_pairs();

    return this->filter_by_statement_type(pairs, statement_type, tuple_stmt_no_extractor);
}

std::unordered_set<std::string> PKB::get_all_procedures_that_use() {
    auto procedures = this->procedure_uses_store->get_all_keys();

    std::unordered_set<std::string> temp;
    for (const Procedure& p : procedures) {
        temp.insert(p.getName());
    }

    return temp;
}

bool PKB::does_procedure_use_any_var(const std::string& procedure) {
    auto p = Procedure(procedure);

    return this->procedure_uses_store->contains_key(p);
}

std::unordered_set<std::tuple<std::string, std::string>> PKB::get_all_procedures_and_var_use_pairs() {
    auto pairs = this->procedure_uses_store->get_all_pairs();

    std::unordered_set<std::tuple<std::string, std::string>> temp;
    for (const auto& [p, v] : pairs) {
        temp.insert(std::make_tuple(p.getName(), v.getName()));
    }

    return temp;
}

std::unordered_map<std::string, std::string> PKB::get_all_follows() const {
    //    return this->follows_store->get_all_follows();
    return this->direct_follows_store->get_all();
}

std::unordered_set<std::string> PKB::get_all_follows_keys() const {
    return this->direct_follows_store->get_all_keys();
}

std::unordered_set<std::string> PKB::get_all_follows_keys(const StatementType& statement_type) const {
    auto stmts_pool = this->direct_follows_store->get_all_keys();
    return this->filter_by_statement_type(stmts_pool, statement_type);
}

std::unordered_set<std::string> PKB::get_all_follows_values() const {
    return this->direct_follows_store->get_all_vals();
}

std::unordered_set<std::string> PKB::get_all_follows_values(const StatementType& statement_type) const {
    auto stmts_pool = this->direct_follows_store->get_all_vals();
    return this->filter_by_statement_type(stmts_pool, statement_type);
}

bool PKB::has_follows_relation(const std::string& stmt1, const std::string& stmt2) const {
    return this->direct_follows_store->contains_key_val_pair(stmt1, stmt2);
}

std::string PKB::get_statement_following(const std::string& s) const {
    return this->direct_follows_store->get_val_by_key(s);
}

std::string PKB::get_statement_following(const std::string& s, const StatementType& statement_type) const {
    auto stmt = this->direct_follows_store->get_val_by_key(s);

    if (this->statement_store->get_val_by_key(stmt) == statement_type) {
        return stmt;
    }

    return "";
}

std::string PKB::get_statement_followed_by(const std::string& s) const {
    return this->direct_follows_store->get_key_by_val(s);
}

std::string PKB::get_statement_followed_by(const std::string& s, const StatementType& statement_type) const {
    auto stmt = this->direct_follows_store->get_key_by_val(s);

    if (this->statement_store->get_val_by_key(stmt) == statement_type) {
        return stmt;
    }

    return "";
}

bool PKB::has_follows_star_relation(const std::string& stmt1, const std::string& stmt2) const {
    return this->follows_star_store->contains_key_val_pair(stmt1, stmt2);
}

std::unordered_map<std::string, std::unordered_set<std::string>> PKB::get_all_follows_star() const {
    return this->follows_star_store->get_all();
}

std::unordered_set<std::string> PKB::get_all_follows_star_keys() const {
    return this->follows_star_store->get_all_keys();
}

std::unordered_set<std::string> PKB::get_all_follows_star_keys(const StatementType& statement_type) const {
    auto stmts_pool = get_all_follows_star_keys();

    return this->filter_by_statement_type(stmts_pool, statement_type);
}

std::unordered_set<std::string> PKB::get_all_follows_star_values() const {
    return this->follows_star_store->get_all_vals();
}

std::unordered_set<std::string> PKB::get_all_follows_star_values(const StatementType& statement_type) const {
    auto stmts_pool = get_all_follows_star_values();
    return this->filter_by_statement_type(stmts_pool, statement_type);
}

std::unordered_set<std::string> PKB::get_follows_stars_following(const std::string& stmt) const {
    return this->follows_star_store->get_vals_by_key(stmt);
}

std::unordered_set<std::string> PKB::get_follows_stars_following(const std::string& stmt,
                                                                 const StatementType& statement_type) const {
    auto stmts_pool = get_follows_stars_following(stmt);

    return this->filter_by_statement_type(stmts_pool, statement_type);
}

std::unordered_set<std::string> PKB::get_follows_stars_by(const std::string& stmt) const {
    return this->follows_star_store->get_keys_by_val(stmt);
}

std::unordered_set<std::string> PKB::get_follows_stars_by(const std::string& stmt,
                                                          const StatementType& statement_type) const {
    auto stmts_pool = get_follows_stars_by(stmt);
    return this->filter_by_statement_type(stmts_pool, statement_type);
}

bool PKB::has_parent_relation(const std::string& parent, const std::string& child) const {
    return this->direct_parent_store->contains_key_val_pair(parent, child);
}

std::unordered_map<std::string, std::unordered_set<std::string>> PKB::get_all_parent() const {
    return this->direct_parent_store->get_all();
}

std::unordered_set<std::string> PKB::get_all_parent_keys() const {
    return this->direct_parent_store->get_all_keys();
}

std::unordered_set<std::string> PKB::get_all_parent_keys(const StatementType& statement_type) const {
    auto stmts_pool = get_all_parent_keys();
    return this->filter_by_statement_type(stmts_pool, statement_type);
}

std::unordered_set<std::string> PKB::get_all_parent_values() const {
    return this->direct_parent_store->get_all_vals();
}

std::unordered_set<std::string> PKB::get_all_parent_values(const StatementType& statement_type) const {
    auto stmts_pool = get_all_parent_values();
    return this->filter_by_statement_type(stmts_pool, statement_type);
}

std::unordered_set<std::string> PKB::get_children_of(const std::string& parent) const {
    return this->direct_parent_store->get_vals_by_key(parent);
}

std::unordered_set<std::string> PKB::get_children_of(const std::string& parent,
                                                     const StatementType& statement_type) const {
    auto stmts_pool = get_children_of(parent);
    return this->filter_by_statement_type(stmts_pool, statement_type);
}

std::string PKB::get_parent_of(const std::string& child) const {
    return this->direct_parent_store->get_key_by_val(child);
}

std::string PKB::get_parent_of(const std::string& child, const StatementType& statement_type) const {
    auto stmt = get_parent_of(child);

    if (this->statement_store->get_val_by_key(stmt) == statement_type) {
        return stmt;
    }

    return "";
}

bool PKB::has_parent_star_relation(const std::string& parent, const std::string& child) const {
    return this->parent_star_store->contains_key_val_pair(parent, child);
}

std::unordered_map<std::string, std::unordered_set<std::string>> PKB::get_all_parent_star() const {
    return this->parent_star_store->get_all();
}

std::unordered_set<std::string> PKB::get_all_parent_star_keys() const {
    return this->parent_star_store->get_all_keys();
}

std::unordered_set<std::string> PKB::get_all_parent_star_keys(const StatementType& statement_type) const {
    auto stmts_pool = get_all_parent_star_keys();

    return this->filter_by_statement_type(stmts_pool, statement_type);
}

std::unordered_set<std::string> PKB::get_all_parent_star_values() const {
    return this->parent_star_store->get_all_vals();
}

std::unordered_set<std::string> PKB::get_all_parent_star_values(const StatementType& statement_type) const {
    auto stmts_pool = get_all_parent_star_values();
    return this->filter_by_statement_type(stmts_pool, statement_type);
}

std::unordered_set<std::string> PKB::get_children_star_of(const std::string& parent) const {
    return this->parent_star_store->get_vals_by_key(parent);
}

std::unordered_set<std::string> PKB::get_children_star_of(const std::string& parent,
                                                          const StatementType& statement_type) const {
    auto stmts_pool = get_children_star_of(parent);
    return this->filter_by_statement_type(stmts_pool, statement_type);
}

std::unordered_set<std::string> PKB::get_parent_star_of(const std::string& child) const {
    return this->parent_star_store->get_keys_by_val(child);
}

std::unordered_set<std::string> PKB::get_parent_star_of(const std::string& child,
                                                        const StatementType& statement_type) const {
    auto stmts_pool = get_parent_star_of(child);
    return this->filter_by_statement_type(stmts_pool, statement_type);
}

bool PKB::has_next_relation(const std::string& before, const std::string& after) const {
    return this->next_store->contains_key_val_pair(before, after);
}

std::unordered_set<std::string> PKB::get_all_next_keys() const {
    return this->next_store->get_all_keys();
}

std::unordered_set<std::string> PKB::get_all_next_values() const {
    return this->next_store->get_all_vals();
}

std::unordered_set<std::string> PKB::get_next_of(const std::string& before) const {
    return this->next_store->get_vals_by_key(before);
}

std::unordered_set<std::string> PKB::get_previous_of(const std::string& after) const {
    return this->next_store->get_keys_by_val(after);
}

bool PKB::has_calls_relation(const std::string& caller, const std::string& callee) const {
    return this->direct_calls_store->contains_key_val_pair(Procedure(caller), Procedure(callee));
}

std::unordered_set<std::string> PKB::get_all_calls_values() const {
    auto procedures = this->direct_calls_store->get_all_vals();

    std::unordered_set<std::string> temp;
    for (const Procedure& p : procedures) {
        temp.insert(p.getName());
    }

    return temp;
}

std::unordered_set<std::string> PKB::get_all_calls_keys() const {
    auto procedures = this->direct_calls_store->get_all_keys();

    std::unordered_set<std::string> temp;
    for (const Procedure& p : procedures) {
        temp.insert(p.getName());
    }

    return temp;
}

std::unordered_set<std::string> PKB::get_callees(const std::string& caller) const {
    auto procedures = this->direct_calls_store->get_vals_by_key(Procedure(caller));

    std::unordered_set<std::string> temp;
    for (const Procedure& p : procedures) {
        temp.insert(p.getName());
    }

    return temp;
}

std::unordered_set<std::string> PKB::get_callers(const std::string& callee) const {
    auto procedures = this->direct_calls_store->get_keys_by_val(Procedure(callee));

    std::unordered_set<std::string> temp;
    for (const Procedure& p : procedures) {
        temp.insert(p.getName());
    }

    return temp;
}

bool PKB::has_calls_star_relation(const std::string& caller, const std::string& callee) const {
    return this->calls_star_store->contains_key_val_pair(Procedure(caller), Procedure(callee));
}

std::unordered_set<std::string> PKB::get_all_calls_star_values() const {
    auto procedures = this->calls_star_store->get_all_vals();

    std::unordered_set<std::string> temp;
    for (const Procedure& p : procedures) {
        temp.insert(p.getName());
    }

    return temp;
}

std::unordered_set<std::string> PKB::get_all_calls_star_keys() const {
    auto procedures = this->calls_star_store->get_all_keys();

    std::unordered_set<std::string> temp;
    for (const Procedure& p : procedures) {
        temp.insert(p.getName());
    }

    return temp;
}

std::unordered_set<std::string> PKB::get_star_callees(const std::string& caller) const {
    auto procedures = this->calls_star_store->get_vals_by_key(Procedure(caller));

    std::unordered_set<std::string> temp;
    for (const Procedure& p : procedures) {
        temp.insert(p.getName());
    }

    return temp;
}

std::unordered_set<std::string> PKB::get_star_callers(const std::string& callee) const {
    auto procedures = this->calls_star_store->get_keys_by_val(Procedure(callee));

    std::unordered_set<std::string> temp;
    for (const Procedure& p : procedures) {
        temp.insert(p.getName());
    }

    return temp;
}

std::unordered_set<std::string> PKB::get_all_assignments_rhs(const std::string& rhs) {
    return this->assignment_store->get_all_assignments_rhs(rhs);
}

std::unordered_set<std::string> PKB::get_all_assignments_rhs_partial(const std::string& rhs) {
    return this->assignment_store->get_all_assignments_rhs_partial(rhs);
}

std::unordered_set<std::string> PKB::get_all_assignments_lhs(const std::string& lhs) {
    auto v = Variable(lhs);

    return this->assignment_store->get_all_assignments_lhs(v);
}

std::unordered_set<std::string> PKB::get_all_assignments_lhs_rhs(const std::string& lhs, const std::string& rhs) {
    auto v = Variable(lhs);

    return this->assignment_store->get_all_assignments_lhs_rhs(v, rhs);
}

std::unordered_set<std::string> PKB::get_all_assignments_lhs_rhs_partial(const std::string& lhs,
                                                                         const std::string& rhs) {
    auto v = Variable(lhs);

    return this->assignment_store->get_all_assignments_lhs_rhs_partial(v, rhs);
}

std::unordered_set<std::string> PKB::get_if_stmts_with_var() {
    return this->if_var_store->get_all_vals();
}

std::unordered_set<std::string> PKB::get_if_stmts_with_var(const std::string& variable) {
    auto v = Variable(variable);

    return this->if_var_store->get_vals_by_key(v);
}

std::unordered_set<std::string> PKB::get_vars_in_any_if() {
    auto temp = this->if_var_store->get_all_keys();

    std::unordered_set<std::string> vars;
    for (const auto& v : temp) {
        vars.insert(v.getName());
    }

    return vars;
}

std::unordered_set<std::string> PKB::get_vars_in_if(const std::string& if_stmt) {
    auto temp = this->if_var_store->get_keys_by_val(if_stmt);

    std::unordered_set<std::string> vars;

    for (const auto& v : temp) {
        vars.insert(v.getName());
    }

    return vars;
}

std::unordered_set<std::tuple<std::string, std::string>> PKB::get_all_if_stmt_var_pairs() {
    auto pairs = this->if_var_store->get_all_pairs();

    std::unordered_set<std::tuple<std::string, std::string>> temp;
    for (const auto& [v, s] : pairs) {
        temp.insert(std::make_tuple(s, v.getName()));
    }

    return temp;
}

std::unordered_set<std::string> PKB::get_while_stmts_with_var() {
    return this->while_var_store->get_all_vals();
}

std::unordered_set<std::string> PKB::get_while_stmts_with_var(const std::string& variable) {
    auto v = Variable(variable);

    return this->while_var_store->get_vals_by_key(v);
}

std::unordered_set<std::string> PKB::get_vars_in_any_while() {
    auto temp = this->while_var_store->get_all_keys();

    std::unordered_set<std::string> vars;
    for (const auto& v : temp) {
        vars.insert(v.getName());
    }

    return vars;
}

std::unordered_set<std::string> PKB::get_vars_in_while(const std::string& while_stmt) {
    auto temp = this->while_var_store->get_keys_by_val(while_stmt);

    std::unordered_set<std::string> vars;

    for (const auto& v : temp) {
        vars.insert(v.getName());
    }

    return vars;
}

std::unordered_set<std::tuple<std::string, std::string>> PKB::get_all_while_stmt_var_pairs() {
    auto pairs = this->while_var_store->get_all_pairs();

    std::unordered_set<std::tuple<std::string, std::string>> temp;
    for (const auto& [v, s] : pairs) {
        temp.insert(std::make_tuple(s, v.getName()));
    }

    return temp;
}

// WriteFacade APIs
void PKB::add_procedure(std::string procedure) {
    Procedure p = Procedure(std::move(procedure));

    this->entity_store->add_procedure(p);
}

void PKB::add_variable(std::string variable) {
    Variable v = Variable(std::move(variable));

    this->entity_store->add_variable(v);
}

void PKB::add_constant(std::string constant) {
    Constant c = Constant(std::move(constant));

    this->entity_store->add_constant(c);
}

void PKB::add_statement(const std::string& statement_number, StatementType statement_type) {
    this->statement_store->add(statement_number, statement_type);
}

void PKB::add_statement_modifies_var(const std::string& statement_number, std::string variable) {
    auto v = Variable(std::move(variable));
    this->statement_modifies_store->add(statement_number, v);
}

void PKB::add_procedure_modifies_var(std::string procedure, std::string variable) {
    auto p = Procedure(std::move(procedure));
    auto v = Variable(std::move(variable));
    this->procedure_modifies_store->add(p, v);
}

void PKB::add_statement_uses_var(const std::string& statement_number, std::string variable) {
    auto v = Variable(std::move(variable));
    this->statement_uses_store->add(statement_number, v);
}

void PKB::add_procedure_uses_var(std::string procedure, std::string variable) {
    auto p = Procedure(std::move(procedure));
    auto v = Variable(std::move(variable));
    this->procedure_uses_store->add(p, v);
}

void PKB::add_follows(const std::string& stmt1, const std::string& stmt2) {
    this->direct_follows_store->add(stmt1, stmt2);
}

void PKB::add_parent(const std::string& parent, const std::string& child) {
    this->direct_parent_store->add(parent, child);
}

void PKB::add_assignment(const std::string& statement_number, const std::string& lhs, const std::string& rhs) {
    auto v = Variable(lhs);
    this->assignment_store->add_assignment(statement_number, v, rhs);
}

void PKB::add_if_var(const std::string& statement_number, const std::string& variable) {
    auto v = Variable(variable);
    this->if_var_store->add(v, statement_number);
}

void PKB::add_while_var(const std::string& statement_number, const std::string& variable) {
    auto v = Variable(variable);
    this->while_var_store->add(v, statement_number);
}

void PKB::add_next(const std::string& stmt1, const std::string& stmt2) {
    this->next_store->add(stmt1, stmt2);
}

void PKB::add_calls(const std::string& caller, const std::string& callee) {
    auto caller_procedure = Procedure(caller);
    auto callee_procedure = Procedure(callee);
    this->direct_calls_store->add(caller_procedure, callee_procedure);
}

void PKB::finalise_pkb() {
    populate_star_from_direct(direct_follows_store, follows_star_store);
    populate_star_from_direct(direct_parent_store, parent_star_store);
    populate_call_star_from_direct(direct_calls_store, calls_star_store);
}