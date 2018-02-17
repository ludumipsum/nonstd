/* Enumerate Smoke Tests
 * =================
 * GOAL: Validate the basic functionality of the enumerate utility.
 */

#include <nonstd/core/enumerate.h>
#include <testrunner/testrunner.h>

#include <array>


namespace nonstd_test {
namespace enumerate {

TEST_CASE("Enumerate API Demo", "[nonstd][api][range]") {
    SECTION("gives you sequential numbers with an iterand") {
        std::array<i16, 10> arr = {0};
        auto iteration = 0;
        for (auto const& [i, value] : nonstd::enumerate(arr)) {
            REQUIRE(i == iteration++);
        }
    }

    SECTION("preserves referentiality") {
        std::vector<i16> arr;
        for (auto i : nonstd::range(10)) {
            arr.push_back(i);
        }

        auto base_address = &arr[0];
        for (auto const& [i, value] : nonstd::enumerate(arr)) {
            REQUIRE(value == arr[i]);
            REQUIRE(value == *(base_address + i));
            REQUIRE(&value == &(arr[i]));
            REQUIRE(&value == base_address + i);
        }
    }

    SECTION("works for bare arrays") {
        i16 arr[16] = {0};
        for (auto const& [i, value] : nonstd::enumerate(arr)) {
            REQUIRE(value == arr[i]);
            REQUIRE(value == *(arr + i));
            REQUIRE(&value == &(arr[i]));
            REQUIRE(&value == arr + i);
        }
    }
}

} /* namespace enumerate */
} /* namespace nonstd_test */
