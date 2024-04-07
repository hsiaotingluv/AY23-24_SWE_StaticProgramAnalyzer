#include "qps/evaluators/clause_evaluators/relationship/uses_s_evaluator.hpp"
#include "qps/parser/entities/synonym.hpp"
#include <memory>

namespace qps {

auto UsesSEvaluator::select_eval_method() const {
    return overloaded{[this](auto&& arg1, auto&& arg2) -> OutputTable {
        return eval_uses_s(std::forward<decltype(arg1)>(arg1), std::forward<decltype(arg2)>(arg2));
    }};
}

auto UsesSEvaluator::evaluate_positive() const -> OutputTable {
    return std::visit(select_eval_method(), uses_s.stmt, uses_s.ent);
}

auto UsesSEvaluator::eval_uses_s(const std::shared_ptr<StmtSynonym>& stmt_synonym,
                                 const std::shared_ptr<VarSynonym>& var_synonym) const -> OutputTable {
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
    return table;
}

auto UsesSEvaluator::eval_uses_s(const std::shared_ptr<StmtSynonym>& stmt_synonym,
                                 const QuotedIdent& quoted_ident) const -> OutputTable {
    auto table = Table{{stmt_synonym}};
    const auto relevant_stmts = stmt_synonym->scan(read_facade);

    const auto statements = read_facade->get_statements_that_use_var(quoted_ident.get_value());

    for (const auto& stmt : statements) {
        if (relevant_stmts.find(stmt) != relevant_stmts.end()) {
            table.add_row({stmt});
        }
    }

    return table;
}

auto UsesSEvaluator::eval_uses_s(const std::shared_ptr<StmtSynonym>& stmt_synonym, const WildCard&) const
    -> OutputTable {
    auto table = Table{{stmt_synonym}};
    const auto relevant_stmts = stmt_synonym->scan(read_facade);

    const auto all_using_stmts = read_facade->get_all_statements_that_use();
    for (const auto& stmt_candidate : all_using_stmts) {
        if (relevant_stmts.find(stmt_candidate) != relevant_stmts.end()) {
            table.add_row({stmt_candidate});
        }
    }

    return table;
}

auto UsesSEvaluator::eval_uses_s(const Integer& stmt_num, const std::shared_ptr<VarSynonym>& var_synonym) const
    -> OutputTable {
    auto table = Table{{var_synonym}};
    const auto used_vars = read_facade->get_vars_used_by_statement(stmt_num.value);
    for (const auto& var : used_vars) {
        table.add_row({var});
    }

    return table;
}

auto UsesSEvaluator::eval_uses_s(const Integer& stmt_num, const QuotedIdent& quoted_ident) const -> OutputTable {
    const auto stmt_num_string = stmt_num.value;
    const auto var_string = quoted_ident.get_value();
    bool stmt_uses_var = read_facade->contains_statement_use_var(stmt_num_string, var_string);

    if (!stmt_uses_var) {
        return Table{};
    }
    return UnitTable{};
}

auto UsesSEvaluator::eval_uses_s(const Integer& stmt_num, const WildCard&) const -> OutputTable {
    const auto stmt_num_string = stmt_num.value;
    bool stmt_uses_var = read_facade->contains_statement_use_var_key(stmt_num_string);

    if (!stmt_uses_var) {
        return Table{};
    }
    return UnitTable{};
}

} // namespace qps