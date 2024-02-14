#pragma once

#include "qps/parser/entities/untyped/relationship.hpp"
#include "qps/parser/entities/untyped/synonym.hpp"
#include "qps/parser/expression_parser.hpp"
#include <variant>

namespace qps::untyped {
struct UntypedSuchThatClause {
    UntypedRelationship rel_ref;

    UntypedSuchThatClause(UntypedRelationship rel_ref) : rel_ref(std::move(rel_ref)) {
    }

    auto operator==(const UntypedSuchThatClause& rhs) const -> bool {
        return rel_ref == rhs.rel_ref;
    }

    friend auto operator<<(std::ostream& os, const UntypedSuchThatClause& clause) -> std::ostream& {
        std::visit(
            [&os](const auto& rel) {
                auto first = std::get<0>(rel);
                os << first;
            },
            clause.rel_ref);

        return os;
    }
};

struct UntypedPatternClause {
    UntypedSynonym assign_synonym;
    UntypedEntRef ent_ref;
    ExpressionSpec expression_spec;

    UntypedPatternClause(UntypedSynonym assign_synonym, UntypedEntRef ent_ref, ExpressionSpec expression_spec)
        : assign_synonym(std::move(assign_synonym)), ent_ref(std::move(ent_ref)),
          expression_spec(std::move(expression_spec)) {
    }

    auto operator==(const UntypedPatternClause& rhs) const -> bool {
        return assign_synonym == rhs.assign_synonym && ent_ref == rhs.ent_ref && expression_spec == rhs.expression_spec;
    }
};

using UntypedClause = std::variant<UntypedSuchThatClause, UntypedPatternClause>;
} // namespace qps::untyped