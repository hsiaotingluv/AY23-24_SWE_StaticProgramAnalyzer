#pragma once

#include "pkb/abstraction/many_to_many_store.h"
#include "pkb/common_types/procedure.h"
#include "pkb/common_types/variable.h"

class ProcedureModifiesStore : public ManyToManyStore<Procedure, Variable> {};