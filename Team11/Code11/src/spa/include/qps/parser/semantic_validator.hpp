#pragma once

#include "qps/parser/entities/clause.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/parser/errors.hpp"
#include "qps/parser/semantic_validator_helper.hpp"
#include "qps/parser/untyped/entities/query.hpp"

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

#include "qps/parser/semantic_validator.tpp"

namespace qps {
template <typename StmtStmtList, typename StmtEntList, typename EntEntList>
class SemanticAnalyser {
  public:
    static auto validate(const Synonyms& declarations, const untyped::UntypedQuery& untyped_query)
        -> std::variant<Query, SemanticError> {
        // Declarations must be unique
        const auto& maybe_mapping = details::enforce_unique_declarations(declarations);
        if (!maybe_mapping.has_value()) {
            return SemanticError{"Non-unique mapping"};
        }
        const auto& mapping = maybe_mapping.value();

        const auto& [references, clauses] = untyped_query;

        // Reference must be declared
        const auto& maybe_reference = details::is_synonym_declared(declarations, mapping, references);
        if (!maybe_reference.has_value()) {
            return SemanticError{"Undeclared reference: " + references.get_name_string()};
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
    }
};

using DefaultSemanticAnalyser = SemanticAnalyser<DefaultStmtStmtList, DefaultStmtEntList, DefaultEntEntList>;
} // namespace qps
