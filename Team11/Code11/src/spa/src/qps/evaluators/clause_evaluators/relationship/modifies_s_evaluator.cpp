#include "qps/evaluators/clause_evaluators/relationship/modifies_s_evaluator.hpp"
#include "qps/evaluators/results_table.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/template_utils.hpp"

#include <memory>

namespace qps {

auto ModifiesSEvaluator::select_eval_method() const {
    return overloaded{[this](auto&& arg1, auto&& arg2) -> OutputTable {
        return eval_modifies_s(std::forward<decltype(arg1)>(arg1), std::forward<decltype(arg2)>(arg2));
    }};
}

auto ModifiesSEvaluator::evaluate_positive() const -> OutputTable {
    return std::visit(select_eval_method(), modifies_s.stmt, modifies_s.ent);
}

auto ModifiesSEvaluator::eval_modifies_s(const std::shared_ptr<qps::StmtSynonym>& stmt_synonym,
                                         const std::shared_ptr<qps::VarSynonym>& var_syn) const -> OutputTable {
    const auto relevant_stmts = get_data(stmt_synonym);
    const auto relevant_variables = get_data(var_syn);

    const auto stmt_vec = std::vector<std::string>{relevant_stmts.begin(), relevant_stmts.end()};
    const auto var_vec = std::vector<std::string>{relevant_variables.begin(), relevant_variables.end()};

    auto table = Table{{stmt_synonym, var_syn}};

    const auto all_pairs = read_facade->get_all_statements_and_var_modify_pairs();
    for (const auto& stmt : stmt_vec) {
        for (const auto& v : var_vec) {
            if (all_pairs.find({stmt, v}) != all_pairs.end()) {
                table.add_row({stmt, v});
            }
        }
    }

    return table;
}

auto ModifiesSEvaluator::eval_modifies_s(const std::shared_ptr<qps::StmtSynonym>& stmt_synonym,
                                         const QuotedIdent& identifier) const -> OutputTable {
    const auto relevant_stmts = get_data(stmt_synonym);
    const auto relevant_stmt_vec = std::vector<std::string>{relevant_stmts.begin(), relevant_stmts.end()};

    const auto stmts = read_facade->get_statements_that_modify_var(identifier.get_value());
    auto table = Table{{stmt_synonym}};

    for (const auto& stmt : relevant_stmt_vec) {
        if (stmts.find(stmt) != stmts.end()) {
            table.add_row({stmt});
        }
    }

    return table;
}

auto ModifiesSEvaluator::eval_modifies_s(const std::shared_ptr<qps::StmtSynonym>& stmt_synonym, const WildCard&) const
    -> OutputTable {
    const auto relevant_stmts = get_data(stmt_synonym);
    const auto stmt_vec = std::vector<std::string>{relevant_stmts.begin(), relevant_stmts.end()};

    const auto statements = read_facade->get_all_statements_that_modify();

    auto table = Table{{stmt_synonym}};
    for (const auto& stmt : stmt_vec) {
        if (statements.find(stmt) != statements.end()) {
            table.add_row({stmt});
        }
    }

    return table;
}

auto ModifiesSEvaluator::eval_modifies_s(const Integer& stmt_num, const std::shared_ptr<qps::VarSynonym>& var_syn) const
    -> OutputTable {
    const auto variables = read_facade->get_vars_modified_by_statement(stmt_num.value);
    auto table = Table{{var_syn}};
    for (const auto& v : variables) {
        table.add_row({v});
    }

    return table;
}

auto ModifiesSEvaluator::eval_modifies_s(const Integer& stmt_num, const QuotedIdent& identifier) const -> OutputTable {
    const auto does_modify = read_facade->contains_statement_modify_var(stmt_num.value, identifier.get_value());

    if (!does_modify) {
        return Table{};
    }
    return UnitTable{};
}

auto ModifiesSEvaluator::eval_modifies_s(const Integer& stmt_num, const WildCard&) const -> OutputTable {
    auto contains_statement_modify_var = false;
    const auto variables = read_facade->get_variables();
    const auto var_vec = std::vector<std::string>{variables.begin(), variables.end()};

    for (const auto& var : var_vec) {
        if (read_facade->contains_statement_modify_var(stmt_num.value, var)) {
            contains_statement_modify_var = true;
            break;
        }
    }

    if (!contains_statement_modify_var) {
        return Table{};
    }
    return UnitTable{};
}

} // namespace qps