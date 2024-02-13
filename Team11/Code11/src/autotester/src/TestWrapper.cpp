#include "TestWrapper.h"
#include "sp/main.hpp"

#include "qps/evaluators/simple_evaluator.hpp"

#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = nullptr;

auto WrapperFactory::createWrapper() -> AbstractWrapper* {
    if (wrapper == nullptr)
        wrapper = new TestWrapper;
    return wrapper;
}

// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

TestWrapper::TestWrapper()
    : source_processor(nullptr), readFacade(nullptr), writeFacade(nullptr), qps_parser(nullptr),
      qps_evaluator(nullptr) {

    auto [readFacade, writeFacade] = PKB::create_facades();

    source_processor = sp::SourceProcessor::get_complete_sp(writeFacade);
    qps_parser = std::make_shared<qps::QueryProcessingSystemParser>();
    qps_evaluator = std::make_shared<qps::Evaluator>(readFacade);
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
    const auto maybe_query_obj = qps_parser->parse(query);
    if (!maybe_query_obj) {
        results.emplace_back("Error: Invalid query");
        return;
    }

    const auto query_obj = maybe_query_obj.value();
    const auto query_results = qps_evaluator->evaluate(query_obj);
    for (const auto& result : query_results) {
        results.emplace_back(result);
    }
}
