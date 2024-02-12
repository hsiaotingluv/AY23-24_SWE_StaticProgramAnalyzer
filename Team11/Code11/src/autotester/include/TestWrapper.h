#pragma once

#include <list>
#include <string>

#include "AbstractWrapper.h"
#include "sp/main.hpp"

#include "PKB/Facades/ReadFacade.h"
#include "PKB/Facades/WriteFacade.h"
#include "PKB/PKB.h"

#include "qps/parser/parser.hpp"

class TestWrapper : public AbstractWrapper {
  private:
    std::shared_ptr<sp::SourceProcessor> source_processor;

    std::shared_ptr<ReadFacade> readFacade;
    std::shared_ptr<WriteFacade> writeFacade;

    std::shared_ptr<qps::QueryProcessingSystemParser> qps_parser;

    auto load_file(const std::string& filename) -> std::string;

  public:
    TestWrapper();

    // method for parsing the SIMPLE source
    void parse(std::string filename) override;

    // method for evaluating a query
    void evaluate(std::string query, std::list<std::string>& results) override;
};
