#include "AbstractWrapper.h"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <numeric>
#include <vector>

auto message() -> std::string {
    return "Usage: local_runner <source_path> <query_path>";
}

struct Query {
    int query_id;
    std::string comment;
    std::string declarations;
    std::string query;
    std::string expected_result;
    int time_limit_ms;
};

auto read_query_file(const std::string& filename) -> std::vector<Query> {
    auto path = std::filesystem::path{filename};
    if (!std::filesystem::exists(path)) {
        throw std::runtime_error("Error: File does not exist");
    }

    std::ifstream file{path};

    if (!file.is_open()) {
        throw std::runtime_error("Error: Unable to open file");
    }

    // Read line by line
    std::string line;
    auto counter = 0;
    auto query = Query{};
    auto query_objects = std::vector<Query>{};

    while (std::getline(file, line)) {
        if (counter == 0) {
            query.query_id = std::stoi(line.substr(0, line.find(" ")));
            query.comment = line.substr(line.find("-") + 1);
        } else if (counter == 1) {
            query.declarations = line;
        } else if (counter == 2) {
            query.query = line;
        } else if (counter == 3) {
            query.expected_result = line;
        } else if (counter == 4) {
            query.time_limit_ms = std::stoi(line);

            query_objects.push_back(query);
            query = Query{};
        }

        counter = ++counter % 5;
    }

    return query_objects;
}

auto measure_parse(const std::unique_ptr<AbstractWrapper>& wrapper, const std::string& filename, int num_repeat = 1000)
    -> void {

    // First run
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    wrapper->parse(filename);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    auto first_run_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

    // Averaged runs
    auto measured_times_ms = std::vector<long>(num_repeat, 0);
    for (int i = 0; i < num_repeat; i++) {
        begin = std::chrono::steady_clock::now();
        wrapper->parse(filename);
        end = std::chrono::steady_clock::now();
        measured_times_ms[i] = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    }
    const auto measured_time_ms = std::accumulate(measured_times_ms.begin(), measured_times_ms.end(), 0.0) / num_repeat;

    std::cout << "First run: " << first_run_ms << "[ms]" << std::endl;
    std::cout << "Average run: " << measured_time_ms << "[ms]" << std::endl;
}

void measure_evaluation(const std::vector<Query>& query_objects, const std::unique_ptr<AbstractWrapper>& wrapper) {
    std::list<std::string> results;

    auto measured_times_ms = std::vector<long>{};
    measured_times_ms.reserve(query_objects.size());

    for (auto& obj : query_objects) {
        auto query_str = obj.declarations + obj.query;
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        wrapper->evaluate(query_str, results);
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        measured_times_ms.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());

        std::cout << "Query: " << obj.query_id << " - " << obj.comment << std::endl;
        std::cout << "Expected: " << obj.expected_result << std::endl;
        std::cout << "Result: ";
        for (const auto& result : results) {
            std::cout << result << " ";
        }
        std::cout << std::endl;
        std::cout << measured_times_ms.back() << "[ms]" << std::endl << std::endl;

        results.clear();
    }

    auto measured_time_ms =
        std::accumulate(measured_times_ms.begin(), measured_times_ms.end(), 0.0) / measured_times_ms.size();
    auto max_time = *std::max_element(measured_times_ms.begin(), measured_times_ms.end());
    auto min_time = *std::min_element(measured_times_ms.begin(), measured_times_ms.end());

    std::cout << "Average run: " << measured_time_ms << "[ms]" << std::endl;
    std::cout << "Max run: " << max_time << "[ms]" << std::endl;
    std::cout << "Min run: " << min_time << "[ms]" << std::endl;
}

auto main(int argc, char** argv) -> int {
    if (argc != 3) {
        std::cerr << message() << std::endl;
        return 1;
    }

    auto wrapper = std::unique_ptr<AbstractWrapper>(WrapperFactory::createWrapper());

    std::cout << "Parsing source file..." << std::endl;
    measure_parse(wrapper, argv[1], 0);
    std::cout << std::endl;

    std::cout << "Evaluating queries..." << std::endl;
    const auto objs = read_query_file(argv[2]);
    measure_evaluation(objs, wrapper);
}