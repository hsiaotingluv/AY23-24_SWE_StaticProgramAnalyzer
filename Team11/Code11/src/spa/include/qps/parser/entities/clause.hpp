#pragma once
#include "qps/parser/entities/relationship.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/parser/expression_parser.hpp"
#include <variant>

namespace qps {
struct SuchThatClause {
    Relationship rel_ref;

    SuchThatClause(Relationship rel_ref) : rel_ref(std::move(rel_ref)) {
    }

    friend auto operator<<(std::ostream& os, const SuchThatClause& such_that_clause) -> std::ostream&;
};

struct PatternClause {
    AssignSynonym assign_synonym;
    EntRef ent_ref;
    ExpressionSpec expression_spec;

    friend auto operator<<(std::ostream& os, const PatternClause& pattern_clause) -> std::ostream&;
};

using Clause = std::variant<SuchThatClause, PatternClause>;
using Clauses = std::vector<Clause>;
} // namespace qps
