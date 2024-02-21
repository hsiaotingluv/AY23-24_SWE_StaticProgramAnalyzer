#include "qps/evaluators/relationship/uses_s_evaluator.hpp"

namespace qps {

auto UsesSEvaluator::eval_uses_s(const std::shared_ptr<ReadFacade>& read_facade) {
    return overloaded{

        // e.g. Uses(a/pn/s, v)
        [read_facade](const std::shared_ptr<StmtSynonym>& stmt_synonym,
                      const std::shared_ptr<VarSynonym>& var_synonym) -> std::optional<Table> {
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
        },

        // e.g. Uses(a/pn/s, "v")
        [read_facade](const std::shared_ptr<StmtSynonym>& stmt_synonym,
                      const qps::QuotedIdent& quoted_ident) -> std::optional<Table> {
            auto table = Table{{stmt_synonym}};

            StatementType statement_type;
            if (const auto assign_stmt = std::dynamic_pointer_cast<AssignSynonym>(stmt_synonym)) {
                statement_type = StatementType::Assign;
            } else if (const auto print_stmt = std::dynamic_pointer_cast<PrintSynonym>(stmt_synonym)) {
                statement_type = StatementType::Print;
            } else if (const auto if_stmt = std::dynamic_pointer_cast<IfSynonym>(stmt_synonym)) {
                statement_type = StatementType::If;
            } else if (const auto while_stmt = std::dynamic_pointer_cast<WhileSynonym>(stmt_synonym)) {
                statement_type = StatementType::While;
            } else {
                statement_type = StatementType::Raw;
            }

            const auto statements = read_facade->get_statements_that_use_var(quoted_ident.get_value(), statement_type);
            for (const auto& stmt : statements) {
                table.add_row({stmt});
            }

            return table;
        },

        // e.g. Uses(a/pn/s, _)
        [read_facade](const std::shared_ptr<StmtSynonym>& stmt_synonym,
                      const qps::WildCard& wild_card) -> std::optional<Table> {
            auto table = Table{{stmt_synonym}};
            const auto relevant_stmts = stmt_synonym->scan(read_facade);

            const auto all_using_stmts = read_facade->get_all_statements_that_use();
            for (const auto& stmt_candidate : all_using_stmts) {
                if (relevant_stmts.find(stmt_candidate) != relevant_stmts.end()) {
                    table.add_row({stmt_candidate});
                }
            }
            return table;
        },

        // e.g. Uses(3, v)
        [read_facade](const qps::Integer& stmt_num,
                      const std::shared_ptr<VarSynonym>& var_synonym) -> std::optional<Table> {
            auto table = Table{{var_synonym}};
            const auto used_vars = read_facade->get_vars_used_by_statement(std::to_string(stmt_num.value));
            for (const auto& var : used_vars) {
                table.add_row({var});
            }
            return table;
        },

        // e.g. Uses(3, "v")
        [read_facade](const qps::Integer& stmt_num, const qps::QuotedIdent& quoted_ident) -> std::optional<Table> {
            const auto stmt_num_string = std::to_string(stmt_num.value);
            const auto var_string = quoted_ident.get_value();
            bool stmt_uses_var = read_facade->does_statement_use_var(stmt_num_string, var_string);

            if (!stmt_uses_var) {
                return std::nullopt;
            }
            return Table{};
        },

        // e.g. Uses(3, _)
        [read_facade](const qps::Integer& stmt_num, const qps::WildCard& wild_card) -> std::optional<Table> {
            const auto stmt_num_string = std::to_string(stmt_num.value);
            bool stmt_uses_var = read_facade->does_statement_use_any_var(stmt_num_string);

            if (!stmt_uses_var) {
                return std::nullopt;
            }
            return Table{};
        }};
}

auto UsesSEvaluator::evaluate() -> std::optional<Table> {
    return std::visit(eval_uses_s(read_facade), uses_s.stmt, uses_s.ent);
}

} // namespace qps