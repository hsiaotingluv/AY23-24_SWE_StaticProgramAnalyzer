#include "pkb/facades/write_facade.h"

#include <utility>

WriteFacade::WriteFacade(std::shared_ptr<PKB> pkb) : pkb(std::move(pkb)) {
}

void WriteFacade::add_procedure(std::string procedure) {
    Procedure p = Procedure(std::move(procedure));

    this->pkb->entity_store->add_procedure(p);
}

void WriteFacade::add_variable(std::string variable) {
    Variable v = Variable(std::move(variable));

    this->pkb->entity_store->add_variable(v);
}

void WriteFacade::add_constant(std::string constant) {
    Constant c = Constant(std::move(constant));

    this->pkb->entity_store->add_constant(c);
}

void WriteFacade::add_statement(const std::string& statement_number, StatementType statement_type) {
    this->pkb->statement_store->add(statement_number, statement_type);
}

void WriteFacade::add_statement_modifies_var(const std::string& statement_number, std::string variable) {
    auto v = Variable(std::move(variable));
    this->pkb->statement_modifies_store->add(statement_number, v);
}

void WriteFacade::add_procedure_modifies_var(std::string procedure, std::string variable) {
    auto p = Procedure(std::move(procedure));
    auto v = Variable(std::move(variable));
    this->pkb->procedure_modifies_store->add(p, v);
}

void WriteFacade::add_statement_uses_var(const std::string& statement_number, std::string variable) {
    auto v = Variable(std::move(variable));
    this->pkb->statement_uses_store->add(statement_number, v);
}

void WriteFacade::add_procedure_uses_var(std::string procedure, std::string variable) {
    auto p = Procedure(std::move(procedure));
    auto v = Variable(std::move(variable));
    this->pkb->procedure_uses_store->add(p, v);
}

void WriteFacade::add_follows(const std::string& stmt1, const std::string& stmt2) {
    this->pkb->direct_follows_store->add(stmt1, stmt2);
}

void WriteFacade::add_parent(const std::string& parent, const std::string& child) {
    this->pkb->direct_parent_store->add(parent, child);
}

void WriteFacade::add_assignment(const std::string& statement_number, const std::string& lhs, const std::string& rhs) {
    auto v = Variable(lhs);
    this->pkb->assignment_store->add_assignment(statement_number, v, rhs);
}

void WriteFacade::finalise_pkb() {
    this->pkb->finalise_pkb();
}
