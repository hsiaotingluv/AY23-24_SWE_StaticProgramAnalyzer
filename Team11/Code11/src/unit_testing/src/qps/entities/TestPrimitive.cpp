#include "catch.hpp"

#include "qps/parser/entities/primitives.hpp"

using namespace qps;

TEST_CASE("Test IDENT") {
    IDENT ident("test");
    REQUIRE(ident.get_value() == "test");
    REQUIRE(ident == "test");
    REQUIRE(ident == IDENT{"test"});
    REQUIRE_FALSE(ident == IDENT{"test2"});
}
