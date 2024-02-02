#pragma once

#include <list>
#include <string>

// include your other headers here
#include "AbstractWrapper.h"

class TestWrapper : public AbstractWrapper {
  public:
    // default constructor
    TestWrapper() = default;

    // method for parsing the SIMPLE source
     void parse(std::string filename) override;

    // method for evaluating a query
     void evaluate(std::string query, std::list<std::string> &results) override;
};
