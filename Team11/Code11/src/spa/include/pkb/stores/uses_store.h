#pragma once

#include "common/hashable_tuple.h"
#include "pkb/abstraction/OneToManyStore.h"
#include "pkb/common_types/procedure.h"
#include "pkb/common_types/variable.h"
#include <unordered_map>
#include <unordered_set>

class UsesStore {
  public:
    using StatementNumber = std::string;
    using VariableSet = std::unordered_set<Variable>;
    using StatementSet = std::unordered_set<StatementNumber>;
    using ProcedureSet = std::unordered_set<Procedure>;
    using StatementNumberVariableTupleSet = std::unordered_set<std::tuple<StatementNumber, Variable>>;
    using ProcedureVariableTupleSet = std::unordered_set<std::tuple<Procedure, Variable>>;
    using ProcedureToVariableSetStore = OneToManyStore<Procedure, Variable>;
    using StatementToVariableSetStore = OneToManyStore<StatementNumber, Variable>;

    UsesStore();

    void add_procedure_uses_var(const Procedure& p, const Variable& v);
    void add_statement_uses_var(const StatementNumber& s, const Variable& v);

    bool does_procedure_use_var(const Procedure& p, const Variable& v);
    bool does_statement_use_var(const StatementNumber& s, const Variable& v);

    VariableSet get_vars_used_by_statement(const StatementNumber& s);
    VariableSet get_vars_used_by_procedure(const Procedure& p);

    StatementSet get_statements_that_use_var(const Variable& v);
    ProcedureSet get_procedures_that_use_var(const Variable& v);

    StatementSet get_all_statements_that_use();
    bool does_statement_use_any_var(const StatementNumber& statement);
    StatementNumberVariableTupleSet get_all_statements_and_var_pairs();

    ProcedureSet get_all_procedures_that_use();
    bool does_procedure_use_any_var(const Procedure& procedure);
    ProcedureVariableTupleSet get_all_procedures_and_var_pairs();

  private:
    ProcedureToVariableSetStore procedure_uses_var_store;
    StatementToVariableSetStore statement_uses_var_store;
};