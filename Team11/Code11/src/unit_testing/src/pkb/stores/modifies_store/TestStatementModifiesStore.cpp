#include <catch.hpp>

#include "pkb/stores/modifies_store/statement_modifies_store.h"

TEST_CASE("ModifiesStore Tests") {
    StatementModifiesStore statement_modifies_store;

    SECTION("Adding and verifying that statement modifies var relationship is added correctly") {
        StatementNumber s = "1";
        Variable v = Variable("x");
        statement_modifies_store.add(s, v);

        REQUIRE(statement_modifies_store.contains_key_val_pair(s, v));
    }

    SECTION("Retrieving all variables modified by a statement") {
        StatementNumber s = "1";
        Variable v1 = Variable("x");
        Variable v2 = Variable("y");
        statement_modifies_store.add(s, v1);
        statement_modifies_store.add(s, v2);

        auto vars = statement_modifies_store.get_vals_by_key(s);

        REQUIRE(vars.size() == 2);
        REQUIRE(vars.count(v1) == 1);
        REQUIRE(vars.count(v2) == 1);
    }

    SECTION("Retrieving all statements that modify a variable") {
        StatementNumber s1 = "1";
        StatementNumber s2 = "2";
        Variable v = Variable("x");
        statement_modifies_store.add(s1, v);
        statement_modifies_store.add(s2, v);

        auto stmts = statement_modifies_store.get_keys_by_val(v);

        REQUIRE(stmts.size() == 2);
        REQUIRE(stmts.count(s1) == 1);
        REQUIRE(stmts.count(s2) == 1);
    }

    SECTION("Retrieving all statements that modify any variable") {
        StatementNumber s1 = "1";
        StatementNumber s2 = "2";
        Variable v1 = Variable("x");
        Variable v2 = Variable("y");
        statement_modifies_store.add(s1, v1);
        statement_modifies_store.add(s2, v2);

        auto stmts = statement_modifies_store.get_all_keys();

        REQUIRE(stmts.size() == 2);
        REQUIRE(stmts.count(s1) == 1);
        REQUIRE(stmts.count(s2) == 1);
    }

    SECTION("Retrieving all statements and var modify pairs") {
        StatementNumber s1 = "1";
        StatementNumber s2 = "2";
        Variable v1 = Variable("x");
        Variable v2 = Variable("y");
        statement_modifies_store.add(s1, v1);
        statement_modifies_store.add(s2, v2);

        auto pairs = statement_modifies_store.get_all_pairs();

        REQUIRE(pairs.size() == 2);
        REQUIRE(pairs.count(std::make_tuple(s1, v1)) == 1);
        REQUIRE(pairs.count(std::make_tuple(s2, v2)) == 1);
    }

    SECTION("Checking if a statement modifies any variable") {
        StatementNumber s = "1";
        Variable v = Variable("x");
        statement_modifies_store.add(s, v);

        REQUIRE(statement_modifies_store.contains_key(s));
    }
}