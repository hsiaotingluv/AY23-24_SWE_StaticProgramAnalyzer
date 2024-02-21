#pragma once

#include "pkb/abstraction/one_to_many_store.h"

#include <string>

using StatementNumber = std::string;

class DirectParentStore : public OneToManyStore<StatementNumber, StatementNumber> {};