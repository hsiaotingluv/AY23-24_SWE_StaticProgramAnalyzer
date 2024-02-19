#pragma once

#include "pkb/facades/read_facade.h"
#include "qps/evaluators/entities/entity_scanner.hpp"
#include "qps/evaluators/results_table.hpp"
#include "qps/parser/entities/primitives.hpp"
#include "qps/template_utils.hpp"
#include <optional>

namespace qps {
auto followsT_evaluator(std::shared_ptr<ReadFacade> read_facade) {
    return overloaded{
        [&read_facade](const qps::WildCard&, const qps::WildCard&) -> std::optional<Table> {
            // TODO: PKB API: is_any_follows?
            auto any_follows = read_facade->get_all_follows_star().size();
            if (any_follows > 0) {
                return Table{}; // True
            }
            return std::nullopt; // False
        },
        [&read_facade](const qps::WildCard&, const qps::Integer& stmt2) -> std::optional<Table> {
            const auto any_follows = read_facade->get_follows_stars_following(std::to_string(stmt2.value)).size();
            if (any_follows > 0) {
                return Table{}; // True
            }
            return std::nullopt; // False
        },
        [&read_facade](const qps::Integer& stmt1, const qps::WildCard&) -> std::optional<Table> {
            const auto any_follows = read_facade->get_follows_stars_by(std::to_string(stmt1.value)).size();
            if (any_follows > 0) {
                return Table{}; // True
            }
            return std::nullopt; // False
        },
        [&read_facade](const qps::Integer& stmt1, const qps::Integer& stmt2) -> std::optional<Table> {
            // Both integers -> return true if stmt1 follows stmt2
            const auto follows =
                read_facade->has_follows_stars(std::to_string(stmt1.value), std::to_string(stmt2.value));
            if (follows) {
                return Table{}; // True
            }
            return std::nullopt; // False
        },
        [&read_facade](const std::shared_ptr<qps::StmtSynonym>& stmt_synonym,
                       const qps::WildCard&) -> std::optional<Table> {
            // TODO: get all follows keys should take in a statement type
            const auto relevant_stmts = scan_entities(read_facade, stmt_synonym);
            auto table = Table{{stmt_synonym}};

            // Get all statements that are followed by anything
            auto possible_answers = read_facade->get_all_follows_star_keys();

            for (const auto& stmt : possible_answers) {
                if (relevant_stmts.find(stmt) != relevant_stmts.end()) {
                    table.add_row({stmt});
                }
            }

            if (table.empty()) {
                return std::nullopt;
            }

            return table;
        },
        [&read_facade](const qps::WildCard&,
                       const std::shared_ptr<qps::StmtSynonym>& stmt_synonym) -> std::optional<Table> {
            // TODO: get all follows values should take in a statement type
            const auto relevant_stmts = scan_entities(read_facade, stmt_synonym);
            auto table = Table{{stmt_synonym}};

            // Get all statements that are following anything
            auto possible_answers = read_facade->get_all_follows_star_values();

            for (const auto& stmt : possible_answers) {
                if (relevant_stmts.find(stmt) != relevant_stmts.end()) {
                    table.add_row({stmt});
                }
            }

            if (table.empty()) {
                return std::nullopt;
            }

            return table;
        },
        [&read_facade](const std::shared_ptr<qps::StmtSynonym>& stmt_synonym1,
                       const std::shared_ptr<qps::StmtSynonym>& stmt_synonym2) -> std::optional<Table> {
            if (stmt_synonym1 == stmt_synonym2) {
                // Follows relation is irreflexive
                return std::nullopt;
            }

            const auto relevant_stmts1 = scan_entities(read_facade, stmt_synonym1);
            const auto relevant_stmts2 = scan_entities(read_facade, stmt_synonym2);
            auto table = Table{{stmt_synonym1, stmt_synonym2}};

            const auto all_follows = read_facade->get_all_follows_star();
            for (auto& [stmt1, stmt2_set] : all_follows) {
                for (auto& stmt2 : stmt2_set) {
                    if (relevant_stmts1.find(stmt1) != relevant_stmts1.end() &&
                        relevant_stmts2.find(stmt2) != relevant_stmts2.end()) {
                        table.add_row({stmt1, stmt2});
                    }
                }
            }

            if (table.empty()) {
                return std::nullopt;
            }
            return table;
        },
        [&read_facade](const std::shared_ptr<qps::StmtSynonym>& stmt_synonym,
                       const std::shared_ptr<qps::Integer>& stmt_num) -> std::optional<Table> {
            const auto relevant_stmts = scan_entities(read_facade, stmt_synonym);
            auto table = Table{{stmt_synonym}};

            const auto relevant_follows = read_facade->get_follows_stars_by(std::to_string(stmt_num->value));
            for (auto& relevant_follows : relevant_follows) {
                if (relevant_stmts.find(relevant_follows) != relevant_stmts.end()) {
                    table.add_row({relevant_follows});
                }
            }

            return std::nullopt;
        },
        [&read_facade](const std::shared_ptr<qps::Integer>& stmt_num,
                       const std::shared_ptr<qps::StmtSynonym>& stmt_synonym) -> std::optional<Table> {
            const auto relevant_stmts = scan_entities(read_facade, stmt_synonym);
            auto table = Table{{stmt_synonym}};

            const auto relevant_follows = read_facade->get_follows_stars_following(std::to_string(stmt_num->value));
            for (auto& relevant_follows : relevant_follows) {
                if (relevant_stmts.find(relevant_follows) != relevant_stmts.end()) {
                    table.add_row({relevant_follows});
                }
            }
            return std::nullopt;
        },
        [](const auto&, const auto&) -> std::optional<Table> {
            return std::nullopt;
        }

    };
}
} // namespace qps
