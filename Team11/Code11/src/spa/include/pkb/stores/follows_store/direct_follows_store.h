#pragma once

#include "pkb/abstraction/one_to_one_store.h"

#include <string>

using StatementNumber = std::string;

class DirectFollowsStore : public OneToOneStore<StatementNumber, StatementNumber> {};