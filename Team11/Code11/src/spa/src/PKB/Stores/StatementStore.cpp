#include "PKB/Stores/StatementStore.h"

StatementStore::StatementStore() = default;

void StatementStore::addStatement(const std::string& statementNumber, const StatementType& statementType) {
    statementMap[statementNumber] = statementType;
    reverseStatementMap[statementType].insert(statementNumber);
}

std::unordered_set<std::string> StatementStore::getStatements() {
    std::unordered_set<std::string> allStatements;
    for (const auto& pair : statementMap) {
        allStatements.insert(pair.first);
    }
    return allStatements;
}

std::unordered_set<std::string> StatementStore::getStatementsOfType(const StatementType& type) {
    return reverseStatementMap[type];
}