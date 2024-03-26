#include "qps/evaluators/clause_evaluators/relationship/calls_t_evaluator.hpp"

namespace qps {

auto CallsTEvaluator::select_eval_method() const {
    return overloaded{
        // e.g. Calls*(p, q)
        [this](const std::shared_ptr<ProcSynonym>& proc_syn_1,
               const std::shared_ptr<ProcSynonym>& proc_syn_2) -> OutputTable {
            return eval_calls_t(proc_syn_1, proc_syn_2);
        },
        // e.g. Calls*(p, "q")
        [this](const std::shared_ptr<ProcSynonym>& proc_syn_1, const QuotedIdent& quoted_proc_2) -> OutputTable {
            return eval_calls_t(proc_syn_1, quoted_proc_2);
        },
        // e.g. Calls*(p, _)
        [this](const std::shared_ptr<ProcSynonym>& proc_syn_1, const WildCard& wild_card_2) -> OutputTable {
            return eval_calls_t(proc_syn_1, wild_card_2);
        },
        // e.g. Calls*("p", q)
        [this](const QuotedIdent& quoted_proc_1, const std::shared_ptr<ProcSynonym>& proc_syn_2) -> OutputTable {
            return eval_calls_t(quoted_proc_1, proc_syn_2);
        },
        // e.g. Calls*("p", "q")
        [this](const QuotedIdent& quoted_proc_1, const QuotedIdent& quoted_proc_2) -> OutputTable {
            return eval_calls_t(quoted_proc_1, quoted_proc_2);
        },
        // e.g. Calls*("p", _)
        [this](const QuotedIdent& quoted_proc_1, const WildCard& wild_card_2) -> OutputTable {
            return eval_calls_t(quoted_proc_1, wild_card_2);
        },
        // e.g. Calls*(_, q)
        [this](const WildCard& wild_card_1, const std::shared_ptr<ProcSynonym>& proc_syn_2) -> OutputTable {
            return eval_calls_t(wild_card_1, proc_syn_2);
        },
        // e.g. Calls*(_, "q")
        [this](const WildCard& wild_card_1, const QuotedIdent& quoted_proc_2) -> OutputTable {
            return eval_calls_t(wild_card_1, quoted_proc_2);
        },
        // e.g. Calls*(_, _)
        [this](const WildCard& wild_card_1, const WildCard& wild_card_2) -> OutputTable {
            return eval_calls_t(wild_card_1, wild_card_2);
        },
    };
}

auto CallsTEvaluator::evaluate() const -> OutputTable {
    return std::visit(select_eval_method(), calls_t.procedure1, calls_t.procedure2);
}

auto CallsTEvaluator::eval_calls_t(const std::shared_ptr<ProcSynonym>& proc_syn_1,
                                   const std::shared_ptr<ProcSynonym>& proc_syn_2) const -> OutputTable {
    // No recursion in SIMPLE
    if (proc_syn_1 == proc_syn_2) {
        return Table{};
    }
    // TODO improve pkb api with get all caller-callee pairs
    auto table = Table{{proc_syn_1, proc_syn_2}};
    const auto all_callers = read_facade->get_all_calls_star_keys();
    for (const auto& caller : all_callers) {
        const auto callees = read_facade->get_star_callees(caller);
        for (const auto& callee : callees) {
            table.add_row({caller, callee});
        }
    }
    return table;
}

auto CallsTEvaluator::eval_calls_t(const std::shared_ptr<ProcSynonym>& proc_syn_1,
                                   const QuotedIdent& quoted_proc_2) const -> OutputTable {
    auto table = Table{{proc_syn_1}};
    const auto all_callers = read_facade->get_star_callers(quoted_proc_2.get_value());
    for (const auto& caller : all_callers) {
        table.add_row({caller});
    }
    return table;
}

auto CallsTEvaluator::eval_calls_t(const std::shared_ptr<ProcSynonym>& proc_syn_1, const WildCard&) const
    -> OutputTable {
    auto table = Table{{proc_syn_1}};
    const auto all_callers = read_facade->get_all_calls_star_keys();
    for (const auto& caller : all_callers) {
        table.add_row({caller});
    }
    return table;
}

auto CallsTEvaluator::eval_calls_t(const QuotedIdent& quoted_proc_1,
                                   const std::shared_ptr<ProcSynonym>& proc_syn_2) const -> OutputTable {
    auto table = Table{{proc_syn_2}};
    const auto all_callees = read_facade->get_star_callees(quoted_proc_1.get_value());
    for (const auto& callee : all_callees) {
        table.add_row({callee});
    }
    return table;
}

auto CallsTEvaluator::eval_calls_t(const QuotedIdent& quoted_proc_1, const QuotedIdent& quoted_proc_2) const
    -> OutputTable {
    const auto& caller_name_string = quoted_proc_1.get_value();
    const auto& callee_name_string = quoted_proc_2.get_value();
    const bool does_call = read_facade->has_calls_star_relation(caller_name_string, callee_name_string);

    if (!does_call) {
        return Table{};
    }

    return UnitTable{};
}

auto CallsTEvaluator::eval_calls_t(const QuotedIdent& quoted_proc_1, const WildCard&) const -> OutputTable {
    const bool is_caller_star = read_facade->contains_calls_star_key(quoted_proc_1.get_value());

    if (!is_caller_star) {
        return Table{};
    }

    return UnitTable{};
}

auto CallsTEvaluator::eval_calls_t(const WildCard&, const std::shared_ptr<ProcSynonym>& proc_syn_2) const
    -> OutputTable {
    auto table = Table{{proc_syn_2}};
    const auto all_callees = read_facade->get_all_calls_star_values();
    for (const auto& callee : all_callees) {
        table.add_row({callee});
    }
    return table;
}

auto CallsTEvaluator::eval_calls_t(const WildCard&, const QuotedIdent& quoted_proc_2) const -> OutputTable {
    const bool is_callee_star = read_facade->contains_calls_star_value(quoted_proc_2.get_value());

    if (!is_callee_star) {
        return Table{};
    }

    return UnitTable{};
}

auto CallsTEvaluator::eval_calls_t(const WildCard&, const WildCard&) const -> OutputTable {
    const bool calls_star_exists = read_facade->has_calls_star_relation();

    if (!calls_star_exists) {
        return Table{};
    }

    return UnitTable{};
}
} // namespace qps