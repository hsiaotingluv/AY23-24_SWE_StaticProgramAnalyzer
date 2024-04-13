#pragma once
#include "qps/parser/entities/attribute.hpp"
#include "qps/parser/entities/relationship.hpp"
#include "qps/parser/entities/syntactic_pattern.hpp"

namespace qps {

class Clause {
    bool is_negated;

  public:
    Clause(bool is_negated) : is_negated(is_negated) {
    }

    virtual ~Clause() = default;

    friend auto operator<<(std::ostream& os, const Clause& clause) -> std::ostream& {
        return os << clause.representation();
    }

    [[nodiscard]] virtual auto representation() const -> std::string = 0;

    auto operator==(const Clause& rhs) const -> bool {
        if (typeid(*this) != typeid(rhs)) {
            return false;
        }
        return is_equal(rhs);
    }

    [[nodiscard]] virtual auto is_equal(const Clause& other) const -> bool = 0;

    [[nodiscard]] virtual auto is_equal_modulo_negation(const Clause& other) const -> bool = 0;

    [[nodiscard]] auto is_negated_clause() const -> bool {
        return is_negated;
    }
};

struct SuchThatClause final : public Clause {
    Relationship rel_ref;

    explicit SuchThatClause(Relationship rel_ref, bool is_negated) : Clause(is_negated), rel_ref(std::move(rel_ref)) {
    }

    [[nodiscard]] auto representation() const -> std::string override;

    [[nodiscard]] auto is_equal_modulo_negation(const SuchThatClause& other) const -> bool;

    [[nodiscard]] auto is_equal_modulo_negation(const Clause& other) const -> bool override {
        if (typeid(other) != typeid(SuchThatClause)) {
            return false;
        }
        return is_equal_modulo_negation(dynamic_cast<const SuchThatClause&>(other));
    }

    auto operator==(const SuchThatClause& other) const -> bool;

    [[nodiscard]] auto is_equal(const Clause& other) const -> bool override {
        if (typeid(other) != typeid(SuchThatClause)) {
            return false;
        }
        return *this == dynamic_cast<const SuchThatClause&>(other);
    }
};

struct PatternClause final : public Clause {
    SyntacticPattern syntactic_pattern;

    explicit PatternClause(SyntacticPattern syntactic_pattern, bool is_negated)
        : Clause(is_negated), syntactic_pattern(std::move(syntactic_pattern)) {
    }

    [[nodiscard]] auto representation() const -> std::string override;

    [[nodiscard]] auto is_equal_modulo_negation(const PatternClause& other) const -> bool;

    [[nodiscard]] auto is_equal_modulo_negation(const Clause& other) const -> bool override {
        if (typeid(other) != typeid(PatternClause)) {
            return false;
        }
        return is_equal_modulo_negation(dynamic_cast<const PatternClause&>(other));
    }

    auto operator==(const PatternClause& other) const -> bool;

    [[nodiscard]] auto is_equal(const Clause& other) const -> bool override {
        if (typeid(other) != typeid(PatternClause)) {
            return false;
        }
        return *this == dynamic_cast<const PatternClause&>(other);
    }
};

struct WithClause final : public Clause {
    TypedRef ref1;
    TypedRef ref2;

    WithClause(TypedRef ref1, TypedRef ref2, bool is_negated)
        : Clause(is_negated), ref1(std::move(ref1)), ref2(std::move(ref2)) {
    }

    [[nodiscard]] auto representation() const -> std::string override;

    [[nodiscard]] auto is_equal_modulo_negation(const WithClause& other) const -> bool;

    [[nodiscard]] auto is_equal_modulo_negation(const Clause& other) const -> bool override {
        if (typeid(other) != typeid(WithClause)) {
            return false;
        }
        return is_equal_modulo_negation(dynamic_cast<const WithClause&>(other));
    }

    auto operator==(const WithClause& other) const -> bool;

    [[nodiscard]] auto is_equal(const Clause& other) const -> bool override {
        if (typeid(other) != typeid(WithClause)) {
            return false;
        }
        return *this == dynamic_cast<const WithClause&>(other);
    }
};

} // namespace qps
