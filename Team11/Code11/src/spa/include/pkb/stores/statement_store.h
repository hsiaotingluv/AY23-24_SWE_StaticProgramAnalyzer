#pragma once

#include <string>

#include "common/statement_type.hpp"
#include "pkb/abstraction/many_to_one_store.h"

using StatementNumber = std::string;

class StatementStore : public ManyToOneStore<StatementNumber, StatementType> {};