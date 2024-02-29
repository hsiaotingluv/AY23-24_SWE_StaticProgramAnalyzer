#include "qps/parser/entities/syntactic_pattern.hpp"

namespace qps {
auto operator<<(std::ostream& os, const PatternAssign& pattern_assign) -> std::ostream& {
    return os << "Assign(" << pattern_assign.assign_synonym << ", " << pattern_assign.ent_ref << ", "
              << pattern_assign.expression_spec << ")";
}

auto PatternAssign::operator==(const PatternAssign& other) const -> bool {
    return assign_synonym == other.assign_synonym && ent_ref == other.ent_ref &&
           expression_spec == other.expression_spec;
}
} // namespace qps
