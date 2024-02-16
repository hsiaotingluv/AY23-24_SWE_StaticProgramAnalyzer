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
    this->pkb->statement_store->add_statement(statement_number, statement_type);
}

void WriteFacade::add_statement_modifies_var(const std::string& statement_number, std::string variable) {
    auto v = Variable(std::move(variable));
    this->pkb->modifies_store->add_statement_modifies_var(statement_number, v);
}

void WriteFacade::add_procedure_modifies_var(std::string procedure, std::string variable) {
    auto p = Procedure(std::move(procedure));
    auto v = Variable(std::move(variable));
    this->pkb->modifies_store->add_procedure_modifies_var(p, v);
}

void WriteFacade::add_statement_uses_var(const std::string& statement_number, std::string variable) {
    auto v = Variable(std::move(variable));
    this->pkb->uses_store->add_statement_uses_var(statement_number, v);
}

void WriteFacade::add_procedure_uses_var(std::string procedure, std::string variable) {
    auto p = Procedure(std::move(procedure));
    auto v = Variable(std::move(variable));
    this->pkb->uses_store->add_procedure_uses_var(p, v);
}

void WriteFacade::add_follows(const std::string& stmt1, const std::string& stmt2) {
    this->pkb->follows_store->add_follows(stmt1, stmt2);
}

void WriteFacade::add_parent(const std::string& parent, const std::string& child) {
    this->pkb->parent_store->add_parent(parent, child);
}

void WriteFacade::add_next(const std::string& before, const std::string& after) {
    this->pkb->next_store->add_next(before, after);
}

void WriteFacade::add_next_star(const std::string& before, const std::string& after) {
    this->pkb->next_store->add_next_star(before, after);
}

void WriteFacade::add_calls(const std::string& caller, const std::string& callee) {
    this->pkb->calls_store->add_calls(caller, callee);
}

void WriteFacade::add_calls_star(const std::string& caller, const std::string& callee) {
    this->pkb->calls_store->add_calls_star(caller, callee);
}

void WriteFacade::finalise_pkb() {
    this->pkb->follows_store->populate_follows_and_reverse_follows_star();
    this->pkb->parent_store->populate_parent_and_reverse_parent_star();
}
