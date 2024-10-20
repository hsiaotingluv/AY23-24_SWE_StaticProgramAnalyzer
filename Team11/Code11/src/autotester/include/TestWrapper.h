#pragma once

#include <list>
#include <memory>
#include <string>

#include "AbstractWrapper.h"
#include "qps/evaluators/query_evaluator.hpp"
#include "sp/main.hpp"

#include "pkb/facades/read_facade.h"
#include "pkb/facades/write_facade.h"
#include "pkb/pkb_manager.h"

#include "qps/parser.hpp"

class TestWrapper : public AbstractWrapper {
  private:
    std::shared_ptr<sp::SourceProcessor> source_processor;

    std::shared_ptr<pkb::ReadFacade> read_facade;
    std::shared_ptr<pkb::WriteFacade> write_facade;

    std::shared_ptr<qps::DefaultParser> qps_parser;
    std::shared_ptr<qps::QueryEvaluator> qps_evaluator;

    auto load_file(const std::string& filename) -> std::string;

  public:
    TestWrapper();

    // method for parsing the SIMPLE source
    void parse(std::string filename) override;

    // method for evaluating a query
    void evaluate(std::string query, std::list<std::string>& results) override;
};
