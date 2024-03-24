#pragma once

#include <utility>

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/relationship/clause_evaluator.hpp"
#include "qps/parser/entities/syntactic_pattern.hpp"

namespace qps {
class PatternWhileEvaluator : public ClauseEvaluator {
    std::shared_ptr<pkb::ReadFacade> read_facade;
    PatternWhile pattern;

    [[nodiscard]] auto select_eval_method() const;

    // e.g. pattern w(v, _)
    [[nodiscard]] auto eval_pattern_while(const std::shared_ptr<Synonym>& syn) const -> OutputTable;

    // e.g. pattern w("v", _)
    [[nodiscard]] auto eval_pattern_while(const qps::QuotedIdent& quoted_var) const -> OutputTable;

    // e.g. pattern w(_, _)
    [[nodiscard]] auto eval_pattern_while(const qps::WildCard&) const -> OutputTable;

  public:
    PatternWhileEvaluator(std::shared_ptr<pkb::ReadFacade> read_facade, PatternWhile pattern)
        : ClauseEvaluator(), read_facade(std::move(read_facade)), pattern(std::move(pattern)) {
    }

    [[nodiscard]] auto evaluate() const -> OutputTable override;
};

} // namespace qps