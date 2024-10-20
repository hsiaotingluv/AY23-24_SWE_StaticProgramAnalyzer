#pragma once

#include <memory>
#include <utility>

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/clause_evaluators/clause_evaluator.hpp"
#include "qps/evaluators/data_source.hpp"
#include "qps/parser/entities/relationship.hpp"

namespace qps {
class FollowsEvaluator : public ClauseEvaluator {
    Follows follows;

    // visitor method to determine which method to employ per clause
    [[nodiscard]] auto select_eval_method() const;

    // different evaluation methods for different param types

    // e.g. Follows(s1, s2)
    [[nodiscard]] auto eval_follows(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                                    const std::shared_ptr<StmtSynonym>& stmt_syn_2) const -> OutputTable;

    // e.g. Follows(s1, 3)
    [[nodiscard]] auto eval_follows(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                                    const qps::Integer& stmt_num_2) const -> OutputTable;

    // e.g. Follows(s1, _)
    [[nodiscard]] auto eval_follows(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const qps::WildCard&) const
        -> OutputTable;

    // e.g. Follows(3, s2)
    [[nodiscard]] auto eval_follows(const qps::Integer& stmt_num_1,
                                    const std::shared_ptr<StmtSynonym>& stmt_syn_2) const -> OutputTable;

    // e.g. Follows(3, 4)
    [[nodiscard]] auto eval_follows(const qps::Integer& stmt_num_1, const qps::Integer& stmt_num_2) const
        -> OutputTable;

    // e.g. Follows(3, _)
    [[nodiscard]] auto eval_follows(const qps::Integer& stmt_num_1, const qps::WildCard&) const -> OutputTable;

    // e.g. Follows(_, s2)
    [[nodiscard]] auto eval_follows(const qps::WildCard&, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const
        -> OutputTable;

    // e.g. Follows(_, 3)
    [[nodiscard]] auto eval_follows(const qps::WildCard&, const qps::Integer& stmt_num_2) const -> OutputTable;

    // e.g. Follows(_, _)
    [[nodiscard]] auto eval_follows(const qps::WildCard&, const qps::WildCard&) const -> OutputTable;

  public:
    FollowsEvaluator(DataSource data_source, std::shared_ptr<pkb::ReadFacade> read_facade, Follows follows,
                     bool is_negated)
        : ClauseEvaluator(std::move(data_source), std::move(read_facade), is_negated), follows(std::move(follows)) {
    }

    [[nodiscard]] auto evaluate_positive() const -> OutputTable override;
};

} // namespace qps
