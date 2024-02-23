#pragma once

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/relationship/clause_evaluator.hpp"
#include "qps/parser/entities/relationship.hpp"

#include <memory>
#include <optional>
#include <utility>

namespace qps {

class ModifiesSEvaluator : public ClauseEvaluator {
    std::shared_ptr<ReadFacade> read_facade;
    ModifiesS modifies_s;

    auto select_eval_method();

    // e.g. Modifies(s1, v)
    auto eval_modifies_s(const std::shared_ptr<qps::StmtSynonym>& stmt_synonym,
                         const std::shared_ptr<qps::VarSynonym>& var_syn) -> std::optional<Table>;

    // e.g. Modifies(s1, "v")
    auto eval_modifies_s(const std::shared_ptr<qps::StmtSynonym>& stmt_synonym,
                         const qps::QuotedIdent& identifier) -> std::optional<Table>;

    // e.g. Modifies(s1, _)
    auto eval_modifies_s(const std::shared_ptr<qps::StmtSynonym>& stmt_synonym,
                         const qps::WildCard&) -> std::optional<Table>;

    // e.g. Modifies(3, v)
    auto eval_modifies_s(const qps::Integer& stmt_num,
                         const std::shared_ptr<qps::VarSynonym>& var_syn) -> std::optional<Table>;

    // e.g. Modifies(3, "v")
    auto eval_modifies_s(const qps::Integer& stmt_num,
                         const qps::QuotedIdent& identifier) -> std::optional<Table>;

    // e.g. Modifies(3, _)
    auto eval_modifies_s(const qps::Integer& stmt_num,
                         const qps::WildCard&) -> std::optional<Table>;

public:
    ModifiesSEvaluator(std::shared_ptr<ReadFacade> read_facade, ModifiesS modifies_s)
        : ClauseEvaluator(), read_facade(std::move(read_facade)), modifies_s(std::move(modifies_s)) {
    }

    auto evaluate() -> std::optional<Table> override;
};
} // namespace qps
