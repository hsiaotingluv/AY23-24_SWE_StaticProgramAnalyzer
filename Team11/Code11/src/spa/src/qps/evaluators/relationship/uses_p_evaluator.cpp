#include "qps/evaluators/relationship/uses_p_evaluator.hpp"
#include "qps/parser/entities/synonym.hpp"
#include <memory>

namespace qps {

auto UsesPEvaluator::select_eval_method() const {
    return overloaded{[this](auto&& arg1, auto&& arg2) -> OutputTable {
        return eval_uses_p(std::forward<decltype(arg1)>(arg1), std::forward<decltype(arg2)>(arg2));
    }};
}

auto UsesPEvaluator::evaluate() const -> OutputTable {
    return std::visit(select_eval_method(), uses_p.ent1, uses_p.ent2);
}

auto UsesPEvaluator::eval_uses_p(const std::shared_ptr<ProcSynonym>& proc_synonym,
                                 const std::shared_ptr<VarSynonym>& var_synonym) const -> OutputTable {
    auto table = Table{{proc_synonym, var_synonym}};
    const auto all_proc_var_use_pairs = read_facade->get_all_procedures_and_var_use_pairs();

    for (const auto& pair : all_proc_var_use_pairs) {
        const auto proc = std::get<0>(pair);
        const auto var_used = std::get<1>(pair);
        table.add_row({proc, var_used});
    }
    return table;
}

auto UsesPEvaluator::eval_uses_p(const std::shared_ptr<ProcSynonym>& proc_synonym, const QuotedIdent& quoted_var) const
    -> OutputTable {
    auto table = Table{{proc_synonym}};
    const auto procedures_that_use_var = read_facade->get_procedures_that_use_var(quoted_var.get_value());

    for (const auto& proc : procedures_that_use_var) {
        table.add_row({proc});
    }

    return table;
}

auto UsesPEvaluator::eval_uses_p(const std::shared_ptr<ProcSynonym>& proc_synonym, const WildCard&) const
    -> OutputTable {
    auto table = Table{{proc_synonym}};
    const auto all_procedures_that_use = read_facade->get_all_procedures_that_use();

    for (const auto& proc : all_procedures_that_use) {
        table.add_row({proc});
    }

    return table;
}

auto UsesPEvaluator::eval_uses_p(const QuotedIdent& quoted_proc, const std::shared_ptr<VarSynonym>& var_synonym) const
    -> OutputTable {
    auto table = Table{{var_synonym}};

    const auto all_used_vars = read_facade->get_vars_used_by_procedure(quoted_proc.get_value());

    for (const auto& used_var : all_used_vars) {
        table.add_row({used_var});
    }

    return table;
}

auto UsesPEvaluator::eval_uses_p(const QuotedIdent& quoted_proc, const QuotedIdent& quoted_var) const -> OutputTable {
    const auto proc_name_string = quoted_proc.get_value();
    const auto var_name_string = quoted_var.get_value();

    const bool proc_uses_var = read_facade->contains_procedure_use_var(proc_name_string, var_name_string);

    if (!proc_uses_var) {
        return Table{};
    }

    return UnitTable{};
}

auto UsesPEvaluator::eval_uses_p(const QuotedIdent& quoted_proc, const WildCard&) const -> OutputTable {
    const auto proc_name_string = quoted_proc.get_value();
    bool proc_uses_var = read_facade->contains_procedure_use_var_key(proc_name_string);

    if (!proc_uses_var) {
        return Table{};
    }

    return UnitTable{};
}

} // namespace qps