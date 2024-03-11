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

auto operator<<(std::ostream& os, const PatternWhile& pattern_while) -> std::ostream& {
    return os << "While(" << pattern_while.while_synonym << ", " << pattern_while.ent_ref << ")";
}

auto PatternWhile::operator==(const PatternWhile& other) const -> bool {
    return while_synonym == other.while_synonym && ent_ref == other.ent_ref;
}

auto operator<<(std::ostream& os, const PatternIf& pattern_if) -> std::ostream& {
    return os << "While(" << pattern_if.if_synonym << ", " << pattern_if.ent_ref << ")";
}

auto PatternIf::operator==(const PatternIf& other) const -> bool {
    return if_synonym == other.if_synonym && ent_ref == other.ent_ref;
}
} // namespace qps
