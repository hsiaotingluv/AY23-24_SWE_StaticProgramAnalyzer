#pragma once

#include "qps/parser/entities/clause.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/parser/errors.hpp"
#include "qps/parser/semantic_analyser_helper.hpp"
#include "qps/parser/untyped/entities/synonym.hpp"
#include "qps/parser/untyped/untyped_parser.hpp"
#include "qps/template_utils.hpp"

#include <memory>

namespace qps::details {
// Forward declarations
template <typename StmtStmtList, typename StmtEntList, typename EntEntList>
auto validate_clause(const Synonyms& declarations,
                     const std::unordered_map<std::string, std::shared_ptr<Synonym>>& mapping,
                     const untyped::UntypedClause& clause) -> std::optional<std::shared_ptr<Clause>>;
} // namespace qps::details

namespace qps {
struct Query {
    Synonyms declared;
    std::shared_ptr<Synonym> reference;
    std::vector<std::shared_ptr<Clause>> clauses;

    Query(Synonyms declared, std::shared_ptr<Synonym> reference, std::vector<std::shared_ptr<Clause>> clauses)
        : declared(std::move(declared)), reference(std::move(reference)), clauses(std::move(clauses)) {
    }

    auto operator<<(std::ostream& os) -> std::ostream& {
        os << "Query:\n";
        os << "\tDeclared:\n";
        for (const auto& declared : declared) {
            os << "\t\t" << declared << "\n";
        }
        os << "\tReference:\n";
        os << "\t\t" << reference << "\n"; // TODO: dispatch to reference's operator<<
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

template <typename StmtStmtList, typename StmtEntList, typename EntEntList, typename UntypedQueryType,
          typename VisitorGenerator>
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
            const auto string_rep = std::visit(
                overloaded{
                    [](const untyped::UntypedSynonym& synonym) {
                        return synonym.get_name_string();
                    },
                },
                references);
            return SemanticError{"Undeclared reference: " + string_rep};
        }

        const auto& reference = maybe_reference.value();

        // Clauses must be valid
        std::vector<std::shared_ptr<Clause>> validated_clauses;
        for (const auto& clause : clauses) {
            const auto& maybe_validated_clause =
                details::validate_clause<StmtStmtList, StmtEntList, EntEntList>(declarations, mapping, clause);
            if (!maybe_validated_clause.has_value()) {
                return SemanticError{"Invalid clause"};
            }
            validated_clauses.push_back(maybe_validated_clause.value());
        }

        return Query{declarations, reference, validated_clauses};
    };
};

const auto visitor_generator = [](const auto& mapping, const auto& declarations) {
    return overloaded{
        [&declarations, &mapping](const untyped::UntypedSynonym& synonym) -> std::optional<std::shared_ptr<Synonym>> {
            return details::is_synonym_declared(declarations, mapping, synonym);
        }};
};
using VisitorGenerator = decltype(visitor_generator);

using DefaultSemanticAnalyser = SemanticAnalyser<DefaultStmtStmtList, DefaultStmtEntList, DefaultEntEntList,
                                                 untyped::DefaultUntypedParser::UntypedQueryType, VisitorGenerator>;
} // namespace qps
