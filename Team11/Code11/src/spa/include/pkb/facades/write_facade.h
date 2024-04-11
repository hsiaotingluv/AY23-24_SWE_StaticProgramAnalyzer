#pragma once

#include "common/statement_type.hpp"
#include "pkb/common_types/constant.h"
#include "pkb/common_types/entity.h"
#include "pkb/common_types/procedure.h"
#include "pkb/common_types/variable.h"
#include "pkb/pkb_manager.h"
#include <unordered_set>
#include <vector>

namespace pkb {
class WriteFacade {
  public:
    typedef std::unordered_set<std::string> StringSet;

    explicit WriteFacade(std::shared_ptr<PkbManager> pkb);

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

    void finalise_pkb(const std::vector<std::string>& procedure_order = {});

  private:
    std::shared_ptr<PkbManager> pkb;
};
} // namespace pkb