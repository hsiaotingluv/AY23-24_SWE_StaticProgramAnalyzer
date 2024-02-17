#include "catch.hpp"

#include "qps/parser/entities/synonym.hpp"
#include <memory>

using namespace qps;

TEST_CASE("Test QPS Synonym") {
    SECTION("Synonym - success") {
        const auto synonym = VarSynonym("a");
        REQUIRE(synonym.get_name() == "a");
    }
}

TEST_CASE("Test is_stmt_synonym") {
    // StmtSynonym := AnyStmtSynonym | ReadSynonym | PrintSynonym | CallSynonym | WhileSynonym | IfSynonym |
    // AssignSynonym
    SECTION("StmtSynonym - AnyStmtSynonym") {
        const std::shared_ptr<Synonym> synonym = std::make_shared<AnyStmtSynonym>("s");
        REQUIRE(is_stmt_synonym(synonym));
    }

    SECTION("StmtSynonym - ReadSynonym") {
        const std::shared_ptr<Synonym> synonym = std::make_shared<ReadSynonym>("r");
        REQUIRE(is_stmt_synonym(synonym));
    }

    SECTION("StmtSynonym - PrintSynonym") {
        const std::shared_ptr<Synonym> synonym = std::make_shared<PrintSynonym>("p");
        REQUIRE(is_stmt_synonym(synonym));
    }

    SECTION("StmtSynonym - CallSynonym") {
        const std::shared_ptr<Synonym> synonym = std::make_shared<CallSynonym>("c");
        REQUIRE(is_stmt_synonym(synonym));
    }

    SECTION("StmtSynonym - WhileSynonym") {
        const std::shared_ptr<Synonym> synonym = std::make_shared<WhileSynonym>("w");
        REQUIRE(is_stmt_synonym(synonym));
    }

    SECTION("StmtSynonym - IfSynonym") {
        const std::shared_ptr<Synonym> synonym = std::make_shared<IfSynonym>("i");
        REQUIRE(is_stmt_synonym(synonym));
    }

    SECTION("StmtSynonym - AssignSynonym") {
        const std::shared_ptr<Synonym> synonym = std::make_shared<AssignSynonym>("a");
        REQUIRE(is_stmt_synonym(synonym));
    }

    SECTION("StmtSynonym - VarSynonym failure") {
        const std::shared_ptr<Synonym> synonym = std::make_shared<VarSynonym>("v");
        REQUIRE_FALSE(is_stmt_synonym(synonym));
    }

    SECTION("StmtSynonym - ProcSynonym failure") {
        const std::shared_ptr<Synonym> synonym = std::make_shared<ProcSynonym>("p");
        REQUIRE_FALSE(is_stmt_synonym(synonym));
    }

    SECTION("StmtSynonym - ConstSynonym failure") {
        const std::shared_ptr<Synonym> synonym = std::make_shared<ConstSynonym>("c");
        REQUIRE_FALSE(is_stmt_synonym(synonym));
    }
}
