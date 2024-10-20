#include "pkb/facades/write_facade.h"

#include <utility>

namespace pkb {
WriteFacade::WriteFacade(std::shared_ptr<PkbManager> pkb) : pkb(std::move(pkb)) {
}

void WriteFacade::add_procedure(std::string procedure) {
    pkb->add_procedure(std::move(procedure));
}

void WriteFacade::add_variable(std::string variable) {
    pkb->add_variable(std::move(variable));
}

void WriteFacade::add_constant(std::string constant) {
    pkb->add_constant(std::move(constant));
}

void WriteFacade::add_statement(const std::string& statement_number, StatementType statement_type) {
    pkb->add_statement(statement_number, statement_type);
}

void WriteFacade::add_statement_modify_var(const std::string& statement_number, std::string variable) {
    pkb->add_statement_modify_var(statement_number, std::move(variable));
}

void WriteFacade::add_procedure_modify_var(std::string procedure, std::string variable) {
    pkb->add_procedure_modify_var(std::move(procedure), std::move(variable));
}

void WriteFacade::add_statement_use_var(const std::string& statement_number, std::string variable) {
    pkb->add_statement_use_var(statement_number, std::move(variable));
}

void WriteFacade::add_procedure_use_var(std::string procedure, std::string variable) {
    pkb->add_procedure_use_var(std::move(procedure), std::move(variable));
}

void WriteFacade::add_follows(const std::string& stmt1, const std::string& stmt2) {
    pkb->add_follows(stmt1, stmt2);
}

void WriteFacade::add_parent(const std::string& parent, const std::string& child) {
    pkb->add_parent(parent, child);
}

void WriteFacade::add_assignment(const std::string& statement_number, const std::string& lhs, const std::string& rhs) {
    pkb->add_assignment(statement_number, lhs, rhs);
}

void WriteFacade::add_if_var(const std::string& statement_number, const std::string& variable) {
    pkb->add_if_var(statement_number, variable);
}

void WriteFacade::add_while_var(const std::string& statement_number, const std::string& variable) {
    pkb->add_while_var(statement_number, variable);
}

void WriteFacade::add_next(const std::string& stmt1, const std::string& stmt2) {
    pkb->add_next(stmt1, stmt2);
}

void WriteFacade::add_calls(const std::string& caller, const std::string& callee) {
    pkb->add_calls(caller, callee);
}

void WriteFacade::add_stmt_no_proc_called_mapping(const std::string& stmt_no, const std::string& proc_called) {
    pkb->add_stmt_no_proc_called_mapping(stmt_no, proc_called);
}

void WriteFacade::add_proc_to_stmt_no_mapping(const std::string& procedure, const std::string& stmt_no) {
    pkb->add_proc_to_stmt_no_mapping(procedure, stmt_no);
}

void WriteFacade::finalise_pkb(const std::vector<std::string>& procedure_order) {
    pkb->finalise_pkb(procedure_order);
}
} // namespace pkb