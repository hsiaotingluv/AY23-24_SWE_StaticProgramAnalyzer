#pragma once

#include "common/StatementType.h"
#include <unordered_map>
#include <unordered_set>

class StatementStore {
  public:
    StatementStore();

    void addStatement(const std::string& statementNumber, const StatementType& statementType);

    std::unordered_set<std::string> getStatements();

    std::unordered_set<std::string> getStatementsOfType(const StatementType& type);

  private:
    std::unordered_map<std::string, StatementType> statementMap;
    std::unordered_map<StatementType, std::unordered_set<std::string>> reverseStatementMap;
};