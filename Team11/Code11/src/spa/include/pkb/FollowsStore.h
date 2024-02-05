#pragma once

#include "PkbTypes.h"

class FollowsStore {
    public:
        typedef PkbTypes::STATEMENT_NUMBER StatementNumber;

    FollowsStore();

    ~FollowsStore();

    void AddFollowsRelation(const StatementNumber &stmt1, const StatementNumber &stmt2);

    bool HasFollowsRelation(const StatementNumber &stmt1, const StatementNumber &stmt2);
};

