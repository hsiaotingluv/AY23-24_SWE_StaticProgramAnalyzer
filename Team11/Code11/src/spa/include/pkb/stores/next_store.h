#pragma once

#include "pkb/abstract_stores/many_to_many_store.h"
#include "pkb/common_types/statement_number.h"

class NextStore : public ManyToManyStore<StatementNumber, StatementNumber> {};