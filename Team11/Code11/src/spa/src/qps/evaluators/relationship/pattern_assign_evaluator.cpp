#include "qps/evaluators/relationship/pattern_assign_evaluator.hpp"

namespace qps {
auto PatternAssignEvaluator::select_eval_method() const {
    return overloaded{[this](auto&& arg1, auto&& arg2) -> OutputTable {
        return eval_pattern_assign(std::forward<decltype(arg1)>(arg1), std::forward<decltype(arg2)>(arg2));
    }};
}

auto PatternAssignEvaluator::evaluate() const -> OutputTable {
    return std::visit(select_eval_method(), pattern.get_ent_ref(), pattern.get_expression_spec());
}

auto PatternAssignEvaluator::eval_pattern_assign(const std::shared_ptr<Synonym>& synonym,
                                                 const qps::PartialMatch& partial_match) const -> OutputTable {
    auto table = Table{{pattern.get_synonym(), synonym}};
    const auto all_partial_matches = read_facade->get_all_assignments_rhs_partial(partial_match.expr.value);
    for (const auto& assign_stmt : all_partial_matches) {
        for (const auto& var : read_facade->get_vars_modified_by_statement(assign_stmt)) {
            table.add_row({assign_stmt, var});
        }
    }

    return table;
}

auto PatternAssignEvaluator::eval_pattern_assign(const std::shared_ptr<Synonym>& synonym, const qps::WildCard&) const
    -> OutputTable {
    auto table = Table{{pattern.get_synonym(), synonym}};
    for (const auto& assign_stmt : read_facade->get_assign_statements()) {
        for (const auto& var : read_facade->get_vars_modified_by_statement(assign_stmt)) {
            table.add_row({assign_stmt, var});
        }
    }

    return table;
}

auto PatternAssignEvaluator::eval_pattern_assign(const qps::QuotedIdent& quoted_ident,
                                                 const qps::PartialMatch& partial_match) const -> OutputTable {
    auto table = Table{{pattern.get_synonym()}};
    const auto all_partial_matches =
        read_facade->get_all_assignments_lhs_rhs_partial(quoted_ident.get_value(), partial_match.expr.value);
    for (const auto& assign_stmt : all_partial_matches) {
        table.add_row({assign_stmt});
    }

    return table;
}

auto PatternAssignEvaluator::eval_pattern_assign(const qps::QuotedIdent& quoted_ident, const qps::WildCard&) const
    -> OutputTable {
    auto table = Table{{pattern.get_synonym()}};
    const auto stmts_that_modify =
        read_facade->get_statements_that_modify_var(quoted_ident.get_value(), StatementType::Assign);
    for (const auto& stmt : stmts_that_modify) {
        table.add_row({stmt});
    }

    return table;
}

auto PatternAssignEvaluator::eval_pattern_assign(const qps::WildCard&, const qps::PartialMatch& partial_match) const
    -> OutputTable {
    auto table = Table{{pattern.get_synonym()}};
    const auto all_partial_matches = read_facade->get_all_assignments_rhs_partial(partial_match.expr.value);
    for (const auto& stmt : all_partial_matches) {
        table.add_row({stmt});
    }

    return table;
}

auto PatternAssignEvaluator::eval_pattern_assign(const qps::WildCard&, const qps::WildCard&) const -> OutputTable {
    auto table = Table{{pattern.get_synonym()}};
    const auto all_assign_stmts = read_facade->get_assign_statements();
    for (const auto& stmt : all_assign_stmts) {
        table.add_row({stmt});
    }

    return table;
}

auto PatternAssignEvaluator::eval_pattern_assign(const WildCard&, const ExactMatch& exact) const -> OutputTable {
    const auto& statements = read_facade->get_all_assignments_rhs(exact.expr.value);
    if (statements.empty()) {
        return Table{};
    }

    auto table = Table{{pattern.get_synonym()}};
    for (const auto& statement : statements) {
        table.add_row({{statement}});
    }
    return table;
}

auto PatternAssignEvaluator::eval_pattern_assign(const std::shared_ptr<Synonym>& synonym, const ExactMatch& exact) const
    -> OutputTable {
    const auto& statements = read_facade->get_all_assignments_rhs(exact.expr.value);
    if (statements.empty()) {
        return Table{};
    }

    auto table = Table{{pattern.get_synonym(), synonym}};
    for (const auto& assign_stmt : statements) {
        const auto& variables = read_facade->get_vars_modified_by_statement(assign_stmt);
        for (const auto& variable : variables) {
            table.add_row({{assign_stmt, variable}});
        }
    }
    return table;
}

auto PatternAssignEvaluator::eval_pattern_assign(const QuotedIdent& quoted_ident, const ExactMatch& exact) const
    -> OutputTable {
    const auto& statements = read_facade->get_all_assignments_lhs_rhs(quoted_ident.get_value(), exact.expr.value);
    if (statements.empty()) {
        return Table{};
    }

    auto table = Table{{pattern.get_synonym()}};
    for (const auto& statement : statements) {
        table.add_row({{statement}});
    }
    return table;
}

} // namespace qps