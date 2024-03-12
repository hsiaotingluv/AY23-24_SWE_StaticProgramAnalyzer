#include "qps/evaluators/relationship/modifies_p_evaluator.hpp"
#include "qps/parser/entities/synonym.hpp"
#include <memory>

namespace qps {

auto ModifiesPEvaluator::select_eval_method() const {
    return overloaded{[this](auto&& arg1, auto&& arg2) -> OutputTable {
        return eval_modifies_p(std::forward<decltype(arg1)>(arg1), std::forward<decltype(arg2)>(arg2));
    }};
}

auto ModifiesPEvaluator::evaluate() const -> OutputTable {
    return std::visit(select_eval_method(), modifies_p.ent1, modifies_p.ent2);
}

auto ModifiesPEvaluator::eval_modifies_p(const std::shared_ptr<ProcSynonym>& proc_synonym,
                                         const std::shared_ptr<VarSynonym>& var_synonym) const -> OutputTable {
    auto table = Table{{proc_synonym, var_synonym}};
    const auto all_proc_var_modify_pairs = read_facade->get_all_procedures_and_var_modify_pairs();

    for (const auto& pair : all_proc_var_modify_pairs) {
        const auto proc = std::get<0>(pair);
        const auto var_modified = std::get<1>(pair);
        table.add_row({proc, var_modified});
    }
    return table;
}

auto ModifiesPEvaluator::eval_modifies_p(const std::shared_ptr<ProcSynonym>& proc_synonym,
                                         const QuotedIdent& quoted_var) const -> OutputTable {
    auto table = Table{{proc_synonym}};
    const auto procedures_that_modify_var = read_facade->get_procedures_that_modify_var(quoted_var.get_value());

    for (const auto& proc : procedures_that_modify_var) {
        table.add_row({proc});
    }

    return table;
}

auto ModifiesPEvaluator::eval_modifies_p(const std::shared_ptr<ProcSynonym>& proc_synonym, const WildCard&) const
    -> OutputTable {
    auto table = Table{{proc_synonym}};
    const auto all_procedures_that_modify = read_facade->get_all_procedures_that_modify();

    for (const auto& proc : all_procedures_that_modify) {
        table.add_row({proc});
    }

    return table;
}

auto ModifiesPEvaluator::eval_modifies_p(const QuotedIdent& quoted_proc,
                                         const std::shared_ptr<VarSynonym>& var_synonym) const -> OutputTable {
    auto table = Table{{var_synonym}};

    const auto all_modified_vars = read_facade->get_vars_modified_by_procedure(quoted_proc.get_value());

    for (const auto& modified_var : all_modified_vars) {
        table.add_row({modified_var});
    }

    return table;
}

auto ModifiesPEvaluator::eval_modifies_p(const QuotedIdent& quoted_proc, const QuotedIdent& quoted_var) const
    -> OutputTable {
    const auto proc_name_string = quoted_proc.get_value();
    const auto var_name_string = quoted_var.get_value();

    const bool proc_modifies_var = read_facade->does_procedure_modify_var(proc_name_string, var_name_string);

    if (!proc_modifies_var) {
        return Table{};
    }

    return UnitTable{};
}

auto ModifiesPEvaluator::eval_modifies_p(const QuotedIdent& quoted_proc, const WildCard&) const -> OutputTable {
    const auto proc_name_string = quoted_proc.get_value();
    bool proc_modifies_var = read_facade->does_procedure_modify_any_var(proc_name_string);

    if (!proc_modifies_var) {
        return Table{};
    }

    return UnitTable{};
}

} // namespace qps