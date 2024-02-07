#include "qps/parser/entities/clause.hpp"

namespace qps {
auto operator<<(std::ostream& os, const SuchThatClause& such_that_clause) -> std::ostream& {
    os << "SuchThat(" << such_that_clause.rel_ref << ")";
    return os;
}

auto operator<<(std::ostream& os, const PatternClause& pattern_clause) -> std::ostream& {
    os << "Pattern(" << pattern_clause.assign_synonym << ", " << pattern_clause.ent_ref << ", "
       << pattern_clause.expression_spec.value << ")";
    return os;
}
} // namespace qps
