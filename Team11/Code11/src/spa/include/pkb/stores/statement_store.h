#pragma once

#include <string>

#include "common/statement_type.hpp"
#include "pkb/abstraction/many_to_one_store.h"

class StatementStore : public ManyToOneStore<StatementNumber, StatementType> {};