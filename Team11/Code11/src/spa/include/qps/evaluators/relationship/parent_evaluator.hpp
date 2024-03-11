#pragma once

#include <utility>

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/relationship/clause_evaluator.hpp"
#include "qps/parser/entities/relationship.hpp"

namespace qps {
class ParentEvaluator : public ClauseEvaluator {
    std::shared_ptr<ReadFacade> read_facade;
    Parent parent;

    [[nodiscard]] auto select_eval_method() const;

    // e.g. Parent(s1, s2)
    [[nodiscard]] auto eval_parent(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                                   const std::shared_ptr<StmtSynonym>& stmt_syn_2) const -> OutputTable;

    // e.g. Parent(s1, 3)
    [[nodiscard]] auto eval_parent(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const qps::Integer& stmt_num_2) const
        -> OutputTable;

    // e.g. Parent(s1, _)
    [[nodiscard]] auto eval_parent(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const qps::WildCard&) const
        -> OutputTable;

    // e.g. Parent(3, s2)
    [[nodiscard]] auto eval_parent(const qps::Integer& stmt_num_1, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const
        -> OutputTable;

    // e.g. Parent(3, 4)
    [[nodiscard]] auto eval_parent(const qps::Integer& stmt_num_1, const qps::Integer& stmt_num_2) const -> OutputTable;

    // e.g. Parent(3, _)
    [[nodiscard]] auto eval_parent(const qps::Integer& stmt_num_1, const qps::WildCard&) const -> OutputTable;

    // e.g. Parent(_, s2)
    [[nodiscard]] auto eval_parent(const qps::WildCard&, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const
        -> OutputTable;

    // e.g. Parent(_, 3)
    [[nodiscard]] auto eval_parent(const qps::WildCard&, const qps::Integer& stmt_num_2) const -> OutputTable;

    // e.g. Parent(_, _)
    [[nodiscard]] auto eval_parent(const qps::WildCard&, const qps::WildCard&) const -> OutputTable;

  public:
    ParentEvaluator(std::shared_ptr<ReadFacade> read_facade, Parent parent)
        : ClauseEvaluator(), read_facade(std::move(read_facade)), parent(std::move(parent)) {
    }

    [[nodiscard]] auto evaluate() const -> OutputTable override;
};

} // namespace qps
