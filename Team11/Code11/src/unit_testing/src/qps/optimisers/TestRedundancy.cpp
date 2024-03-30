#include "catch.hpp"

#include "qps/optimisers/optimiser.hpp"
#include "qps/optimisers/redundancy.hpp"
#include "qps/parser/entities/relationship.hpp"
#include "qps/parser/entities/synonym.hpp"
#include <memory>
#include <variant>
#include <vector>

using namespace qps;

TEST_CASE("Test RedundancyOptimiser - Remove redundant clause") {
    const std::shared_ptr<Optimiser> optimiser = std::make_shared<RedundancyOptimiser>();

    SECTION("Redundancy - No redundant clause") {
        const auto query = Query{
            Synonyms{
                std::make_shared<ReadSynonym>("r"),
            },
            std::vector<Elem>{AttrRef{std::make_shared<ReadSynonym>("r"), VarName{}, AttrRef::Type::Name}},
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{std::make_shared<ReadSynonym>("r"), WildCard{}}, false),
            },
        };

        const auto results = optimiser->optimise(std::vector<Query>{query});
        REQUIRE(results.size() == 1);

        const auto result = results[0];
        REQUIRE(result.declared == query.declared);
        REQUIRE(std::holds_alternative<std::vector<Elem>>(query.reference));
        REQUIRE(std::holds_alternative<std::vector<Elem>>(result.reference));
        REQUIRE(std::get<std::vector<Elem>>(query.reference) == std::get<std::vector<Elem>>(result.reference));
        REQUIRE(result.clauses == query.clauses);
    }

    SECTION("Redundancy - Remove 1 redundant clause") {
        const auto query = Query{
            Synonyms{
                std::make_shared<ReadSynonym>("r"),
            },
            std::vector<Elem>{AttrRef{std::make_shared<ReadSynonym>("r"), VarName{}, AttrRef::Type::Name}},
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{std::make_shared<ReadSynonym>("r"), WildCard{}}, false),
                std::make_shared<SuchThatClause>(Follows{std::make_shared<ReadSynonym>("r"), WildCard{}}, false),
            },
        };

        const auto results = optimiser->optimise(std::vector<Query>{query});
        REQUIRE(results.size() == 1);

        const auto result = results[0];
        REQUIRE(result.declared == query.declared);
        REQUIRE(std::holds_alternative<std::vector<Elem>>(query.reference));
        REQUIRE(std::holds_alternative<std::vector<Elem>>(result.reference));
        REQUIRE(std::get<std::vector<Elem>>(query.reference) == std::get<std::vector<Elem>>(result.reference));
        REQUIRE(result.clauses.size() == 1);
        REQUIRE(result.clauses[0] == query.clauses[0]);
    }
}