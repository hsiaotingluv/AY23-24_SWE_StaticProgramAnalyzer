#pragma once

#include <utility>

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/clause_evaluators/clause_evaluator.hpp"
#include "qps/parser/entities/relationship.hpp"

namespace qps {
class ModifiesPEvaluator : public ClauseEvaluator {
    ModifiesP modifies_p;

    [[nodiscard]] auto select_eval_method() const;

    // e.g. Modifies(p, v)
    [[nodiscard]] auto eval_modifies_p(const std::shared_ptr<ProcSynonym>& proc_synonym,
                                       const std::shared_ptr<VarSynonym>& var_synonym) const -> OutputTable;

    // e.g. Modifies(p, "v")
    [[nodiscard]] auto eval_modifies_p(const std::shared_ptr<ProcSynonym>& proc_synonym,
                                       const qps::QuotedIdent& quoted_var) const -> OutputTable;

    // e.g. Modifies(p, _)
    [[nodiscard]] auto eval_modifies_p(const std::shared_ptr<ProcSynonym>& proc_synonym, const qps::WildCard&) const
        -> OutputTable;

    // e.g. Modifies("p", v)
    [[nodiscard]] auto eval_modifies_p(const qps::QuotedIdent& quoted_proc,
                                       const std::shared_ptr<VarSynonym>& var_synonym) const -> OutputTable;

    // e.g. Modifies("p", "v")
    [[nodiscard]] auto eval_modifies_p(const qps::QuotedIdent& quoted_proc, const qps::QuotedIdent& quoted_var) const
        -> OutputTable;

    // e.g. Modifies("p", _)
    [[nodiscard]] auto eval_modifies_p(const qps::QuotedIdent& quoted_proc, const qps::WildCard&) const -> OutputTable;

  public:
    ModifiesPEvaluator(std::shared_ptr<pkb::ReadFacade> read_facade, ModifiesP modifies_p, bool is_negated)
        : ClauseEvaluator(std::move(read_facade), is_negated), modifies_p(std::move(modifies_p)) {
    }

    [[nodiscard]] auto evaluate_positive() const -> OutputTable override;
};

} // namespace qps