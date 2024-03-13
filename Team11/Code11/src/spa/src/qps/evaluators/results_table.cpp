#include "qps/evaluators/results_table.hpp"
#include "pkb/facades/read_facade.h"
#include "qps/parser/analysers/semantic_analyser.hpp"
#include "qps/parser/entities/select.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/template_utils.hpp"

#include <algorithm>
#include <iterator>
#include <memory>
#include <numeric>
#include <set>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

namespace qps::detail {
auto to_synonyms(const std::vector<Elem>& elems) -> Synonyms {
    auto synonyms = Synonyms{};
    synonyms.reserve(elems.size());
    std::transform(elems.begin(), elems.end(), std::back_inserter(synonyms),
                   [](const auto& elem) -> std::shared_ptr<Synonym> {
                       return std::visit(overloaded{[](const std::shared_ptr<Synonym>& synonym) {
                                             return synonym;
                                         }},
                                         elem);
                   });
    return synonyms;
}

auto compare_rows(const std::vector<std::string>& row1, const std::vector<std::string>& row2,
                  const std::vector<int>& idxs) -> bool {
    for (auto idx : idxs) {
        const auto compareResult = row1[idx].compare(row2[idx]);
        if (compareResult != 0) {
            return compareResult < 0; // Sort by the i-th column
        }
    }
    return false; // If all first n columns are equal, don't change the order
}

void print(const std::vector<std::vector<std::string>>& records) {
    for (const auto& row : records) {
        for (const auto& col : row) {
            std::cout << col << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

auto sort_and_get_order(std::vector<std::shared_ptr<Synonym>>& column_names) -> std::vector<int> {
    auto ordering = std::vector<int>(column_names.size());
    std::iota(std::begin(ordering), std::end(ordering), 0);
    std::sort(ordering.begin(), ordering.end(), [&column_names](int i, int j) {
        return column_names[i] < column_names[j];
    });
    reorder(column_names, ordering);
    return ordering;
}

auto double_pointer_merge(std::vector<std::shared_ptr<Synonym>> column1, std::vector<std::shared_ptr<Synonym>> column2)
    -> std::vector<std::shared_ptr<Synonym>> {
    // Sort + double pointer merge to remove duplicates
    // Time complexity: O(nlogn) with n = max(column1.size(), column2.size())

    std::sort(column1.begin(), column1.end());
    std::sort(column2.begin(), column2.end());

    auto it1 = column1.begin();
    auto it2 = column2.begin();

    auto new_column = std::vector<std::shared_ptr<Synonym>>{};
    new_column.reserve(column1.size() + column2.size());

    while (it1 != column1.end() && it2 != column2.end()) {
        if (*it1 < *it2) {
            new_column.push_back(*it1);
            it1++;
        } else if (*it2 < *it1) {
            new_column.push_back(*it2);
            it2++;
        } else {
            new_column.push_back(*it1);
            it1++;
            it2++;
        }
    }

    while (it1 != column1.end()) {
        new_column.push_back(*it1);
        it1++;
    }

    while (it2 != column2.end()) {
        new_column.push_back(*it2);
        it2++;
    }
    return new_column;
}

/**
 * @brief Merge two columns while removing duplicates. Returns the new column and the ordering that can be used to
 * reorder the original columns to match the new column.
 *
 * This function will sort the input columns.
 *
 * @param column1
 * @param column2
 * @return std::tuple<std::vector<std::shared_ptr<Synonym>>, std::vector<int>, std::vector<int>, std::vector<int>,
 * std::vector<int>>
 */
auto double_pointer_merge_with_ordering(std::vector<std::shared_ptr<Synonym>>& column1,
                                        std::vector<std::shared_ptr<Synonym>>& column2)
    -> std::tuple<std::vector<std::shared_ptr<Synonym>>, std::vector<int>, std::vector<int>,
                  std::vector<std::tuple<int, int>>> {
    // Sort + double pointer merge to remove duplicates
    // Time complexity: O(nlogn) with n = max(column1.size(), column2.size())

    auto ordering1 = sort_and_get_order(column1);
    auto ordering2 = sort_and_get_order(column2);

    auto it1 = column1.begin();
    auto it2 = column2.begin();

    auto new_column = std::vector<std::shared_ptr<Synonym>>{};
    new_column.reserve(column1.size() + column2.size());

    auto curr_idx1 = 0;
    auto curr_idx2 = 0;

    auto common_column_idxs = std::vector<std::tuple<int, int>>{};

    while (it1 != column1.end() && it2 != column2.end()) {
        if (*it1 < *it2) {
            new_column.push_back(*it1);
            it1++;
            curr_idx1++;
        } else if (*it2 < *it1) {
            new_column.push_back(*it2);
            it2++;
            curr_idx2++;
        } else {
            new_column.push_back(*it1);
            it1++;
            it2++;
            // Common column found
            common_column_idxs.emplace_back(curr_idx1, curr_idx2);

            curr_idx1++;
            curr_idx2++;
        }
    }

    while (it1 != column1.end()) {
        new_column.push_back(*it1);
        it1++;
    }

    while (it2 != column2.end()) {
        new_column.push_back(*it2);
        it2++;
    }
    return std::make_tuple(new_column, ordering1, ordering2, common_column_idxs);
}

auto unordered_set_merge(const std::vector<std::shared_ptr<Synonym>>& column1,
                         const std::vector<std::shared_ptr<Synonym>>& column2)
    -> std::vector<std::shared_ptr<Synonym>> {
    // Set merge to remove duplicates
    // Time complexity: O(nlogn) with n = max(column1.size(), column2.size())
    const auto& columnBig = column1.size() > column2.size() ? column1 : column2;
    const auto& columnSmall = column1.size() > column2.size() ? column2 : column1;

    auto new_column_set = std::unordered_set<std::shared_ptr<Synonym>>{columnBig.begin(), columnBig.end()};
    for (const auto& synonym : columnSmall) {
        new_column_set.insert(synonym);
    }

    auto new_column = std::vector<std::shared_ptr<Synonym>>{new_column_set.begin(), new_column_set.end()};
    std::sort(new_column.begin(), new_column.end());
    return new_column;
}

auto ordered_set_merge(const std::vector<std::shared_ptr<Synonym>>& column1,
                       const std::vector<std::shared_ptr<Synonym>>& column2) -> std::vector<std::shared_ptr<Synonym>> {
    // Set merge to remove duplicates
    // Time complexity: O(nlogn) with n = max(column1.size(), column2.size())
    auto new_column_set = std::set<std::shared_ptr<Synonym>>{column1.begin(), column1.end()};
    for (const auto& synonym : column2) {
        new_column_set.insert(synonym);
    }

    auto new_column = std::vector<std::shared_ptr<Synonym>>{new_column_set.begin(), new_column_set.end()};
    return new_column;
}

/**
 * @brief Build an index mapping from old_column to new_column.
 *
 * This function does not assume that the columns are sorted.
 *
 * @param old_column Column names from the old table
 * @param new_column Column names from the new table
 * @return std::unordered_map<int, int> Index mapping from old_column to new_column
 */
auto build_mapping(const std::vector<std::shared_ptr<Synonym>>& old_column,
                   const std::vector<std::shared_ptr<Synonym>>& new_column) -> std::unordered_map<int, int> {
    auto mapping = std::unordered_map<int, int>{};
    mapping.reserve(old_column.size());
    for (int i = 0; i < static_cast<int>(old_column.size()); i++) {
        for (int j = 0; j < static_cast<int>(new_column.size()); j++) {
            if (old_column[i] == new_column[j]) {
                mapping[i] = j;
            }
        }
    }
    return mapping;
}

/**
 * @brief Build an index mapping from old_column to new_column.
 * This function assumes that the columns are sorted.
 *
 * @param old_column Column names from the old table
 * @param new_column Column names from the new table
 * @return std::unordered_map<int, int> Index mapping from old_column to new_column
 */
auto build_mapping_sorted(const std::vector<std::shared_ptr<Synonym>>& old_column,
                          const std::vector<std::shared_ptr<Synonym>>& new_column) -> std::unordered_map<int, int> {
    auto mapping = std::unordered_map<int, int>{};
    mapping.reserve(old_column.size());

    auto it1 = 0;
    auto it2 = 0;
    while (it1 < static_cast<int>(old_column.size()) && it2 < static_cast<int>(new_column.size())) {
        if (old_column[it1] == new_column[it2]) {
            mapping[it1] = it2;
            it1++;
            it2++;
        } else if (old_column[it1] < new_column[it2]) {
            it1++;
        } else {
            it2++;
        }
    }

    return mapping;
}

void nested_loop_join_records(const Table& table1, const Table& table2, Table& new_table) {
    const auto column1 = table1.get_column();
    const auto column2 = table2.get_column();
    const auto new_column = new_table.get_column();

    const auto table1_mask = build_mapping(column1, new_column);
    const auto table2_mask = build_mapping(column2, new_column);

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
                if (!new_record[table2_mask.at(i)].empty() && new_record[table2_mask.at(i)] != record2[i]) {
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
            new_table.add_row(new_record);
        }
    }
}

template <typename ColumnMergeStrategy, typename JoinRecordsStrategy>
auto join(Table&& table1, Table&& table2, ColumnMergeStrategy column_merge_strategy,
          JoinRecordsStrategy join_records_strategy) -> OutputTable {
    // Step 0: Short-circuit if either table is empty
    if (table1.empty()) {
        return table2;
    } else if (table2.empty()) {
        return table1;
    }

    // Step 1: join columns
    const auto new_column = column_merge_strategy(table1.get_column(), table2.get_column());
    auto new_table = Table{new_column};

    // Step 2: join records
    join_records_strategy(table1, table2, new_table);

    return new_table;
}

auto sort_on_column(std::vector<std::vector<std::string>>& table1_contents,
                    std::vector<std::vector<std::string>>& table2_contents,
                    const std::vector<std::tuple<int, int>>& common_column_idxs) -> void {
    auto first_idxs = std::vector<int>{};
    first_idxs.reserve(common_column_idxs.size());
    auto second_idxs = std::vector<int>{};
    second_idxs.reserve(common_column_idxs.size());

    for (auto [idx1, idx2] : common_column_idxs) {
        first_idxs.push_back(idx1);
        second_idxs.push_back(idx2);
    }

    std::sort(table1_contents.begin(), table1_contents.end(),
              [&first_idxs](const auto& row1, const auto& row2) -> bool {
                  return compare_rows(row1, row2, first_idxs);
              });

    std::sort(table2_contents.begin(), table2_contents.end(),
              [&second_idxs](const auto& row1, const auto& row2) -> bool {
                  return compare_rows(row1, row2, second_idxs);
              });
}

void reorder_contents(std::vector<std::vector<std::string>>& table_contents, const std::vector<int>& order) {
    std::for_each(table_contents.begin(), table_contents.end(), [&order](auto& row) {
        reorder(row, order);
    });
}

auto merge_join(Table&& table1, Table&& table2) -> OutputTable {
    // Step 0: Short-circuit if either table is empty
    if (table1.empty()) {
        return table2;
    } else if (table2.empty()) {
        return table1;
    }

    // Step 1: Reorder columns and rows
    const auto& tableA = table1.get_column().size() < table2.get_column().size() ? table1 : table2;
    const auto& tableB = table1.get_column().size() < table2.get_column().size() ? table2 : table1;

    auto table1_contents = tableA.get_records();
    auto table2_contents = tableB.get_records();
    auto table1_column_names = tableA.get_column();
    auto table2_column_names = tableB.get_column();

    const auto& [new_column, ordering1, ordering2, common_column_idxs] =
        double_pointer_merge_with_ordering(table1_column_names, table2_column_names);
    auto new_table = Table{new_column};

    const auto table1_mask = build_mapping_sorted(table1_column_names, new_column);
    const auto table2_mask = build_mapping_sorted(table2_column_names, new_column);

    reorder_contents(table1_contents, ordering1);
    reorder_contents(table2_contents, ordering2);

    // Sort based on values in the common column
    sort_on_column(table1_contents, table2_contents, common_column_idxs);

    // Step 2: join records
    auto curr_row1 = table1_contents.begin();
    auto curr_row2 = table2_contents.begin();

    while (curr_row1 != table1_contents.end() && curr_row2 != table2_contents.end()) {
        // Shift row pointers to the first row with the same value in the common column
        auto all_same = true;
        for (auto [col_idx1, col_idx2] : common_column_idxs) {
            if ((*curr_row1)[col_idx1] < (*curr_row2)[col_idx2]) {
                curr_row1++;
                all_same = false;
                break;
            } else if ((*curr_row1)[col_idx1] > (*curr_row2)[col_idx2]) {
                curr_row2++;
                all_same = false;
                break;
            }
        }
        if (!all_same) {
            continue;
        }

        // Save right pointer
        const auto right_ptr = curr_row2;

        while (true) {
            // All common columns are equal -> safe to join the records
            auto new_record = std::vector<std::string>(new_column.size(), "");
            // Populate new_record with values from records
            for (int i = 0; i < static_cast<int>(curr_row1->size()); i++) {
                new_record[table1_mask.at(i)] = (*curr_row1)[i];
            }
            for (int i = 0; i < static_cast<int>(curr_row2->size()); i++) {
                new_record[table2_mask.at(i)] = (*curr_row2)[i];
            }

            new_table.add_row(new_record);

            // Advance right pointer
            curr_row2 = std::next(curr_row2);

            // Stop if we have reached the end of the table
            if (curr_row2 == table2_contents.end()) {
                // Reset right pointer
                curr_row2 = right_ptr;
                break;
            }

            // Stop if the common columns are no longer equal
            all_same = true;
            for (auto [col_idx1, col_idx2] : common_column_idxs) {
                if ((*curr_row1)[col_idx1] != (*curr_row2)[col_idx2]) {
                    all_same = false;
                    break;
                }
            }
            if (!all_same) {
                // Reset right pointer
                curr_row2 = right_ptr;
                break;
            }
        }

        // Move to the next row
        curr_row1++;
    }

    return new_table;
}

auto cross_merge_join(Table&& table1, Table&& table2) -> OutputTable {
    // Step 0: Short-circuit if either table is empty
    if (table1.empty()) {
        return table2;
    } else if (table2.empty()) {
        return table1;
    }

    // Step 1: Reorder columns and rows
    auto& tableA = table1.get_column().size() < table2.get_column().size() ? table1 : table2;
    auto& tableB = table1.get_column().size() < table2.get_column().size() ? table2 : table1;

    auto table1_contents = tableA.get_records();
    auto table2_contents = tableB.get_records();
    auto table1_column_names = tableA.get_column();
    auto table2_column_names = tableB.get_column();

    const auto& [new_column, ordering1, ordering2, common_column_idxs] =
        double_pointer_merge_with_ordering(table1_column_names, table2_column_names);

    if (common_column_idxs.empty()) {
        return cross_join(std::move(tableA), std::move(tableB));
    } else {
        return merge_join(std::move(tableA), std::move(tableB));
    }
}

/**
 * @brief Assumes that the input tables have no common column names
 *
 * @param table1
 * @param table2
 * @return OutputTable
 */
auto cross_join(Table&& table1, Table&& table2) -> OutputTable {
    // Step 0: Short-circuit if either table is empty
    if (table1.empty()) {
        return table2;
    } else if (table2.empty()) {
        return table1;
    }

    // Step 1: Reorder columns and rows
    auto& table1_contents = table1.get_records();
    auto& table2_contents = table2.get_records();
    auto& table1_column_names = table1.get_column();
    auto& table2_column_names = table2.get_column();

    auto new_column = [&table1_column_names, &table2_column_names]() {
        std::vector<std::shared_ptr<Synonym>> new_column{};
        new_column.reserve(table1_column_names.size() + table2_column_names.size());
        new_column.insert(new_column.end(), std::make_move_iterator(table1_column_names.begin()),
                          std::make_move_iterator(table1_column_names.end()));
        new_column.insert(new_column.end(), std::make_move_iterator(table2_column_names.begin()),
                          std::make_move_iterator(table2_column_names.end()));
        return new_column;
    }();

    auto new_table = Table{new_column};

    // Step 2: join records
    for (const auto& record1 : table1_contents) {
        for (const auto& record2 : table2_contents) {
            auto new_record = std::vector<std::string>{};
            new_record.reserve(new_column.size());
            new_record.insert(new_record.end(), record1.begin(), record1.end());
            new_record.insert(new_record.end(), record2.begin(), record2.end());
            new_table.add_row(new_record);
        }
    }

    return new_table;
}

auto cross_join_with_conflict_checks(Table&& table1, Table&& table2) -> OutputTable {
    return join(std::move(table1), std::move(table2), double_pointer_merge, nested_loop_join_records);
}

auto build_table(const Synonyms& synonyms, const std::shared_ptr<pkb::ReadFacade>& read_facade) -> OutputTable {
    auto table = Table{};
    for (const auto& synonym : synonyms) {
        auto curr_table = Table{{synonym}};
        for (const auto& result : synonym->scan(read_facade)) {
            curr_table.add_row({result});
        }
        table = std::get<Table>(detail::cross_join(std::move(table), std::move(curr_table)));
    }
    return table;
}

} // namespace qps::detail

namespace qps {

auto is_unit(const OutputTable& table) -> bool {
    return std::holds_alternative<UnitTable>(table);
}

auto is_empty(const OutputTable& table) -> bool {
    return std::holds_alternative<Table>(table) && std::get<Table>(table).empty();
}

auto join(OutputTable&& table1, OutputTable&& table2) -> OutputTable {
    return std::visit(overloaded{
                          [](Table&& table1, Table&& table2) -> OutputTable {
                              return detail::cross_merge_join(std::move(table1), std::move(table2));
                          },
                          [](UnitTable&&, UnitTable&&) -> OutputTable {
                              return UnitTable{};
                          },
                          [](Table&& table, UnitTable&&) -> OutputTable {
                              return table;
                          },
                          [](UnitTable&&, Table&& table) -> OutputTable {
                              return table;
                          },
                      },
                      std::move(table1), std::move(table2));
}

auto to_string(const Table& table) -> std::vector<std::string> {
    auto results = std::unordered_set<std::string>{};
    for (const auto& row : table.get_records()) {
        auto ss = std::stringstream{};
        ss << row.front();
        for (auto it = std::next(row.begin()); it != row.end(); it++) {
            ss << " " << *it;
        }
        results.insert(ss.str());
    }

    return {results.begin(), results.end()};
}

auto to_string(const Table& table, const std::vector<int>& column_indices) -> std::vector<std::string> {
    auto results = std::unordered_set<std::string>{};
    for (const auto& row : table.get_records()) {
        auto ss = std::stringstream{};
        ss << row.at(column_indices.front());
        for (auto it = std::next(column_indices.begin()); it != column_indices.end(); it++) {
            ss << " " << row.at(*it);
        }
        results.insert(ss.str());
    }

    return {results.begin(), results.end()};
}

/**
 * @brief Build a table from the given synonyms by selecting columns from the input table
 *
 * @param table
 * @param synonyms
 * @return Table
 */
auto project(const Table& table, const Synonyms& synonyms) -> Table {
    // Contract: synonyms <= table.get_column()
    const auto new_idx_to_old_idx = [&]() {
        const auto column_names = table.get_column();
        auto new_idx_to_old_idx = std::vector<int>{};
        new_idx_to_old_idx.reserve(synonyms.size());
        std::for_each(synonyms.begin(), synonyms.end(),
                      [&new_idx_to_old_idx, &column_names](const auto& synonym) -> void {
                          for (int i = 0; i < static_cast<int>(column_names.size()); i++) {
                              if (column_names[i] == synonym) {
                                  new_idx_to_old_idx.push_back(i);
                                  break;
                              }
                          }
                      });
        return new_idx_to_old_idx;
    }();

    auto new_table = Table{synonyms};
    for (const auto& row : table.get_records()) {
        auto new_row = std::vector<std::string>{};
        new_row.reserve(new_idx_to_old_idx.size());
        for (const auto& old_idx : new_idx_to_old_idx) {
            new_row.push_back(row.at(old_idx));
        }
        new_table.add_row(new_row);
    }

    return new_table;
}

/**
 * @brief Build a table from the given synonyms by reading from the PKB
 *
 * @param read_facade
 * @param synonyms
 * @return Table
 */
auto build_table(const std::shared_ptr<pkb::ReadFacade>& read_facade, const Synonyms& synonyms) -> Table {
    auto table = Table{};
    for (const auto& synonym : synonyms) {
        auto curr_table = Table{{synonym}};
        for (const auto& result : synonym->scan(read_facade)) {
            curr_table.add_row({result});
        }
        table = std::get<Table>(detail::cross_join(std::move(table), std::move(curr_table)));
    }
    return table;
}

auto build_and_project(const std::shared_ptr<pkb::ReadFacade>& read_facade, const Table& table, Synonyms synonyms)
    -> std::vector<std::string> {
    if (table.empty()) {
        // Table is empty --> contradiction
        return {};
    }

    // Find synonyms in synonyms that are not in table
    auto column_names = table.get_column();
    std::sort(column_names.begin(), column_names.end());
    std::sort(synonyms.begin(), synonyms.end());

    // Synonyms that are in table
    const auto& available_synonyms = [&]() {
        auto available_synonyms = std::vector<std::shared_ptr<Synonym>>{};
        std::set_intersection(column_names.begin(), column_names.end(), synonyms.begin(), synonyms.end(),
                              std::back_inserter(available_synonyms));
        const auto reference_set = std::unordered_set<std::shared_ptr<Synonym>>{synonyms.begin(), synonyms.end()};
        return available_synonyms;
    }();

    // Synonyms that are not in table
    const auto& missing_synonyms = [&]() {
        auto missing_synonyms = std::vector<std::shared_ptr<Synonym>>{};
        std::set_difference(synonyms.begin(), synonyms.end(), available_synonyms.begin(), available_synonyms.end(),
                            std::back_inserter(missing_synonyms));
        return missing_synonyms;
    }();

    const auto final_table =
        missing_synonyms.empty()
            ? project(table, available_synonyms)
            : std::get<Table>(detail::cross_join(project(table, available_synonyms),
                                                 std::get<Table>(detail::build_table(missing_synonyms, read_facade))));

    // Reorder the columns to match the requested order
    const auto new_idx_to_old_idx = [&final_table, &synonyms]() {
        const auto column_names = final_table.get_column();
        auto new_idx_to_old_idx = std::vector<int>{};
        new_idx_to_old_idx.reserve(synonyms.size());
        std::for_each(synonyms.begin(), synonyms.end(),
                      [&new_idx_to_old_idx, &column_names](const auto& synonym) -> void {
                          for (int i = 0; i < static_cast<int>(column_names.size()); i++) {
                              if (column_names[i] == synonym) {
                                  new_idx_to_old_idx.push_back(i);
                                  break;
                              }
                          }
                      });
        return new_idx_to_old_idx;
    }();

    return to_string(final_table, new_idx_to_old_idx);
}

auto project(const std::shared_ptr<pkb::ReadFacade>& read_facade, const OutputTable& table, const Reference& reference)
    -> std::vector<std::string> {
    static constexpr auto TRUE_STRING = "TRUE";
    static constexpr auto FALSE_STRING = "FALSE";

    return std::visit(
        overloaded{
            [](const Table& table, const BooleanReference&) -> std::vector<std::string> {
                return table.empty() ? std::vector<std::string>{FALSE_STRING} : std::vector<std::string>{TRUE_STRING};
            },
            [](const UnitTable&, const BooleanReference&) -> std::vector<std::string> {
                return {TRUE_STRING};
            },
            [&read_facade](const UnitTable&, const std::vector<Elem>& elems) -> std::vector<std::string> {
                return to_string(std::get<Table>(detail::build_table(detail::to_synonyms(elems), read_facade)));
            },
            [&read_facade](const Table& table, const std::vector<Elem>& elems) -> std::vector<std::string> {
                return build_and_project(read_facade, table, detail::to_synonyms(elems));
            },
        },
        table, reference);
}

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
    std::cout << std::endl;
}
} // namespace qps
