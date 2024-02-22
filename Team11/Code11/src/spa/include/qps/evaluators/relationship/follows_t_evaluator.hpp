#pragma once

#include <utility>

#include "pkb/facades/read_facade.h"

#include "qps/evaluators/relationship/clause_evaluator.hpp"
#include "qps/parser/entities/relationship.hpp"

namespace qps {
class FollowsTEvaluator : public ClauseEvaluator {
    std::shared_ptr<ReadFacade> read_facade;
    FollowsT follows_t;

    auto select_eval_method();

    // e.g. Follows*(s1, s2)
    auto eval_follows_t(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                        const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> std::optional<Table>;

    // e.g. Follows*(s1, 3)
    auto eval_follows_t(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                        const qps::Integer& stmt_num_2) -> std::optional<Table>;

    // e.g. Follows*(s1, _)
    auto eval_follows_t(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                        const qps::WildCard&) -> std::optional<Table>;

    // e.g. Follows*(3, s2)
    auto eval_follows_t(const qps::Integer& stmt_num_1,
                        const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> std::optional<Table>;

    // e.g. Follows*(3, 4)
    auto eval_follows_t(const qps::Integer& stmt_num_1,
                        const qps::Integer& stmt_num_2) -> std::optional<Table>;

    // e.g. Follows*(3, _)
    auto eval_follows_t(const qps::Integer& stmt_num_1,
                        const qps::WildCard&) -> std::optional<Table>;

    // e.g. Follows*(_, s2)
    auto eval_follows_t(const qps::WildCard&,
                        const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> std::optional<Table>;

    // e.g. Follows*(_, 3)
    auto eval_follows_t(const qps::WildCard&,
                        const qps::Integer& stmt_num_2) -> std::optional<Table>;

    // e.g. Follows*(_, _)
    auto eval_follows_t(const qps::WildCard&,
                        const qps::WildCard&) -> std::optional<Table>;

public:
    FollowsTEvaluator(std::shared_ptr<ReadFacade> read_facade, FollowsT follows_t)
        : ClauseEvaluator(), read_facade(std::move(read_facade)), follows_t(std::move(follows_t)) {
    }

    auto evaluate() -> std::optional<Table> override;
};

} // namespace qps