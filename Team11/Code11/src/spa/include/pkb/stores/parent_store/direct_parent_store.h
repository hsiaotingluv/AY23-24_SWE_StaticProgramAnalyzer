#pragma once

#include "pkb/abstraction/one_to_many_store.h"
#include "pkb/common_types/statement_number.h"

#include <string>

class DirectParentStore : public OneToManyStore<StatementNumber, StatementNumber> {};