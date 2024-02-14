#pragma once

#include <list>
#include <memory>
#include <string>

#include "AbstractWrapper.h"
#include "qps/evaluators/simple_evaluator.hpp"
#include "sp/main.hpp"

#include "pkb/facades/read_facade.h"
#include "pkb/facades/write_facade.h"
#include "pkb/pkb.h"

#include "qps/qps.hpp"

class TestWrapper : public AbstractWrapper {
  private:
    std::shared_ptr<sp::SourceProcessor> source_processor;

    std::shared_ptr<ReadFacade> read_facade;
    std::shared_ptr<WriteFacade> write_facade;

    std::shared_ptr<qps::QueryProcessingSystem> qps_parser;
    std::shared_ptr<qps::Evaluator> qps_evaluator;

    auto load_file(const std::string& filename) -> std::string;

  public:
    TestWrapper();

    // method for parsing the SIMPLE source
    void parse(std::string filename) override;

    // method for evaluating a query
    void evaluate(std::string query, std::list<std::string>& results) override;
};
