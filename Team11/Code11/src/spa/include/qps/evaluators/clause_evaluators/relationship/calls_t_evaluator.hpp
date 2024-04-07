#pragma once

#include "qps/evaluators/clause_evaluators/clause_evaluator.hpp"

namespace qps {
class CallsTEvaluator : public ClauseEvaluator {
    CallsT calls_t;

    [[nodiscard]] auto select_eval_method() const;

    // e.g. Calls*(p, q)
    [[nodiscard]] auto eval_calls_t(const std::shared_ptr<ProcSynonym>& proc_syn_1,
                                    const std::shared_ptr<ProcSynonym>& proc_syn_2) const -> OutputTable;

    // e.g. Calls*(p, "q")
    [[nodiscard]] auto eval_calls_t(const std::shared_ptr<ProcSynonym>& proc_syn_1,
                                    const qps::QuotedIdent& quoted_proc_2) const -> OutputTable;

    // e.g. Calls*(p, _)
    [[nodiscard]] auto eval_calls_t(const std::shared_ptr<ProcSynonym>& proc_syn_1, const qps::WildCard&) const
        -> OutputTable;

    // e.g. Calls*("p", q)
    [[nodiscard]] auto eval_calls_t(const qps::QuotedIdent& quoted_proc_1,
                                    const std::shared_ptr<ProcSynonym>& proc_syn_2) const -> OutputTable;

    // e.g. Calls*("p", "q")
    [[nodiscard]] auto eval_calls_t(const qps::QuotedIdent& quoted_proc_1, const qps::QuotedIdent& quoted_proc_2) const
        -> OutputTable;

    // e.g. Calls*("p", _)
    [[nodiscard]] auto eval_calls_t(const qps::QuotedIdent& quoted_proc_1, const qps::WildCard&) const -> OutputTable;

    // e.g. Calls*(_, q)
    [[nodiscard]] auto eval_calls_t(const qps::WildCard&, const std::shared_ptr<ProcSynonym>& proc_syn_2) const
        -> OutputTable;

    // e.g. Calls*(_, "q")
    [[nodiscard]] auto eval_calls_t(const qps::WildCard&, const qps::QuotedIdent& quoted_proc_2) const -> OutputTable;

    // e.g. Calls*(_, _)
    [[nodiscard]] auto eval_calls_t(const qps::WildCard&, const qps::WildCard&) const -> OutputTable;

  public:
    CallsTEvaluator(std::shared_ptr<pkb::ReadFacade> read_facade, CallsT calls_t, bool is_negated)
        : ClauseEvaluator(std::move(read_facade), is_negated), calls_t(std::move(calls_t)) {
    }

    [[nodiscard]] auto evaluate_positive() const -> OutputTable override;
};
} // namespace qps