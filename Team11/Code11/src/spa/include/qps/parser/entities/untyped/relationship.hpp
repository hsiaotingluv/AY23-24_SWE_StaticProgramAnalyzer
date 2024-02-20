#pragma once

#include "qps/parser/entities/untyped/synonym.hpp"
#include <tuple>

namespace qps::untyped {

using UntypedStmtStmtRel = std::tuple<std::string, UntypedStmtRef, UntypedStmtRef>;
using UntypedStmtEntRel = std::tuple<std::string, UntypedStmtRef, UntypedEntRef>;
using UntypedEntEntRel = std::tuple<std::string, UntypedEntRef, UntypedEntRef>;

using UntypedRelationship = std::variant<UntypedStmtStmtRel, UntypedStmtEntRel, UntypedEntEntRel>;
} // namespace qps::untyped
