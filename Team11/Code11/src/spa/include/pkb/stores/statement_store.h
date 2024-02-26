#pragma once

#include "common/statement_type.hpp"
#include "pkb/abstract_stores/many_to_one_store.h"
#include "pkb/common_types/statement_number.h"

class StatementStore : public ManyToOneStore<StatementNumber, StatementType> {};