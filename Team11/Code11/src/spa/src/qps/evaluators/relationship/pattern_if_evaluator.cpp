#include "qps/evaluators/relationship/pattern_if_evaluator.hpp"

namespace qps {
auto PatternIfEvaluator::select_eval_method() const {
    return overloaded{[this](auto&& arg) -> OutputTable {
        return eval_pattern_if(std::forward<decltype(arg)>(arg));
    }};
}

auto PatternIfEvaluator::evaluate() const -> OutputTable {
    return std::visit(select_eval_method(), pattern.get_ent_ref());
}

auto PatternIfEvaluator::eval_pattern_if(const std::shared_ptr<Synonym>& syn) const -> OutputTable {
    auto table = Table{{pattern.get_synonym(), syn}};
    const auto if_var_pairs = read_facade->get_all_if_stmt_var_pairs();
    for (const auto& pair : if_var_pairs) {
        table.add_row({std::get<0>(pair), std::get<1>(pair)});
    }
    return table;
}

auto PatternIfEvaluator::eval_pattern_if(const QuotedIdent& quoted_var) const -> OutputTable {
    auto table = Table{{pattern.get_synonym()}};
    const auto relevant_if_stmts = read_facade->get_if_stmts_with_var(quoted_var.get_value());
    for (const auto& stmt : relevant_if_stmts) {
        table.add_row({stmt});
    }
    return table;
}

auto PatternIfEvaluator::eval_pattern_if(const WildCard&) const -> OutputTable {
    auto table = Table{{pattern.get_synonym()}};
    const auto relevant_if_stmts = read_facade->get_if_stmts_with_var();
    for (const auto& stmt : relevant_if_stmts) {
        table.add_row({stmt});
    }
    return table;
}
} // namespace qps