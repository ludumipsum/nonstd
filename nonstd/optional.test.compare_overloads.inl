// Possibly duplicate includes from inlined code
#include <nonstd/optional.h>
#include <thirdparty/catch.h>

namespace compare_overloads {

using nonstd::optional;
using nonstd::nullopt;
using nonstd::compare;
using nonstd::equal_to;

TEST_CASE("Optionals + Utility Extensions", "[nonstd][optional]") {
SECTION("Optionals should correctly interact with")
    SECTION("nonstd::compare") {
        optional<u32> maybe_int = { 1 };
        optional<u32> none_int  = { };
        optional<c_cstr> maybe_string = { "Foo" };
        optional<c_cstr> none_string  = { };

        /* Remember */
        REQUIRE(0 == strcmp("a", "a"));
        REQUIRE(0 <  strcmp("b", "a"));
        REQUIRE(0 >  strcmp("a", "b"));

        REQUIRE(0 == compare(maybe_int, optional<u32> { 1 }));
        REQUIRE(0 <  compare(maybe_int, none_int));
        REQUIRE(0 <  compare(maybe_int, nullopt));
        REQUIRE(0 == compare(maybe_int, 1));
        REQUIRE(0 >  compare(maybe_int, 2));
        REQUIRE(0 == strcmp("Foo", "Foo"));
        REQUIRE(0 == compare(optional<c_cstr> { "Foo" }, "Foo"));
        REQUIRE(0 <  strcmp("Foo", "Bar"));
        REQUIRE(0 <  compare(optional<c_cstr> { "Foo" }, "Bar"));
        REQUIRE(0 == strcmp("Foo", "Foo"));
        REQUIRE(0 == compare(maybe_string, "Foo"));
        REQUIRE(0 <  strcmp("Foo", "Bar"));
        REQUIRE(0 <  compare(maybe_string, "Bar"));
        REQUIRE(0 <  compare(maybe_string, none_string));
        REQUIRE(0 == compare(nullopt, none_string));
    }

    SECTION("nonstd::equal_to") {
        optional<u32> maybe_int = { 1 };
        optional<u32> none_int  = { };
        optional<c_cstr> maybe_string = { "Foo" };
        optional<c_cstr> none_string  = { };

        REQUIRE( equal_to(maybe_int, optional<u32> { 1 }));
        REQUIRE(!equal_to(maybe_int, none_int));
        REQUIRE(!equal_to(maybe_int, nullopt));
        REQUIRE( equal_to(maybe_int, 1));
        REQUIRE(!equal_to(maybe_int, 2));
        REQUIRE(0 == strcmp("Foo", "Foo"));
        REQUIRE( equal_to(optional<c_cstr> { "Foo" }, "Foo"));
        REQUIRE(0 != strcmp("Foo", "Bar"));
        REQUIRE(!equal_to(optional<c_cstr> { "Foo" }, "Bar"));
        REQUIRE(0 == strcmp("Foo", "Foo"));
        REQUIRE( equal_to(maybe_string, "Foo"));
        REQUIRE(0 != strcmp("Foo", "Bar"));
        REQUIRE(!equal_to(maybe_string, "Bar"));
        REQUIRE(!equal_to(maybe_string, none_string));
        REQUIRE( equal_to(nullopt, none_string));
    }
}

} /* namespace compare_overloads */
