#pragma once

#include "pkb/abstraction/many_to_many_store.h"
#include "pkb/common_types/statement_number.h"
#include "pkb/common_types/variable.h"

class StatementUsesStore : public ManyToManyStore<StatementNumber, Variable> {};