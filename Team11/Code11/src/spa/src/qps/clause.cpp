#include "qps/parser/entities/clause.hpp"

namespace qps {
auto SuchThatClause::operator<<(std::ostream& os) const -> std::ostream& {
    os << "SuchThat(" << rel_ref << ")";
    return os;
}

auto SuchThatClause::operator==(const SuchThatClause& other) const -> bool {
    return rel_ref == other.rel_ref;
}

auto PatternClause::operator<<(std::ostream& os) const -> std::ostream& {
    os << "Pattern(" << assign_synonym << ", " << ent_ref << ", " << expression_spec.value << ")";
    return os;
}

auto PatternClause::operator==(const PatternClause& other) const -> bool {
    return assign_synonym == other.assign_synonym && ent_ref == other.ent_ref &&
           expression_spec == other.expression_spec;
}
} // namespace qps
