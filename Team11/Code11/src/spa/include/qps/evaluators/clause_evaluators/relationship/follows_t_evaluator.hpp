#pragma once

#include <utility>

#include "pkb/facades/read_facade.h"

#include "qps/evaluators/clause_evaluators/clause_evaluator.hpp"
#include "qps/parser/entities/relationship.hpp"

namespace qps {
class FollowsTEvaluator : public ClauseEvaluator {
    std::shared_ptr<pkb::ReadFacade> read_facade;
    FollowsT follows_t;

    [[nodiscard]] auto select_eval_method() const;

    // e.g. Follows*(s1, s2)
    [[nodiscard]] auto eval_follows_t(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                                      const std::shared_ptr<StmtSynonym>& stmt_syn_2) const -> OutputTable;

    // e.g. Follows*(s1, 3)
    [[nodiscard]] auto eval_follows_t(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                                      const qps::Integer& stmt_num_2) const -> OutputTable;

    // e.g. Follows*(s1, _)
    [[nodiscard]] auto eval_follows_t(const std::shared_ptr<StmtSynonym>& stmt_syn_1, const qps::WildCard&) const
        -> OutputTable;

    // e.g. Follows*(3, s2)
    [[nodiscard]] auto eval_follows_t(const qps::Integer& stmt_num_1,
                                      const std::shared_ptr<StmtSynonym>& stmt_syn_2) const -> OutputTable;

    // e.g. Follows*(3, 4)
    [[nodiscard]] auto eval_follows_t(const qps::Integer& stmt_num_1, const qps::Integer& stmt_num_2) const
        -> OutputTable;

    // e.g. Follows*(3, _)
    [[nodiscard]] auto eval_follows_t(const qps::Integer& stmt_num_1, const qps::WildCard&) const -> OutputTable;

    // e.g. Follows*(_, s2)
    [[nodiscard]] auto eval_follows_t(const qps::WildCard&, const std::shared_ptr<StmtSynonym>& stmt_syn_2) const
        -> OutputTable;

    // e.g. Follows*(_, 3)
    [[nodiscard]] auto eval_follows_t(const qps::WildCard&, const qps::Integer& stmt_num_2) const -> OutputTable;

    // e.g. Follows*(_, _)
    [[nodiscard]] auto eval_follows_t(const qps::WildCard&, const qps::WildCard&) const -> OutputTable;

  public:
    FollowsTEvaluator(std::shared_ptr<pkb::ReadFacade> read_facade, FollowsT follows_t)
        : ClauseEvaluator(), read_facade(std::move(read_facade)), follows_t(std::move(follows_t)) {
    }

    auto evaluate() const -> OutputTable override;
};

} // namespace qps