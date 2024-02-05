#include "pkb/Stores/FollowsStore.h"

FollowsStore::FollowsStore() = default;

FollowsStore::~FollowsStore() = default;

void FollowsStore::AddFollowsRelation(const StatementNumber &stmt1, const StatementNumber &stmt2) {
}

bool FollowsStore::HasFollowsRelation(const StatementNumber &stmt1, const StatementNumber &stmt2) {
    return true;
}
