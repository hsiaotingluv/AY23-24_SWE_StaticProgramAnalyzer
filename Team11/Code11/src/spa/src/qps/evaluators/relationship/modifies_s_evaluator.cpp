#include "qps/evaluators/relationship/modifies_s_evaluator.hpp"
#include "qps/evaluators/results_table.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/template_utils.hpp"

#include <memory>
#include <optional>

namespace qps {

auto ModifiesSEvaluator::select_eval_method() const {
    return overloaded{
        // e.g. Modifies(s1, v)
        [this](const std::shared_ptr<qps::StmtSynonym>& stmt_synonym,
               const std::shared_ptr<qps::VarSynonym>& var_syn) -> std::optional<Table> {
            return eval_modifies_s(stmt_synonym, var_syn);
        },
        // e.g. Modifies(s1, "v")
        [this](const std::shared_ptr<qps::StmtSynonym>& stmt_synonym,
               const qps::QuotedIdent& identifier) -> std::optional<Table> {
            return eval_modifies_s(stmt_synonym, identifier);
        },
        // e.g. Modifies(s1, _)
        [this](const std::shared_ptr<qps::StmtSynonym>& stmt_synonym,
               const qps::WildCard& wild_card) -> std::optional<Table> {
            return eval_modifies_s(stmt_synonym, wild_card);
        },
        // e.g. Modifies(3, v)
        [this](const qps::Integer& stmt_num, const std::shared_ptr<qps::VarSynonym>& var_syn) -> std::optional<Table> {
            return eval_modifies_s(stmt_num, var_syn);
        },
        // e.g. Modifies(3, "v")
        [this](const qps::Integer& stmt_num, const qps::QuotedIdent& identifier) -> std::optional<Table> {
            return eval_modifies_s(stmt_num, identifier);
        },
        // e.g. Modifies(3, _)
        [this](const qps::Integer& stmt_num, const qps::WildCard& wild_card) -> std::optional<Table> {
            return eval_modifies_s(stmt_num, wild_card);
        }};
}

auto ModifiesSEvaluator::evaluate() const -> std::optional<Table> {
    return std::visit(select_eval_method(), modifies_s.stmt, modifies_s.ent);
}

auto ModifiesSEvaluator::eval_modifies_s(const std::shared_ptr<qps::StmtSynonym>& stmt_synonym,
                                         const std::shared_ptr<qps::VarSynonym>& var_syn) const
    -> std::optional<Table> {
    // TODO: Improve pkb API: Get all statement that modifies and all variables that are modified
    const auto relevant_stmts = stmt_synonym->scan(read_facade);
    const auto relevant_variables = var_syn->scan(read_facade);

    const auto stmt_vec = std::vector<std::string>{relevant_stmts.begin(), relevant_stmts.end()};
    const auto var_vec = std::vector<std::string>{relevant_variables.begin(), relevant_variables.end()};

    auto table = Table{{stmt_synonym, var_syn}};

    for (const auto& stmt : stmt_vec) {
        for (const auto& v : var_vec) {
            if (read_facade->does_statement_modify_var(stmt, v)) {
                table.add_row({stmt, v});
            }
        }
    }

    if (table.empty()) {
        return std::nullopt;
    }
    return table;
}

auto ModifiesSEvaluator::eval_modifies_s(const std::shared_ptr<qps::StmtSynonym>& stmt_synonym,
                                         const QuotedIdent& identifier) const -> std::optional<Table> {
    const auto relevant_stmts = stmt_synonym->scan(read_facade);
    const auto relevant_stmt_vec = std::vector<std::string>{relevant_stmts.begin(), relevant_stmts.end()};

    const auto stmts = read_facade->get_statements_that_modify_var(identifier.get_value());
    auto table = Table{{stmt_synonym}};

    for (const auto& stmt : relevant_stmt_vec) {
        if (stmts.find(stmt) != stmts.end()) {
            table.add_row({stmt});
        }
    }
    if (table.empty()) {
        return std::nullopt;
    }
    return table;
}

auto ModifiesSEvaluator::eval_modifies_s(const std::shared_ptr<qps::StmtSynonym>& stmt_synonym, const WildCard&) const
    -> std::optional<Table> {
    // TODO: Improve pkb API: Get all statement that modifies
    const auto relevant_stmts = stmt_synonym->scan(read_facade);
    auto table = Table{{stmt_synonym}};

    const auto variables = read_facade->get_variables();
    const auto var_vec = std::vector<std::string>{variables.begin(), variables.end()};

    for (const auto& var : var_vec) {
        const auto statements = read_facade->get_statements_that_modify_var(var);
        const auto stmt_vec = std::vector<std::string>{statements.begin(), statements.end()};

        for (const auto& stmt : stmt_vec) {
            if (relevant_stmts.find(stmt) != relevant_stmts.end()) {
                table.add_row({stmt});
            }
        }
    }

    if (table.empty()) {
        return std::nullopt;
    }
    return table;
}

auto ModifiesSEvaluator::eval_modifies_s(const Integer& stmt_num, const std::shared_ptr<qps::VarSynonym>& var_syn) const
    -> std::optional<Table> {
    const auto variables = read_facade->get_vars_modified_by_statement(std::to_string(stmt_num.value));
    auto table = Table{{var_syn}};
    for (const auto& v : variables) {
        table.add_row({v});
    }

    if (table.empty()) {
        return std::nullopt;
    }
    return table;
}

auto ModifiesSEvaluator::eval_modifies_s(const Integer& stmt_num, const QuotedIdent& identifier) const
    -> std::optional<Table> {
    const auto does_modify =
        read_facade->does_statement_modify_var(std::to_string(stmt_num.value), identifier.get_value());

    if (!does_modify) {
        return std::nullopt;
    }
    return Table{};
}

auto ModifiesSEvaluator::eval_modifies_s(const Integer& stmt_num, const WildCard&) const -> std::optional<Table> {
    auto does_statement_modify_var = false;
    const auto variables = read_facade->get_variables();
    const auto var_vec = std::vector<std::string>{variables.begin(), variables.end()};

    for (const auto& var : var_vec) {
        if (read_facade->does_statement_modify_var(std::to_string(stmt_num.value), var)) {
            does_statement_modify_var = true;
            break;
        }
    }

    if (!does_statement_modify_var) {
        return std::nullopt;
    }
    return Table{};
};

} // namespace qps