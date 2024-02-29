#include "qps/parser/entities/clause.hpp"

#include <sstream>
#include <string>

namespace qps {
auto SuchThatClause::representation() const -> std::string {
    std::stringstream ss;
    ss << "SuchThat(" << rel_ref << ")";
    return ss.str();
}

auto SuchThatClause::operator==(const SuchThatClause& other) const -> bool {
    return rel_ref == other.rel_ref;
}

auto PatternClause::representation() const -> std::string {
    std::stringstream ss;
    ss << "Pattern(" << syntactic_pattern << ")";
    return ss.str();
}

auto PatternClause::operator==(const PatternClause& other) const -> bool {
    return syntactic_pattern == other.syntactic_pattern;
}
} // namespace qps
