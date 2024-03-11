#pragma once

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/relationship/clause_evaluator.hpp"
#include "qps/parser/entities/relationship.hpp"

#include <memory>
#include <utility>

namespace qps {

class ModifiesSEvaluator : public ClauseEvaluator {
    std::shared_ptr<ReadFacade> read_facade;
    ModifiesS modifies_s;

    [[nodiscard]] auto select_eval_method() const;

    // e.g. Modifies(s1, v)
    [[nodiscard]] auto eval_modifies_s(const std::shared_ptr<qps::StmtSynonym>& stmt_synonym,
                                       const std::shared_ptr<qps::VarSynonym>& var_syn) const -> OutputTable;

    // e.g. Modifies(s1, "v")
    [[nodiscard]] auto eval_modifies_s(const std::shared_ptr<qps::StmtSynonym>& stmt_synonym,
                                       const qps::QuotedIdent& identifier) const -> OutputTable;

    // e.g. Modifies(s1, _)
    [[nodiscard]] auto eval_modifies_s(const std::shared_ptr<qps::StmtSynonym>& stmt_synonym,
                                       const qps::WildCard&) const -> OutputTable;

    // e.g. Modifies(3, v)
    [[nodiscard]] auto eval_modifies_s(const qps::Integer& stmt_num,
                                       const std::shared_ptr<qps::VarSynonym>& var_syn) const -> OutputTable;

    // e.g. Modifies(3, "v")
    [[nodiscard]] auto eval_modifies_s(const qps::Integer& stmt_num, const qps::QuotedIdent& identifier) const
        -> OutputTable;

    // e.g. Modifies(3, _)
    [[nodiscard]] auto eval_modifies_s(const qps::Integer& stmt_num, const qps::WildCard&) const -> OutputTable;

  public:
    ModifiesSEvaluator(std::shared_ptr<ReadFacade> read_facade, ModifiesS modifies_s)
        : ClauseEvaluator(), read_facade(std::move(read_facade)), modifies_s(std::move(modifies_s)) {
    }

    [[nodiscard]] auto evaluate() const -> OutputTable override;
};
} // namespace qps
