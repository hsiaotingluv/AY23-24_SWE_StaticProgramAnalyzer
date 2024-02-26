#pragma once

#include "pkb/abstract_stores/many_to_many_store.h"
#include "pkb/common_types/statement_number.h"

#include <string>

class FollowsStarStore : public ManyToManyStore<StatementNumber, StatementNumber> {};