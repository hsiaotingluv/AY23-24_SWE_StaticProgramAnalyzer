#pragma once

#include "qps/parser/expression_parser.hpp"
#include "qps/parser/untyped/entities/relationship.hpp"
#include "qps/parser/untyped/entities/synonym.hpp"
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
    UntypedSynonym synonym;
    UntypedEntRef ent_ref;
    ExpressionSpec expression_spec;
    int num_arg;

    UntypedPatternClause(UntypedSynonym synonym, UntypedEntRef ent_ref, ExpressionSpec expression_spec, int num_arg = 2)
        : synonym(std::move(synonym)), ent_ref(std::move(ent_ref)), expression_spec(std::move(expression_spec)),
          num_arg(num_arg) {
    }

    auto operator==(const UntypedPatternClause& rhs) const -> bool {
        return synonym == rhs.synonym && ent_ref == rhs.ent_ref && expression_spec == rhs.expression_spec;
    }
};

using UntypedClause = std::variant<UntypedSuchThatClause, UntypedPatternClause>;
} // namespace qps::untyped