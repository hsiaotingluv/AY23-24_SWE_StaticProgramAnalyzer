#include "TestWrapper.h"
#include "qps/parser/errors.hpp"
#include "qps/template_utils.hpp"
#include "sp/main.hpp"

#include "qps/evaluators/query_evaluator.hpp"

#include <filesystem>
#include <fstream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <variant>

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = nullptr;

auto WrapperFactory::createWrapper() -> AbstractWrapper* {
    if (wrapper == nullptr) {
        wrapper = new TestWrapper;
    }
    return wrapper;
}

// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

TestWrapper::TestWrapper()
    : source_processor(nullptr), read_facade(nullptr), write_facade(nullptr), qps_parser(nullptr),
      qps_evaluator(nullptr) {

    auto [read_facade, write_facade] = pkb::PKB::create_facades();

    source_processor = sp::SourceProcessor::get_complete_sp(write_facade);
    qps_parser = std::make_shared<qps::DefaultParser>();
    qps_evaluator = std::make_shared<qps::QueryEvaluator>(read_facade);
}

auto TestWrapper::load_file(const std::string& filename) -> std::string {
    auto path = std::filesystem::path{filename};
    if (!std::filesystem::exists(path)) {
        throw std::runtime_error("Error: File does not exist");
    }

    std::ifstream file{path};

    if (!file.is_open()) {
        throw std::runtime_error("Error: Unable to open file");
    }

    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

void TestWrapper::parse(std::string filename) {
    auto input = load_file(filename);
    auto ast = source_processor->process(input);
}

void TestWrapper::evaluate(std::string query, std::list<std::string>& results) {
    const auto output = qps_parser->parse(query);
    const auto maybe_query_obj =
        std::visit(qps::overloaded{[&results](const qps::SyntaxError& e) -> std::optional<qps::Query> {
                                       results.emplace_back("SyntaxError");
                                       return std::nullopt;
                                   },
                                   [&results](const qps::SemanticError& e) -> std::optional<qps::Query> {
                                       results.emplace_back("SemanticError");
                                       return std::nullopt;
                                   },
                                   [&results, this](const qps::Query& query_obj) -> std::optional<qps::Query> {
                                       return std::make_optional(query_obj);
                                   }},
                   output);
    if (!maybe_query_obj.has_value()) {
        return;
    }

    const auto query_obj = maybe_query_obj.value();
    const auto query_results = qps_evaluator->evaluate(query_obj);
    for (const auto& result : query_results) {
        results.emplace_back(result);
    }
}
