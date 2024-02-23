#pragma once

#include <utility>

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/relationship/clause_evaluator.hpp"
#include "qps/parser/entities/relationship.hpp"

namespace qps {
class UsesSEvaluator : public ClauseEvaluator {
    std::shared_ptr<ReadFacade> read_facade;
    UsesS uses_s;

    auto select_eval_method();

    // e.g. Uses(a/pn/s, v)
    auto eval_uses_s(const std::shared_ptr<StmtSynonym>& stmt_synonym,
                     const std::shared_ptr<VarSynonym>& var_synonym) -> std::optional<Table>;

    // e.g. Uses(a/pn/s, "v")
    auto eval_uses_s(const std::shared_ptr<StmtSynonym>& stmt_synonym,
                     const qps::QuotedIdent& quoted_ident) -> std::optional<Table>;

    // e.g. Uses(a/pn/s, _)
    auto eval_uses_s(const std::shared_ptr<StmtSynonym>& stmt_synonym,
                     const qps::WildCard&) -> std::optional<Table>;

    // e.g. Uses(3, v)
    auto eval_uses_s(const qps::Integer& stmt_num,
                     const std::shared_ptr<VarSynonym>& var_synonym) -> std::optional<Table>;

    // e.g. Uses(3, "v")
    auto eval_uses_s(const qps::Integer& stmt_num,
                     const qps::QuotedIdent& quoted_ident) -> std::optional<Table>;

    // e.g. Uses(3, _)
    auto eval_uses_s(const qps::Integer& stmt_num,
                     const qps::WildCard&) -> std::optional<Table>;

  public:
    UsesSEvaluator(std::shared_ptr<ReadFacade> read_facade, UsesS uses_s)
        : ClauseEvaluator(), read_facade(std::move(read_facade)), uses_s(std::move(uses_s)) {
    }

    auto evaluate() -> std::optional<Table> override;
};

} // namespace qps