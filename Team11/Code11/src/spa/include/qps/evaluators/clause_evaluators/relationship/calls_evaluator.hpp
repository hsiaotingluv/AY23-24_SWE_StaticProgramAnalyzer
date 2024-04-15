#pragma once

#include "qps/evaluators/clause_evaluators/clause_evaluator.hpp"
#include "qps/evaluators/data_source.hpp"

namespace qps {
class CallsEvaluator : public ClauseEvaluator {
    Calls calls;

    [[nodiscard]] auto select_eval_method() const;

    // e.g. Calls(p, q)
    [[nodiscard]] auto eval_calls(const std::shared_ptr<ProcSynonym>& proc_syn_1,
                                  const std::shared_ptr<ProcSynonym>& proc_syn_2) const -> OutputTable;

    // e.g. Calls(p, "q")
    [[nodiscard]] auto eval_calls(const std::shared_ptr<ProcSynonym>& proc_syn_1,
                                  const qps::QuotedIdent& quoted_proc_2) const -> OutputTable;

    // e.g. Calls(p, _)
    [[nodiscard]] auto eval_calls(const std::shared_ptr<ProcSynonym>& proc_syn_1, const qps::WildCard&) const
        -> OutputTable;

    // e.g. Calls("p", q)
    [[nodiscard]] auto eval_calls(const qps::QuotedIdent& quoted_proc_1,
                                  const std::shared_ptr<ProcSynonym>& proc_syn_2) const -> OutputTable;

    // e.g. Calls("p", "q")
    [[nodiscard]] auto eval_calls(const qps::QuotedIdent& quoted_proc_1, const qps::QuotedIdent& quoted_proc_2) const
        -> OutputTable;

    // e.g. Calls("p", _)
    [[nodiscard]] auto eval_calls(const qps::QuotedIdent& quoted_proc_1, const qps::WildCard&) const -> OutputTable;

    // e.g. Calls(_, q)
    [[nodiscard]] auto eval_calls(const qps::WildCard&, const std::shared_ptr<ProcSynonym>& proc_syn_2) const
        -> OutputTable;

    // e.g. Calls(_, "q")
    [[nodiscard]] auto eval_calls(const qps::WildCard&, const qps::QuotedIdent& quoted_proc_2) const -> OutputTable;

    // e.g. Calls(_, _)
    [[nodiscard]] auto eval_calls(const qps::WildCard&, const qps::WildCard&) const -> OutputTable;

  public:
    CallsEvaluator(DataSource data_source, std::shared_ptr<pkb::ReadFacade> read_facade, Calls calls, bool is_negated)
        : ClauseEvaluator(std::move(data_source), std::move(read_facade), is_negated), calls(std::move(calls)) {
    }

    [[nodiscard]] auto evaluate_positive() const -> OutputTable override;
};
} // namespace qps