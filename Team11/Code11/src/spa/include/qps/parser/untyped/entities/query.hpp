#pragma once

#include "qps/parser/untyped/entities/clause.hpp"
#include "qps/parser/untyped/entities/synonym.hpp"

namespace qps::untyped {
using UntypedReference = UntypedSynonym;
using UntypedQuery = std::tuple<UntypedReference, std::vector<UntypedClause>>;
} // namespace qps::untyped