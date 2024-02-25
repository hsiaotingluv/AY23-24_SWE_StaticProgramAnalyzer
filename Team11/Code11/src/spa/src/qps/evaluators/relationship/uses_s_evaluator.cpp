#include "qps/evaluators/relationship/uses_s_evaluator.hpp"
#include "qps/parser/entities/synonym.hpp"
#include <memory>
#include <optional>

namespace qps {

auto UsesSEvaluator::select_eval_method() const {
    return overloaded{
        // e.g. Uses(a/pn/s, v)
        [this](const std::shared_ptr<StmtSynonym>& stmt_synonym,
               const std::shared_ptr<VarSynonym>& var_synonym) -> std::optional<Table> {
            return eval_uses_s(stmt_synonym, var_synonym);
        },
        // e.g. Uses(a/pn/s, "v")
        [this](const std::shared_ptr<StmtSynonym>& stmt_synonym,
               const qps::QuotedIdent& quoted_ident) -> std::optional<Table> {
            return eval_uses_s(stmt_synonym, quoted_ident);
        },
        // e.g. Uses(a/pn/s, _)
        [this](const std::shared_ptr<StmtSynonym>& stmt_synonym,
               const qps::WildCard& wild_card) -> std::optional<Table> {
            return eval_uses_s(stmt_synonym, wild_card);
        },
        // e.g. Uses(3, v)
        [this](const qps::Integer& stmt_num, const std::shared_ptr<VarSynonym>& var_synonym) -> std::optional<Table> {
            return eval_uses_s(stmt_num, var_synonym);
        },
        // e.g. Uses(3, "v")
        [this](const qps::Integer& stmt_num, const qps::QuotedIdent& quoted_ident) -> std::optional<Table> {
            return eval_uses_s(stmt_num, quoted_ident);
        },
        // e.g. Uses(3, _)
        [this](const qps::Integer& stmt_num, const qps::WildCard& wild_card) -> std::optional<Table> {
            return eval_uses_s(stmt_num, wild_card);
        }};
}

auto UsesSEvaluator::evaluate() const -> std::optional<Table> {
    return std::visit(select_eval_method(), uses_s.stmt, uses_s.ent);
}

auto UsesSEvaluator::eval_uses_s(const std::shared_ptr<StmtSynonym>& stmt_synonym,
                                 const std::shared_ptr<VarSynonym>& var_synonym) const -> std::optional<Table> {
    auto table = Table{{stmt_synonym, var_synonym}};
    const auto relevant_stmts = stmt_synonym->scan(read_facade);

    const auto all_stmt_var_use_pairs = read_facade->get_all_statements_and_var_use_pairs();
    for (const auto& pair : all_stmt_var_use_pairs) {
        const auto stmt_candidate = std::get<0>(pair);
        const auto var_candidate = std::get<1>(pair);
        if (relevant_stmts.find(stmt_candidate) != relevant_stmts.end()) {
            table.add_row({stmt_candidate, var_candidate});
        }
    }
    if (table.empty()) {
        return std::nullopt;
    }
    return table;
}

auto UsesSEvaluator::eval_uses_s(const std::shared_ptr<StmtSynonym>& stmt_synonym,
                                 const QuotedIdent& quoted_ident) const -> std::optional<Table> {
    auto table = Table{{stmt_synonym}};
    const auto relevant_stmts = stmt_synonym->scan(read_facade);

    const auto statements = read_facade->get_statements_that_use_var(quoted_ident.get_value());

    for (const auto& stmt : statements) {
        if (relevant_stmts.find(stmt) != relevant_stmts.end()) {
            table.add_row({stmt});
        }
    }

    if (table.empty()) {
        return std::nullopt;
    }

    return table;
}

auto UsesSEvaluator::eval_uses_s(const std::shared_ptr<StmtSynonym>& stmt_synonym, const WildCard&) const
    -> std::optional<Table> {
    auto table = Table{{stmt_synonym}};
    const auto relevant_stmts = stmt_synonym->scan(read_facade);

    const auto all_using_stmts = read_facade->get_all_statements_that_use();
    for (const auto& stmt_candidate : all_using_stmts) {
        if (relevant_stmts.find(stmt_candidate) != relevant_stmts.end()) {
            table.add_row({stmt_candidate});
        }
    }
    if (table.empty()) {
        return std::nullopt;
    }
    return table;
}

auto UsesSEvaluator::eval_uses_s(const Integer& stmt_num, const std::shared_ptr<VarSynonym>& var_synonym) const
    -> std::optional<Table> {
    auto table = Table{{var_synonym}};
    const auto used_vars = read_facade->get_vars_used_by_statement(std::to_string(stmt_num.value));
    for (const auto& var : used_vars) {
        table.add_row({var});
    }
    if (table.empty()) {
        return std::nullopt;
    }
    return table;
}

auto UsesSEvaluator::eval_uses_s(const Integer& stmt_num, const QuotedIdent& quoted_ident) const
    -> std::optional<Table> {
    const auto stmt_num_string = std::to_string(stmt_num.value);
    const auto var_string = quoted_ident.get_value();
    bool stmt_uses_var = read_facade->does_statement_use_var(stmt_num_string, var_string);

    if (!stmt_uses_var) {
        return std::nullopt;
    }
    return Table{};
}

auto UsesSEvaluator::eval_uses_s(const Integer& stmt_num, const WildCard&) const -> std::optional<Table> {
    const auto stmt_num_string = std::to_string(stmt_num.value);
    bool stmt_uses_var = read_facade->does_statement_use_any_var(stmt_num_string);

    if (!stmt_uses_var) {
        return std::nullopt;
    }
    return Table{};
}

} // namespace qps