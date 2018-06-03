/* Math Utility Function Smoke Tests
 * =================================
 * Smoke tests for common math functions.
 */

#include <nonstd/math.h>
#include <testrunner/testrunner.h>


namespace nonstd_test {
namespace math {

using nonstd::mask_lowest_bits;
using nonstd::is_power_of_two;
using nonstd::ceil_power_of_two;
using nonstd::floor_power_of_two;


TEST_CASE("Math Utilities", "[nonstd]") {

    SECTION("should yield sane and correct bit masks") {
        REQUIRE(mask_lowest_bits<u8>(0) == 0b00000000);
        REQUIRE(mask_lowest_bits<u8>(1) == 0b00000001);
        REQUIRE(mask_lowest_bits<u8>(3) == 0b00000111);
        REQUIRE(mask_lowest_bits<u8>(7) == 0b01111111);
        REQUIRE(mask_lowest_bits<u8>(8) == 0b11111111);
        REQUIRE(mask_lowest_bits<u8>(9) == 0b11111111);

        REQUIRE(mask_lowest_bits<u32>(31) == 0x7FFFFFFF);
        REQUIRE(mask_lowest_bits<u32>(32) == 0xFFFFFFFF);
        REQUIRE(mask_lowest_bits<u32>(33) == 0xFFFFFFFF);

        REQUIRE(mask_lowest_bits<u64>(64) == 0xFFFFFFFFFFFFFFFF);
    }

    SECTION("should yield sane and correct bit masks (signed)") {
        REQUIRE(mask_lowest_bits<i8>(0) == 0b00000000);
        REQUIRE(mask_lowest_bits<i8>(1) == 0b00000001);
        REQUIRE(mask_lowest_bits<i8>(3) == 0b00000111);
        REQUIRE(mask_lowest_bits<i8>(7) == 0b01111111);
        REQUIRE(mask_lowest_bits<i8>(8) == (i8)(0b11111111));

        REQUIRE(mask_lowest_bits<i32>(32) == (i32)(0xFFFFFFFF));

        REQUIRE(mask_lowest_bits<i64>(64) == (i32)(0xFFFFFFFFFFFFFFFF));
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
        REQUIRE(ceil_power_of_two((u8)0)  == 1);
        REQUIRE(ceil_power_of_two((u8)1)  == 1);
        REQUIRE(ceil_power_of_two((u8)2)  == 2);
        REQUIRE(ceil_power_of_two((u8)3)  == 4);
        REQUIRE(ceil_power_of_two((u8)4)  == 4);
        REQUIRE(ceil_power_of_two((u8)5)  == 8);
        REQUIRE(ceil_power_of_two((u8)7)  == 8);
        REQUIRE(ceil_power_of_two((u8)63) == 64);
        REQUIRE(ceil_power_of_two((u8)65) == 128);

        REQUIRE(ceil_power_of_two((i8)0)  == 1);
        REQUIRE(ceil_power_of_two((i8)1)  == 1);
        REQUIRE(ceil_power_of_two((i8)2)  == 2);
        REQUIRE(ceil_power_of_two((i8)3)  == 4);
        REQUIRE(ceil_power_of_two((i8)4)  == 4);
        REQUIRE(ceil_power_of_two((i8)5)  == 8);
        REQUIRE(ceil_power_of_two((i8)7)  == 8);
        REQUIRE(ceil_power_of_two((i8)63) == 64);

        REQUIRE(ceil_power_of_two((u16)0) == 1);
        REQUIRE(ceil_power_of_two((u16)1) == 1);
        REQUIRE(ceil_power_of_two((u16)2) == 2);
        REQUIRE(ceil_power_of_two((u16)3) == 4);
        REQUIRE(ceil_power_of_two((u16)4) == 4);
        REQUIRE(ceil_power_of_two((u16)5) == 8);
        REQUIRE(ceil_power_of_two((u16)7) == 8);

        REQUIRE(ceil_power_of_two((u32)0) == 1);
        REQUIRE(ceil_power_of_two((u32)1) == 1);
        REQUIRE(ceil_power_of_two((u32)2) == 2);
        REQUIRE(ceil_power_of_two((u32)3) == 4);
        REQUIRE(ceil_power_of_two((u32)4) == 4);
        REQUIRE(ceil_power_of_two((u32)5) == 8);
        REQUIRE(ceil_power_of_two((u32)7) == 8);

        REQUIRE(ceil_power_of_two((u32)(0x07000000)) == (u32)(0x08000000));
        REQUIRE(ceil_power_of_two((u32)(0x08000001)) == (u32)(0x10000000));
        REQUIRE(ceil_power_of_two((u32)(0x80000000)) == (u32)(0x80000000));

        REQUIRE(ceil_power_of_two((u64)(0x07FFFFFFFFFFFFF))
                ==                (u64)(0x080000000000000));
        REQUIRE(ceil_power_of_two((u64)(0x080000000000001))
                ==                (u64)(0x100000000000000));
        REQUIRE(ceil_power_of_two((u64)(0x800000000000000))
                ==                (u64)(0x800000000000000));

        /* UNDESIRABLE BEHAVIOR
         * ====================
         * This is a limitation of C, though, so there's really no helping it.
         */
        REQUIRE(ceil_power_of_two((i8)65) == (i8)(-128));
        REQUIRE(ceil_power_of_two((u32)(0x90000000)) == (u32)(0x00000000));
    }

    SECTION("should correctly round numbers down to the nearest power of two") {
        REQUIRE(floor_power_of_two((u32)0) == 0);
        REQUIRE(floor_power_of_two((u32)1) == 1);
        REQUIRE(floor_power_of_two((u32)2) == 2);
        REQUIRE(floor_power_of_two((u32)3) == 2);
        REQUIRE(floor_power_of_two((u32)4) == 4);
        REQUIRE(floor_power_of_two((u32)5) == 4);
        REQUIRE(floor_power_of_two((u32)7) == 4);
        REQUIRE(floor_power_of_two((u32)8) == 8);

        REQUIRE(floor_power_of_two((u32)(0x07000000)) == (u32)(0x04000000));
        REQUIRE(floor_power_of_two((u32)(0x08000001)) == (u32)(0x08000000));
        REQUIRE(floor_power_of_two((u32)(0x80000000)) == (u32)(0x80000000));

        REQUIRE(floor_power_of_two((u64)(0x07FFFFFFFFFFFFF))
                ==                 (u64)(0x040000000000000));
        REQUIRE(floor_power_of_two((u64)(0x080000000000001))
                ==                 (u64)(0x080000000000000));
        REQUIRE(floor_power_of_two((u64)(0x800000000000000))
                ==                 (u64)(0x800000000000000));
    }
}

} /* namespace math_utils */
} /* namespace nonstd_test */
