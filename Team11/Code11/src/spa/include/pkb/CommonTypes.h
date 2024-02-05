#pragma once

#include <string>

class CommonTypes {
  public:
    typedef std::string PROCEDURE;
    typedef std::string VARIABLE;
    typedef std::string CONSTANT;
    typedef std::string STATEMENT_NUMBER;

    typedef enum _ {
        READ, PRINT, CALL, WHILE, IF, ASSIGN
    } STATEMENT_TYPE;
};