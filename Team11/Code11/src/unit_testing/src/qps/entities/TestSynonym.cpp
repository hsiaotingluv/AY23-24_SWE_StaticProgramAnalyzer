#include "catch.hpp"

#include "qps/parser/entities/synonym.hpp"
#include <memory>
#include <sstream>

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

TEST_CASE("Printing Synonym") {
    SECTION("VarSynonym") {
        const auto synonym = std::make_shared<VarSynonym>("a");
        std::ostringstream oss;
        oss << synonym;
        REQUIRE(oss.str() == "VarSynonym(a)");
    }

    SECTION("ConstSynonym") {
        const auto synonym = std::make_shared<ConstSynonym>("c");
        std::ostringstream oss;
        oss << synonym;
        REQUIRE(oss.str() == "ConstSynonym(c)");
    }

    SECTION("ProcSynonym") {
        const auto synonym = std::make_shared<ProcSynonym>("p");
        std::ostringstream oss;
        oss << synonym;
        REQUIRE(oss.str() == "ProcSynonym(p)");
    }

    SECTION("AnyStmtSynonym") {
        const auto synonym = std::make_shared<AnyStmtSynonym>("s");
        std::ostringstream oss;
        oss << synonym;
        REQUIRE(oss.str() == "AnyStmtSynonym(s)");
    }

    SECTION("ReadSynonym") {
        const auto synonym = std::make_shared<ReadSynonym>("r");
        std::ostringstream oss;
        oss << synonym;
        REQUIRE(oss.str() == "ReadSynonym(r)");
    }

    SECTION("PrintSynonym") {
        const auto synonym = std::make_shared<PrintSynonym>("p");
        std::ostringstream oss;
        oss << synonym;
        REQUIRE(oss.str() == "PrintSynonym(p)");
    }

    SECTION("CallSynonym") {
        const auto synonym = std::make_shared<CallSynonym>("c");
        std::ostringstream oss;
        oss << synonym;
        REQUIRE(oss.str() == "CallSynonym(c)");
    }

    SECTION("WhileSynonym") {
        const auto synonym = std::make_shared<WhileSynonym>("w");
        std::ostringstream oss;
        oss << synonym;
        REQUIRE(oss.str() == "WhileSynonym(w)");
    }

    SECTION("IfSynonym") {
        const auto synonym = std::make_shared<IfSynonym>("i");
        std::ostringstream oss;
        oss << synonym;
        REQUIRE(oss.str() == "IfSynonym(i)");
    }

    SECTION("AssignSynonym") {
        const auto synonym = std::make_shared<AssignSynonym>("a");
        std::ostringstream oss;
        oss << synonym;
        REQUIRE(oss.str() == "AssignSynonym(a)");
    }

    SECTION("nullptr") {
        const auto synonym = std::shared_ptr<Synonym>{nullptr};
        std::ostringstream oss;
        oss << synonym;
        REQUIRE(oss.str() == "Synonym(nullptr)");
    }
}

TEST_CASE("Compare Synonym - direct") {
    SECTION("VarSynonym") {
        const auto synonym1 = std::make_shared<VarSynonym>("a");
        const auto synonym2 = std::make_shared<VarSynonym>("a");
        REQUIRE(synonym1 == synonym2);
    }

    SECTION("ConstSynonym") {
        const auto synonym1 = std::make_shared<ConstSynonym>("c");
        const auto synonym2 = std::make_shared<ConstSynonym>("c");
        REQUIRE(synonym1 == synonym2);
    }

    SECTION("ProcSynonym") {
        const auto synonym1 = std::make_shared<ProcSynonym>("p");
        const auto synonym2 = std::make_shared<ProcSynonym>("p");
        REQUIRE(synonym1 == synonym2);
    }

    SECTION("AnyStmtSynonym") {
        const auto synonym1 = std::make_shared<AnyStmtSynonym>("s");
        const auto synonym2 = std::make_shared<AnyStmtSynonym>("s");
        REQUIRE(synonym1 == synonym2);
    }

    SECTION("ReadSynonym") {
        const auto synonym1 = std::make_shared<ReadSynonym>("r");
        const auto synonym2 = std::make_shared<ReadSynonym>("r");
        REQUIRE(synonym1 == synonym2);
    }

    SECTION("PrintSynonym") {
        const auto synonym1 = std::make_shared<PrintSynonym>("p");
        const auto synonym2 = std::make_shared<PrintSynonym>("p");
        REQUIRE(synonym1 == synonym2);
    }

    SECTION("CallSynonym") {
        const auto synonym1 = std::make_shared<CallSynonym>("c");
        const auto synonym2 = std::make_shared<CallSynonym>("c");
        REQUIRE(synonym1 == synonym2);
    }

    SECTION("WhileSynonym") {
        const auto synonym1 = std::make_shared<WhileSynonym>("w");
        const auto synonym2 = std::make_shared<WhileSynonym>("w");
        REQUIRE(synonym1 == synonym2);
    }

    SECTION("IfSynonym") {
        const auto synonym1 = std::make_shared<IfSynonym>("i");
        const auto synonym2 = std::make_shared<IfSynonym>("i");
        REQUIRE(synonym1 == synonym2);
    }

    SECTION("AssignSynonym") {
        const auto synonym1 = std::make_shared<AssignSynonym>("a");
        const auto synonym2 = std::make_shared<AssignSynonym>("a");
        REQUIRE(synonym1 == synonym2);
    }

    SECTION("different name") {
        const auto synonym1 = std::make_shared<VarSynonym>("a");
        const auto synonym2 = std::make_shared<VarSynonym>("b");
        REQUIRE_FALSE(synonym1 == synonym2);
    }
}

TEST_CASE("Compare Synonym - Synonym") {
    SECTION("nullptr") {
        const std::shared_ptr<Synonym> synonym1 = std::shared_ptr<Synonym>{nullptr};
        const std::shared_ptr<Synonym> synonym2 = std::shared_ptr<Synonym>{nullptr};
        REQUIRE(synonym1 == synonym2);

        const std::shared_ptr<Synonym> synonym3 = std::make_shared<VarSynonym>("a");
        REQUIRE_FALSE(synonym1 == synonym3);
        REQUIRE_FALSE(synonym3 == synonym1);
    }
    SECTION("VarSynonym") {
        const std::shared_ptr<Synonym> synonym1 = std::make_shared<VarSynonym>("a");
        const std::shared_ptr<Synonym> synonym2 = std::make_shared<VarSynonym>("a");
        REQUIRE(synonym1 == synonym2);
    }

    SECTION("ConstSynonym") {
        const std::shared_ptr<Synonym> synonym1 = std::make_shared<ConstSynonym>("c");
        const std::shared_ptr<Synonym> synonym2 = std::make_shared<ConstSynonym>("c");
        REQUIRE(synonym1 == synonym2);
    }

    SECTION("ProcSynonym") {
        const std::shared_ptr<Synonym> synonym1 = std::make_shared<ProcSynonym>("p");
        const std::shared_ptr<Synonym> synonym2 = std::make_shared<ProcSynonym>("p");
        REQUIRE(synonym1 == synonym2);
    }

    SECTION("AnyStmtSynonym") {
        const std::shared_ptr<Synonym> synonym1 = std::make_shared<AnyStmtSynonym>("s");
        const std::shared_ptr<Synonym> synonym2 = std::make_shared<AnyStmtSynonym>("s");
        REQUIRE(synonym1 == synonym2);
    }

    SECTION("ReadSynonym") {
        const std::shared_ptr<Synonym> synonym1 = std::make_shared<ReadSynonym>("r");
        const std::shared_ptr<Synonym> synonym2 = std::make_shared<ReadSynonym>("r");
        REQUIRE(synonym1 == synonym2);
    }

    SECTION("PrintSynonym") {
        const std::shared_ptr<Synonym> synonym1 = std::make_shared<PrintSynonym>("p");
        const std::shared_ptr<Synonym> synonym2 = std::make_shared<PrintSynonym>("p");
        REQUIRE(synonym1 == synonym2);
    }

    SECTION("CallSynonym") {
        const std::shared_ptr<Synonym> synonym1 = std::make_shared<CallSynonym>("c");
        const std::shared_ptr<Synonym> synonym2 = std::make_shared<CallSynonym>("c");
        REQUIRE(synonym1 == synonym2);
    }

    SECTION("WhileSynonym") {
        const std::shared_ptr<Synonym> synonym1 = std::make_shared<WhileSynonym>("w");
        const std::shared_ptr<Synonym> synonym2 = std::make_shared<WhileSynonym>("w");
        REQUIRE(synonym1 == synonym2);
    }

    SECTION("IfSynonym") {
        const std::shared_ptr<Synonym> synonym1 = std::make_shared<IfSynonym>("i");
        const std::shared_ptr<Synonym> synonym2 = std::make_shared<IfSynonym>("i");
        REQUIRE(synonym1 == synonym2);
    }

    SECTION("AssignSynonym") {
        const std::shared_ptr<Synonym> synonym1 = std::make_shared<AssignSynonym>("a");
        const std::shared_ptr<Synonym> synonym2 = std::make_shared<AssignSynonym>("a");
        REQUIRE(synonym1 == synonym2);
    }

    SECTION("different name") {
        const std::shared_ptr<Synonym> synonym1 = std::make_shared<VarSynonym>("a");
        const std::shared_ptr<Synonym> synonym2 = std::make_shared<VarSynonym>("b");
        REQUIRE_FALSE(synonym1 == synonym2);
    }

    SECTION("nullptr") {
        const std::shared_ptr<Synonym> synonym1 = std::shared_ptr<Synonym>{nullptr};
        const std::shared_ptr<Synonym> synonym2 = std::shared_ptr<Synonym>{nullptr};
        REQUIRE(synonym1 == synonym2);
    }

    SECTION("nullptr and non-nullptr") {
        const std::shared_ptr<Synonym> synonym1 = std::shared_ptr<Synonym>{nullptr};
        const std::shared_ptr<Synonym> synonym2 = std::make_shared<VarSynonym>("a");
        REQUIRE_FALSE(synonym1 == synonym2);
    }

    SECTION("non-nullptr and nullptr") {
        const std::shared_ptr<Synonym> synonym1 = std::make_shared<VarSynonym>("a");
        const std::shared_ptr<Synonym> synonym2 = std::shared_ptr<Synonym>{nullptr};
        REQUIRE_FALSE(synonym1 == synonym2);
    }

    SECTION("different type") {
        const std::shared_ptr<Synonym> synonym1 = std::make_shared<VarSynonym>("a");
        const std::shared_ptr<Synonym> synonym2 = std::make_shared<ConstSynonym>("c");
        REQUIRE_FALSE(synonym1 == synonym2);
    }
}

TEST_CASE("Synonym - keyword") {
    SECTION("VarSynonym") {
        const auto synonym = VarSynonym("a");
        REQUIRE(synonym.get_keyword() == "variable");
    }

    SECTION("ConstSynonym") {
        const auto synonym = ConstSynonym("c");
        REQUIRE(synonym.get_keyword() == "constant");
    }

    SECTION("ProcSynonym") {
        const auto synonym = ProcSynonym("p");
        REQUIRE(synonym.get_keyword() == "procedure");
    }

    SECTION("AnyStmtSynonym") {
        const auto synonym = AnyStmtSynonym("s");
        REQUIRE(synonym.get_keyword() == "stmt");
    }

    SECTION("ReadSynonym") {
        const auto synonym = ReadSynonym("r");
        REQUIRE(synonym.get_keyword() == "read");
    }

    SECTION("PrintSynonym") {
        const auto synonym = PrintSynonym("p");
        REQUIRE(synonym.get_keyword() == "print");
    }

    SECTION("CallSynonym") {
        const auto synonym = CallSynonym("c");
        REQUIRE(synonym.get_keyword() == "call");
    }

    SECTION("WhileSynonym") {
        const auto synonym = WhileSynonym("w");
        REQUIRE(synonym.get_keyword() == "while");
    }

    SECTION("IfSynonym") {
        const auto synonym = IfSynonym("i");
        REQUIRE(synonym.get_keyword() == "if");
    }

    SECTION("AssignSynonym") {
        const auto synonym = AssignSynonym("a");
        REQUIRE(synonym.get_keyword() == "assign");
    }
}

TEST_CASE("Comparison - Less Than Relation") {
    SECTION("VarSynonym") {
        const auto synonym1 = std::make_shared<VarSynonym>("a");
        const auto synonym2 = std::make_shared<VarSynonym>("b");
        REQUIRE(synonym1 < synonym2);
    }

    SECTION("ConstSynonym") {
        const auto synonym1 = std::make_shared<ConstSynonym>("c");
        const auto synonym2 = std::make_shared<ConstSynonym>("d");
        REQUIRE(synonym1 < synonym2);
    }

    SECTION("Different Type") {
        const std::shared_ptr<Synonym> synonym1 = std::make_shared<VarSynonym>("a");
        const std::shared_ptr<Synonym> synonym2 = std::make_shared<ConstSynonym>("c");
        REQUIRE(synonym1 < synonym2);
    }

    SECTION("Sort") {
        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<VarSynonym>("a"),
            std::make_shared<ConstSynonym>("c"),
            std::make_shared<VarSynonym>("b"),
            std::make_shared<ConstSynonym>("d"),
        };

        std::sort(synonyms.begin(), synonyms.end());
        REQUIRE(synonyms[0]->get_name() == "a");
        REQUIRE(synonyms[1]->get_name() == "b");
        REQUIRE(synonyms[2]->get_name() == "c");
        REQUIRE(synonyms[3]->get_name() == "d");
    }
}