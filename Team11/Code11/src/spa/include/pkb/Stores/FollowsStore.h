#pragma once

#include "pkb/CommonTypes.h"

class FollowsStore {
  public:
    typedef CommonTypes::STATEMENT_NUMBER StatementNumber;

    FollowsStore();

    ~FollowsStore();

    void AddFollowsRelation(const StatementNumber& stmt1, const StatementNumber& stmt2);

    bool HasFollowsRelation(const StatementNumber& stmt1, const StatementNumber& stmt2);
};
