#include "catch.hpp"

#include "qps/evaluators/results_table.hpp"
#include "qps/parser/entities/synonym.hpp"
#include <cstdlib>
#include <functional>
#include <memory>
#include <unordered_set>
#include <vector>

using namespace qps;

template <typename T>
auto init_column_names(int num_col, std::string value) -> std::vector<std::shared_ptr<Synonym>> {
    auto column = std::vector<std::shared_ptr<Synonym>>{};
    column.reserve(num_col);

    for (int i = 0; i < num_col; i++) {
        column.push_back(std::make_shared<T>(value + std::to_string(i)));
    }
    return column;
}

template <typename T>
auto init_column_names(int num_col) -> std::vector<std::shared_ptr<Synonym>> {
    auto column = std::vector<std::shared_ptr<Synonym>>{};
    column.reserve(num_col);

    for (int i = 0; i < num_col; i++) {
        // Randomly generate a string
        // Pick a character from the set {a, b, c, d, e, f, g, h, i, j} 5 times
        std::string name;
        for (int j = 0; j < 5; j++) {
            name.push_back('a' + (std::rand() % 10));
        }
        column.push_back(std::make_shared<T>(name));
    }
    return column;
}

auto init_row(int num_col, std::string value) -> std::vector<std::string> {
    auto row = std::vector<std::string>{};
    row.reserve(num_col);

    for (int i = 0; i < num_col; i++) {
        std::string name;
        for (int j = 0; j < 3; j++) {
            name.push_back('a' + (std::rand() % 10));
        }
        row.push_back(value + name + std::to_string(i));
    }
    return row;
}

TEST_CASE("Test Table") {
    auto table = Table{{std::make_shared<AnyStmtSynonym>("s")}};
    table.add_row({"1"});
    table.add_row({"2"});
    table.add_row({"3"});

    auto column = table.get_column();
    REQUIRE(column.size() == 1);
    REQUIRE(column[0]->get_name() == "s");

    auto records = table.get_records();
    REQUIRE(records.size() == 3);
    REQUIRE(records[0].size() == 1);
    REQUIRE(records[0][0] == "1");
    REQUIRE(records[1].size() == 1);
    REQUIRE(records[1][0] == "2");
    REQUIRE(records[2].size() == 1);
    REQUIRE(records[2][0] == "3");
}

TEST_CASE("Test Join") {
    SECTION("Cross-Product") {
        auto table1 = Table{{std::make_shared<AnyStmtSynonym>("s")}};
        table1.add_row({"1"});
        table1.add_row({"2"});
        table1.add_row({"3"});

        auto table2 = Table{{std::make_shared<VarSynonym>("v")}};
        table2.add_row({"x"});
        table2.add_row({"y"});
        table2.add_row({"z"});

        // Must be alphabetically sorted
        const auto expected_column = std::vector<std::shared_ptr<Synonym>>{std::make_shared<AnyStmtSynonym>("s"),
                                                                           std::make_shared<VarSynonym>("v")};

        // Expected set can be in any order row-wise. Column-wise, it should be in the order of expected_column
        const auto expected_set = std::unordered_set<std::tuple<std::string, std::string>>{
            {"1", "x"}, {"1", "y"}, {"1", "z"}, {"2", "x"}, {"2", "y"}, {"2", "z"}, {"3", "x"}, {"3", "y"}, {"3", "z"}};

        auto table1_variant = OutputTable{table1};
        const auto result = join(std::move(table1_variant), std::move(table2));
        REQUIRE(!is_empty(result));
        REQUIRE(!is_unit(result));
        REQUIRE(std::holds_alternative<Table>(result));
        const auto table = std::get<Table>(result);

        auto column = table.get_column();
        const auto ordering = detail::sort_and_get_order(column);

        REQUIRE(column == expected_column);
        auto records = table.get_records();
        REQUIRE(records.size() == expected_set.size());

        for (auto& record : records) {
            reorder(record, ordering);
            REQUIRE(expected_set.find(std::make_tuple(record[0], record[1])) != expected_set.end());
        }
    }
}

#ifdef ENABLE_BENCHMARK
TEST_CASE("BENCHMARK - Merge") {
    constexpr int num_cols = 200;  // Suppose at most 200 synonyms
    constexpr int num_overlap = 2; // Suppose at most 10 overlapping synonyms
    constexpr int num_tries = 1000;

    std::cout << "Benchmarking merge strategies with " << num_cols << " columns and " << num_overlap
              << " overlapping synonyms, averaged over " << num_tries << " tries" << std::endl;

    auto timings = std::vector<int>{};
    for (int c = 0; c < num_tries; c++) {
        auto column1 = init_column_names<ConstSynonym>(num_cols, "c");
        auto column2 = init_column_names<VarSynonym>(num_cols, "v");
        auto column3 = init_column_names<AssignSynonym>(num_overlap); // 10 overlapping synonyms

        column1.insert(column1.end(), column3.begin(), column3.end());
        column2.insert(column2.end(), column3.begin(), column3.end());

        std::shuffle(column1.begin(), column1.end(), std::mt19937(std::random_device()()));
        std::shuffle(column2.begin(), column2.end(), std::mt19937(std::random_device()()));

        auto begin = std::chrono::high_resolution_clock::now();
        auto result = detail::double_pointer_merge(column1, column2);
        auto end = std::chrono::high_resolution_clock::now();

        REQUIRE(result.size() == 2 * num_cols + num_overlap);

        auto average = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
        timings.push_back(average);
    }

    auto average = std::accumulate(timings.begin(), timings.end(), 0.0) / num_tries;
    std::cout << "\t [double_pointer_merge]: " << average << "ns" << std::endl;

    timings.clear();

    for (int c = 0; c < num_tries; c++) {
        auto column1 = init_column_names<ConstSynonym>(num_cols, "c");
        auto column2 = init_column_names<VarSynonym>(num_cols, "v");
        auto column3 = init_column_names<AssignSynonym>(num_overlap); // 10 overlapping synonyms

        column1.insert(column1.end(), column3.begin(), column3.end());
        column2.insert(column2.end(), column3.begin(), column3.end());

        std::shuffle(column1.begin(), column1.end(), std::mt19937(std::random_device()()));
        std::shuffle(column2.begin(), column2.end(), std::mt19937(std::random_device()()));

        auto begin = std::chrono::high_resolution_clock::now();
        auto result = detail::unordered_set_merge(column1, column2);
        auto end = std::chrono::high_resolution_clock::now();
        REQUIRE(result.size() == 2 * num_cols + num_overlap);

        auto average = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
        timings.push_back(average);
    }

    average = std::accumulate(timings.begin(), timings.end(), 0.0) / num_tries;
    std::cout << "\t [unordered_set_merge]: " << average << "ns" << std::endl;

    timings.clear();

    for (int c = 0; c < num_tries; c++) {
        auto column1 = init_column_names<ConstSynonym>(num_cols, "c");
        auto column2 = init_column_names<VarSynonym>(num_cols, "v");
        auto column3 = init_column_names<AssignSynonym>(num_overlap); // 10 overlapping synonyms

        column1.insert(column1.end(), column3.begin(), column3.end());
        column2.insert(column2.end(), column3.begin(), column3.end());

        std::shuffle(column1.begin(), column1.end(), std::mt19937(std::random_device()()));
        std::shuffle(column2.begin(), column2.end(), std::mt19937(std::random_device()()));

        auto begin = std::chrono::high_resolution_clock::now();
        auto result = detail::ordered_set_merge(column1, column2);
        auto end = std::chrono::high_resolution_clock::now();
        REQUIRE(result.size() == 2 * num_cols + num_overlap);

        auto average = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
        timings.push_back(average);
    }

    average = std::accumulate(timings.begin(), timings.end(), 0.0) / num_tries;
    std::cout << "\t [ordered_set_merge]: " << average << "ns" << std::endl;

    std::cout << std::endl;
}

TEST_CASE("BENCHMARK - Cross Product") {
    constexpr int num_rows = 100; // Suppose at most 100 rows
    constexpr int num_cols = 20;  // Suppose at most 20 synonyms
    constexpr int num_tries = 50;

    std::cout << "Benchmarking cross product of 2 tables with " << num_rows << " rows and " << num_cols
              << " columns, averaged over " << num_tries << " tries" << std::endl;

    auto timings = std::vector<int>{};
    timings.reserve(num_tries);
    for (int c = 0; c < num_tries; c++) {
        const auto column1 = init_column_names<VarSynonym>(num_cols, "v");
        auto table1 = Table{column1};
        for (int i = 0; i < num_rows; i++) {
            table1.add_row(init_row(num_cols, "x"));
        }
        // print(table1);

        const auto column2 = init_column_names<ConstSynonym>(num_cols, "c");
        auto table2 = Table{column2};
        for (int i = 0; i < num_rows; i++) {
            table2.add_row(init_row(num_cols, "x"));
        }
        // print(table2);

        auto begin = std::chrono::high_resolution_clock::now();
        auto result = detail::cross_join_with_conflict_checks(std::move(table1), std::move(table2));
        auto end = std::chrono::high_resolution_clock::now();

        REQUIRE(!is_empty(result));
        REQUIRE(!is_unit(result));
        const auto table = std::get<Table>(result);
        REQUIRE(table.get_column().size() == 2 * num_cols); // No overlapping synonyms
        REQUIRE(table.get_records().size() == num_rows * num_rows);

        timings.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());
    }

    auto average = std::accumulate(timings.begin(), timings.end(), 0.0) / num_tries / 1000000.0;
    std::cout << "\t[cross_join_with_conflict_checks]: " << average << "ms" << std::endl;

    timings.clear();
    for (int c = 0; c < num_tries; c++) {
        const auto column1 = init_column_names<VarSynonym>(num_cols, "v");
        auto table1 = Table{column1};
        for (int i = 0; i < num_rows; i++) {
            table1.add_row(init_row(num_cols, "x"));
        }

        const auto column2 = init_column_names<ConstSynonym>(num_cols, "c");
        auto table2 = Table{column2};
        for (int i = 0; i < num_rows; i++) {
            table2.add_row(init_row(num_cols, "x"));
        }

        auto begin = std::chrono::high_resolution_clock::now();
        auto result = detail::cross_join(std::move(table1), std::move(table2));
        auto end = std::chrono::high_resolution_clock::now();

        REQUIRE(!is_empty(result));
        REQUIRE(!is_unit(result));
        const auto table = std::get<Table>(result);
        REQUIRE(table.get_column().size() == 2 * num_cols); // No overlapping synonyms
        REQUIRE(table.get_records().size() == num_rows * num_rows);

        timings.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());
    }

    average = std::accumulate(timings.begin(), timings.end(), 0.0) / num_tries / 1000000.0;
    std::cout << "\t[cross_join]: " << average << "ms" << std::endl;

    timings.clear();
    for (int c = 0; c < num_tries; c++) {
        const auto column1 = init_column_names<VarSynonym>(num_cols, "v");
        auto table1 = Table{column1};
        for (int i = 0; i < num_rows; i++) {
            table1.add_row(init_row(num_cols, "x"));
        }

        const auto column2 = init_column_names<ConstSynonym>(num_cols, "c");
        auto table2 = Table{column2};
        for (int i = 0; i < num_rows; i++) {
            table2.add_row(init_row(num_cols, "x"));
        }

        auto begin = std::chrono::high_resolution_clock::now();
        auto result = detail::merge_join(std::move(table1), std::move(table2));
        auto end = std::chrono::high_resolution_clock::now();

        REQUIRE(!is_empty(result));
        REQUIRE(!is_unit(result));
        const auto table = std::get<Table>(result);
        REQUIRE(table.get_column().size() == 2 * num_cols); // No overlapping synonyms
        REQUIRE(table.get_records().size() == num_rows * num_rows);

        timings.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());
    }

    average = std::accumulate(timings.begin(), timings.end(), 0.0) / num_tries / 1000000.0;
    std::cout << "\t[merge_join]: " << average << "ms" << std::endl;

    timings.clear();
    for (int c = 0; c < num_tries; c++) {
        const auto column1 = init_column_names<VarSynonym>(num_cols, "v");
        auto table1 = Table{column1};
        for (int i = 0; i < num_rows; i++) {
            table1.add_row(init_row(num_cols, "x"));
        }

        const auto column2 = init_column_names<ConstSynonym>(num_cols, "c");
        auto table2 = Table{column2};
        for (int i = 0; i < num_rows; i++) {
            table2.add_row(init_row(num_cols, "x"));
        }

        auto begin = std::chrono::high_resolution_clock::now();
        auto result = detail::cross_merge_join(std::move(table1), std::move(table2));
        auto end = std::chrono::high_resolution_clock::now();

        REQUIRE(!is_empty(result));
        REQUIRE(!is_unit(result));
        const auto table = std::get<Table>(result);
        REQUIRE(table.get_column().size() == 2 * num_cols); // No overlapping synonyms
        REQUIRE(table.get_records().size() == num_rows * num_rows);

        timings.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());
    }

    average = std::accumulate(timings.begin(), timings.end(), 0.0) / num_tries / 1000000.0;
    std::cout << "\t[cross_merge_join]: " << average << "ms" << std::endl;

    std::cout << std::endl;
}

TEST_CASE("BENCHMARK - Inner Join") {
    constexpr int num_rows = 100;  // Suppose at most 100 rows
    constexpr int num_cols = 20;   // Suppose at most 20 synonyms
    constexpr int num_overlap = 2; // Suppose at most 10 overlapping synonyms
    constexpr int num_tries = 50;

    std::cout << "Benchmarking inner join of 2 tables with " << num_rows << " rows, " << num_cols << " columns, and "
              << num_overlap << " overlapping synonyms, averaged over " << num_tries << " tries " << std::endl;

    auto timings = std::vector<int>{};
    timings.reserve(num_tries);
    for (int c = 0; c < num_tries; c++) {
        auto column1 = init_column_names<ConstSynonym>(num_cols, "c");
        auto column2 = init_column_names<VarSynonym>(num_cols, "v");
        auto column3 = init_column_names<AssignSynonym>(num_overlap);

        auto common_values = std::vector<std::string>{};
        for (int i = 0; i < 5; i++) {
            common_values.push_back("x" + std::to_string(i));
        }

        column1.insert(column1.end(), column3.begin(), column3.end());
        column2.insert(column2.end(), column3.begin(), column3.end());

        auto table1 = Table{column1};
        for (int i = 0; i < num_rows; i++) {
            auto row = init_row(num_cols, "x");

            if (i % 2 == 0) {
                // Fill overlapping synonyms with common values
                for (int j = 0; j < num_overlap; j++) {
                    row.push_back(common_values[j]);
                }
            } else {
                // Fill overlapping synonyms with random values
                for (int j = 0; j < num_overlap; j++) {
                    row.push_back("y" + std::to_string(std::rand() % 10000));
                }
            }
            table1.add_row(row);
        }

        auto table2 = Table{column2};
        for (int i = 0; i < num_rows; i++) {
            auto row = init_row(num_cols, "x");

            if (i % 2 == 0) {
                // Fill overlapping synonyms with common values
                for (int j = 0; j < num_overlap; j++) {
                    row.push_back(common_values[j]);
                }
            } else {
                // Fill overlapping synonyms with random values
                for (int j = 0; j < num_overlap; j++) {
                    row.push_back("y" + std::to_string(std::rand() % 10000));
                }
            }
            table2.add_row(row);
        }

        auto begin = std::chrono::high_resolution_clock::now();
        auto result = detail::cross_join_with_conflict_checks(std::move(table1), std::move(table2));
        auto end = std::chrono::high_resolution_clock::now();

        REQUIRE(!is_empty(result));
        REQUIRE(!is_unit(result));
        const auto table = std::get<Table>(result);
        REQUIRE(table.get_column().size() == 2 * num_cols + num_overlap); // No overlapping synonyms
        REQUIRE(table.get_records().size() == num_rows * num_rows / 4);

        timings.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());
    }

    auto average = std::accumulate(timings.begin(), timings.end(), 0.0) / num_tries / 1000000.0;
    std::cout << "\t[cross_join_with_conflict_checks]: " << average << "ms" << std::endl;

    timings.clear();
    for (int c = 0; c < num_tries; c++) {
        auto column1 = init_column_names<ConstSynonym>(num_cols, "c");
        auto column2 = init_column_names<VarSynonym>(num_cols, "v");
        auto column3 = init_column_names<AssignSynonym>(num_overlap);

        auto common_values = std::vector<std::string>{};
        for (int i = 0; i < 5; i++) {
            common_values.push_back("x" + std::to_string(i));
        }

        column1.insert(column1.end(), column3.begin(), column3.end());
        column2.insert(column2.end(), column3.begin(), column3.end());

        auto table1 = Table{column1};
        for (int i = 0; i < num_rows; i++) {
            auto row = init_row(num_cols, "x");

            if (i % 2 == 0) {
                // Fill overlapping synonyms with common values
                for (int j = 0; j < num_overlap; j++) {
                    row.push_back(common_values[j]);
                }
            } else {
                // Fill overlapping synonyms with random values
                for (int j = 0; j < num_overlap; j++) {
                    row.push_back("y" + std::to_string(std::rand() % 10000));
                }
            }
            table1.add_row(row);
        }

        auto table2 = Table{column2};
        for (int i = 0; i < num_rows; i++) {
            auto row = init_row(num_cols, "x");

            if (i % 2 == 0) {
                // Fill overlapping synonyms with common values
                for (int j = 0; j < num_overlap; j++) {
                    row.push_back(common_values[j]);
                }
            } else {
                // Fill overlapping synonyms with random values
                for (int j = 0; j < num_overlap; j++) {
                    row.push_back("y" + std::to_string(std::rand() % 10000));
                }
            }
            table2.add_row(row);
        }

        auto begin = std::chrono::high_resolution_clock::now();
        auto result = detail::merge_join(std::move(table1), std::move(table2));
        auto end = std::chrono::high_resolution_clock::now();

        REQUIRE(!is_empty(result));
        REQUIRE(!is_unit(result));
        const auto table = std::get<Table>(result);
        REQUIRE(table.get_column().size() == 2 * num_cols + num_overlap); // No overlapping synonyms
        REQUIRE(table.get_records().size() == num_rows * num_rows / 4);

        timings.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());
    }

    average = std::accumulate(timings.begin(), timings.end(), 0.0) / num_tries / 1000000.0;
    std::cout << "\t[merge_join]: " << average << "ms" << std::endl;

    timings.clear();
    for (int c = 0; c < num_tries; c++) {
        auto column1 = init_column_names<ConstSynonym>(num_cols, "c");
        auto column2 = init_column_names<VarSynonym>(num_cols, "v");
        auto column3 = init_column_names<AssignSynonym>(num_overlap);

        auto common_values = std::vector<std::string>{};
        for (int i = 0; i < 5; i++) {
            common_values.push_back("x" + std::to_string(i));
        }

        column1.insert(column1.end(), column3.begin(), column3.end());
        column2.insert(column2.end(), column3.begin(), column3.end());

        auto table1 = Table{column1};
        for (int i = 0; i < num_rows; i++) {
            auto row = init_row(num_cols, "x");

            if (i % 2 == 0) {
                // Fill overlapping synonyms with common values
                for (int j = 0; j < num_overlap; j++) {
                    row.push_back(common_values[j]);
                }
            } else {
                // Fill overlapping synonyms with random values
                for (int j = 0; j < num_overlap; j++) {
                    row.push_back("y" + std::to_string(std::rand() % 10000));
                }
            }
            table1.add_row(row);
        }

        auto table2 = Table{column2};
        for (int i = 0; i < num_rows; i++) {
            auto row = init_row(num_cols, "x");

            if (i % 2 == 0) {
                // Fill overlapping synonyms with common values
                for (int j = 0; j < num_overlap; j++) {
                    row.push_back(common_values[j]);
                }
            } else {
                // Fill overlapping synonyms with random values
                for (int j = 0; j < num_overlap; j++) {
                    row.push_back("y" + std::to_string(std::rand() % 10000));
                }
            }
            table2.add_row(row);
        }

        auto begin = std::chrono::high_resolution_clock::now();
        auto result = detail::cross_merge_join(std::move(table1), std::move(table2));
        auto end = std::chrono::high_resolution_clock::now();

        REQUIRE(!is_empty(result));
        REQUIRE(!is_unit(result));
        const auto table = std::get<Table>(result);
        REQUIRE(table.get_column().size() == 2 * num_cols + num_overlap); // No overlapping synonyms
        REQUIRE(table.get_records().size() == num_rows * num_rows / 4);

        timings.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());
    }

    average = std::accumulate(timings.begin(), timings.end(), 0.0) / num_tries / 1000000.0;
    std::cout << "\t[cross_merge_join]: " << average << "ms" << std::endl;
}
#endif
