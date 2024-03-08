#pragma once

#include "common/statement_type.hpp"
#include "pkb/common_types/constant.h"
#include "pkb/common_types/entity.h"
#include "pkb/common_types/procedure.h"
#include "pkb/common_types/variable.h"
#include "pkb/pkb.h"
#include <unordered_set>

class WriteFacade {
  public:
    typedef std::unordered_set<std::string> StringSet;

    explicit WriteFacade(std::shared_ptr<PKB> pkb);

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
    std::shared_ptr<PKB> pkb;
};
