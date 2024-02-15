#pragma once

#include "qps/parser/entities/clause.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/parser/errors.hpp"
#include "qps/parser/parser.hpp"

#include <memory>

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

class SemanticValidator {
  public:
    static auto validate(const Synonyms& declarations, const untyped::UntypedQuery& untyped_query)
        -> std::variant<Query, SemanticError>;
};
} // namespace qps
