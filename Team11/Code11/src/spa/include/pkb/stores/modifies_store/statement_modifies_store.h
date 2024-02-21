#pragma once

#include "pkb/abstraction/many_to_many_store.h"
#include "pkb/common_types/variable.h"

using StatementNumber = std::string;

class StatementModifiesStore : public ManyToManyStore<StatementNumber, Variable> {};