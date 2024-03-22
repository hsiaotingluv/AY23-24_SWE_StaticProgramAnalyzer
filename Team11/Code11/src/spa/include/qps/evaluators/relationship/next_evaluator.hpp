#pragma once

#include <memory>
#include <utility>

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/relationship/clause_evaluator.hpp"
#include "qps/parser/entities/relationship.hpp"

namespace qps {
class NextEvaluator : public ClauseEvaluator {
    std::shared_ptr<pkb::ReadFacade> read_facade;
    Next next;

    // visitor method to determine which method to employ per clause
    [[nodiscard]] auto select_eval_method() const;

    // different evaluation methods for different param types

    // e.g. Next(s1, s2)
    [[nodiscard]] auto eval_next(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                                 const std::shared_ptr<StmtSynonym>& stmt_syn_2) const -> OutputTable;

    // e.g. Next(s1, 3)
    [[nodiscard]] auto eval_next(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const qps::Integer& stmt_num_2) const
        -> OutputTable;

    // e.g. Next(s1, _)
    [[nodiscard]] auto eval_next(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const qps::WildCard&) const
        -> OutputTable;

    // e.g. Next(3, s2)
    [[nodiscard]] auto eval_next(const qps::Integer& stmt_num_1, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const
        -> OutputTable;

    // e.g. Next(3, 4)
    [[nodiscard]] auto eval_next(const qps::Integer& stmt_num_1, const qps::Integer& stmt_num_2) const -> OutputTable;

    // e.g. Next(3, _)
    [[nodiscard]] auto eval_next(const qps::Integer& stmt_num_1, const qps::WildCard&) const -> OutputTable;

    // e.g. Next(_, s2)
    [[nodiscard]] auto eval_next(const qps::WildCard&, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const
        -> OutputTable;

    // e.g. Next(_, 3)
    [[nodiscard]] auto eval_next(const qps::WildCard&, const qps::Integer& stmt_num_2) const -> OutputTable;

    // e.g. Next(_, _)
    [[nodiscard]] auto eval_next(const qps::WildCard&, const qps::WildCard&) const -> OutputTable;

public:
    NextEvaluator(std::shared_ptr<pkb::ReadFacade> read_facade, Next next)
        : ClauseEvaluator(), read_facade(std::move(read_facade)), next(std::move(next)) {
    }

    [[nodiscard]] auto evaluate() const -> OutputTable override;
};

} // namespace qps
