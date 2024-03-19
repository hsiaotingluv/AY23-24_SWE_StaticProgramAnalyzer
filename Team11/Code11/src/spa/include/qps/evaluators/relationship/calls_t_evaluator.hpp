#pragma once

#include "clause_evaluator.hpp"

namespace qps {
class CallsTEvaluator : public ClauseEvaluator {
    std::shared_ptr<pkb::ReadFacade> read_facade;
    CallsT calls_t;

    [[nodiscard]] auto select_eval_method() const;

    // e.g. Calls*(p, q)
    [[nodiscard]] auto eval_calls_t(const std::shared_ptr<ProcSynonym>& proc_1,
                                    const std::shared_ptr<ProcSynonym>& proc_2) const -> OutputTable;

    // e.g. Calls*(p, "q")
    [[nodiscard]] auto eval_calls_t(const std::shared_ptr<ProcSynonym>& proc_1,
                                    const qps::QuotedIdent& quoted_proc_2) const -> OutputTable;

    // e.g. Calls*(p, _)
    [[nodiscard]] auto eval_calls_t(const std::shared_ptr<ProcSynonym>& proc_1, const qps::WildCard&) const
        -> OutputTable;

    // e.g. Calls*("p", q)
    [[nodiscard]] auto eval_calls_t(const qps::QuotedIdent& quoted_proc_1,
                                    const std::shared_ptr<ProcSynonym>& proc_2) const -> OutputTable;

    // e.g. Calls*("p", "q")
    [[nodiscard]] auto eval_calls_t(const qps::QuotedIdent& quoted_proc_1, const qps::QuotedIdent& quoted_proc_2) const
        -> OutputTable;

    // e.g. Calls*("p", _)
    [[nodiscard]] auto eval_calls_t(const qps::QuotedIdent& quoted_proc_1, const qps::WildCard&) const -> OutputTable;

    // e.g. Calls*(_, q)
    [[nodiscard]] auto eval_calls_t(const qps::WildCard&, const std::shared_ptr<ProcSynonym>& proc_2) const
        -> OutputTable;

    // e.g. Calls*(_, "q")
    [[nodiscard]] auto eval_calls_t(const qps::WildCard&, const qps::QuotedIdent& quoted_proc_2) const -> OutputTable;

    // e.g. Calls*(_, _)
    [[nodiscard]] auto eval_calls_t(const qps::WildCard&, const qps::WildCard&) const -> OutputTable;

  public:
    CallsTEvaluator(std::shared_ptr<pkb::ReadFacade> read_facade, CallsT calls_t)
        : ClauseEvaluator(), read_facade(std::move(read_facade)), calls_t(std::move(calls_t)) {
    }

    [[nodiscard]] auto evaluate() const -> OutputTable override;
};
} // namespace qps