#pragma once

#include "pkb/abstract_stores/many_to_many_store.h"
#include "pkb/common_types/procedure.h"

class CallsStore : public ManyToManyStore<Procedure, Procedure> {};