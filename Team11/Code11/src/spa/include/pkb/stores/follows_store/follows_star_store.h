#pragma once

#include "pkb/abstraction/many_to_many_store.h"

#include <string>

using StatementNumber = std::string;

class FollowsStarStore : public ManyToManyStore<StatementNumber, StatementNumber> {};