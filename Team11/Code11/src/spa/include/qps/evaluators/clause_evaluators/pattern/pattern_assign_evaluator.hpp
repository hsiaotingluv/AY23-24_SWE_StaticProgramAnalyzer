#pragma once

#include <utility>

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/clause_evaluators/clause_evaluator.hpp"
#include "qps/evaluators/data_source.hpp"
#include "qps/parser/entities/syntactic_pattern.hpp"

namespace qps {
class PatternAssignEvaluator : public ClauseEvaluator {
    PatternAssign pattern;

    [[nodiscard]] auto select_eval_method() const;

    // e.g. pattern a(x, _"v"_)
    [[nodiscard]] auto eval_pattern_assign(const std::shared_ptr<Synonym>& synonym,
                                           const qps::PartialMatch& partial_match) const -> OutputTable;

    // e.g. pattern a(x, _)
    [[nodiscard]] auto eval_pattern_assign(const std::shared_ptr<Synonym>& synonym, const qps::WildCard&) const
        -> OutputTable;

    // e.g. pattern a("x", _"v"_)
    [[nodiscard]] auto eval_pattern_assign(const qps::QuotedIdent& quoted_ident,
                                           const qps::PartialMatch& partial_match) const -> OutputTable;

    // e.g. pattern a("x", _)
    [[nodiscard]] auto eval_pattern_assign(const qps::QuotedIdent& quoted_ident, const qps::WildCard&) const
        -> OutputTable;

    // e.g. pattern a(_, _"v"_)
    [[nodiscard]] auto eval_pattern_assign(const qps::WildCard& wild_card, const qps::PartialMatch& partial_match) const
        -> OutputTable;

    // e.g. pattern a(_, _)
    [[nodiscard]] auto eval_pattern_assign(const qps::WildCard&, const qps::WildCard&) const -> OutputTable;

    // e.g. pattern a(_, "v")
    [[nodiscard]] auto eval_pattern_assign(const WildCard&, const ExactMatch& exact) const -> OutputTable;

    // e.g. pattern a(x, "v")
    [[nodiscard]] auto eval_pattern_assign(const std::shared_ptr<Synonym>& synonym, const ExactMatch& exact) const
        -> OutputTable;

    // e.g. pattern a("x", "v")
    [[nodiscard]] auto eval_pattern_assign(const QuotedIdent& quoted_ident, const ExactMatch& exact) const
        -> OutputTable;

  public:
    PatternAssignEvaluator(DataSource data_source, std::shared_ptr<pkb::ReadFacade> read_facade, PatternAssign pattern,
                           bool is_negated)
        : ClauseEvaluator(std::move(data_source), std::move(read_facade), is_negated), pattern(std::move(pattern)) {
    }

    [[nodiscard]] auto evaluate_positive() const -> OutputTable override;
};

} // namespace qps