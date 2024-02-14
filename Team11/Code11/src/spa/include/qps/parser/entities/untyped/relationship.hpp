#pragma once

#include "qps/parser/entities/untyped/synonym.hpp"
#include <tuple>

namespace qps::untyped {

#ifdef MILESTONE1
using AnyStmtSynonymtmtRel = std::tuple<std::string, UntypedStmtRef, UntypedStmtRef>;
using UntypedStmtEntRel = std::tuple<std::string, UntypedStmtRef, UntypedEntRef>;
using UntypedRelationship = std::variant<AnyStmtSynonymtmtRel, UntypedStmtEntRel>;
#else
using AnyStmtSynonymtmtRel = std::tuple<std::string, UntypedStmtRef, UntypedStmtRef>;
using UntypedStmtEntRel = std::tuple<std::string, UntypedStmtRef, UntypedEntRef>;
using UntypedEntEntRel = std::tuple<std::string, UntypedEntRef, UntypedEntRef>;

using UntypedRelationship = std::variant<AnyStmtSynonymtmtRel, UntypedStmtEntRel, UntypedEntEntRel>;
#endif

} // namespace qps::untyped
