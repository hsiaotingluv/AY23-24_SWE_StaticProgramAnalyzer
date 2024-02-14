#include <catch.hpp>
#include <memory>

#include "pkb/facades/read_facade.h"
#include "pkb/facades/write_facade.h"

TEST_CASE("Simple Entity Test") {
    SECTION("Procedure Test") {
        auto [read_facade, write_facade] = PKB::create_facades();

        write_facade->add_procedure("procedure1");

        REQUIRE(read_facade->get_procedures().size() == 1);
        REQUIRE(read_facade->get_variables().empty());
        REQUIRE(read_facade->get_entities().size() == 1);
    }
}

TEST_CASE("Simple Statement Test") {
    SECTION("Statement Test") {
        auto [read_facade, write_facade] = PKB::create_facades();

        write_facade->add_statement("1", StatementType::Read);
        //        write_facade->add_statement("2", StatementType::Print);
        //        write_facade->add_statement("3", StatementType::Assign);
        //        write_facade->add_statement("4", StatementType::If);
        //        write_facade->add_statement("5", StatementType::While);
        //        write_facade->add_statement("6", StatementType::Call);
        //        write_facade->add_statement("7", StatementType::Raw);
        //
        //        REQUIRE(read_facade->get_all_statements().size() == 7);
        //        REQUIRE(read_facade->get_read_statements().size() == 1);
        //        REQUIRE(read_facade->get_assign_statements().size() == 1);
        //        REQUIRE(read_facade->get_print_statements().size() == 1);
        //        REQUIRE(read_facade->get_if_statements().size() == 1);
        //        REQUIRE(read_facade->get_while_statements().size() == 1);
        //        REQUIRE(read_facade->get_call_statements().size() == 1);
        //        REQUIRE(read_facade->get_raw_statements().size() == 1);
    }
}

TEST_CASE("Simple Modify Test") {
    SECTION("Modify Test") {
        auto [read_facade, write_facade] = PKB::create_facades();

        write_facade->add_statement_modifies_var("1", "x");
        write_facade->add_statement_modifies_var("1", "y");
        write_facade->add_statement_modifies_var("2", "x");
        write_facade->add_statement_modifies_var("2", "z");

        REQUIRE(read_facade->does_statement_modify_var("1", "x"));
        REQUIRE(!read_facade->does_statement_modify_var("1", "z"));
        REQUIRE(read_facade->get_statements_that_modify_var("x").size() == 2);
        REQUIRE(read_facade->get_statements_that_modify_var("z").size() == 1);
        REQUIRE(read_facade->get_vars_modified_by_statement("1").size() == 2);
    }
}

TEST_CASE("Simple Use Test") {
    SECTION("Use Test") {
        auto [read_facade, write_facade] = PKB::create_facades();

        write_facade->add_statement_uses_var("1", "x");
        write_facade->add_statement_uses_var("1", "y");
        write_facade->add_statement_uses_var("2", "x");
        write_facade->add_statement_uses_var("2", "z");

        REQUIRE(read_facade->does_statement_use_var("1", "x"));
        REQUIRE(!read_facade->does_statement_use_var("1", "z"));
        REQUIRE(read_facade->get_statements_that_use_var("x").size() == 2);
        REQUIRE(read_facade->get_statements_that_use_var("z").size() == 1);
        REQUIRE(read_facade->get_vars_used_by_statement("1").size() == 2);
    }
}