#include "catch.hpp"

#include "qps/optimisers/optimiser.hpp"
#include "qps/optimisers/priority.hpp"
#include "qps/parser/analysers/semantic_analyser.hpp"
#include "qps/parser/entities/clause.hpp"
#include "qps/parser/entities/relationship.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "utils.hpp"
#include <memory>
#include <variant>

using namespace qps;

TEST_CASE("Test PriorityOptimiser - Prioritise positive clauses") {
    const std::shared_ptr<Optimiser> optimiser = std::make_shared<PriorityOptimiser>();

    SECTION("Grouping - Priority") {
        const auto query = Query{
            std::vector<Elem>{std::make_shared<AnyStmtSynonym>("s")},
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(
                    NextT{std::make_shared<AnyStmtSynonym>("s"), std::make_shared<AnyStmtSynonym>("s1")}, false),
                std::make_shared<SuchThatClause>(
                    FollowsT{std::make_shared<AnyStmtSynonym>("s"), std::make_shared<AnyStmtSynonym>("s1")}, true),
            },
        };

        const auto results = optimiser->optimise(std::vector<Query>{query});
        REQUIRE(results.size() == 1);

        const auto query1 = results[0];
        std::cout << query1 << std::endl;
        REQUIRE(std::holds_alternative<std::vector<Elem>>(query1.reference));
        const auto reference2 = std::get<std::vector<Elem>>(query1.reference);
        REQUIRE(reference2.size() == 1);
        require_value<AnyStmtSynonym>(reference2, "s");
        REQUIRE(query1.clauses.size() == 2);

        const auto clause1 = query1.clauses[0];
        REQUIRE(!clause1->is_negated_clause());

        const auto clause2 = query1.clauses[1];
        REQUIRE(clause2->is_negated_clause());
    }
}