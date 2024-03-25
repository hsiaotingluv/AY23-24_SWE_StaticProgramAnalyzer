#pragma once

#include "clause_evaluator.hpp"

namespace qps {
class CallsEvaluator : public ClauseEvaluator {
    std::shared_ptr<pkb::ReadFacade> read_facade;
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
    CallsEvaluator(std::shared_ptr<pkb::ReadFacade> read_facade, Calls calls)
        : ClauseEvaluator(), read_facade(std::move(read_facade)), calls(std::move(calls)) {
    }

    [[nodiscard]] auto evaluate() const -> OutputTable override;
};
} // namespace qps