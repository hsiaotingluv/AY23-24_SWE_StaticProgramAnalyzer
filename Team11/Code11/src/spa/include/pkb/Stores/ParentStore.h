#pragma once

#include "pkb/CommonTypes.h"

class ParentStore {
  public:
    typedef CommonTypes::STATEMENT_NUMBER StatementNumber;

    ParentStore();

    ~ParentStore();

    void AddParentRelation(const StatementNumber &stmt_1, const StatementNumber &stmt_2);

    bool HasParentRelation(const StatementNumber &stmt_1, const StatementNumber &stmt_2);
};