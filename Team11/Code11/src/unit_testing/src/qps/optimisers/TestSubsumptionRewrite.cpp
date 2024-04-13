#include "catch.hpp"

#include "qps/optimisers/optimiser.hpp"
#include "qps/optimisers/subsumption_rewrite.hpp"
#include "qps/parser/entities/primitives.hpp"
#include "qps/parser/entities/relationship.hpp"
#include "qps/parser/entities/synonym.hpp"
#include <memory>
#include <variant>
#include <vector>

using namespace qps;

TEST_CASE("Test SubsumptionRewrite - Remove subsumed clause") {
    const std::shared_ptr<Optimiser> optimiser = std::make_shared<SubsumptionRewriteOptimiser>();

    SECTION("SubsumptionRewrite - No subsumed clause") {
        const auto query = Query{
            std::vector<Elem>{AttrRef{std::make_shared<ReadSynonym>("r"), VarName{}, AttrRef::Type::Name}},
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{std::make_shared<ReadSynonym>("r"), WildCard{}}, false),
                std::make_shared<SuchThatClause>(
                    FollowsT{std::make_shared<ReadSynonym>("r"), std::make_shared<ReadSynonym>("r")}, false),
            },
        };

        const auto results = optimiser->optimise(std::vector<Query>{query});
        REQUIRE(results.size() == 1);

        const auto result = results[0];
        REQUIRE(std::holds_alternative<std::vector<Elem>>(query.reference));
        REQUIRE(std::holds_alternative<std::vector<Elem>>(result.reference));
        REQUIRE(std::get<std::vector<Elem>>(query.reference) == std::get<std::vector<Elem>>(result.reference));
        REQUIRE(result.clauses == query.clauses);
    }

    SECTION("SubsumptionRewrite - 1 subsumed clause [subsumed Arg for StmtStmt Relationship]") {
        const auto query = Query{
            std::vector<Elem>{AttrRef{std::make_shared<ReadSynonym>("r"), VarName{}, AttrRef::Type::Name}},
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{std::make_shared<ReadSynonym>("r"), Integer{"2"}}, false),
                std::make_shared<SuchThatClause>(Follows{std::make_shared<ReadSynonym>("r"), WildCard{}}, false),
            },
        };

        const auto results = optimiser->optimise(std::vector<Query>{query});
        REQUIRE(results.size() == 1);

        const auto result = results[0];
        REQUIRE(std::holds_alternative<std::vector<Elem>>(query.reference));
        REQUIRE(std::holds_alternative<std::vector<Elem>>(result.reference));
        REQUIRE(std::get<std::vector<Elem>>(query.reference) == std::get<std::vector<Elem>>(result.reference));
        REQUIRE(result.clauses.size() == 1);
    }

    SECTION("SubsumptionRewrite - 1 subsumed clause [subsumed Arg for non-StmtStmt Relationship]") {
        const auto query = Query{
            std::vector<Elem>{AttrRef{std::make_shared<ReadSynonym>("r"), VarName{}, AttrRef::Type::Name}},
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(ModifiesS{std::make_shared<ReadSynonym>("r"), QuotedIdent{"v"}},
                                                 false),
                std::make_shared<SuchThatClause>(ModifiesS{std::make_shared<ReadSynonym>("r"), WildCard{}}, false),
            },
        };

        const auto results = optimiser->optimise(std::vector<Query>{query});
        REQUIRE(results.size() == 1);

        const auto result = results[0];
        REQUIRE(std::holds_alternative<std::vector<Elem>>(query.reference));
        REQUIRE(std::holds_alternative<std::vector<Elem>>(result.reference));
        REQUIRE(std::get<std::vector<Elem>>(query.reference) == std::get<std::vector<Elem>>(result.reference));
        REQUIRE(result.clauses.size() == 1);
    }

    SECTION("SubsumptionRewrite - 1 subsumed clause [subsumed Type]") {
        const auto query = Query{
            std::vector<Elem>{AttrRef{std::make_shared<ReadSynonym>("r"), VarName{}, AttrRef::Type::Name}},
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{std::make_shared<ReadSynonym>("r"), WildCard{}}, false),
                std::make_shared<SuchThatClause>(FollowsT{std::make_shared<ReadSynonym>("r"), WildCard{}}, false),
            },
        };

        const auto results = optimiser->optimise(std::vector<Query>{query});
        REQUIRE(results.size() == 1);

        const auto result = results[0];
        REQUIRE(std::holds_alternative<std::vector<Elem>>(query.reference));
        REQUIRE(std::holds_alternative<std::vector<Elem>>(result.reference));
        REQUIRE(std::get<std::vector<Elem>>(query.reference) == std::get<std::vector<Elem>>(result.reference));
        REQUIRE(result.clauses.size() == 1);
    }

    SECTION("SubsumptionRewrite - Many clauses with no Subsumption relationship") {
        const auto query =
            Query{std::vector<Elem>{std::make_shared<ProcSynonym>("p1")},
                  {
                      std::make_shared<SuchThatClause>(
                          CallsT{std::make_shared<ProcSynonym>("p1"), std::make_shared<ProcSynonym>("p2")}, true),
                      std::make_shared<SuchThatClause>(
                          CallsT{std::make_shared<ProcSynonym>("p2"), std::make_shared<ProcSynonym>("p3")}, true),
                      std::make_shared<SuchThatClause>(
                          CallsT{std::make_shared<ProcSynonym>("p3"), std::make_shared<ProcSynonym>("p4")}, true),
                      std::make_shared<SuchThatClause>(
                          CallsT{std::make_shared<ProcSynonym>("p4"), std::make_shared<ProcSynonym>("p5")}, true),
                      std::make_shared<SuchThatClause>(
                          CallsT{std::make_shared<ProcSynonym>("p5"), std::make_shared<ProcSynonym>("p6")}, true),
                  }};

        const auto results = optimiser->optimise(std::vector<Query>{query});
        REQUIRE(results.size() == 1);
        REQUIRE(results[0].clauses.size() == query.clauses.size());
    }
}