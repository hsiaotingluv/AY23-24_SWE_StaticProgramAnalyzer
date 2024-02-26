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

auto PatternAssignClause::representation() const -> std::string {
    std::stringstream ss;
    ss << "Pattern(" << assign_synonym << ", " << ent_ref << ", " << expression_spec << ")";
    return ss.str();
}

auto PatternAssignClause::operator==(const PatternAssignClause& other) const -> bool {
    return assign_synonym == other.assign_synonym && ent_ref == other.ent_ref &&
           expression_spec == other.expression_spec;
}
} // namespace qps
