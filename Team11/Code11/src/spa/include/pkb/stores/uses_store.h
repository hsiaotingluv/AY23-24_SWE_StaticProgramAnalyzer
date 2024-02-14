#pragma once

#include "pkb/common_types/procedure.h"
#include "pkb/common_types/variable.h"
#include <unordered_map>
#include <unordered_set>

class UsesStore {
  public:
    using StatementNumber = std::string;

    UsesStore();

    void add_procedure_uses_var(const Procedure& p, const Variable& v);
    void add_statement_uses_var(const StatementNumber& s, const Variable& v);

    bool does_procedure_use_var(const Procedure& p, const Variable& v);
    bool does_statement_use_var(const StatementNumber& s, const Variable& v);

    std::unordered_set<Variable> get_vars_used_by_statement(const StatementNumber& s);
    std::unordered_set<Variable> get_vars_used_by_procedure(const Procedure& p);

    std::unordered_set<StatementNumber> get_statements_that_use_var(const Variable& v);
    std::unordered_set<Procedure> get_procedures_that_use_var(const Variable& v);

  private:
    std::unordered_map<Procedure, std::unordered_set<Variable>> procedure_uses_var_store;
    std::unordered_map<StatementNumber, std::unordered_set<Variable>> statement_uses_var_store;
    std::unordered_map<Variable, std::unordered_set<Procedure>> reverse_procedure_uses_var_store;
    std::unordered_map<Variable, std::unordered_set<StatementNumber>> reverse_statement_uses_var_store;
};