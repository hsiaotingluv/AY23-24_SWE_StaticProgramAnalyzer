#include "qps/evaluators/relationship/pattern_evaluator.hpp"

namespace qps {
auto PatternEvaluator::select_eval_method() const {
    return overloaded{
        // e.g. pattern a(x, _"v"_)
        [this](const std::shared_ptr<Synonym>& synonym,
               const qps::PartialMatch& partial_match) -> std::optional<Table> {
            return eval_pattern(synonym, partial_match);
        },
        // e.g. pattern a(x, _)
        [this](const std::shared_ptr<Synonym>& synonym, const qps::WildCard& wild_card) -> std::optional<Table> {
            return eval_pattern(synonym, wild_card);
        },
        // e.g. pattern a("x", _"v"_)
        [this](const qps::QuotedIdent& quoted_ident, const qps::PartialMatch& partial_match) -> std::optional<Table> {
            return eval_pattern(quoted_ident, partial_match);
        },
        // e.g. pattern a("x", _)
        [this](const qps::QuotedIdent& quoted_ident, const qps::WildCard& wild_card) -> std::optional<Table> {
            return eval_pattern(quoted_ident, wild_card);
        },
        // e.g. pattern a(_, _"v"_)
        [this](const qps::WildCard& wild_card, const qps::PartialMatch& partial_match) -> std::optional<Table> {
            return eval_pattern(wild_card, partial_match);
        },
        // e.g. pattern a(_, _)
        [this](const qps::WildCard& wild_card_1, const qps::WildCard& wild_card_2) -> std::optional<Table> {
            return eval_pattern(wild_card_1, wild_card_2);
        },
        // e.g. pattern a(_, "v")
        [this](const WildCard& wild_card, const ExactMatch& exact) -> std::optional<Table> {
            return eval_pattern(wild_card, exact);
        },
        // e.g. pattern a(x, "v")
        [this](const std::shared_ptr<Synonym>& synonym, const ExactMatch& exact) -> std::optional<Table> {
            return eval_pattern(synonym, exact);
        },
        // e.g. pattern a("x", "v")
        [this](const QuotedIdent& quoted_ident, const ExactMatch& exact) -> std::optional<Table> {
            return eval_pattern(quoted_ident, exact);
        }};
}

auto PatternEvaluator::evaluate() const -> std::optional<Table> {
    return std::visit(select_eval_method(), pattern.ent_ref, pattern.expression_spec);
}

auto PatternEvaluator::eval_pattern(const std::shared_ptr<Synonym>& synonym, const PartialMatch& partial_match) const
    -> std::optional<Table> {
    auto table = Table{{pattern.assign_synonym, synonym}};
    const auto all_partial_matches = read_facade->get_all_assignments_rhs_partial(partial_match.expr.value);
    for (const auto& assign_stmt : all_partial_matches) {
        for (const auto& var : read_facade->get_vars_modified_by_statement(assign_stmt)) {
            table.add_row({assign_stmt, var});
        }
    }

    if (table.empty()) {
        return std::nullopt;
    }
    return table;
}

auto PatternEvaluator::eval_pattern(const std::shared_ptr<Synonym>& synonym, const WildCard& wild_card) const
    -> std::optional<Table> {
    auto table = Table{{pattern.assign_synonym, synonym}};
    for (const auto& assign_stmt : read_facade->get_assign_statements()) {
        for (const auto& var : read_facade->get_vars_modified_by_statement(assign_stmt)) {
            table.add_row({assign_stmt, var});
        }
    }

    if (table.empty()) {
        return std::nullopt;
    }
    return table;
}

auto PatternEvaluator::eval_pattern(const QuotedIdent& quoted_ident, const PartialMatch& partial_match) const
    -> std::optional<Table> {
    auto table = Table{{pattern.assign_synonym}};
    const auto all_partial_matches =
        read_facade->get_all_assignments_lhs_rhs_partial(quoted_ident.get_value(), partial_match.expr.value);
    for (const auto& assign_stmt : all_partial_matches) {
        table.add_row({assign_stmt});
    }

    if (table.empty()) {
        return std::nullopt;
    }
    return table;
}

auto PatternEvaluator::eval_pattern(const QuotedIdent& quoted_ident, const WildCard& wild_card) const
    -> std::optional<Table> {
    auto table = Table{{pattern.assign_synonym}};
    const auto stmts_that_modify =
        read_facade->get_statements_that_modify_var(quoted_ident.get_value(), StatementType::Assign);
    for (const auto& stmt : stmts_that_modify) {
        table.add_row({stmt});
    }

    if (table.empty()) {
        return std::nullopt;
    }
    return table;
}

auto PatternEvaluator::eval_pattern(const WildCard& wild_card, const PartialMatch& partial_match) const
    -> std::optional<Table> {
    auto table = Table{{pattern.assign_synonym}};
    const auto all_partial_matches = read_facade->get_all_assignments_rhs_partial(partial_match.expr.value);
    for (const auto& stmt : all_partial_matches) {
        table.add_row({stmt});
    }

    if (table.empty()) {
        return std::nullopt;
    }
    return table;
}

auto PatternEvaluator::eval_pattern(const WildCard& wild_card_1, const WildCard& wild_card_2) const
    -> std::optional<Table> {
    auto table = Table{{pattern.assign_synonym}};
    const auto all_assign_stmts = read_facade->get_assign_statements();
    for (const auto& stmt : all_assign_stmts) {
        table.add_row({stmt});
    }

    if (table.empty()) {
        return std::nullopt;
    }
    return table;
}

auto PatternEvaluator::eval_pattern(const WildCard&, const ExactMatch& exact) const -> std::optional<Table> {
    const auto& statements = read_facade->get_all_assignments_rhs(exact.expr.value);
    if (statements.empty()) {
        return std::nullopt;
    }

    auto table = Table{{pattern.assign_synonym}};
    for (const auto& statement : statements) {
        table.add_row({{statement}});
    }
    return table;
}

auto PatternEvaluator::eval_pattern(const std::shared_ptr<Synonym>& synonym, const ExactMatch& exact) const
    -> std::optional<Table> {
    const auto& statements = read_facade->get_all_assignments_rhs(exact.expr.value);
    if (statements.empty()) {
        return std::nullopt;
    }

    auto table = Table{{pattern.assign_synonym, synonym}};
    for (const auto& assign_stmt : statements) {
        const auto& variables = read_facade->get_vars_modified_by_statement(assign_stmt);
        for (const auto& variable : variables) {
            table.add_row({{assign_stmt, variable}});
        }
    }
    return table;
}

auto PatternEvaluator::eval_pattern(const QuotedIdent& quoted_ident, const ExactMatch& exact) const
    -> std::optional<Table> {
    const auto& statements = read_facade->get_all_assignments_lhs_rhs(quoted_ident.get_value(), exact.expr.value);
    if (statements.empty()) {
        return std::nullopt;
    }

    auto table = Table{{pattern.assign_synonym}};
    for (const auto& statement : statements) {
        table.add_row({{statement}});
    }
    return table;
}

} // namespace qps