#pragma once

#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include "common/tokeniser/runner.hpp"
#include "common/tokeniser/tokenizer.hpp"

#include "qps/parser/entities/clause.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/tokeniser/tokeniser.hpp"

namespace qps {
using Token = tokenizer::Token;
using ParserInput = std::vector<tokenizer::Token>;

template <typename T>
struct ParserSuccess {
    T value;
    ParserInput rest;
};

struct Query {
    Synonyms declared;
    Synonym reference;
    Clauses clauses;

    Query(Synonyms declared, Synonym reference, Clauses clauses)
        : declared(std::move(declared)), reference(std::move(reference)), clauses(std::move(clauses)) {
    }

    friend auto operator<<(std::ostream& os, const Query& query) -> std::ostream& {
        os << "Query:\n";
        os << "\tDeclared:\n";
        for (const auto& declared : query.declared) {
            os << "\t\t" << declared << "\n";
        }
        os << "\tReference:\n";
        os << "\t\t" << query.reference << "\n";
        os << "\tClauses:\n";
        for (const auto& clause : query.clauses) {
            os << "\t\t" << clause << "\n";
        }
        return os;
    };
};

class QueryProcessingSystemParser {
  private:
    static inline const auto tokeniser_runner =
        tokenizer::TokenizerRunner{std::make_unique<QueryProcessingSystemTokenizer>()};

  public:
    static auto parse(std::string query) -> std::optional<Query>;
};

auto parse_declarations(std::vector<Token>::iterator it, const std::vector<Token>::iterator& end)
    -> std::optional<std::tuple<Synonyms, std::vector<Token>::iterator>>;
auto parse_reference(const Synonyms& declared, std::vector<Token>::iterator it, const std::vector<Token>::iterator& end)
    -> std::optional<std::tuple<Synonym, std::vector<Token>::iterator>>;
auto parse_such_that_clause(const Synonyms& declared, std::vector<Token>::iterator it,
                            const std::vector<Token>::iterator& end)
    -> std::optional<std::tuple<SuchThatClause, std::vector<Token>::iterator>>;
auto parse_pattern_clause(const Synonyms& declared, std::vector<Token>::iterator it,
                          const std::vector<Token>::iterator& end)
    -> std::optional<std::tuple<PatternClause, std::vector<Token>::iterator>>;

template <typename... T>
struct TypeList {};

using SupportedSynonyms = TypeList<ProcSynonym, VarSynonym, ConstSynonym, RawStmtSynonym, ReadSynonym, CallSynonym,
                                   WhileSynonym, IfSynonym, AssignSynonym, PrintSynonym>;
} // namespace qps
