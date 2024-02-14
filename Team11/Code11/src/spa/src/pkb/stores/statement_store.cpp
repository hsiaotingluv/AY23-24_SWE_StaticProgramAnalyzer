#include "pkb/stores/statement_store.h"

StatementStore::StatementStore() = default;

void StatementStore::add_statement(const std::string& statement_number, const StatementType& statement_type) {
    statement_map[statement_number] = statement_type;
    reverse_statement_map[statement_type].insert(statement_number);
}

std::unordered_set<std::string> StatementStore::get_statements() {
    std::unordered_set<std::string> allStatements;
    for (const auto& pair : statement_map) {
        allStatements.insert(pair.first);
    }
    return allStatements;
}

std::unordered_set<std::string> StatementStore::get_statements_of_type(const StatementType& type) {
    return reverse_statement_map[type];
}