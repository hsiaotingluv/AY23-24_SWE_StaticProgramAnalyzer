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
    std::vector<std::shared_ptr<Clause>> clauses;

    Query(Synonyms declared, Synonym reference, std::vector<std::shared_ptr<Clause>> clauses)
        : declared(std::move(declared)), reference(std::move(reference)), clauses(std::move(clauses)) {
    }

    auto operator<<(std::ostream& os) -> std::ostream& {
        os << "Query:\n";
        os << "\tDeclared:\n";
        for (const auto& declared : declared) {
            os << "\t\t" << declared << "\n";
        }
        os << "\tReference:\n";
        os << "\t\t" << reference << "\n";
        os << "\tClauses:\n";
        for (const auto& clause : clauses) {
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

auto parse_declarations(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<Synonyms, std::vector<Token>::const_iterator>>;
auto parse_reference(const Synonyms& declared, std::vector<Token>::const_iterator it,
                     const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<Synonym, std::vector<Token>::const_iterator>>;
auto parse_such_that_clause(const Synonyms& declared, std::vector<Token>::const_iterator it,
                            const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<std::shared_ptr<SuchThatClause>, std::vector<Token>::const_iterator>>;
auto parse_pattern_clause(const Synonyms& declared, std::vector<Token>::const_iterator it,
                          const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<std::shared_ptr<PatternClause>, std::vector<Token>::const_iterator>>;

template <typename... T>
struct TypeList {};

using SupportedSynonyms = TypeList<ProcSynonym, VarSynonym, ConstSynonym, RawStmtSynonym, ReadSynonym, CallSynonym,
                                   WhileSynonym, IfSynonym, AssignSynonym, PrintSynonym>;
} // namespace qps
