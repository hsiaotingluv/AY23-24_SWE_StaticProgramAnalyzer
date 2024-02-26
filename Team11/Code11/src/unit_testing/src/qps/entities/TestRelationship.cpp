#include "catch.hpp"

#include "qps/parser/entities/relationship.hpp"
#include "qps/parser/entities/synonym.hpp"
#include "qps/template_utils.hpp"

#include <memory>
#include <sstream>

using namespace qps;

TEST_CASE("Test Relationship") {
    SECTION("Test Follows") {
        Follows follows{StmtRef{std::make_shared<AnyStmtSynonym>("s")}, StmtRef{std::make_shared<AnyStmtSynonym>("s")}};
        std::ostringstream os;
        os << follows;
        REQUIRE(os.str() == "Follows(AnyStmtSynonym(s), AnyStmtSynonym(s))");

        Relationship rel = follows;
        std::ostringstream os2;
        os2 << rel;
        REQUIRE(os2.str() == "Follows(AnyStmtSynonym(s), AnyStmtSynonym(s))");
    }

    SECTION("Test FollowsT") {
        FollowsT followsT{StmtRef{std::make_shared<AnyStmtSynonym>("s")},
                          StmtRef{std::make_shared<AnyStmtSynonym>("s")}};
        std::ostringstream os;
        os << followsT;
        REQUIRE(os.str() == "Follows*(AnyStmtSynonym(s), AnyStmtSynonym(s))");

        Relationship rel = followsT;
        std::ostringstream os2;
        os2 << rel;
        REQUIRE(os2.str() == "Follows*(AnyStmtSynonym(s), AnyStmtSynonym(s))");
    }

    SECTION("Test Parent") {
        Parent parent{StmtRef{std::make_shared<AnyStmtSynonym>("s")}, StmtRef{std::make_shared<AnyStmtSynonym>("s")}};
        std::ostringstream os;
        os << parent;
        REQUIRE(os.str() == "Parent(AnyStmtSynonym(s), AnyStmtSynonym(s))");

        Relationship rel = parent;
        std::ostringstream os2;
        os2 << rel;
        REQUIRE(os2.str() == "Parent(AnyStmtSynonym(s), AnyStmtSynonym(s))");
    }

    SECTION("Test ParentT") {
        ParentT parentT{StmtRef{std::make_shared<AnyStmtSynonym>("s")}, StmtRef{std::make_shared<AnyStmtSynonym>("s")}};
        std::ostringstream os;
        os << parentT;
        REQUIRE(os.str() == "Parent*(AnyStmtSynonym(s), AnyStmtSynonym(s))");

        Relationship rel = parentT;
        std::ostringstream os2;
        os2 << rel;
        REQUIRE(os2.str() == "Parent*(AnyStmtSynonym(s), AnyStmtSynonym(s))");
    }

    SECTION("Test UsesS") {
        UsesS usesS{StmtRefNoWildcard{std::make_shared<AnyStmtSynonym>("s")},
                    VarRef{std::make_shared<VarSynonym>("v")}};
        std::ostringstream os;
        os << usesS;
        REQUIRE(os.str() == "UsesS(AnyStmtSynonym(s), VarSynonym(v))");

        Relationship rel = usesS;
        std::ostringstream os2;
        os2 << rel;
        REQUIRE(os2.str() == "UsesS(AnyStmtSynonym(s), VarSynonym(v))");
    }

    SECTION("Test ModifiesS") {
        ModifiesS modifiesS{StmtRefNoWildcard{std::make_shared<AnyStmtSynonym>("s")},
                            VarRef{std::make_shared<VarSynonym>("v")}};
        std::ostringstream os;
        os << modifiesS;
        REQUIRE(os.str() == "ModifiesS(AnyStmtSynonym(s), VarSynonym(v))");

        Relationship rel = modifiesS;
        std::ostringstream os2;
        os2 << rel;
        REQUIRE(os2.str() == "ModifiesS(AnyStmtSynonym(s), VarSynonym(v))");
    }
}
