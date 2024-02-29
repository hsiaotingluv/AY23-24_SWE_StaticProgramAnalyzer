#pragma once
#include "qps/parser/entities/relationship.hpp"
#include "qps/parser/entities/syntactic_pattern.hpp"

namespace qps {

struct Clause {
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
};

struct SuchThatClause : public Clause {
    Relationship rel_ref;

    explicit SuchThatClause(Relationship rel_ref) : rel_ref(std::move(rel_ref)) {
    }

    [[nodiscard]] auto representation() const -> std::string override;

    auto operator==(const SuchThatClause& other) const -> bool;

    [[nodiscard]] auto is_equal(const Clause& other) const -> bool override {
        if (typeid(other) != typeid(SuchThatClause)) {
            return false;
        }
        return *this == dynamic_cast<const SuchThatClause&>(other);
    }
};

struct PatternClause : public Clause {
    SyntacticPattern syntactic_pattern;

    explicit PatternClause(SyntacticPattern syntactic_pattern) : syntactic_pattern(std::move(syntactic_pattern)) {
    }

    [[nodiscard]] auto representation() const -> std::string override;

    auto operator==(const PatternClause& other) const -> bool;

    [[nodiscard]] auto is_equal(const Clause& other) const -> bool override {
        if (typeid(other) != typeid(PatternClause)) {
            return false;
        }
        return *this == dynamic_cast<const PatternClause&>(other);
    }
};

} // namespace qps
