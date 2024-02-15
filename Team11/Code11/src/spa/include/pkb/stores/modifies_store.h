#pragma once

#include "common/hashable_tuple.h"
#include "pkb/common_types/procedure.h"
#include "pkb/common_types/variable.h"
#include <tuple>
#include <unordered_map>
#include <unordered_set>

class ModifiesStore {
  public:
    using StatementNumber = std::string;
    using StatementNumberVariableTupleSet = std::unordered_set<std::tuple<StatementNumber, Variable>>;
    using ProcedureVariableTupleSet = std::unordered_set<std::tuple<Procedure, Variable>>;

    ModifiesStore();

    void add_procedure_modifies_var(const Procedure& p, const Variable& v);
    void add_statement_modifies_var(const StatementNumber& s, const Variable& v);

    bool does_procedure_modify_var(const Procedure& p, const Variable& v);
    bool does_statement_modify_var(const StatementNumber& s, const Variable& v);

    std::unordered_set<Variable> get_vars_modified_by_statement(const StatementNumber& s);
    std::unordered_set<Variable> get_vars_modified_by_procedure(const Procedure& p);

    std::unordered_set<StatementNumber> get_statements_that_modify_var(const Variable& v);
    std::unordered_set<Procedure> get_procedures_that_modify_var(const Variable& v);

    std::unordered_set<StatementNumber> get_all_statements_that_modify();
    bool does_statement_modify_any_var(const StatementNumber& statement);
    StatementNumberVariableTupleSet get_all_statements_and_var_pairs();

    std::unordered_set<Procedure> get_all_procedures_that_modify();
    bool does_procedure_modify_any_var(const Procedure& procedure);
    ProcedureVariableTupleSet get_all_procedures_and_var_pairs();

  private:
    std::unordered_map<Procedure, std::unordered_set<Variable>> procedure_modifies_var_store;
    std::unordered_map<StatementNumber, std::unordered_set<Variable>> statement_modifies_var_store;
    std::unordered_map<Variable, std::unordered_set<Procedure>> reverse_procedure_modifies_var_store;
    std::unordered_map<Variable, std::unordered_set<StatementNumber>> reverse_statement_modifies_var_store;
};