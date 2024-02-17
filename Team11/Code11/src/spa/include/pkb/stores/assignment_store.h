#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "pkb/common_types/variable.h"

class AssignmentStore {
  public:
    using StatementNumber = std::string;
    using AssignmentStoreType =
        std::unordered_map<std::string, std::unordered_map<Variable, std::unordered_set<std::string>>>;

    AssignmentStore();

    void add_assignment(const StatementNumber& s, const Variable& lhs, const std::string& rhs);

    // lhs wildcard, rhs exact match
    std::unordered_set<std::string> get_all_assignments_rhs(const std::string& rhs);

    // lhs wildcard, rhs partial match
    std::unordered_set<std::string> get_all_assignments_rhs_partial(const std::string& rhs);

    // lhs exact match, rhs wildcard
    std::unordered_set<std::string> get_all_assignments_lhs(const Variable& lhs);

    // lhs exact match, rhs exact match
    std::unordered_set<std::string> get_all_assignments_lhs_rhs(const Variable& lhs, const std::string& rhs);

    // lhs exact match, rhs partial match
    std::unordered_set<std::string> get_all_assignments_lhs_rhs_partial(const Variable& lhs, const std::string& rhs);

  private:
    AssignmentStoreType assignment_store;
};