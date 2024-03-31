#pragma once

#include "qps/parser/expression_parser.hpp"
#include "qps/parser/untyped/entities/attribute.hpp"
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
  private:
    static constexpr auto NUM_ARG = 2;

  public:
    UntypedSynonym synonym;
    UntypedEntRef ent_ref;
    ExpressionSpec expression_spec;
    int num_arg;

    UntypedPatternClause(UntypedSynonym synonym, UntypedEntRef ent_ref, ExpressionSpec expression_spec,
                         int num_arg = NUM_ARG)
        : synonym(std::move(synonym)), ent_ref(std::move(ent_ref)), expression_spec(std::move(expression_spec)),
          num_arg(num_arg) {
    }

    auto operator==(const UntypedPatternClause& rhs) const -> bool {
        return synonym == rhs.synonym && ent_ref == rhs.ent_ref && expression_spec == rhs.expression_spec;
    }

    friend auto operator<<(std::ostream& os, const UntypedPatternClause& clause) -> std::ostream& {
        os << clause.synonym << "(" << clause.ent_ref << ", " << clause.expression_spec << ") " << clause.num_arg;
        return os;
    }
};

struct UntypedWithClause {
    UntypedRef ref1;
    UntypedRef ref2;

    UntypedWithClause(UntypedRef ref1, UntypedRef ref2) : ref1(std::move(ref1)), ref2(std::move(ref2)) {
    }

    auto operator==(const UntypedWithClause& rhs) const -> bool {
        return ref1 == rhs.ref1 && ref2 == rhs.ref2;
    }

    friend auto operator<<(std::ostream& os, const UntypedWithClause& clause) -> std::ostream& {
        os << clause.ref1 << " = " << clause.ref2;
        return os;
    }
};

using UntypedClause = std::variant<UntypedSuchThatClause, UntypedPatternClause, UntypedWithClause>;
} // namespace qps::untyped