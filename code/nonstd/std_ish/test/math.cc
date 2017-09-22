/* Math Utility Function Smoke Tests
 * =================================
 * Smoke tests for common math functions.
 */

#include <testrunner/testrunner.h>

#include "nonstd/std_ish/math.h"


namespace nonstd_test {
namespace math_utils {

using nonstd::maskLowestBits;
using nonstd::is_power_of_two;
using nonstd::next_power_of_two;
using nonstd::previous_power_of_two;


TEST_CASE("Math Utilities", "[nonstd][utils]") {

    SECTION("should yield sane and correct bit masks") {
        REQUIRE(maskLowestBits<u8>(0) == 0b00000000);
        REQUIRE(maskLowestBits<u8>(1) == 0b00000001);
        REQUIRE(maskLowestBits<u8>(3) == 0b00000111);
        REQUIRE(maskLowestBits<u8>(7) == 0b01111111);
        REQUIRE(maskLowestBits<u8>(8) == 0b11111111);

        REQUIRE(maskLowestBits<u32>(32) == 0xFFFFFFFF);

        REQUIRE(maskLowestBits<u64>(64) == 0xFFFFFFFFFFFFFFFF);
    }

    SECTION("should yield sane and correct bit masks (signed)") {
        REQUIRE(maskLowestBits<i8>(0) == 0b00000000);
        REQUIRE(maskLowestBits<i8>(1) == 0b00000001);
        REQUIRE(maskLowestBits<i8>(3) == 0b00000111);
        REQUIRE(maskLowestBits<i8>(7) == 0b01111111);
        REQUIRE(maskLowestBits<i8>(8) == (i8)(0b11111111));

        REQUIRE(maskLowestBits<i32>(32) == (i32)(0xFFFFFFFF));

        REQUIRE(maskLowestBits<i64>(64) == (i32)(0xFFFFFFFFFFFFFFFF));
    }

    SECTION("should correctly detect power-of-two numbers") {
        REQUIRE(is_power_of_two(1));
        REQUIRE(is_power_of_two(2));
        REQUIRE(is_power_of_two(4));
        REQUIRE(is_power_of_two(8));
        REQUIRE(is_power_of_two( (u64)(0x8000000000000000) ));

        REQUIRE_FALSE(is_power_of_two(0));
        REQUIRE_FALSE(is_power_of_two(3));
        REQUIRE_FALSE(is_power_of_two(5));
        REQUIRE_FALSE(is_power_of_two( (u64)(0x7FFFFFFFFFFFFFFF) ));
        REQUIRE_FALSE(is_power_of_two( (u64)(0x8000000000000001) ));
    }

    SECTION("should correctly round numbers up to the nearest power of two") {
        REQUIRE(next_power_of_two((u32)1) == 1);
        REQUIRE(next_power_of_two((u32)2) == 2);
        REQUIRE(next_power_of_two((u32)3) == 4);
        REQUIRE(next_power_of_two((u32)4) == 4);
        REQUIRE(next_power_of_two((u32)5) == 8);
        REQUIRE(next_power_of_two((u32)7) == 8);

        REQUIRE(next_power_of_two((u32)(0x07000000)) == (u32)(0x08000000));
        REQUIRE(next_power_of_two((u32)(0x08000001)) == (u32)(0x10000000));
        REQUIRE(next_power_of_two((u32)(0x80000000)) == (u32)(0x80000000));

        REQUIRE(next_power_of_two((u64)(0x07FFFFFFFFFFFFF))
                ==                (u64)(0x080000000000000));
        REQUIRE(next_power_of_two((u64)(0x080000000000001))
                ==                (u64)(0x100000000000000));
        REQUIRE(next_power_of_two((u64)(0x800000000000000))
                ==                (u64)(0x800000000000000));

        /* UNDESIRABLE BEHAVIOR
         * ====================
         * The first is a limitation of the `next_power_of_two` function,
         * the second is a limitation of C. */
        REQUIRE(next_power_of_two((u32)0) == 0);
        REQUIRE(next_power_of_two((u32)(0x90000000)) == (u32)(0x00000000));
    }

    SECTION("should correctly round numbers down to the nearest power of two") {
        REQUIRE(previous_power_of_two((u32)0) == 0);
        REQUIRE(previous_power_of_two((u32)1) == 1);
        REQUIRE(previous_power_of_two((u32)2) == 2);
        REQUIRE(previous_power_of_two((u32)3) == 2);
        REQUIRE(previous_power_of_two((u32)4) == 4);
        REQUIRE(previous_power_of_two((u32)5) == 4);
        REQUIRE(previous_power_of_two((u32)7) == 4);
        REQUIRE(previous_power_of_two((u32)8) == 8);

        REQUIRE(previous_power_of_two((u32)(0x07000000)) == (u32)(0x04000000));
        REQUIRE(previous_power_of_two((u32)(0x08000001)) == (u32)(0x08000000));
        REQUIRE(previous_power_of_two((u32)(0x80000000)) == (u32)(0x80000000));

        REQUIRE(previous_power_of_two((u64)(0x07FFFFFFFFFFFFF))
                ==                    (u64)(0x040000000000000));
        REQUIRE(previous_power_of_two((u64)(0x080000000000001))
                ==                    (u64)(0x080000000000000));
        REQUIRE(previous_power_of_two((u64)(0x800000000000000))
                ==                    (u64)(0x800000000000000));
    }
}

} /* namespace math_utils */
} /* namespace nonstd_test */
