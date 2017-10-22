/* Compare Utility Function Smoke Tests
 * =====================================
 * Smoke tests for the (typically noexcept) nonstd::compare and nonstd::equal_to
 * family of functions.
 */

#include <testrunner/testrunner.h>

#include "nonstd/std_ish/compare.h"
#include "nonstd/utility/optional.h"


namespace nonstd_test {
namespace compare {

using nonstd::compare;
using nonstd::equal_to;

struct BadComp {
    int x;

    bool operator==(BadComp const & other) noexcept(false) {
        if (x == 0) { throw std::runtime_error("What a bad class"); }
        return x == other.x;
    }
    bool operator<(BadComp const & other) noexcept(false) {
        if (x == 0) { throw std::runtime_error("What a bad class"); }
        return x < other.x;
    }
    bool operator>(BadComp const & other) noexcept(false) {
        if (x == 0) { throw std::runtime_error("What a bad class"); }
        return x > other.x;
    }
};

static_assert( noexcept(compare((u64)1,(u64)2)),
               "nonstd::compare(u64,u64) is correctly noexcept");
static_assert( noexcept(compare((u64)1,(u32)2)),
               "nonstd::compare(u64,u32) is correctly noexcept");
static_assert(!noexcept(compare("a","b")),
               "nonstd::compare(c_int,c_int) cannot be noexcept");
static_assert(!noexcept(compare(BadComp{1}, BadComp{2})),
               "nonstd::equal_to(BadComp,BadComp) cannot be noexcept");

static_assert( noexcept(equal_to((u64)1,(u64)2)),
               "nonstd::equal_to(u64,u64) is correctly noexcept");
static_assert( noexcept(equal_to((u64)1,(u32)2)),
               "nonstd::equal_to(u64,u32) is correctly noexcept");
static_assert(!noexcept(equal_to("a","b")),
               "nonstd::equal_to(c_int,c_int) cannot be noexcept");
static_assert(!noexcept(equal_to(BadComp{1}, BadComp{2})),
               "nonstd::equal_to(BadComp,BadComp) is explicitly not noexcept");

TEST_CASE("Comparison utilities") {
    SECTION("should correctly compare values") {
        SECTION("of the same type") {
            // strcmp baseline
            REQUIRE(strcmp("1", "1") == 0);
            REQUIRE(strcmp("1", "2") <  0);
            REQUIRE(strcmp("2", "1") >  0);

            REQUIRE(compare(1, 1) == 0);
            REQUIRE(compare(1, 2) <  0);
            REQUIRE(compare(2, 1) >  0);

            REQUIRE(compare("abc", "abc") == 0);
            REQUIRE(compare("abc", "def") <  0);
            REQUIRE(compare("fed", "cba") >  0);

            REQUIRE(compare(BadComp{1}, BadComp{1}) == 0);
            REQUIRE(compare(BadComp{1}, BadComp{2}) <  0);
            REQUIRE(compare(BadComp{2}, BadComp{1}) >  0);
        }

        SECTION("of differing types") {
            REQUIRE(compare((i32)1, (u64)1) == 0);
            REQUIRE(compare((f64)1, (i8)2)  <  0);
            REQUIRE(compare((u8)2,  (u64)1) >  0);
        }

        SECTION("of Optional variations") {
            nonstd::Optional<u32> maybe_int = { 1 };
            nonstd::Optional<u32> none_int  = { };
            nonstd::Optional<c_cstr> maybe_string = { "Foo" };
            nonstd::Optional<c_cstr> none_string  = { };

            /* Remember */
            REQUIRE(0 == strcmp("a", "a"));
            REQUIRE(0 <  strcmp("b", "a"));
            REQUIRE(0 >  strcmp("a", "b"));

            REQUIRE(0 == nonstd::compare(maybe_int, nonstd::just<u32>(1)));
            REQUIRE(0 <  nonstd::compare(maybe_int, none_int));
            REQUIRE(0 <  nonstd::compare(maybe_int, nonstd::nullopt));
            REQUIRE(0 == nonstd::compare(maybe_int, 1));
            REQUIRE(0 >  nonstd::compare(maybe_int, 2));
            REQUIRE(0 == strcmp("A", "A"));
            REQUIRE(0 == nonstd::compare(nonstd::just<c_cstr>("A"), "A"));
            REQUIRE(0 >  strcmp("A", "B"));
            REQUIRE(0 >  nonstd::compare(nonstd::just<c_cstr>("A"), "B"));
            REQUIRE(0 == strcmp("Foo", "Foo"));
            REQUIRE(0 == nonstd::compare(maybe_string, "Foo"));
            REQUIRE(0 <  strcmp("Foo", "Bar"));
            REQUIRE(0 <  nonstd::compare(maybe_string, "Bar"));
            REQUIRE(0 <  nonstd::compare(maybe_string, none_string));
            REQUIRE(0 == nonstd::compare(nonstd::nullopt, none_string));
        }
    }

    SECTION("should correctly calculate equality") {
        REQUIRE(equal_to(1, 1));
        REQUIRE(equal_to(42.f, 42.f));
        REQUIRE_FALSE(equal_to(1, 2));

        REQUIRE(equal_to("abc", "abc"));
        REQUIRE_FALSE(equal_to("abc", "def"));
        REQUIRE_FALSE(equal_to("abc", "cba"));

        SECTION("of differing types") {
            REQUIRE(equal_to((i32)1, (u64)1));
            REQUIRE(equal_to((u8)2,  (u64)2));
            REQUIRE(equal_to((f64)2, (i8)2));
        }

        SECTION("of Optional variations") {
            nonstd::Optional<u32> maybe_int = { 1 };
            nonstd::Optional<u32> none_int  = { };
            nonstd::Optional<c_cstr> maybe_string = { "Foo" };
            nonstd::Optional<c_cstr> none_string  = { };

            REQUIRE( nonstd::equal_to(maybe_int, nonstd::just<u32>(1)));
            REQUIRE(!nonstd::equal_to(maybe_int, none_int));
            REQUIRE(!nonstd::equal_to(maybe_int, nonstd::nullopt));
            REQUIRE( nonstd::equal_to(maybe_int, 1));
            REQUIRE(!nonstd::equal_to(maybe_int, 2));
            REQUIRE(0 == strcmp("A", "A"));
            REQUIRE( nonstd::equal_to(nonstd::just<c_cstr>("A"), "A"));
            REQUIRE(0 != strcmp("A", "B"));
            REQUIRE(!nonstd::equal_to(nonstd::just<c_cstr>("A"), "B"));
            REQUIRE(0 == strcmp("Foo", "Foo"));
            REQUIRE( nonstd::equal_to(maybe_string, "Foo"));
            REQUIRE(0 != strcmp("Foo", "Bar"));
            REQUIRE(!nonstd::equal_to(maybe_string, "Bar"));
            REQUIRE(!nonstd::equal_to(maybe_string, none_string));
            REQUIRE( nonstd::equal_to(nonstd::nullopt, none_string));
        }
    }
}

} /* namespace nonstd_test */
} /* namespace compare */
