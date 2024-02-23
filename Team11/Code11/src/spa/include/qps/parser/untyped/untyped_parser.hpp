#pragma once

#include "common/tokeniser/runner.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/parser/untyped/entities/clause.hpp"
#include "qps/parser/untyped/entities/query.hpp"
#include "qps/parser/untyped/entities/synonym.hpp"

#include "qps/parser/parser_helper.hpp"
#include "qps/template_utils.hpp"
#include "qps/tokeniser/tokeniser.hpp"

namespace qps::untyped {
class UntypedParser {
  private:
    static inline const auto tokeniser_runner =
        tokenizer::TokenizerRunner{std::make_unique<QueryProcessingSystemTokenizer>()};

  public:
    static auto parse(std::string query) -> std::optional<std::tuple<Synonyms, untyped::UntypedQuery>>;
};

class SelectSynonymParser {
    static constexpr auto keywords = std::array<std::string_view, 1>{"Select"};

  public:
    static auto parse(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
        -> std::optional<std::tuple<UntypedSynonym, std::vector<Token>::const_iterator>>;
};

using SupportedSelectParsers = TypeList<SelectSynonymParser>;

// design-entity : 'stmt' | 'read' | 'print' | 'while' | 'if' | 'assign' | 'call' | 'variable' | 'constant' |
// 'procedure'

using SupportedSynonyms = TypeList<AnyStmtSynonym, ReadSynonym, PrintSynonym, WhileSynonym, IfSynonym, AssignSynonym,
                                   CallSynonym, VarSynonym, ConstSynonym, ProcSynonym>;

class SuchThatClausesParser {
    static constexpr auto keywords = std::array<std::string_view, 2>{"such", "that"};

  public:
    static auto parse(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
        -> std::optional<std::tuple<UntypedSuchThatClause, std::vector<Token>::const_iterator>>;
};

class PatternClausesParser {
    static constexpr auto keywords = std::array<std::string_view, 1>{"pattern"};

  public:
    static auto parse(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
        -> std::optional<std::tuple<UntypedPatternClause, std::vector<Token>::const_iterator>>;
};

using SupportedClauseParsers = TypeList<SuchThatClausesParser, PatternClausesParser>;

} // namespace qps::untyped

namespace qps::untyped::detail {

auto build_untyped_query(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<UntypedQuery>;

auto parse_declarations(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<Synonyms, std::vector<Token>::const_iterator>>;

auto parse_references(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedReference, std::vector<Token>::const_iterator>>;

auto parse_synonym(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedSynonym, std::vector<Token>::const_iterator>>;

auto parse_ent_ref(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedEntRef, std::vector<Token>::const_iterator>>;

auto parse_stmt_ref(const Token& token) -> UntypedStmtRef;

auto parse_rel_ref(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<UntypedRelationship, std::vector<Token>::const_iterator>>;

template <unsigned long N>
auto parse_keywords(std::array<std::string_view, N> keywords, std::vector<Token>::const_iterator it,
                    const std::vector<Token>::const_iterator& end)
    -> std::optional<std::vector<Token>::const_iterator> {
    if (N == 0) {
        // Always successful in consuming nothing
        return it;
    }

    if (it == end) {
        // Cannot consume anything if there are no tokens
        return std::nullopt;
    }

    for (const auto& keyword : keywords) {
        if (it == end || !is_keyword(*it, std::string{keyword})) {
            return std::nullopt;
        }
        it = std::next(it);
    }

    return it;
}
} // namespace qps::untyped::detail