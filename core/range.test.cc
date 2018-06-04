/* Range Smoke Tests
 * =================
 * GOAL: Validate the basic functionality of the Range utility.
 */

#include <nonstd/core/range.h>
#include <testrunner/testrunner.h>

#include <array>
#include <vector>

#include <nonstd/core/enumerate.h>
#include <nonstd/core/type_traits_ext.h>
#include <nonstd/type_name.h>


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
    SECTION("do exactly what's advertised") {
        auto count = 0;
        auto sum   = 0;

        // 10 numbers, from 0 to 9.
        for (auto i : range(10)) {
            count++;
            sum += i;
        }

        REQUIRE(count == 10);
        REQUIRE(sum   == 45); // The sum of [0,9] is 45, by the by.
    }

    SECTION("can be given a start and end") {
        auto count = 0;
        auto sum   = 0;

        // 10 numbers, from 10, to 19
        for (auto i : range(10, 20)) {
            count++;
            sum += i;
        }

        REQUIRE(count == 10);
        REQUIRE(sum   == 145); // The sum of [10,19] is 145.
    }

    SECTION("can be given a range, and an amount to step by") {
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

    SECTION("never over-step") {
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

    SECTION("noop if told to go zero steps") {
        for (auto i : range(0)) {
            REQUIRE(false);
        }
    }

    SECTION("can fill containers") {
        SECTION("C arrays") {
            i8 arr[16];
            range(16).fill(arr);
            for (auto const& [i, value] : nonstd::enumerate(arr)) {
                REQUIRE(i == value);
            }
        }
        SECTION("STL arrays") {
            std::array<i16, 10> arr;
            // Ranges larger than the destination will be truncated
            range(100).fill(arr);
            for (auto const& [i, value] : nonstd::enumerate(arr)) {
                REQUIRE(i == value);
            }
        }
        SECTION("STL vectors") {
            std::vector<i16> vec;
            // Vectors will grow to hold the whole range
            range(100).fill(vec);
            for (auto const& [i, value] : nonstd::enumerate(vec)) {
                REQUIRE(i == value);
            }
            REQUIRE(vec.size() == 100);
        }
    }

    SECTION("can initialize containers") {
        SECTION("by assignment to vector") {
            std::vector<i16> vec = range(16);
            REQUIRE(vec.size() == 16);
            for (auto const& [i, value] : nonstd::enumerate(vec)) {
                REQUIRE(i == value);
            }
        }
        SECTION("by assignment to array") {
            std::array<i16, 10> arr = range(16);
            for (auto const& [i, value] : nonstd::enumerate(arr)) {
                REQUIRE(i == value);
            }
        }
        SECTION("by vector uniform initializer") {
            std::vector<i16> vec { range(16) };
            REQUIRE(vec.size() == 16);
            for (auto const& [i, value] : nonstd::enumerate(vec)) {
                REQUIRE(i == value);
            }
        }
        // Array uniform initializer will never work, because array is required
        // to deduce the inner type of braced inits as a value_type.
        SECTION("by vector constructor call") {
            std::vector<i16> vec ( range(16) );
            REQUIRE(vec.size() == 16);
            for (auto const& [i, value] : nonstd::enumerate(vec)) {
                REQUIRE(i == value);
            }
        }
        SECTION("by array constructor call") {
            std::array<i16, 10> arr ( range(16) );
            for (auto const& [i, value] : nonstd::enumerate(arr)) {
                REQUIRE(i == value);
            }
        }
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
