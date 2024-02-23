#pragma once

#include "qps/parser/untyped/entities/synonym.hpp"
#include <tuple>

namespace qps::untyped {

using UntypedStmtStmtRel = std::tuple<std::string, UntypedStmtRef, UntypedStmtRef>;
using UntypedRefEntRel = std::tuple<std::string, UntypedRef, UntypedEntRef>;

using UntypedRelationship = std::variant<UntypedStmtStmtRel, UntypedRefEntRel>;
} // namespace qps::untyped
