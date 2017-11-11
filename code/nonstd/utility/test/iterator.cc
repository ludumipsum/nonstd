/* Iterator Smoke Tests
 * ====================
 * GOAL: Validate the basic functionality of Iterator utilites.
 */

#include <testrunner/testrunner.h>
#include <nonstd/preprocessor/homogenize.h>

#include "nonstd/utility/iterator.h"


namespace nonstd_test {
namespace iterator {


constexpr int constexpr_sum(int f, int l) noexcept {
    int sum = 0;
    for (auto s : nonstd::range(f, l)) { sum += s; }
    return sum;
}

using nonstd::range;

/** Range API DEMONSTRATION
 *  =======================
 *  The Range utilites are very simple and act much like Python ranges. It won't
 *  take much to demonstrate the use and validity of these utils.
 */

TEST_CASE("Range API Demo", "[nonstd][api][range]") {
    SECTION("Do exactly what's advertised") {
        auto count = 0;
        auto sum   = 0;

        // 10 numbers, from 0 to 9.
        for (auto i : range(10)) {
            count++;
            sum += i;
        }

        REQUIRE(count == 10);
        REQUIRE(sum   == 45); // The sum of (0,9) is 45, by the by.
    }

    SECTION("They can be given a start and end") {
        auto count = 0;
        auto sum   = 0;

        // 10 numbers, from 10, to 19
        for (auto i : range(10, 20)) {
            count++;
            sum += i;
        }

        REQUIRE(count == 10);
        REQUIRE(sum   == 145); // The sum of (10,19) is 145.
    }

    SECTION("They can be given a range, and an amount to step by") {
        auto count = 0;
        auto sum   = 0;

        // Only the even numbers (and zero) between 0 and 20.
        for (auto i : range(0, 20, 2)) {
            count++;
            sum += i;
        }

        REQUIRE(count == 10);
        REQUIRE(sum   == 90); // Just... trust me on this one?
    }

    SECTION("They never over-step") {
        auto count = 0;
        auto sum   = 0;

        // Only hit 0, 9, and 18.
        for (auto i : range(0, 20, 9)) {
            count++;
            sum += i;
        }

        REQUIRE(count ==  3);
        REQUIRE(sum   == 27);
    }

#if !defined(NONSTD_OS_WINDOWS)
    SECTION("They're even constexpr, if you're not on MSVC") {
        constexpr auto result = constexpr_sum(10, 20);
        static_assert(result == 145, "Definitely evaluated at compile time");
        REQUIRE(result == 145);
    }
#endif
}

} /* namespace iterator */
} /* namespace nonstd_test */
