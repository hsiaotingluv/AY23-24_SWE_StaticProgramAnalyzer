#pragma once

#include <memory>
#include <utility>

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/relationship/clause_evaluator.hpp"
#include "qps/parser/entities/relationship.hpp"

namespace qps {
class FollowsEvaluator : public ClauseEvaluator {
    std::shared_ptr<ReadFacade> read_facade;
    Follows follows;

    // visitor method to determine which method to employ per clause
    auto select_eval_method();

    // different evaluation methods for different param types

    // e.g. Follows(s1, s2)
    auto eval_follows(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                      const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> std::optional<Table>;

    // e.g. Follows(s1, 3)
    auto eval_follows(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                      const qps::Integer& stmt_num_2) -> std::optional<Table>;

    // e.g. Follows(s1, _)
    auto eval_follows(const std::shared_ptr<StmtSynonym>& stmt_syn_1,
                      const qps::WildCard&) -> std::optional<Table>;

    // e.g. Follows(3, s2)
    auto eval_follows(const qps::Integer& stmt_num_1,
                      const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> std::optional<Table>;

    // e.g. Follows(3, 4)
    auto eval_follows(const qps::Integer& stmt_num_1,
                      const qps::Integer& stmt_num_2) -> std::optional<Table>;

    // e.g. Follows(3, _)
    auto eval_follows(const qps::Integer& stmt_num_1,
                      const qps::WildCard&) -> std::optional<Table>;

    // e.g. Follows(_, s2)
    auto eval_follows(const qps::WildCard&,
                      const std::shared_ptr<StmtSynonym>& stmt_syn_2) -> std::optional<Table>;

    // e.g. Follows(_, 3)
    auto eval_follows(const qps::WildCard&,
                      const qps::Integer& stmt_num_2) -> std::optional<Table>;

    // e.g. Follows(_, _)
    auto eval_follows(const qps::WildCard&,
                      const qps::WildCard&) -> std::optional<Table>;

public:
    FollowsEvaluator(std::shared_ptr<ReadFacade> read_facade, Follows follows)
        : ClauseEvaluator(), read_facade(std::move(read_facade)), follows(std::move(follows)) {
    }

    auto evaluate() -> std::optional<Table> override;
};

} // namespace qps
