#pragma once

#include <utility>

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/relationship/clause_evaluator.hpp"
#include "qps/parser/entities/clause.hpp"

namespace qps {
class PatternEvaluator : public ClauseEvaluator {
    std::shared_ptr<ReadFacade> read_facade;
    PatternClause pattern;

    [[nodiscard]] auto select_eval_method() const;

    // e.g. pattern a(x, _"v"_)
    [[nodiscard]] auto eval_pattern(const std::shared_ptr<Synonym>& synonym,
                                    const qps::PartialMatch& partial_match) const -> std::optional<Table>;

    // e.g. pattern a(x, _)
    [[nodiscard]] auto eval_pattern(const std::shared_ptr<Synonym>& synonym, const qps::WildCard&) const
        -> std::optional<Table>;

    // e.g. pattern a("x", _"v"_)
    [[nodiscard]] auto eval_pattern(const qps::QuotedIdent& quoted_ident, const qps::PartialMatch& partial_match) const
        -> std::optional<Table>;

    // e.g. pattern a("x", _)
    [[nodiscard]] auto eval_pattern(const qps::QuotedIdent& quoted_ident, const qps::WildCard&) const
        -> std::optional<Table>;

    // e.g. pattern a(_, _"v"_)
    [[nodiscard]] auto eval_pattern(const qps::WildCard& wild_card, const qps::PartialMatch& partial_match) const
        -> std::optional<Table>;

    // e.g. pattern a(_, _)
    [[nodiscard]] auto eval_pattern(const qps::WildCard&, const qps::WildCard&) const -> std::optional<Table>;

    // e.g. pattern a(_, "v")
    [[nodiscard]] auto eval_pattern(const WildCard&, const ExactMatch& exact) const -> std::optional<Table>;

    // e.g. pattern a(x, "v")
    [[nodiscard]] auto eval_pattern(const std::shared_ptr<Synonym>& synonym, const ExactMatch& exact) const
        -> std::optional<Table>;

    // e.g. pattern a("x", "v")
    [[nodiscard]] auto eval_pattern(const QuotedIdent& quoted_ident, const ExactMatch& exact) const
        -> std::optional<Table>;

  public:
    PatternEvaluator(std::shared_ptr<ReadFacade> read_facade, PatternClause pattern)
        : ClauseEvaluator(), read_facade(std::move(read_facade)), pattern(std::move(pattern)) {
    }

    auto evaluate() const -> std::optional<Table> override;
};

} // namespace qps