#include "qps/parser/entities/clause.hpp"

#include <sstream>
#include <string>

namespace qps {
auto SuchThatClause::representation() const -> std::string {
    std::stringstream ss;
    if (is_negated_clause()) {
        ss << "Not";
    }
    ss << "SuchThat(" << rel_ref << ")";
    return ss.str();
}

auto SuchThatClause::operator==(const SuchThatClause& other) const -> bool {
    return rel_ref == other.rel_ref && is_negated_clause() == other.is_negated_clause();
}

auto PatternClause::representation() const -> std::string {
    std::stringstream ss;
    if (is_negated_clause()) {
        ss << "Not";
    }
    ss << "Pattern(" << syntactic_pattern << ")";
    return ss.str();
}

auto PatternClause::operator==(const PatternClause& other) const -> bool {
    return syntactic_pattern == other.syntactic_pattern && is_negated_clause() == other.is_negated_clause();
}

auto WithClause::representation() const -> std::string {
    std::stringstream ss;
    if (is_negated_clause()) {
        ss << "Not";
    }
    ss << "With(" << ref1 << ", " << ref2 << ")";
    return ss.str();
}

auto WithClause::operator==(const WithClause& other) const -> bool {
    return ref1 == other.ref1 && ref2 == other.ref2 && is_negated_clause() == other.is_negated_clause();
}
} // namespace qps
