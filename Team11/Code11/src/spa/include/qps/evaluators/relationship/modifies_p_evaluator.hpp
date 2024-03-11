#pragma once

#include <utility>

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/relationship/clause_evaluator.hpp"
#include "qps/parser/entities/relationship.hpp"

namespace qps {
    class ModifiesPEvaluator : public ClauseEvaluator {
        std::shared_ptr<ReadFacade> read_facade;
        ModifiesP modifies_p;

        [[nodiscard]] auto select_eval_method() const;

        // e.g. Modifies(p, v)
        [[nodiscard]] auto eval_modifies_p(const std::shared_ptr<ProcSynonym>& proc_synonym,
                                       const std::shared_ptr<VarSynonym>& var_synonym) const -> OutputTable;

        // e.g. Modifies(p, "v")
        [[nodiscard]] auto eval_modifies_p(const std::shared_ptr<ProcSynonym>& proc_synonym,
                                       const qps::QuotedIdent& quoted_var) const -> OutputTable;

        // e.g. Modifies(p, _)
        [[nodiscard]] auto eval_modifies_p(const std::shared_ptr<ProcSynonym>& proc_synonym,
                                       const qps::WildCard&) const -> OutputTable;

        // e.g. Modifies("p", v)
        [[nodiscard]] auto eval_modifies_p(const qps::QuotedIdent& quoted_proc,
                                       const std::shared_ptr<VarSynonym>& var_synonym) const -> OutputTable;

        // e.g. Modifies("p", "v")
        [[nodiscard]] auto eval_modifies_p(const qps::QuotedIdent& quoted_proc,
                                       const qps::QuotedIdent& quoted_var) const -> OutputTable;

        // e.g. Modifies("p", _)
        [[nodiscard]] auto eval_modifies_p(const qps::QuotedIdent& quoted_proc,
                                       const qps::WildCard&) const -> OutputTable;

    public:
        ModifiesPEvaluator(std::shared_ptr<ReadFacade> read_facade, ModifiesP modifies_p)
                : ClauseEvaluator(), read_facade(std::move(read_facade)), modifies_p(std::move(modifies_p)) {
        }

        [[nodiscard]] auto evaluate() const -> OutputTable override;
    };

} // namespace qps