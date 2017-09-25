/* Compare Utility Function Smoke Tests
 * =====================================
 * Smoke tests for the (typically noexcept) nonstd::compare and nonstd::equal_to
 * family of functions.
 */

#include <testrunner/testrunner.h>

#include "nonstd/std_ish/compare.h"


namespace nonstd_test {
namespace compare {

using nonstd::compare;
using nonstd::equal_to;


static_assert( noexcept(compare((u64)1,(u64)2)),
               "nonstd::compare(u64,u64) is correctly noexcept");
static_assert( noexcept(compare((u64)1,(u32)2)),
               "nonstd::compare(u64,u32) is correctly noexcept");
static_assert(!noexcept(compare("a","b")),
               "nonstd::compare(c_int,c_int) cannot be noexcept");

static_assert( noexcept(equal_to((u64)1,(u64)2)),
               "nonstd::equal_to(u64,u64) is correctly noexcept");
static_assert( noexcept(equal_to((u64)1,(u32)2)),
               "nonstd::equal_to(u64,u32) is correctly noexcept");
static_assert(!noexcept(equal_to("a","b")),
               "nonstd::equal_to(c_int,c_int) cannot be noexcept");

TEST_CASE("Comparison utilities") {
    SECTION("should correctly compare values") {
        REQUIRE(compare(1, 1) == 0);
        REQUIRE(compare(1, 2) <  0);
        REQUIRE(compare(2, 1) >  0);

        REQUIRE(compare("abc", "abc") == 0);
        REQUIRE(compare("abc", "def") <  0);
        REQUIRE(compare("fed", "cba") >  0);

        SECTION("of differing types") {
            REQUIRE(compare((i32)1, (u64)1) == 0);
            REQUIRE(compare((f64)1, (i8)2)  <  0);
            REQUIRE(compare((u8)2,  (u64)1) >  0);
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
    }
}

} /* namespace nonstd_test */
} /* namespace compare */
