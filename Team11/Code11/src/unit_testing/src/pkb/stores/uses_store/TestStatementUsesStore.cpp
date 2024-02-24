#include <catch.hpp>

#include "pkb/stores/uses_store/statement_uses_store.h"

TEST_CASE("UsesStore Tests") {
    StatementUsesStore statement_uses_store;

    SECTION("Adding and verifying that statement uses var relationship is added correctly") {
        StatementNumber s = "1";
        Variable v = Variable("x");
        statement_uses_store.add(s, v);

        REQUIRE(statement_uses_store.contains_key_val_pair(s, v));
    }

    SECTION("Retrieving all variables used by a statement") {
        StatementNumber s = "1";
        Variable v1 = Variable("x");
        Variable v2 = Variable("y");
        statement_uses_store.add(s, v1);
        statement_uses_store.add(s, v2);

        auto vars = statement_uses_store.get_vals_by_key(s);

        REQUIRE(vars.size() == 2);
        REQUIRE(vars.count(v1) == 1);
        REQUIRE(vars.count(v2) == 1);
    }

    SECTION("Retrieving all statements that use a variable") {
        StatementNumber s1 = "1";
        StatementNumber s2 = "2";
        Variable v = Variable("x");
        statement_uses_store.add(s1, v);
        statement_uses_store.add(s2, v);

        auto stmts = statement_uses_store.get_keys_by_val(v);

        REQUIRE(stmts.size() == 2);
        REQUIRE(stmts.count(s1) == 1);
        REQUIRE(stmts.count(s2) == 1);
    }

    SECTION("Retrieving all statements that use any variable") {
        StatementNumber s1 = "1";
        StatementNumber s2 = "2";
        Variable v1 = Variable("x");
        Variable v2 = Variable("y");
        statement_uses_store.add(s1, v1);
        statement_uses_store.add(s2, v2);

        auto stmts = statement_uses_store.get_all_keys();

        REQUIRE(stmts.size() == 2);
        REQUIRE(stmts.count(s1) == 1);
        REQUIRE(stmts.count(s2) == 1);
    }

    SECTION("Retrieving all statements and var use pairs") {
        StatementNumber s1 = "1";
        StatementNumber s2 = "2";
        Variable v1 = Variable("x");
        Variable v2 = Variable("y");
        statement_uses_store.add(s1, v1);
        statement_uses_store.add(s2, v2);

        auto pairs = statement_uses_store.get_all_pairs();

        REQUIRE(pairs.size() == 2);
        REQUIRE(pairs.count(std::make_tuple(s1, v1)) == 1);
        REQUIRE(pairs.count(std::make_tuple(s2, v2)) == 1);
    }

    SECTION("Checking if a statement uses any variable") {
        StatementNumber s = "1";
        Variable v = Variable("x");
        statement_uses_store.add(s, v);

        REQUIRE(statement_uses_store.contains_key(s));
    }
}