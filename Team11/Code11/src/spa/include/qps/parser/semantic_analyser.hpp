#pragma once

#include "qps/parser/entities/clause.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/parser/errors.hpp"
#include "qps/parser/pattern_analyser.hpp"
#include "qps/parser/semantic_analyser_helper.hpp"
#include "qps/parser/untyped/entities/synonym.hpp"
#include "qps/parser/untyped/untyped_parser.hpp"
#include "qps/template_utils.hpp"

#include <memory>
#include <optional>
#include <variant>
#include <vector>

namespace qps::details {
// Forward declarations
template <typename StmtStmtList, typename StmtEntList, typename EntEntList, typename PatternAnalysersList>
auto validate_clause(const Synonyms& declarations,
                     const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                     const untyped::UntypedClause& clause) -> std::optional<std::shared_ptr<Clause>>;
} // namespace qps::details

namespace qps {
struct BooleanReference {};

inline auto operator<<(std::ostream& os, const BooleanReference&) -> std::ostream& {
    os << "BOOLEAN()";
    return os;
}

using Reference = std::variant<std::shared_ptr<Synonym>, Synonyms, BooleanReference>;

inline auto operator<<(std::ostream& os, const Synonyms& reference) -> std::ostream& {
    for (const auto& synonym : reference) {
        os << synonym << " ";
    }
    return os;
}

struct Query {
    Synonyms declared;
    Reference reference;
    std::vector<std::shared_ptr<Clause>> clauses;

    Query(Synonyms declared, Reference reference, std::vector<std::shared_ptr<Clause>> clauses)
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
} // namespace qps

#include "qps/parser/semantic_analyser.tpp"

namespace qps {

template <typename StmtStmtList, typename StmtEntList, typename EntEntList, typename PatternAnalysersList,
          typename UntypedQueryType, typename VisitorGenerator>
class SemanticAnalyser {
  public:
    static auto analyse(const Synonyms& declarations, const UntypedQueryType& untyped_query)
        -> std::variant<Query, SemanticError> {
        // Declarations must be unique
        const auto& maybe_mapping = details::enforce_unique_declarations(declarations);
        if (!maybe_mapping.has_value()) {
            return SemanticError{"Non-unique mapping"};
        }
        const auto& mapping = maybe_mapping.value();

        const auto& [references, clauses] = untyped_query;

        // Reference must be declared
        const auto& maybe_reference = std::visit(VisitorGenerator{}(mapping, declarations), references);
        if (!maybe_reference.has_value()) {
            return SemanticError{"Undeclared reference"};
        }

        const auto& reference = maybe_reference.value();

        // Clauses must be valid
        std::vector<std::shared_ptr<Clause>> validated_clauses;
        for (const auto& clause : clauses) {
            const auto& maybe_validated_clause =
                details::validate_clause<StmtStmtList, StmtEntList, EntEntList, PatternAnalysersList>(declarations,
                                                                                                      mapping, clause);
            if (!maybe_validated_clause.has_value()) {
                return SemanticError{"Invalid clause"};
            }
            validated_clauses.push_back(maybe_validated_clause.value());
        }

        return Query{declarations, reference, validated_clauses};
    };
};

struct VisitorGenerator {
    auto operator()(const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                    const Synonyms& declarations) const {
        return overloaded{
            [&declarations, &mapping](const untyped::UntypedSynonym& synonym) -> std::optional<Reference> {
                static constexpr auto BOOLEAN_STRING = "BOOLEAN";
                const auto maybe_results = details::is_synonym_declared(declarations, mapping, synonym);
                if (maybe_results.has_value()) {
                    return std::optional<Reference>{maybe_results.value()};
                }

                // Special case: BOOLEAN
                if (synonym.get_name_string() == BOOLEAN_STRING) {
                    return std::optional<Reference>{BooleanReference{}};
                }

                return std::nullopt;
            },
            [&declarations,
             &mapping](const std::vector<untyped::UntypedSynonym>& synonyms) -> std::optional<Reference> {
                std::vector<std::shared_ptr<Synonym>> validated_synonyms;
                for (const auto& synonym : synonyms) {
                    const auto& maybe_synonym = details::is_synonym_declared(declarations, mapping, synonym);
                    if (!maybe_synonym.has_value()) {
                        return std::nullopt;
                    }
                    validated_synonyms.push_back(maybe_synonym.value());
                }
                return validated_synonyms;
            },
        };
    }
};

using DefaultSemanticAnalyser =
    SemanticAnalyser<DefaultStmtStmtList, DefaultStmtEntList, DefaultEntEntList, DefaultPatternAnalysersList,
                     untyped::DefaultUntypedParser::UntypedQueryType, VisitorGenerator>;
} // namespace qps
