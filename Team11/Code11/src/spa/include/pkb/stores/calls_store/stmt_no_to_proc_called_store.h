#pragma once

#include "pkb/abstract_stores/many_to_one_store.h"
#include "pkb/common_types/procedure.h"
#include "pkb/common_types/statement_number.h"

class StmtNoToProcCalledStore : public ManyToOneStore<StatementNumber, Procedure> {};