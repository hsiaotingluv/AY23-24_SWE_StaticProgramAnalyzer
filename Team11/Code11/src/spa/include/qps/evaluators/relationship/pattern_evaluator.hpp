#pragma once

#include <utility>

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/relationship/clause_evaluator.hpp"
#include "qps/parser/entities/clause.hpp"

namespace qps {
class PatternEvaluator : public ClauseEvaluator {
    std::shared_ptr<ReadFacade> read_facade;
    PatternClause pattern;

    auto select_eval_method();

    // e.g. pattern a(x, _"v"_)
    auto eval_pattern(const std::shared_ptr<Synonym>& synonym,
                      const qps::PartialMatch& partial_match) -> std::optional<Table>;

    // e.g. pattern a(x, _)
    auto eval_pattern(const std::shared_ptr<Synonym>& synonym,
                      const qps::WildCard&) -> std::optional<Table>;

    // e.g. pattern a("x", _"v"_)
    auto eval_pattern(const qps::QuotedIdent& quoted_ident,
                      const qps::PartialMatch& partial_match) -> std::optional<Table>;

    // e.g. pattern a("x", _)
    auto eval_pattern(const qps::QuotedIdent& quoted_ident,
                      const qps::WildCard&) -> std::optional<Table>;

    // e.g. pattern a(_, _"v"_)
    auto eval_pattern(const qps::WildCard& wild_card,
                      const qps::PartialMatch& partial_match) -> std::optional<Table>;

    // e.g. pattern a(_, _)
    auto eval_pattern(const qps::WildCard&,
                      const qps::WildCard&) -> std::optional<Table>;

    // e.g. pattern a(_, "v")
    auto eval_pattern(const WildCard&,
                      const ExactMatch& exact) -> std::optional<Table>;

    // e.g. pattern a(x, "v")
    auto eval_pattern(const std::shared_ptr<Synonym>& synonym,
                      const ExactMatch& exact) -> std::optional<Table>;

    // e.g. pattern a("x", "v")
    auto eval_pattern(const QuotedIdent& quoted_ident,
                      const ExactMatch& exact) -> std::optional<Table>;

  public:
    PatternEvaluator(std::shared_ptr<ReadFacade> read_facade, PatternClause pattern)
        : ClauseEvaluator(), read_facade(std::move(read_facade)), pattern(std::move(pattern)) {
    }

    auto evaluate() -> std::optional<Table> override;
};

} // namespace qps