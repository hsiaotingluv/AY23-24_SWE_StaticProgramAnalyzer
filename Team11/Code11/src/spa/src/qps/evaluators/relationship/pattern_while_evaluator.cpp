#include "qps/evaluators/relationship/pattern_while_evaluator.hpp"

namespace qps {
auto PatternWhileEvaluator::select_eval_method() const {
    return overloaded{[this](auto&& arg) -> OutputTable {
        return eval_pattern_while(std::forward<decltype(arg)>(arg));
    }};
}

auto PatternWhileEvaluator::evaluate() const -> OutputTable {
    return std::visit(select_eval_method(), pattern.get_ent_ref());
}

auto PatternWhileEvaluator::eval_pattern_while(const std::shared_ptr<Synonym>& syn) const -> OutputTable {
    auto table = Table{{pattern.get_synonym(), syn}};
    const auto while_var_pairs = read_facade->get_all_while_stmt_var_pairs();
    for (const auto& pair : while_var_pairs) {
        table.add_row({std::get<0>(pair), std::get<1>(pair)});
    }
    return table;
}

auto PatternWhileEvaluator::eval_pattern_while(const QuotedIdent& quoted_var) const -> OutputTable {
    auto table = Table{{pattern.get_synonym()}};
    const auto relevant_while_stmts = read_facade->get_while_stmts_with_var(quoted_var.get_value());
    for (const auto& stmt : relevant_while_stmts) {
        table.add_row({stmt});
    }
    return table;
}

auto PatternWhileEvaluator::eval_pattern_while(const WildCard&) const -> OutputTable {
    auto table = Table{{pattern.get_synonym()}};
    const auto relevant_while_stmts = read_facade->get_while_stmts_with_var();
    for (const auto& stmt : relevant_while_stmts) {
        table.add_row({stmt});
    }
    return table;
}
} // namespace qps