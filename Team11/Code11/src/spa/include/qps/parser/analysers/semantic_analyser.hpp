#pragma once

#include "qps/parser/analysers/pattern_analyser.hpp"
#include "qps/parser/analysers/relationship_analyser.hpp"
#include "qps/parser/analysers/semantic_analyser_helper.hpp"
#include "qps/parser/entities/clause.hpp"
#include "qps/parser/entities/select.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/parser/errors.hpp"
#include "qps/parser/untyped/entities/attribute.hpp"
#include "qps/parser/untyped/entities/boolean.hpp"
#include "qps/parser/untyped/entities/synonym.hpp"
#include "qps/parser/untyped/untyped_parser.hpp"
#include "qps/template_utils.hpp"

#include <memory>
#include <optional>
#include <variant>
#include <vector>

namespace qps {
struct BooleanReference {};

inline auto operator<<(std::ostream& os, const BooleanReference&) -> std::ostream& {
    os << "BOOLEAN()";
    return os;
}

using Reference = std::variant<BooleanReference, std::vector<Elem>>;

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

    Query(Synonyms declared, std::shared_ptr<Synonym> synonym, std::vector<std::shared_ptr<Clause>> clauses)
        : declared(std::move(declared)), reference(std::vector<Elem>{std::move(synonym)}), clauses(std::move(clauses)) {
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

#include "qps/parser/analysers/semantic_analyser.tpp"

namespace qps::detail {
template <typename T>
auto analyse(const Synonyms& declarations, const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
             const T& element) -> std::optional<Elem> {
    return std::visit(overloaded{[&](const untyped::UntypedSynonym& synonym) -> std::optional<Elem> {
                                     const auto& maybe_synonym =
                                         detail::is_synonym_declared(declarations, mapping, synonym);
                                     if (!maybe_synonym.has_value()) {
                                         return std::nullopt;
                                     }
                                     return std::make_optional(Elem{maybe_synonym.value()});
                                 },
                                 [&](const untyped::UntypedAttrRef& attr_ref) -> std::optional<Elem> {
                                     const auto& maybe_synonym =
                                         detail::is_synonym_declared(declarations, mapping, attr_ref.synonym);
                                     if (!maybe_synonym.has_value()) {
                                         return std::nullopt;
                                     }
                                     return std::make_optional(Elem{maybe_synonym.value()});
                                 }},
                      element);
}

template <typename T>
auto analyse(const Synonyms& declarations, const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
             const std::vector<T>& elements) -> std::optional<Reference> {
    auto constructed_elements = std::vector<Elem>{};
    constructed_elements.reserve(elements.size());
    for (const auto& element : elements) {
        const auto& maybe_constructed_element = analyse(declarations, mapping, element);
        if (!maybe_constructed_element.has_value()) {
            return std::nullopt;
        }
        constructed_elements.push_back(maybe_constructed_element.value());
    }
    return std::optional<Reference>{constructed_elements};
}
}; // namespace qps::detail

namespace qps {

template <typename RelationshipAnalysersList, typename PatternAnalysersList, typename ReferenceAnalyser,
          typename UntypedQueryType>
class SemanticAnalyser {
  public:
    static auto analyse(const Synonyms& declarations, const UntypedQueryType& untyped_query)
        -> std::variant<Query, SemanticError> {
        // Declarations must be unique
        const auto& maybe_mapping = detail::enforce_unique_declarations(declarations);
        if (!maybe_mapping.has_value()) {
            return SemanticError{"Non-unique mapping"};
        }
        const auto& mapping = maybe_mapping.value();

        const auto& [references, clauses] = untyped_query;

        // Reference must be declared
        const auto& maybe_reference = std::visit(ReferenceAnalyser{}(mapping, declarations), references);
        if (!maybe_reference.has_value()) {
            return SemanticError{"Undeclared reference"};
        }

        const auto& reference = maybe_reference.value();

        // Clauses must be valid
        std::vector<std::shared_ptr<Clause>> validated_clauses;
        for (const auto& clause : clauses) {
            const auto& maybe_validated_clause = std::visit(
                detail::untyped_clause_visitor<RelationshipAnalysersList, PatternAnalysersList>(declarations, mapping),
                clause);
            if (!maybe_validated_clause.has_value()) {
                return SemanticError{"Invalid clause"};
            }
            validated_clauses.push_back(maybe_validated_clause.value());
        }

        return Query{declarations, reference, validated_clauses};
    };
};

struct ReferenceAnalyser {
    auto operator()(const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                    const Synonyms& declarations) const {
        return overloaded{[&declarations, &mapping](const untyped::UntypedBoolean&) -> std::optional<Reference> {
                              const auto maybe_results = detail::is_synonym_declared(
                                  declarations, mapping,
                                  untyped::UntypedSynonym{IDENT{untyped::UntypedBoolean::keyword}});
                              if (maybe_results.has_value()) {
                                  return std::optional<Reference>{std::vector<Elem>{maybe_results.value()}};
                              }
                              return std::optional<Reference>{BooleanReference{}};
                          },
                          [&declarations, &mapping](const auto& x) -> std::optional<Reference> {
                              return detail::analyse(declarations, mapping, x);
                          }};
    }
};

using DefaultRelationshipAnalysersList =
    TypeList<RelationshipAnalyser<DefaultStmtStmtList, DefaultStmtEntList, DefaultEntEntList>>;
using DefaultSemanticAnalyser = SemanticAnalyser<DefaultRelationshipAnalysersList, DefaultPatternAnalysersList,
                                                 ReferenceAnalyser, untyped::DefaultUntypedParser::UntypedQueryType>;
} // namespace qps
