#pragma once

#include <utility>

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/clause_evaluators/clause_evaluator.hpp"
#include "qps/evaluators/data_source.hpp"
#include "qps/parser/entities/relationship.hpp"

namespace qps {
class UsesSEvaluator : public ClauseEvaluator {
    UsesS uses_s;

    [[nodiscard]] auto select_eval_method() const;

    // e.g. Uses(a/pn/s, v)
    [[nodiscard]] auto eval_uses_s(const std::shared_ptr<StmtSynonym>& stmt_synonym,
                                   const std::shared_ptr<VarSynonym>& var_synonym) const -> OutputTable;

    // e.g. Uses(a/pn/s, "v")
    [[nodiscard]] auto eval_uses_s(const std::shared_ptr<StmtSynonym>& stmt_synonym,
                                   const qps::QuotedIdent& quoted_ident) const -> OutputTable;

    // e.g. Uses(a/pn/s, _)
    [[nodiscard]] auto eval_uses_s(const std::shared_ptr<StmtSynonym>& stmt_synonym, const qps::WildCard&) const
        -> OutputTable;

    // e.g. Uses(3, v)
    [[nodiscard]] auto eval_uses_s(const qps::Integer& stmt_num, const std::shared_ptr<VarSynonym>& var_synonym) const
        -> OutputTable;

    // e.g. Uses(3, "v")
    [[nodiscard]] auto eval_uses_s(const qps::Integer& stmt_num, const qps::QuotedIdent& quoted_ident) const
        -> OutputTable;

    // e.g. Uses(3, _)
    [[nodiscard]] auto eval_uses_s(const qps::Integer& stmt_num, const qps::WildCard&) const -> OutputTable;

  public:
    UsesSEvaluator(DataSource data_source, std::shared_ptr<pkb::ReadFacade> read_facade, UsesS uses_s, bool is_negated)
        : ClauseEvaluator(std::move(data_source), std::move(read_facade), is_negated), uses_s(std::move(uses_s)) {
    }

    [[nodiscard]] auto evaluate_positive() const -> OutputTable override;
};

} // namespace qps