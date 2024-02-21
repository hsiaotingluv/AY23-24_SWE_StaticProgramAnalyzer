#pragma once

#include "pkb/abstraction/many_to_many_store.h"

#include <string>

using StatementNumber = std::string;

class ParentStarStore : public ManyToManyStore<StatementNumber, StatementNumber> {};