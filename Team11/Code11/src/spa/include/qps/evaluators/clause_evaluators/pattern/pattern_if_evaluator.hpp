#pragma once

#include <utility>

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/clause_evaluators/clause_evaluator.hpp"
#include "qps/parser/entities/syntactic_pattern.hpp"

namespace qps {
class PatternIfEvaluator : public ClauseEvaluator {
    PatternIf pattern;

    [[nodiscard]] auto select_eval_method() const;

    // e.g. pattern if(v, _, _)
    [[nodiscard]] auto eval_pattern_if(const std::shared_ptr<Synonym>& syn) const -> OutputTable;

    // e.g. pattern if("v", _, _)
    [[nodiscard]] auto eval_pattern_if(const qps::QuotedIdent& quoted_var) const -> OutputTable;

    // e.g. pattern if(_, _, _)
    [[nodiscard]] auto eval_pattern_if(const qps::WildCard&) const -> OutputTable;

  public:
    PatternIfEvaluator(std::shared_ptr<pkb::ReadFacade> read_facade, PatternIf pattern, bool is_negated)
        : ClauseEvaluator(std::move(read_facade), is_negated), pattern(std::move(pattern)) {
    }

    [[nodiscard]] auto evaluate_positive() const -> OutputTable override;
};

} // namespace qps