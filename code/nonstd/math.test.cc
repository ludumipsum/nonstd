/* Math Utility Function Smoke Tests
 * =================================
 * Smoke tests for common math functions.
 */

#include <nonstd/math.h>
#include <testrunner/testrunner.h>


namespace nonstd_test {
namespace math {

using nonstd::maskLowestBits;
using nonstd::isPowerOfTwo;
using nonstd::roundUpToPowerOfTwo;
using nonstd::roundDownToPowerOfTwo;


TEST_CASE("Math Utilities", "[nonstd]") {

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
        REQUIRE(isPowerOfTwo(1));
        REQUIRE(isPowerOfTwo(2));
        REQUIRE(isPowerOfTwo(4));
        REQUIRE(isPowerOfTwo(8));
        REQUIRE(isPowerOfTwo( (u64)(0x8000000000000000) ));

        REQUIRE_FALSE(isPowerOfTwo(0));
        REQUIRE_FALSE(isPowerOfTwo(3));
        REQUIRE_FALSE(isPowerOfTwo(5));
        REQUIRE_FALSE(isPowerOfTwo( (u64)(0x7FFFFFFFFFFFFFFF) ));
        REQUIRE_FALSE(isPowerOfTwo( (u64)(0x8000000000000001) ));
    }

    SECTION("should correctly round numbers up to the nearest power of two") {
        REQUIRE(roundUpToPowerOfTwo((u32)0) == 0);
        REQUIRE(roundUpToPowerOfTwo((u32)1) == 1);
        REQUIRE(roundUpToPowerOfTwo((u32)2) == 2);
        REQUIRE(roundUpToPowerOfTwo((u32)3) == 4);
        REQUIRE(roundUpToPowerOfTwo((u32)4) == 4);
        REQUIRE(roundUpToPowerOfTwo((u32)5) == 8);
        REQUIRE(roundUpToPowerOfTwo((u32)7) == 8);

        REQUIRE(roundUpToPowerOfTwo((u32)(0x07000000)) == (u32)(0x08000000));
        REQUIRE(roundUpToPowerOfTwo((u32)(0x08000001)) == (u32)(0x10000000));
        REQUIRE(roundUpToPowerOfTwo((u32)(0x80000000)) == (u32)(0x80000000));

        REQUIRE(roundUpToPowerOfTwo((u64)(0x07FFFFFFFFFFFFF))
                ==                (u64)(0x080000000000000));
        REQUIRE(roundUpToPowerOfTwo((u64)(0x080000000000001))
                ==                (u64)(0x100000000000000));
        REQUIRE(roundUpToPowerOfTwo((u64)(0x800000000000000))
                ==                (u64)(0x800000000000000));

        /* UNDESIRABLE BEHAVIOR
         * ====================
         * The first is a limitation of the `roundUpToPowerOfTwo` function,
         * the second is a limitation of C. */
        REQUIRE(roundUpToPowerOfTwo((u32)0) == 0);
        REQUIRE(roundUpToPowerOfTwo((u32)(0x90000000)) == (u32)(0x00000000));
    }

    SECTION("should correctly round numbers down to the nearest power of two") {
        REQUIRE(roundDownToPowerOfTwo((u32)0) == 0);
        REQUIRE(roundDownToPowerOfTwo((u32)1) == 1);
        REQUIRE(roundDownToPowerOfTwo((u32)2) == 2);
        REQUIRE(roundDownToPowerOfTwo((u32)3) == 2);
        REQUIRE(roundDownToPowerOfTwo((u32)4) == 4);
        REQUIRE(roundDownToPowerOfTwo((u32)5) == 4);
        REQUIRE(roundDownToPowerOfTwo((u32)7) == 4);
        REQUIRE(roundDownToPowerOfTwo((u32)8) == 8);

        REQUIRE(roundDownToPowerOfTwo((u32)(0x07000000)) == (u32)(0x04000000));
        REQUIRE(roundDownToPowerOfTwo((u32)(0x08000001)) == (u32)(0x08000000));
        REQUIRE(roundDownToPowerOfTwo((u32)(0x80000000)) == (u32)(0x80000000));

        REQUIRE(roundDownToPowerOfTwo((u64)(0x07FFFFFFFFFFFFF))
                ==                    (u64)(0x040000000000000));
        REQUIRE(roundDownToPowerOfTwo((u64)(0x080000000000001))
                ==                    (u64)(0x080000000000000));
        REQUIRE(roundDownToPowerOfTwo((u64)(0x800000000000000))
                ==                    (u64)(0x800000000000000));
    }
}

} /* namespace math_utils */
} /* namespace nonstd_test */
