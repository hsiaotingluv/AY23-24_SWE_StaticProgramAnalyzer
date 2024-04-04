#pragma once

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/clause_evaluators/clause_evaluator.hpp"
#include "qps/parser/entities/relationship.hpp"

#include <memory>
#include <utility>

namespace qps {

class ModifiesSEvaluator : public ClauseEvaluator {
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
    ModifiesSEvaluator(DataSource data_source, std::shared_ptr<pkb::ReadFacade> read_facade, ModifiesS modifies_s,
                       bool is_negated)
        : ClauseEvaluator(std::move(data_source), std::move(read_facade), is_negated),
          modifies_s(std::move(modifies_s)) {
    }

    [[nodiscard]] auto evaluate_positive() const -> OutputTable override;
};
} // namespace qps
