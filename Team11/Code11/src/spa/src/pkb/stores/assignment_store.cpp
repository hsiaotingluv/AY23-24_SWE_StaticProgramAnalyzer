#include "pkb/stores/assignment_store.h"

AssignmentStore::AssignmentStore() = default;

void AssignmentStore::add_assignment(const StatementNumber& s, const Variable& lhs, const std::string& rhs) {
    assignment_store[rhs][lhs].insert(s);
}

std::unordered_set<std::string> AssignmentStore::get_all_assignments_rhs(const std::string& rhs) {
    if (assignment_store.find(rhs) == assignment_store.end()) {
        return {};
    }

    auto lhs_stmt_map = assignment_store.at(rhs);

    std::unordered_set<std::string> result;

    for (const auto& [lhs, stmts] : lhs_stmt_map) {
        for (const auto& stmt : stmts) {
            result.insert(stmt);
        }
    }

    return result;
}

std::unordered_set<std::string> AssignmentStore::get_all_assignments_lhs(const Variable& lhs) {
    std::unordered_set<std::string> result;

    for (const auto& [rhs, stmts] : assignment_store) {
        if (stmts.find(lhs) != stmts.end()) {
            for (const auto& stmt : stmts.at(lhs)) {
                result.insert(stmt);
            }
        }
    }

    return result;
}

std::unordered_set<std::string> AssignmentStore::get_all_assignments_lhs_rhs(const Variable& lhs,
                                                                             const std::string& rhs) {
    if (assignment_store.find(rhs) == assignment_store.end()) {
        return {};
    }

    if (assignment_store.at(rhs).find(lhs) == assignment_store.at(rhs).end()) {
        return {};
    }

    return assignment_store.at(rhs).at(lhs);
}

std::unordered_set<std::string> AssignmentStore::get_all_assignments_rhs_partial(const std::string& rhs) {
    std::unordered_set<std::string> result;

    for (const auto& [rhs_key, lhs_stmts] : assignment_store) {
        if (rhs_key.find(rhs) != std::string::npos) {
            for (const auto& [lhs, stmts] : lhs_stmts) {
                for (const auto& s : stmts) {
                    result.insert(s);
                }
            }
        }
    }

    return result;
}

std::unordered_set<std::string> AssignmentStore::get_all_assignments_lhs_rhs_partial(const Variable& lhs,
                                                                                     const std::string& rhs) {
    std::unordered_set<std::string> result;

    for (const auto& [rhs_key, lhs_stmts] : assignment_store) {
        if (rhs_key.find(rhs) != std::string::npos) {
            if (lhs_stmts.find(lhs) != lhs_stmts.end()) {
                for (const auto& s : lhs_stmts.at(lhs)) {
                    result.insert(s);
                }
            }
        }
    }

    return result;
}