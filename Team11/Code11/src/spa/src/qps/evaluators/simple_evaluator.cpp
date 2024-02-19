#include "qps/evaluators/simple_evaluator.hpp"
#include "qps/evaluators/entities/entity_scanner.hpp"
#include "qps/evaluators/relationship/clause_evaluator_selector.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/evaluators/relationship/pattern_evaluator.hpp"

#include <algorithm>
#include <memory>
#include <optional>
#include <unordered_set>
#include <vector>

namespace qps {
void print(const Table& table) {
    for (const auto& col : table.get_column()) {
        std::cout << col->get_name() << "\t";
    }

    std::cout << std::endl;
    for (const auto& row : table.get_records()) {
        for (const auto& col : row) {
            std::cout << col << "\t";
        }
        std::cout << std::endl;
    }
}

auto join(const Table& table1, const Table& table2) -> std::optional<Table> {
    const auto column1 = table1.get_column();
    const auto column2 = table2.get_column();

    // Step 0: Short-circuit if either table is empty
    if (column1.empty()) {
        return table2;
    } else if (column2.empty()) {
        return table1;
    }

    // Step 1: join columns
    std::unordered_set<std::shared_ptr<Synonym>> new_column_set{column1.begin(), column1.end()};
    for (const auto& synonym : column2) {
        if (new_column_set.find(synonym) == new_column_set.end()) {
            new_column_set.insert(synonym);
        }
    }
    const auto new_column = std::vector<std::shared_ptr<Synonym>>{new_column_set.begin(), new_column_set.end()};
    auto new_table = Table{new_column};

    const auto fn = [&new_column](const std::vector<std::shared_ptr<Synonym>>& column) {
        auto mapping = std::unordered_map<int, int>{};
        for (int i = 0; i < static_cast<int>(column.size()); i++) {
            for (int j = 0; j < static_cast<int>(new_column.size()); j++) {
                if (column[i] == new_column[j]) {
                    mapping[j] = i;
                }
            }
        }
        return mapping;
    };
    const auto table1_mask = fn(column1);
    const auto table2_mask = fn(column2);

    // Step 2: join records
    const auto record1 = table1.get_records();
    const auto record2 = table2.get_records();

    for (const auto& record1 : table1.get_records()) {
        for (const auto& record2 : table2.get_records()) {
            auto new_record = std::vector<std::string>(new_column.size(), "");
            auto has_conflict = false;

            // Populate new_record with values from record1
            for (int i = 0; i < static_cast<int>(record1.size()); i++) {
                if (table1_mask.find(i) == table1_mask.end()) {
                    continue;
                }
                new_record[table1_mask.at(i)] = record1[i];
            }

            // Populate new_record with values from record2, checking for conflicts
            for (int i = 0; i < static_cast<int>(record2.size()); i++) {
                if (table2_mask.find(i) == table2_mask.end()) {
                    continue;
                }
                if (new_record[table2_mask.at(i)].empty() && new_record[table2_mask.at(i)] != record2[i]) {
                    // Conflict detected
                    has_conflict = true;
                    break;
                }
                new_record[table2_mask.at(i)] = record2[i];
            }

            if (has_conflict) {
                // Skip this record if there is a conflict
                continue;
            }
        }
    }

    if (new_table.get_records().empty()) {
        // Failed -> everything conflicts
        return std::nullopt;
    }

    return new_table;
}

auto project(const std::shared_ptr<ReadFacade>& read_facade, const Table& table,
             const std::shared_ptr<Synonym>& synonym) -> std::vector<std::string> {
    if (table.get_column().empty()) {
        // Table is empty -> no constraints -> return everything
        const auto results = scan_entities(read_facade, synonym);
        return {results.begin(), results.end()};
    }

    const auto column = table.get_column();
    const auto record = table.get_records();

    const auto col_idx = std::find(column.begin(), column.end(), synonym) - column.begin();
    if (col_idx == static_cast<long>(column.size())) {
        // Synonym not found in table
        return {};
    }

    auto results = std::vector<std::string>{};
    for (const auto& row : record) {
        results.push_back(row.at(col_idx));
    }
    return results;
}

auto Evaluator::evaluate(const qps::Query& query_obj) -> std::vector<std::string> {
    const auto reference = query_obj.reference;
    const auto results = scan_entities(read_facade, reference);

    auto curr_table = Table{};

    if (query_obj.clauses.empty()) {
        // Short-circuit if there are no clauses
        return {results.begin(), results.end()};
    }

    // Step 1: populate all synonyms
    for (const auto& clause : query_obj.clauses) {

        if (const auto such_that_clause = std::dynamic_pointer_cast<qps::SuchThatClause>(clause)) {
            const auto relationship = such_that_clause->rel_ref;
            evaluator = std::visit(clause_evaluator_selector(read_facade), relationship);
        } else if (const auto pattern_clause = std::dynamic_pointer_cast<qps::PatternClause>(clause)) {
            evaluator = std::make_shared<PatternEvaluator>(read_facade, *pattern_clause);
        }

        const auto maybe_table = evaluator->evaluate();
        if (!maybe_table.has_value()) {
            return {};
        }

        const auto& next_table = maybe_table.value();
        const auto maybe_new_table = join(curr_table, next_table);
        if (!maybe_new_table.has_value()) {
            // Conflict detected -> no results
            return {};
        }
        curr_table = maybe_new_table.value();
    }

    // Step 2: project to relevant synonym
    return project(read_facade, curr_table, query_obj.reference);
};

} // namespace qps