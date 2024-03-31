#pragma once

#include "qps/parser/expression_parser.hpp"
#include "qps/parser/untyped/entities/attribute.hpp"
#include "qps/parser/untyped/entities/relationship.hpp"
#include "qps/parser/untyped/entities/synonym.hpp"
#include <variant>

namespace qps::untyped {
struct UntypedSuchThatClause {
    UntypedRelationship rel_ref;
    bool is_negated;

    UntypedSuchThatClause(UntypedRelationship rel_ref, bool is_negated)
        : rel_ref(std::move(rel_ref)), is_negated(is_negated) {
    }

    auto operator==(const UntypedSuchThatClause& rhs) const -> bool {
        return rel_ref == rhs.rel_ref && is_negated == rhs.is_negated;
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
  private:
    static constexpr auto NUM_ARG = 2;

  public:
    UntypedSynonym synonym;
    UntypedEntRef ent_ref;
    ExpressionSpec expression_spec;
    int num_arg;
    bool is_negated;

    UntypedPatternClause(UntypedSynonym synonym, UntypedEntRef ent_ref, ExpressionSpec expression_spec, bool is_negated,
                         int num_arg = NUM_ARG)
        : synonym(std::move(synonym)), ent_ref(std::move(ent_ref)), expression_spec(std::move(expression_spec)),
          num_arg(num_arg), is_negated(is_negated) {
    }

    auto operator==(const UntypedPatternClause& rhs) const -> bool {
        return synonym == rhs.synonym && ent_ref == rhs.ent_ref && expression_spec == rhs.expression_spec &&
               is_negated == rhs.is_negated;
    }

    friend auto operator<<(std::ostream& os, const UntypedPatternClause& clause) -> std::ostream& {
        os << clause.synonym << "(" << clause.ent_ref << ", " << clause.expression_spec << ") " << clause.num_arg;
        return os;
    }
};

struct UntypedWithClause {
    UntypedRef ref1;
    UntypedRef ref2;
    bool is_negated;

    UntypedWithClause(UntypedRef ref1, UntypedRef ref2, bool is_negated)
        : ref1(std::move(ref1)), ref2(std::move(ref2)), is_negated(is_negated) {
    }

    auto operator==(const UntypedWithClause& rhs) const -> bool {
        return ref1 == rhs.ref1 && ref2 == rhs.ref2 && is_negated == rhs.is_negated;
    }

    friend auto operator<<(std::ostream& os, const UntypedWithClause& clause) -> std::ostream& {
        os << clause.ref1 << " = " << clause.ref2;
        return os;
    }
};

using UntypedClause = std::variant<UntypedSuchThatClause, UntypedPatternClause, UntypedWithClause>;
} // namespace qps::untyped