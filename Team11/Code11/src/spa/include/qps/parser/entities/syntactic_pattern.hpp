#pragma once

#include "qps/parser/entities/relationship.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/parser/expression_parser.hpp"

namespace qps {
class PatternAssign {
    std::shared_ptr<AssignSynonym> assign_synonym;
    EntRef ent_ref;
    ExpressionSpec expression_spec;

  public:
    PatternAssign(std::shared_ptr<AssignSynonym> assign_synonym, EntRef ent_ref, ExpressionSpec expression_spec)
        : assign_synonym(std::move(assign_synonym)), ent_ref(std::move(ent_ref)),
          expression_spec(std::move(expression_spec)) {
    }

    friend auto operator<<(std::ostream& os, const PatternAssign& follows) -> std::ostream&;

    auto operator==(const PatternAssign& other) const -> bool;

    [[nodiscard]] auto get_assign_synonym() const -> std::shared_ptr<AssignSynonym> {
        return assign_synonym;
    }

    [[nodiscard]] auto get_ent_ref() const -> EntRef {
        return ent_ref;
    }

    [[nodiscard]] auto get_expression_spec() const -> ExpressionSpec {
        return expression_spec;
    }
};

using SyntacticPattern = std::variant<PatternAssign>;
} // namespace qps
