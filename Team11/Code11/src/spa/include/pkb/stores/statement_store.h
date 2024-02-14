#pragma once

#include "common/statement_type.hpp"
#include <string>
#include <unordered_map>
#include <unordered_set>

class StatementStore {
  public:
    StatementStore();

    void add_statement(const std::string& statement_number, const StatementType& statement_type);

    std::unordered_set<std::string> get_statements();

    std::unordered_set<std::string> get_statements_of_type(const StatementType& type);

  private:
    std::unordered_map<std::string, StatementType> statement_map;
    std::unordered_map<StatementType, std::unordered_set<std::string>> reverse_statement_map;
};