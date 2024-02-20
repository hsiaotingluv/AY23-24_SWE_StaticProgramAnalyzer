#pragma once

#include <tuple>
#include <unordered_map>
#include <unordered_set>

#include "common/hashable_tuple.h"
#include "pkb/abstraction/OneToManyStore.h"
#include "pkb/common_types/procedure.h"
#include "pkb/common_types/variable.h"

class ModifiesStore {
  public:
    using StatementNumber = std::string;
    using VariableSet = std::unordered_set<Variable>;
    using StatementSet = std::unordered_set<StatementNumber>;
    using ProcedureSet = std::unordered_set<Procedure>;
    using StatementNumberVariableTupleSet = std::unordered_set<std::tuple<StatementNumber, Variable>>;
    using ProcedureVariableTupleSet = std::unordered_set<std::tuple<Procedure, Variable>>;
    using ProcedureToVariableSetStore = OneToManyStore<Procedure, Variable>;
    using StatementToVariableSetStore = OneToManyStore<StatementNumber, Variable>;

    ModifiesStore();

    void add_procedure_modifies_var(const Procedure& p, const Variable& v);
    void add_statement_modifies_var(const StatementNumber& s, const Variable& v);

    bool does_procedure_modify_var(const Procedure& p, const Variable& v);
    bool does_statement_modify_var(const StatementNumber& s, const Variable& v);

    VariableSet get_vars_modified_by_statement(const StatementNumber& s);
    VariableSet get_vars_modified_by_procedure(const Procedure& p);

    StatementSet get_statements_that_modify_var(const Variable& v);
    ProcedureSet get_procedures_that_modify_var(const Variable& v);

    StatementSet get_all_statements_that_modify();
    bool does_statement_modify_any_var(const StatementNumber& statement);
    StatementNumberVariableTupleSet get_all_statements_and_var_pairs();

    ProcedureSet get_all_procedures_that_modify();
    bool does_procedure_modify_any_var(const Procedure& procedure);
    ProcedureVariableTupleSet get_all_procedures_and_var_pairs();

  private:
    ProcedureToVariableSetStore procedure_modifies_var_store;
    StatementToVariableSetStore statement_modifies_var_store;
};