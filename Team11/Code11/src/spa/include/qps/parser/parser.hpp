#pragma once

#include "common/tokeniser/runner.hpp"
#include "qps/parser/entities/untyped/clause.hpp"
#include "qps/parser/entities/untyped/synonym.hpp"

#include "qps/parser/parser_helper.hpp"
#include "qps/template_utils.hpp"
#include "qps/tokeniser/tokeniser.hpp"

namespace qps::untyped {
using UntypedReference = UntypedSynonym;
using UntypedQuery = std::tuple<UntypedReference, std::vector<UntypedClause>>;

auto parse(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<UntypedQuery>;

auto parse_references(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedReference, std::vector<Token>::const_iterator>>;

auto parse_such_that_clause(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedSuchThatClause, std::vector<Token>::const_iterator>>;

auto parse_pattern_clause(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedPatternClause, std::vector<Token>::const_iterator>>;
} // namespace qps::untyped

namespace qps {
class QPSParser {
  private:
    static inline const auto tokeniser_runner =
        tokenizer::TokenizerRunner{std::make_unique<QueryProcessingSystemTokenizer>()};

  public:
    static auto parse(std::string query) -> std::optional<std::tuple<Synonyms, untyped::UntypedQuery>>;
};

#ifndef MILESTONE1
using SupportedSynonyms = TypeList<ProcSynonym, VarSynonym, ConstSynonym, AnyStmtSynonym, ReadSynonym, CallSynonym,
                                   WhileSynonym, IfSynonym, AssignSynonym, PrintSynonym>;
#else
// design-entity : 'stmt' | 'read' | 'print' | 'while' | 'if' | 'assign' | 'variable' | 'constant' | 'procedure'
using SupportedSynonyms = TypeList<AnyStmtSynonym, ReadSynonym, PrintSynonym, WhileSynonym, IfSynonym, AssignSynonym,
                                   VarSynonym, ConstSynonym, ProcSynonym>;
#endif

auto parse_declarations(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<Synonyms, std::vector<Token>::const_iterator>>;

} // namespace qps
