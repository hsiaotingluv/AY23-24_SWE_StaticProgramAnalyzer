#include "qps/evaluators/relationship/calls_evaluator.hpp"

namespace qps {

auto CallsEvaluator::select_eval_method() const {
    return overloaded{
      // e.g. Calls(p, q)
      [this](const std::shared_ptr<ProcSynonym> &proc_syn_1,
             const std::shared_ptr<ProcSynonym> &proc_syn_2) -> OutputTable {
          return eval_calls(proc_syn_1, proc_syn_2);
      },
      // e.g. Calls(p, "q")
      [this](const std::shared_ptr<ProcSynonym> &proc_syn_1,
             const QuotedIdent &quoted_proc_2) -> OutputTable {
          return eval_calls(proc_syn_1, quoted_proc_2);
      },
      // e.g. Calls(p, _)
      [this](const std::shared_ptr<ProcSynonym> &proc_syn_1,
             const WildCard& wild_card_2) -> OutputTable {
          return eval_calls(proc_syn_1, wild_card_2);
      },
      // e.g. Calls("p", q)
      [this](const QuotedIdent &quoted_proc_1,
             const std::shared_ptr<ProcSynonym> &proc_syn_2) -> OutputTable {
          return eval_calls(quoted_proc_1, proc_syn_2);
      },
      // e.g. Calls("p", "q")
      [this](const QuotedIdent &quoted_proc_1,
             const QuotedIdent &quoted_proc_2) -> OutputTable {
          return eval_calls(quoted_proc_1, quoted_proc_2);
      },
      // e.g. Calls("p", _)
      [this](const QuotedIdent &quoted_proc_1,
             const WildCard &wild_card_2) -> OutputTable {
          return eval_calls(quoted_proc_1, wild_card_2);
      },
      // e.g. Calls(_, q)
      [this](const WildCard& wild_card_1,
             const std::shared_ptr<ProcSynonym> &proc_syn_2) -> OutputTable {
          return eval_calls(wild_card_1, proc_syn_2);
      },
      // e.g. Calls(_, "q")
      [this](const WildCard &wild_card_1,
             const QuotedIdent &quoted_proc_2) -> OutputTable {
          return eval_calls(wild_card_1, quoted_proc_2);
      },
      // e.g. Calls(_, _)
      [this](const WildCard &wild_card_1,
             const WildCard &wild_card_2) -> OutputTable {
          return eval_calls(wild_card_1, wild_card_2);
      },
    };
}

auto CallsEvaluator::evaluate() const -> OutputTable {
    return std::visit(select_eval_method(), calls.procedure1, calls.procedure2);
}

auto CallsEvaluator::eval_calls(const std::shared_ptr<ProcSynonym> &proc_syn_1,
                                const std::shared_ptr<ProcSynonym> &proc_syn_2) const -> OutputTable {
    // TODO improve pkb api with get all caller-callee pairs
    auto table = Table{{proc_syn_1, proc_syn_2}};
    const auto all_callers = read_facade->get_all_calls_callers();
    for (const auto& caller : all_callers) {
        const auto callees = read_facade->get_callees(caller);
        for (const auto& callee : callees) {
            table.add_row({caller, callee});
        }
    }
    return table;
}

auto CallsEvaluator::eval_calls(const std::shared_ptr<ProcSynonym> &proc_syn_1,
                                const QuotedIdent &quoted_proc_2) const -> OutputTable {
    auto table = Table{{proc_syn_1}};
    const auto all_callers = read_facade->get_callers(quoted_proc_2.get_value());
    for (const auto& caller : all_callers) {
        table.add_row({caller});
    }
    return table;
}

auto CallsEvaluator::eval_calls(const std::shared_ptr<ProcSynonym> &proc_syn_1,
                                const WildCard &) const -> OutputTable {
    auto table = Table{{proc_syn_1}};
    const auto all_callers = read_facade->get_all_calls_callers();
    for (const auto& caller : all_callers) {
        table.add_row({caller});
    }
    return table;
}

auto CallsEvaluator::eval_calls(const QuotedIdent &quoted_proc_1,
                                const std::shared_ptr<ProcSynonym> &proc_syn_2) const -> OutputTable {
    auto table = Table{{proc_syn_2}};
    const auto all_callees = read_facade->get_callees(quoted_proc_1.get_value());
    for (const auto& callee : all_callees) {
        table.add_row({callee});
    }
    return table;
}

auto CallsEvaluator::eval_calls(const QuotedIdent &quoted_proc_1,
                                const QuotedIdent &quoted_proc_2) const -> OutputTable {
    const auto& caller_name_string = quoted_proc_1.get_value();
    const auto& callee_name_string = quoted_proc_2.get_value();
    const bool does_call = read_facade->has_calls_relation(caller_name_string, callee_name_string);

    if (!does_call) {
        return Table{};
    }

    return UnitTable{};
}

auto CallsEvaluator::eval_calls(const QuotedIdent &quoted_proc_1,
                                const WildCard &) const -> OutputTable {
    const auto all_callees = read_facade->get_callees(quoted_proc_1.get_value());

    if (all_callees.empty()) {
        return Table{};
    }

    return UnitTable{};
}

auto CallsEvaluator::eval_calls(const WildCard &,
                                const std::shared_ptr<ProcSynonym> &proc_syn_2) const -> OutputTable {
    auto table = Table{{proc_syn_2}};
    const auto all_callees = read_facade->get_all_calls_callees();
    for (const auto& callee : all_callees) {
        table.add_row({callee});
    }
    return table;
}

auto CallsEvaluator::eval_calls(const WildCard &,
                                const QuotedIdent &quoted_proc_2) const -> OutputTable {
    const auto all_callers = read_facade->get_callers(quoted_proc_2.get_value());

    if (all_callers.empty()) {
        return Table{};
    }

    return UnitTable{};
}

    const auto all_callers = read_facade->get_all_calls_callees();

    if (all_callers.empty()) {
        return Table{};
    }

    return UnitTable{};
}
}