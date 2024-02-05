#pragma once

#include "PkbTypes.h"

class ParentStore {
    public:
        typedef PkbTypes::STATEMENT_NUMBER StatementNumber;

    ParentStore();

    ~ParentStore();

    void AddParentRelation(const StatementNumber &stmt_1, const StatementNumber &stmt_2);

    bool HasParentRelation(const StatementNumber &stmt_1, const StatementNumber &stmt_2);

};

