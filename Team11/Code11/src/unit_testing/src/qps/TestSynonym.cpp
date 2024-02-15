// #include "catch.hpp"

// #include "utils.hpp"

// #include "qps/parser/entities/synonym.hpp"
// #include <memory>
// #include <variant>

// using namespace qps;

// TEST_CASE("Test QPS Synonym") {
//     SECTION("Synonym - success") {
//         const auto synonym = VarSynonym("a");
//         REQUIRE(synonym.get_name() == "a");
//     }
// }

// TEST_CASE("Test is_stmt_synonym") {
//     // StmtSynonym := AnyStmtSynonym | ReadSynonym | PrintSynonym | CallSynonym | WhileSynonym | IfSynonym |
//     // AssignSynonym
//     SECTION("StmtSynonym - AnyStmtSynonym") {
//         const std::shared_ptr<Synonym> synonym = std::make_shared<AnyStmtSynonym>("s");
//         REQUIRE(is_stmt_synonym(synonym));
//     }

//     SECTION("StmtSynonym - ReadSynonym") {
//         const Synonym synonym = ReadSynonym("r");
//         REQUIRE(is_stmt_synonym(synonym));
//     }

//     SECTION("StmtSynonym - PrintSynonym") {
//         const Synonym synonym = PrintSynonym("p");
//         REQUIRE(is_stmt_synonym(synonym));
//     }

//     SECTION("StmtSynonym - CallSynonym") {
//         const Synonym synonym = CallSynonym("c");
//         REQUIRE(is_stmt_synonym(synonym));
//     }

//     SECTION("StmtSynonym - WhileSynonym") {
//         const Synonym synonym = WhileSynonym("w");
//         REQUIRE(is_stmt_synonym(synonym));
//     }

//     SECTION("StmtSynonym - IfSynonym") {
//         const Synonym synonym = IfSynonym("i");
//         REQUIRE(is_stmt_synonym(synonym));
//     }

//     SECTION("StmtSynonym - AssignSynonym") {
//         const Synonym synonym = AssignSynonym("a");
//         REQUIRE(is_stmt_synonym(synonym));
//     }

//     SECTION("StmtSynonym - VarSynonym failure") {
//         const Synonym synonym = VarSynonym("v");
//         REQUIRE_FALSE(is_stmt_synonym(synonym));
//     }

//     SECTION("StmtSynonym - ProcSynonym failure") {
//         const Synonym synonym = ProcSynonym("p");
//         REQUIRE_FALSE(is_stmt_synonym(synonym));
//     }

//     SECTION("StmtSynonym - ConstSynonym failure") {
//         const Synonym synonym = ConstSynonym("c");
//         REQUIRE_FALSE(is_stmt_synonym(synonym));
//     }
// }

// TEST_CASE("Test get_stmt_synonym") {
//     SECTION("StmtSynonym - AnyStmtSynonym") {
//         const Synonym synonym = AnyStmtSynonym("s");
//         const auto maybe_stmt_syn = get_stmt_synonym(synonym);
//         REQUIRE(maybe_stmt_syn.has_value());

//         const auto stmt_syn = maybe_stmt_syn.value();
//         REQUIRE(std::visit(
//                     [](const auto& stmt_syn) {
//                         return stmt_syn.get_name();
//                     },
//                     stmt_syn) == "s");
//     }

//     SECTION("StmtSynonym - ReadSynonym") {
//         const Synonym synonym = ReadSynonym("r");
//         const auto maybe_stmt_syn = get_stmt_synonym(synonym);
//         REQUIRE(maybe_stmt_syn.has_value());

//         const auto stmt_syn = maybe_stmt_syn.value();
//         REQUIRE(std::visit(
//                     [](const auto& stmt_syn) {
//                         return stmt_syn.get_name();
//                     },
//                     stmt_syn) == "r");
//     }

//     SECTION("StmtSynonym - PrintSynonym") {
//         const Synonym synonym = PrintSynonym("p");
//         const auto maybe_stmt_syn = get_stmt_synonym(synonym);
//         REQUIRE(maybe_stmt_syn.has_value());

//         const auto stmt_syn = maybe_stmt_syn.value();
//         REQUIRE(std::visit(
//                     [](const auto& stmt_syn) {
//                         return stmt_syn.get_name();
//                     },
//                     stmt_syn) == "p");
//     }

//     SECTION("StmtSynonym - CallSynonym") {
//         const Synonym synonym = CallSynonym("c");
//         const auto maybe_stmt_syn = get_stmt_synonym(synonym);
//         REQUIRE(maybe_stmt_syn.has_value());

//         const auto stmt_syn = maybe_stmt_syn.value();
//         REQUIRE(std::visit(
//                     [](const auto& stmt_syn) {
//                         return stmt_syn.get_name();
//                     },
//                     stmt_syn) == "c");
//     }

//     SECTION("StmtSynonym - WhileSynonym") {
//         const Synonym synonym = WhileSynonym("w");
//         const auto maybe_stmt_syn = get_stmt_synonym(synonym);
//         REQUIRE(maybe_stmt_syn.has_value());

//         const auto stmt_syn = maybe_stmt_syn.value();
//         REQUIRE(std::visit(
//                     [](const auto& stmt_syn) {
//                         return stmt_syn.get_name();
//                     },
//                     stmt_syn) == "w");
//     }

//     SECTION("StmtSynonym - IfSynonym") {
//         const Synonym synonym = IfSynonym("i");
//         const auto maybe_stmt_syn = get_stmt_synonym(synonym);
//         REQUIRE(maybe_stmt_syn.has_value());

//         const auto stmt_syn = maybe_stmt_syn.value();
//         REQUIRE(std::visit(
//                     [](const auto& stmt_syn) {
//                         return stmt_syn.get_name();
//                     },
//                     stmt_syn) == "i");
//     }

//     SECTION("StmtSynonym - AssignSynonym") {
//         const Synonym synonym = AssignSynonym("a");
//         const auto maybe_stmt_syn = get_stmt_synonym(synonym);
//         REQUIRE(maybe_stmt_syn.has_value());

//         const auto stmt_syn = maybe_stmt_syn.value();
//         REQUIRE(std::visit(
//                     [](const auto& stmt_syn) {
//                         return stmt_syn.get_name();
//                     },
//                     stmt_syn) == "a");
//     }

//     SECTION("StmtSynonym - VarSynonym failure") {
//         const Synonym synonym = VarSynonym("v");
//         const auto maybe_stmt_syn = get_stmt_synonym(synonym);
//         REQUIRE_FALSE(maybe_stmt_syn.has_value());
//     }

//     SECTION("StmtSynonym - ProcSynonym failure") {
//         const Synonym synonym = ProcSynonym("p");
//         const auto maybe_stmt_syn = get_stmt_synonym(synonym);
//         REQUIRE_FALSE(maybe_stmt_syn.has_value());
//     }

//     SECTION("StmtSynonym - ConstSynonym failure") {
//         const Synonym synonym = ConstSynonym("c");
//         const auto maybe_stmt_syn = get_stmt_synonym(synonym);
//         REQUIRE_FALSE(maybe_stmt_syn.has_value());
//     }
// }