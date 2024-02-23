#pragma once

#include "pkb/abstraction/one_to_one_store.h"
#include "pkb/common_types/statement_number.h"

#include <string>

class DirectFollowsStore : public OneToOneStore<StatementNumber, StatementNumber> {};