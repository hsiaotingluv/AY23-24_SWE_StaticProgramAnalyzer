#pragma once

#include "pkb/abstract_stores/one_to_many_store.h"
#include "pkb/common_types/statement_number.h"
#include "pkb/common_types/variable.h"

#include <string>

class IfVarStore : public ManyToManyStore<Variable, StatementNumber> {};