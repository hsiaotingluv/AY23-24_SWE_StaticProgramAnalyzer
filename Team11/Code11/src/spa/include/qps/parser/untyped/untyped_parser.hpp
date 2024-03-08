#pragma once

#include "qps/parser/entities/relationship.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/parser/untyped/clauses_parser.tpp"
#include "qps/parser/untyped/parser_strategies.hpp"
#include "qps/parser/untyped/result_cl_parser.hpp"
#include "qps/parser/untyped/untyped_parser.tpp"
#include "qps/template_utils.hpp"

namespace qps::untyped {
using DefaultSupportedSynonyms = TypeList<AnyStmtSynonym, ReadSynonym, PrintSynonym, WhileSynonym, IfSynonym,
                                          AssignSynonym, CallSynonym, VarSynonym, ConstSynonym, ProcSynonym>;

using DefaultSupportedStmtStmtRelationships = DefaultStmtStmtList;
using DefaultSupportedRefEntRelationships = concat_t<DefaultStmtEntList, DefaultEntEntList>;

using DefaultSuchThatClausesParser =
    ClausesParser<SuchThatParserStrategy<DefaultSupportedStmtStmtRelationships, DefaultSupportedRefEntRelationships>>;
using DefaultPatternClausesParser = ClausesParser<PatternParserStrategy>;
using DefaultWithClausesParser = ClausesParser<WithParserStrategy>;

using DefaultSupportedClauseParsers =
    TypeList<DefaultSuchThatClausesParser, DefaultPatternClausesParser, DefaultWithClausesParser>;

using DefaultUntypedParser =
    UntypedParser<DefaultSupportedSynonyms, DefaultSupportedSelectParsers, DefaultSupportedClauseParsers>;

} // namespace qps::untyped