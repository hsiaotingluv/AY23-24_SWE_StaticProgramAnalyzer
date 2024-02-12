#pragma once
#include "qps/parser/entities/relationship.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/parser/expression_parser.hpp"

namespace qps {

struct Clause {
    virtual ~Clause() = default;

    virtual auto operator<<(std::ostream& os) const -> std::ostream& {
        return os;
    }

    auto operator==(const Clause& rhs) const -> bool {
        if (typeid(*this) != typeid(rhs)) {
            return false;
        }
        return is_equal(rhs);
    }

    [[nodiscard]] virtual auto is_equal(const Clause& other) const -> bool = 0;
};

struct SuchThatClause : Clause {
    Relationship rel_ref;

    SuchThatClause(Relationship rel_ref) : rel_ref(std::move(rel_ref)) {
    }

    auto operator<<(std::ostream& os) const -> std::ostream& override;

    auto operator==(const SuchThatClause& other) const -> bool;

    [[nodiscard]] auto is_equal(const Clause& other) const -> bool override {
        if (typeid(other) != typeid(SuchThatClause)) {
            return false;
        }
        return *this == dynamic_cast<const SuchThatClause&>(other);
    }
};

struct PatternClause : Clause {
    AssignSynonym assign_synonym;
    EntRef ent_ref;
    ExpressionSpec expression_spec;

    PatternClause(AssignSynonym assign_synonym, EntRef ent_ref, ExpressionSpec expression_spec)
        : assign_synonym(std::move(assign_synonym)), ent_ref(std::move(ent_ref)),
          expression_spec(std::move(expression_spec)) {
    }

    auto operator<<(std::ostream& os) const -> std::ostream& override;

    auto operator==(const PatternClause& other) const -> bool;

    [[nodiscard]] auto is_equal(const Clause& other) const -> bool override {
        if (typeid(other) != typeid(PatternClause)) {
            return false;
        }
        return *this == dynamic_cast<const PatternClause&>(other);
    }
};

} // namespace qps
