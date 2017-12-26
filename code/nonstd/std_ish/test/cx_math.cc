/** Constexpr Math Utility Smoke Tests
 *  =================================
 *  The target is to maintain parity with the functions defined in <cmath>.
 */

#include <testrunner/testrunner.h>

#include <nonstd/std_ish/cx_math.h>


namespace nonstd_test {
namespace math {

TEST_CASE("Constexpr Math Utilities", "[nonstd][cx]") {

    // Have a pile of floating point variables. Just... so many floating point
    // variables. A painful number of floating point variables.
    //
    // These should be pretty self explanitory. There's a type prefix, so we
    // know if we're dealing with 8, 16, or an unknown number of bytes. The
    // middle bit, if present, describes what we're putting into the variable --
    // specifically if we're putting something that's definitely larger than the
    // destination into the source. The last piece describes what the number is.
    //
    // Note that math (at least math that explicitly pushes a float out
    // of bounds) cannot be evaluated in a constexpr context. We make as many of
    // these variables constexpr as we can but, alas, we cannot get them all.
    // Additionally, some of these vars are commented out. As with the
    // non-constexpr variables, we cannot assign a known value that our
    // destination isn't wide enough to contain. The three obviously impossible
    // assignments in this set are the three that are commented out. We leave
    // them in, because it makes codemod easier.
    constexpr f32 f32__positive        =  1.0;                                      UNUSED(f32__positive);
    constexpr f32 f32__zero            =  0.0;                                      UNUSED(f32__zero);
    constexpr f32 f32__negative        = -1.0;                                      UNUSED(f32__negative);
    constexpr f32 f32__f32_min         = std::numeric_limits<f32>::min();           UNUSED(f32__f32_min);
    constexpr f32 f32__f32_max         = std::numeric_limits<f32>::max();           UNUSED(f32__f32_max);
    constexpr f32 f32__f64_min         = std::numeric_limits<f64>::min();           UNUSED(f32__f64_min);
//  constexpr f32 f32__f64_max         = std::numeric_limits<f64>::max();           UNUSED(f32__f64_max);
    constexpr f32 f32__f_long_min      = std::numeric_limits<f_long>::min();        UNUSED(f32__f_long_min);
//  constexpr f32 f32__f_long_max      = std::numeric_limits<f_long>::max();        UNUSED(f32__f_long_max);
    constexpr f32 f32__f32_nan         = std::numeric_limits<f32>::quiet_NaN();     UNUSED(f32__f32_nan);
    constexpr f32 f32__f64_nan         = std::numeric_limits<f64>::quiet_NaN();     UNUSED(f32__f64_nan);
    constexpr f32 f32__f_long_nan      = std::numeric_limits<f_long>::quiet_NaN();  UNUSED(f32__f_long_nan);
    constexpr f32 f32__f32_inf         = std::numeric_limits<f32>::infinity();      UNUSED(f32__f32_inf);
    constexpr f32 f32__f64_inf         = std::numeric_limits<f64>::infinity();      UNUSED(f32__f64_inf);
    constexpr f32 f32__f_long_inf      = std::numeric_limits<f_long>::infinity();   UNUSED(f32__f_long_inf);
              f32 f32__f32_less_min    = std::numeric_limits<f32>::min() / 2.0f;    UNUSED(f32__f32_less_min);
              f32 f32__f32_more_max    = std::numeric_limits<f32>::max() * 2.0f;    UNUSED(f32__f32_more_max);
              f32 f32__f64_less_min    = std::numeric_limits<f64>::min() / 2.0f;    UNUSED(f32__f64_less_min);
              f32 f32__f64_more_max    = std::numeric_limits<f64>::max() * 2.0f;    UNUSED(f32__f64_more_max);
              f32 f32__f_long_less_min = std::numeric_limits<f_long>::min() / 2.0f; UNUSED(f32__f_long_less_min);
              f32 f32__f_long_more_max = std::numeric_limits<f_long>::max() * 2.0f; UNUSED(f32__f_long_more_max);

    constexpr f64 f64__positive        =  1.0;                                      UNUSED(f64__positive);
    constexpr f64 f64__zero            =  0.0;                                      UNUSED(f64__zero);
    constexpr f64 f64__negative        = -1.0;                                      UNUSED(f64__negative);
    constexpr f64 f64__f32_min         = std::numeric_limits<f32>::min();           UNUSED(f64__f32_min);
    constexpr f64 f64__f32_max         = std::numeric_limits<f32>::max();           UNUSED(f64__f32_max);
    constexpr f64 f64__f64_min         = std::numeric_limits<f64>::min();           UNUSED(f64__f64_min);
    constexpr f64 f64__f64_max         = std::numeric_limits<f64>::max();           UNUSED(f64__f64_max);
    constexpr f64 f64__f_long_min      = std::numeric_limits<f_long>::min();        UNUSED(f64__f_long_min);
//  constexpr f64 f64__f_long_max      = std::numeric_limits<f_long>::max();        UNUSED(f64__f_long_max);
    constexpr f64 f64__f32_nan         = std::numeric_limits<f32>::quiet_NaN();     UNUSED(f64__f32_nan);
    constexpr f64 f64__f64_nan         = std::numeric_limits<f64>::quiet_NaN();     UNUSED(f64__f64_nan);
    constexpr f64 f64__f_long_nan      = std::numeric_limits<f_long>::quiet_NaN();  UNUSED(f64__f_long_nan);
    constexpr f64 f64__f32_inf         = std::numeric_limits<f32>::infinity();      UNUSED(f64__f32_inf);
    constexpr f64 f64__f64_inf         = std::numeric_limits<f64>::infinity();      UNUSED(f64__f64_inf);
    constexpr f64 f64__f_long_inf      = std::numeric_limits<f_long>::infinity();   UNUSED(f64__f_long_inf);
              f64 f64__f32_less_min    = std::numeric_limits<f32>::min() / 2.0;     UNUSED(f64__f32_less_min);
              f64 f64__f32_more_max    = std::numeric_limits<f32>::max() * 2.0;     UNUSED(f64__f32_more_max);
              f64 f64__f64_less_min    = std::numeric_limits<f64>::min() / 2.0;     UNUSED(f64__f64_less_min);
              f64 f64__f64_more_max    = std::numeric_limits<f64>::max() * 2.0;     UNUSED(f64__f64_more_max);
              f64 f64__f_long_less_min = std::numeric_limits<f_long>::min() / 2.0;  UNUSED(f64__f_long_less_min);
              f64 f64__f_long_more_max = std::numeric_limits<f_long>::max() * 2.0;  UNUSED(f64__f_long_more_max);

    constexpr f_long f_long__positive        =  1.0;                                      UNUSED(f_long__positive);
    constexpr f_long f_long__zero            =  0.0;                                      UNUSED(f_long__zero);
    constexpr f_long f_long__negative        = -1.0;                                      UNUSED(f_long__negative);
    constexpr f_long f_long__f32_min         = std::numeric_limits<f32>::min();           UNUSED(f_long__f32_min);
    constexpr f_long f_long__f32_max         = std::numeric_limits<f32>::max();           UNUSED(f_long__f32_max);
    constexpr f_long f_long__f64_min         = std::numeric_limits<f64>::min();           UNUSED(f_long__f64_min);
    constexpr f_long f_long__f64_max         = std::numeric_limits<f64>::max();           UNUSED(f_long__f64_max);
    constexpr f_long f_long__f_long_min      = std::numeric_limits<f_long>::min();        UNUSED(f_long__f_long_min);
    constexpr f_long f_long__f_long_max      = std::numeric_limits<f_long>::max();        UNUSED(f_long__f_long_max);
    constexpr f_long f_long__f32_nan         = std::numeric_limits<f32>::quiet_NaN();     UNUSED(f_long__f32_nan);
    constexpr f_long f_long__f64_nan         = std::numeric_limits<f64>::quiet_NaN();     UNUSED(f_long__f64_nan);
    constexpr f_long f_long__f_long_nan      = std::numeric_limits<f_long>::quiet_NaN();  UNUSED(f_long__f_long_nan);
    constexpr f_long f_long__f32_inf         = std::numeric_limits<f32>::infinity();      UNUSED(f_long__f32_inf);
    constexpr f_long f_long__f64_inf         = std::numeric_limits<f64>::infinity();      UNUSED(f_long__f64_inf);
    constexpr f_long f_long__f_long_inf      = std::numeric_limits<f_long>::infinity();   UNUSED(f_long__f_long_inf);
              f_long f_long__f32_less_min    = std::numeric_limits<f32>::min() / 2.0l;    UNUSED(f_long__f32_less_min);
              f_long f_long__f32_more_max    = std::numeric_limits<f32>::max() * 2.0l;    UNUSED(f_long__f32_more_max);
              f_long f_long__f64_less_min    = std::numeric_limits<f64>::min() / 2.0l;    UNUSED(f_long__f64_less_min);
              f_long f_long__f64_more_max    = std::numeric_limits<f64>::max() * 2.0l;    UNUSED(f_long__f64_more_max);
              f_long f_long__f_long_less_min = std::numeric_limits<f_long>::min() / 2.0l; UNUSED(f_long__f_long_less_min);
              f_long f_long__f_long_more_max = std::numeric_limits<f_long>::max() * 2.0l; UNUSED(f_long__f_long_more_max);

    // You thought we were done? There are Integral overloads for the functions
    // we want to test, too.
    constexpr u64 u64__positive =  1;                               UNUSED(u64__positive);
    constexpr u64 u64__zero     =  0;                               UNUSED(u64__zero);
    constexpr u64 u64__max      =  std::numeric_limits<u64>::max(); UNUSED(u64__max);
    constexpr i64 i64__positive =  1;                               UNUSED(i64__positive);
    constexpr i64 i64__zero     =  0;                               UNUSED(i64__zero);
    constexpr i64 i64__negative = -1;                               UNUSED(i64__negative);
    constexpr i64 i64__min      =  std::numeric_limits<i64>::min(); UNUSED(i64__min);
    constexpr i64 i64__max      =  std::numeric_limits<i64>::max(); UNUSED(i64__max);


    SECTION("isinf") {
        constexpr auto calls_to_isinf_are_constexpr = nonstd::cx::isinf(f32__positive);

        REQUIRE(std::isinf(f32__positive)        == nonstd::cx::isinf(f32__positive));
        REQUIRE(std::isinf(f32__negative)        == nonstd::cx::isinf(f32__negative));
        REQUIRE(std::isinf(f32__f32_min)         == nonstd::cx::isinf(f32__f32_min));
        REQUIRE(std::isinf(f32__f32_max)         == nonstd::cx::isinf(f32__f32_max));
        REQUIRE(std::isinf(f32__f64_min)         == nonstd::cx::isinf(f32__f64_min));
    //  REQUIRE(std::isinf(f32__f64_max)         == nonstd::cx::isinf(f32__f64_max));
        REQUIRE(std::isinf(f32__f_long_min)      == nonstd::cx::isinf(f32__f_long_min));
    //  REQUIRE(std::isinf(f32__f_long_max)      == nonstd::cx::isinf(f32__f_long_max));
        REQUIRE(std::isinf(f32__f32_nan)         == nonstd::cx::isinf(f32__f32_nan));
        REQUIRE(std::isinf(f32__f64_nan)         == nonstd::cx::isinf(f32__f64_nan));
        REQUIRE(std::isinf(f32__f_long_nan)      == nonstd::cx::isinf(f32__f_long_nan));
        REQUIRE(std::isinf(f32__f32_inf)         == nonstd::cx::isinf(f32__f32_inf));
        REQUIRE(std::isinf(f32__f64_inf)         == nonstd::cx::isinf(f32__f64_inf));
        REQUIRE(std::isinf(f32__f_long_inf)      == nonstd::cx::isinf(f32__f_long_inf));
        REQUIRE(std::isinf(f32__f32_less_min)    == nonstd::cx::isinf(f32__f32_less_min));
        REQUIRE(std::isinf(f32__f32_more_max)    == nonstd::cx::isinf(f32__f32_more_max));
        REQUIRE(std::isinf(f32__f64_less_min)    == nonstd::cx::isinf(f32__f64_less_min));
        REQUIRE(std::isinf(f32__f64_more_max)    == nonstd::cx::isinf(f32__f64_more_max));
        REQUIRE(std::isinf(f32__f_long_less_min) == nonstd::cx::isinf(f32__f_long_less_min));
        REQUIRE(std::isinf(f32__f_long_more_max) == nonstd::cx::isinf(f32__f_long_more_max));

        REQUIRE(std::isinf(f64__positive)        == nonstd::cx::isinf(f64__positive));
        REQUIRE(std::isinf(f64__negative)        == nonstd::cx::isinf(f64__negative));
        REQUIRE(std::isinf(f64__f32_min)         == nonstd::cx::isinf(f64__f32_min));
        REQUIRE(std::isinf(f64__f32_max)         == nonstd::cx::isinf(f64__f32_max));
        REQUIRE(std::isinf(f64__f64_min)         == nonstd::cx::isinf(f64__f64_min));
        REQUIRE(std::isinf(f64__f64_max)         == nonstd::cx::isinf(f64__f64_max));
        REQUIRE(std::isinf(f64__f_long_min)      == nonstd::cx::isinf(f64__f_long_min));
    //  REQUIRE(std::isinf(f64__f_long_max)      == nonstd::cx::isinf(f64__f_long_max));
        REQUIRE(std::isinf(f64__f32_nan)         == nonstd::cx::isinf(f64__f32_nan));
        REQUIRE(std::isinf(f64__f64_nan)         == nonstd::cx::isinf(f64__f64_nan));
        REQUIRE(std::isinf(f64__f_long_nan)      == nonstd::cx::isinf(f64__f_long_nan));
        REQUIRE(std::isinf(f64__f32_inf)         == nonstd::cx::isinf(f64__f32_inf));
        REQUIRE(std::isinf(f64__f64_inf)         == nonstd::cx::isinf(f64__f64_inf));
        REQUIRE(std::isinf(f64__f_long_inf)      == nonstd::cx::isinf(f64__f_long_inf));
        REQUIRE(std::isinf(f64__f32_less_min)    == nonstd::cx::isinf(f64__f32_less_min));
        REQUIRE(std::isinf(f64__f32_more_max)    == nonstd::cx::isinf(f64__f32_more_max));
        REQUIRE(std::isinf(f64__f64_less_min)    == nonstd::cx::isinf(f64__f64_less_min));
        REQUIRE(std::isinf(f64__f64_more_max)    == nonstd::cx::isinf(f64__f64_more_max));
        REQUIRE(std::isinf(f64__f_long_less_min) == nonstd::cx::isinf(f64__f_long_less_min));
        REQUIRE(std::isinf(f64__f_long_more_max) == nonstd::cx::isinf(f64__f_long_more_max));

        REQUIRE(std::isinf(f_long__positive)        == nonstd::cx::isinf(f_long__positive));
        REQUIRE(std::isinf(f_long__negative)        == nonstd::cx::isinf(f_long__negative));
        REQUIRE(std::isinf(f_long__f32_min)         == nonstd::cx::isinf(f_long__f32_min));
        REQUIRE(std::isinf(f_long__f32_max)         == nonstd::cx::isinf(f_long__f32_max));
        REQUIRE(std::isinf(f_long__f64_min)         == nonstd::cx::isinf(f_long__f64_min));
        REQUIRE(std::isinf(f_long__f64_max)         == nonstd::cx::isinf(f_long__f64_max));
        REQUIRE(std::isinf(f_long__f_long_min)      == nonstd::cx::isinf(f_long__f_long_min));
        REQUIRE(std::isinf(f_long__f_long_max)      == nonstd::cx::isinf(f_long__f_long_max));
        REQUIRE(std::isinf(f_long__f32_nan)         == nonstd::cx::isinf(f_long__f32_nan));
        REQUIRE(std::isinf(f_long__f64_nan)         == nonstd::cx::isinf(f_long__f64_nan));
        REQUIRE(std::isinf(f_long__f_long_nan)      == nonstd::cx::isinf(f_long__f_long_nan));
        REQUIRE(std::isinf(f_long__f32_inf)         == nonstd::cx::isinf(f_long__f32_inf));
        REQUIRE(std::isinf(f_long__f64_inf)         == nonstd::cx::isinf(f_long__f64_inf));
        REQUIRE(std::isinf(f_long__f_long_inf)      == nonstd::cx::isinf(f_long__f_long_inf));
        REQUIRE(std::isinf(f_long__f32_less_min)    == nonstd::cx::isinf(f_long__f32_less_min));
        REQUIRE(std::isinf(f_long__f32_more_max)    == nonstd::cx::isinf(f_long__f32_more_max));
        REQUIRE(std::isinf(f_long__f64_less_min)    == nonstd::cx::isinf(f_long__f64_less_min));
        REQUIRE(std::isinf(f_long__f64_more_max)    == nonstd::cx::isinf(f_long__f64_more_max));
        REQUIRE(std::isinf(f_long__f_long_less_min) == nonstd::cx::isinf(f_long__f_long_less_min));
        REQUIRE(std::isinf(f_long__f_long_more_max) == nonstd::cx::isinf(f_long__f_long_more_max));

#if !defined(NONSTD_COMPILER_MSVC)
        // MSVC fails on these std::isinf calls with an 'ambiguous overload'
        // error. This is out of spec, so keep testing these elsewhere.
        REQUIRE(std::isinf(u64__positive) == nonstd::cx::isinf(u64__positive));
        REQUIRE(std::isinf(u64__zero)     == nonstd::cx::isinf(u64__zero));
        REQUIRE(std::isinf(u64__max)      == nonstd::cx::isinf(u64__max));
        REQUIRE(std::isinf(i64__positive) == nonstd::cx::isinf(i64__positive));
        REQUIRE(std::isinf(i64__zero)     == nonstd::cx::isinf(i64__zero));
        REQUIRE(std::isinf(i64__negative) == nonstd::cx::isinf(i64__negative));
        REQUIRE(std::isinf(i64__min)      == nonstd::cx::isinf(i64__min));
        REQUIRE(std::isinf(i64__max)      == nonstd::cx::isinf(i64__max));
#endif
    }

    SECTION("isnan") {
        constexpr auto calls_to_isnan_are_constexpr = nonstd::cx::isnan(f32__positive);

        REQUIRE(std::isnan(f32__positive)        == nonstd::cx::isnan(f32__positive));
        REQUIRE(std::isnan(f32__negative)        == nonstd::cx::isnan(f32__negative));
        REQUIRE(std::isnan(f32__f32_min)         == nonstd::cx::isnan(f32__f32_min));
        REQUIRE(std::isnan(f32__f32_max)         == nonstd::cx::isnan(f32__f32_max));
        REQUIRE(std::isnan(f32__f64_min)         == nonstd::cx::isnan(f32__f64_min));
    //  REQUIRE(std::isnan(f32__f64_max)         == nonstd::cx::isnan(f32__f64_max));
        REQUIRE(std::isnan(f32__f_long_min)      == nonstd::cx::isnan(f32__f_long_min));
    //  REQUIRE(std::isnan(f32__f_long_max)      == nonstd::cx::isnan(f32__f_long_max));
        REQUIRE(std::isnan(f32__f32_nan)         == nonstd::cx::isnan(f32__f32_nan));
        REQUIRE(std::isnan(f32__f64_nan)         == nonstd::cx::isnan(f32__f64_nan));
        REQUIRE(std::isnan(f32__f_long_nan)      == nonstd::cx::isnan(f32__f_long_nan));
        REQUIRE(std::isnan(f32__f32_inf)         == nonstd::cx::isnan(f32__f32_inf));
        REQUIRE(std::isnan(f32__f64_inf)         == nonstd::cx::isnan(f32__f64_inf));
        REQUIRE(std::isnan(f32__f_long_inf)      == nonstd::cx::isnan(f32__f_long_inf));
        REQUIRE(std::isnan(f32__f32_less_min)    == nonstd::cx::isnan(f32__f32_less_min));
        REQUIRE(std::isnan(f32__f32_more_max)    == nonstd::cx::isnan(f32__f32_more_max));
        REQUIRE(std::isnan(f32__f64_less_min)    == nonstd::cx::isnan(f32__f64_less_min));
        REQUIRE(std::isnan(f32__f64_more_max)    == nonstd::cx::isnan(f32__f64_more_max));
        REQUIRE(std::isnan(f32__f_long_less_min) == nonstd::cx::isnan(f32__f_long_less_min));
        REQUIRE(std::isnan(f32__f_long_more_max) == nonstd::cx::isnan(f32__f_long_more_max));

        REQUIRE(std::isnan(f64__positive)        == nonstd::cx::isnan(f64__positive));
        REQUIRE(std::isnan(f64__negative)        == nonstd::cx::isnan(f64__negative));
        REQUIRE(std::isnan(f64__f32_min)         == nonstd::cx::isnan(f64__f32_min));
        REQUIRE(std::isnan(f64__f32_max)         == nonstd::cx::isnan(f64__f32_max));
        REQUIRE(std::isnan(f64__f64_min)         == nonstd::cx::isnan(f64__f64_min));
        REQUIRE(std::isnan(f64__f64_max)         == nonstd::cx::isnan(f64__f64_max));
        REQUIRE(std::isnan(f64__f_long_min)      == nonstd::cx::isnan(f64__f_long_min));
    //  REQUIRE(std::isnan(f64__f_long_max)      == nonstd::cx::isnan(f64__f_long_max));
        REQUIRE(std::isnan(f64__f32_nan)         == nonstd::cx::isnan(f64__f32_nan));
        REQUIRE(std::isnan(f64__f64_nan)         == nonstd::cx::isnan(f64__f64_nan));
        REQUIRE(std::isnan(f64__f_long_nan)      == nonstd::cx::isnan(f64__f_long_nan));
        REQUIRE(std::isnan(f64__f32_inf)         == nonstd::cx::isnan(f64__f32_inf));
        REQUIRE(std::isnan(f64__f64_inf)         == nonstd::cx::isnan(f64__f64_inf));
        REQUIRE(std::isnan(f64__f_long_inf)      == nonstd::cx::isnan(f64__f_long_inf));
        REQUIRE(std::isnan(f64__f32_less_min)    == nonstd::cx::isnan(f64__f32_less_min));
        REQUIRE(std::isnan(f64__f32_more_max)    == nonstd::cx::isnan(f64__f32_more_max));
        REQUIRE(std::isnan(f64__f64_less_min)    == nonstd::cx::isnan(f64__f64_less_min));
        REQUIRE(std::isnan(f64__f64_more_max)    == nonstd::cx::isnan(f64__f64_more_max));
        REQUIRE(std::isnan(f64__f_long_less_min) == nonstd::cx::isnan(f64__f_long_less_min));
        REQUIRE(std::isnan(f64__f_long_more_max) == nonstd::cx::isnan(f64__f_long_more_max));

        REQUIRE(std::isnan(f_long__positive)        == nonstd::cx::isnan(f_long__positive));
        REQUIRE(std::isnan(f_long__negative)        == nonstd::cx::isnan(f_long__negative));
        REQUIRE(std::isnan(f_long__f32_min)         == nonstd::cx::isnan(f_long__f32_min));
        REQUIRE(std::isnan(f_long__f32_max)         == nonstd::cx::isnan(f_long__f32_max));
        REQUIRE(std::isnan(f_long__f64_min)         == nonstd::cx::isnan(f_long__f64_min));
        REQUIRE(std::isnan(f_long__f64_max)         == nonstd::cx::isnan(f_long__f64_max));
        REQUIRE(std::isnan(f_long__f_long_min)      == nonstd::cx::isnan(f_long__f_long_min));
        REQUIRE(std::isnan(f_long__f_long_max)      == nonstd::cx::isnan(f_long__f_long_max));
        REQUIRE(std::isnan(f_long__f32_nan)         == nonstd::cx::isnan(f_long__f32_nan));
        REQUIRE(std::isnan(f_long__f64_nan)         == nonstd::cx::isnan(f_long__f64_nan));
        REQUIRE(std::isnan(f_long__f_long_nan)      == nonstd::cx::isnan(f_long__f_long_nan));
        REQUIRE(std::isnan(f_long__f32_inf)         == nonstd::cx::isnan(f_long__f32_inf));
        REQUIRE(std::isnan(f_long__f64_inf)         == nonstd::cx::isnan(f_long__f64_inf));
        REQUIRE(std::isnan(f_long__f_long_inf)      == nonstd::cx::isnan(f_long__f_long_inf));
        REQUIRE(std::isnan(f_long__f32_less_min)    == nonstd::cx::isnan(f_long__f32_less_min));
        REQUIRE(std::isnan(f_long__f32_more_max)    == nonstd::cx::isnan(f_long__f32_more_max));
        REQUIRE(std::isnan(f_long__f64_less_min)    == nonstd::cx::isnan(f_long__f64_less_min));
        REQUIRE(std::isnan(f_long__f64_more_max)    == nonstd::cx::isnan(f_long__f64_more_max));
        REQUIRE(std::isnan(f_long__f_long_less_min) == nonstd::cx::isnan(f_long__f_long_less_min));
        REQUIRE(std::isnan(f_long__f_long_more_max) == nonstd::cx::isnan(f_long__f_long_more_max));

#if !defined(NONSTD_COMPILER_MSVC)
        // MSVC fails on these std::isnan calls with an 'ambiguous overload'
        // error. This is out of spec, so keep testing these elsewhere.
        REQUIRE(std::isnan(u64__positive) == nonstd::cx::isnan(u64__positive));
        REQUIRE(std::isnan(u64__zero)     == nonstd::cx::isnan(u64__zero));
        REQUIRE(std::isnan(u64__max)      == nonstd::cx::isnan(u64__max));
        REQUIRE(std::isnan(i64__positive) == nonstd::cx::isnan(i64__positive));
        REQUIRE(std::isnan(i64__zero)     == nonstd::cx::isnan(i64__zero));
        REQUIRE(std::isnan(i64__negative) == nonstd::cx::isnan(i64__negative));
        REQUIRE(std::isnan(i64__min)      == nonstd::cx::isnan(i64__min));
        REQUIRE(std::isnan(i64__max)      == nonstd::cx::isnan(i64__max));
#endif
    }

    SECTION("abs") {
        constexpr auto calls_to_abs_are_constexpr = nonstd::cx::abs(f32__positive);

        using std::isnan;
        REQUIRE(( ( std::abs(f32__positive)        == nonstd::cx::abs(f32__positive) )        || ( isnan(std::abs(f32__positive))        && isnan(nonstd::cx::abs(f32__positive)) )        ));
        REQUIRE(( ( std::abs(f32__negative)        == nonstd::cx::abs(f32__negative) )        || ( isnan(std::abs(f32__negative))        && isnan(nonstd::cx::abs(f32__negative)) )        ));
        REQUIRE(( ( std::abs(f32__f32_min)         == nonstd::cx::abs(f32__f32_min) )         || ( isnan(std::abs(f32__f32_min))         && isnan(nonstd::cx::abs(f32__f32_min)) )         ));
        REQUIRE(( ( std::abs(f32__f32_max)         == nonstd::cx::abs(f32__f32_max) )         || ( isnan(std::abs(f32__f32_max))         && isnan(nonstd::cx::abs(f32__f32_max)) )         ));
        REQUIRE(( ( std::abs(f32__f64_min)         == nonstd::cx::abs(f32__f64_min) )         || ( isnan(std::abs(f32__f64_min))         && isnan(nonstd::cx::abs(f32__f64_min)) )         ));
    //  REQUIRE(( ( std::abs(f32__f64_max)         == nonstd::cx::abs(f32__f64_max) )         || ( isnan(std::abs(f32__f64_max))         && isnan(nonstd::cx::abs(f32__f64_max)) )         ));
        REQUIRE(( ( std::abs(f32__f_long_min)      == nonstd::cx::abs(f32__f_long_min) )      || ( isnan(std::abs(f32__f_long_min))      && isnan(nonstd::cx::abs(f32__f_long_min)) )      ));
    //  REQUIRE(( ( std::abs(f32__f_long_max)      == nonstd::cx::abs(f32__f_long_max) )      || ( isnan(std::abs(f32__f_long_max))      && isnan(nonstd::cx::abs(f32__f_long_max)) )      ));
        REQUIRE(( ( std::abs(f32__f32_nan)         == nonstd::cx::abs(f32__f32_nan) )         || ( isnan(std::abs(f32__f32_nan))         && isnan(nonstd::cx::abs(f32__f32_nan)) )         ));
        REQUIRE(( ( std::abs(f32__f64_nan)         == nonstd::cx::abs(f32__f64_nan) )         || ( isnan(std::abs(f32__f64_nan))         && isnan(nonstd::cx::abs(f32__f64_nan)) )         ));
        REQUIRE(( ( std::abs(f32__f_long_nan)      == nonstd::cx::abs(f32__f_long_nan) )      || ( isnan(std::abs(f32__f_long_nan))      && isnan(nonstd::cx::abs(f32__f_long_nan)) )      ));
        REQUIRE(( ( std::abs(f32__f32_inf)         == nonstd::cx::abs(f32__f32_inf) )         || ( isnan(std::abs(f32__f32_inf))         && isnan(nonstd::cx::abs(f32__f32_inf)) )         ));
        REQUIRE(( ( std::abs(f32__f64_inf)         == nonstd::cx::abs(f32__f64_inf) )         || ( isnan(std::abs(f32__f64_inf))         && isnan(nonstd::cx::abs(f32__f64_inf)) )         ));
        REQUIRE(( ( std::abs(f32__f_long_inf)      == nonstd::cx::abs(f32__f_long_inf) )      || ( isnan(std::abs(f32__f_long_inf))      && isnan(nonstd::cx::abs(f32__f_long_inf)) )      ));
        REQUIRE(( ( std::abs(f32__f32_less_min)    == nonstd::cx::abs(f32__f32_less_min) )    || ( isnan(std::abs(f32__f32_less_min))    && isnan(nonstd::cx::abs(f32__f32_less_min)) )    ));
        REQUIRE(( ( std::abs(f32__f32_more_max)    == nonstd::cx::abs(f32__f32_more_max) )    || ( isnan(std::abs(f32__f32_more_max))    && isnan(nonstd::cx::abs(f32__f32_more_max)) )    ));
        REQUIRE(( ( std::abs(f32__f64_less_min)    == nonstd::cx::abs(f32__f64_less_min) )    || ( isnan(std::abs(f32__f64_less_min))    && isnan(nonstd::cx::abs(f32__f64_less_min)) )    ));
        REQUIRE(( ( std::abs(f32__f64_more_max)    == nonstd::cx::abs(f32__f64_more_max) )    || ( isnan(std::abs(f32__f64_more_max))    && isnan(nonstd::cx::abs(f32__f64_more_max)) )    ));
        REQUIRE(( ( std::abs(f32__f_long_less_min) == nonstd::cx::abs(f32__f_long_less_min) ) || ( isnan(std::abs(f32__f_long_less_min)) && isnan(nonstd::cx::abs(f32__f_long_less_min)) ) ));
        REQUIRE(( ( std::abs(f32__f_long_more_max) == nonstd::cx::abs(f32__f_long_more_max) ) || ( isnan(std::abs(f32__f_long_more_max)) && isnan(nonstd::cx::abs(f32__f_long_more_max)) ) ));

        REQUIRE(( ( std::abs(f64__positive)        == nonstd::cx::abs(f64__positive) )        || ( isnan(std::abs(f64__positive))        && isnan(nonstd::cx::abs(f64__positive)) )        ));
        REQUIRE(( ( std::abs(f64__negative)        == nonstd::cx::abs(f64__negative) )        || ( isnan(std::abs(f64__negative))        && isnan(nonstd::cx::abs(f64__negative)) )        ));
        REQUIRE(( ( std::abs(f64__f32_min)         == nonstd::cx::abs(f64__f32_min) )         || ( isnan(std::abs(f64__f32_min))         && isnan(nonstd::cx::abs(f64__f32_min)) )         ));
        REQUIRE(( ( std::abs(f64__f32_max)         == nonstd::cx::abs(f64__f32_max) )         || ( isnan(std::abs(f64__f32_max))         && isnan(nonstd::cx::abs(f64__f32_max)) )         ));
        REQUIRE(( ( std::abs(f64__f64_min)         == nonstd::cx::abs(f64__f64_min) )         || ( isnan(std::abs(f64__f64_min))         && isnan(nonstd::cx::abs(f64__f64_min)) )         ));
        REQUIRE(( ( std::abs(f64__f64_max)         == nonstd::cx::abs(f64__f64_max) )         || ( isnan(std::abs(f64__f64_max))         && isnan(nonstd::cx::abs(f64__f64_max)) )         ));
        REQUIRE(( ( std::abs(f64__f_long_min)      == nonstd::cx::abs(f64__f_long_min) )      || ( isnan(std::abs(f64__f_long_min))      && isnan(nonstd::cx::abs(f64__f_long_min)) )      ));
    //  REQUIRE(( ( std::abs(f64__f_long_max)      == nonstd::cx::abs(f64__f_long_max) )      || ( isnan(std::abs(f64__f_long_max))      && isnan(nonstd::cx::abs(f64__f_long_max)) )      ));
        REQUIRE(( ( std::abs(f64__f32_nan)         == nonstd::cx::abs(f64__f32_nan) )         || ( isnan(std::abs(f64__f32_nan))         && isnan(nonstd::cx::abs(f64__f32_nan)) )         ));
        REQUIRE(( ( std::abs(f64__f64_nan)         == nonstd::cx::abs(f64__f64_nan) )         || ( isnan(std::abs(f64__f64_nan))         && isnan(nonstd::cx::abs(f64__f64_nan)) )         ));
        REQUIRE(( ( std::abs(f64__f_long_nan)      == nonstd::cx::abs(f64__f_long_nan) )      || ( isnan(std::abs(f64__f_long_nan))      && isnan(nonstd::cx::abs(f64__f_long_nan)) )      ));
        REQUIRE(( ( std::abs(f64__f32_inf)         == nonstd::cx::abs(f64__f32_inf) )         || ( isnan(std::abs(f64__f32_inf))         && isnan(nonstd::cx::abs(f64__f32_inf)) )         ));
        REQUIRE(( ( std::abs(f64__f64_inf)         == nonstd::cx::abs(f64__f64_inf) )         || ( isnan(std::abs(f64__f64_inf))         && isnan(nonstd::cx::abs(f64__f64_inf)) )         ));
        REQUIRE(( ( std::abs(f64__f_long_inf)      == nonstd::cx::abs(f64__f_long_inf) )      || ( isnan(std::abs(f64__f_long_inf))      && isnan(nonstd::cx::abs(f64__f_long_inf)) )      ));
        REQUIRE(( ( std::abs(f64__f32_less_min)    == nonstd::cx::abs(f64__f32_less_min) )    || ( isnan(std::abs(f64__f32_less_min))    && isnan(nonstd::cx::abs(f64__f32_less_min)) )    ));
        REQUIRE(( ( std::abs(f64__f32_more_max)    == nonstd::cx::abs(f64__f32_more_max) )    || ( isnan(std::abs(f64__f32_more_max))    && isnan(nonstd::cx::abs(f64__f32_more_max)) )    ));
        REQUIRE(( ( std::abs(f64__f64_less_min)    == nonstd::cx::abs(f64__f64_less_min) )    || ( isnan(std::abs(f64__f64_less_min))    && isnan(nonstd::cx::abs(f64__f64_less_min)) )    ));
        REQUIRE(( ( std::abs(f64__f64_more_max)    == nonstd::cx::abs(f64__f64_more_max) )    || ( isnan(std::abs(f64__f64_more_max))    && isnan(nonstd::cx::abs(f64__f64_more_max)) )    ));
        REQUIRE(( ( std::abs(f64__f_long_less_min) == nonstd::cx::abs(f64__f_long_less_min) ) || ( isnan(std::abs(f64__f_long_less_min)) && isnan(nonstd::cx::abs(f64__f_long_less_min)) ) ));
        REQUIRE(( ( std::abs(f64__f_long_more_max) == nonstd::cx::abs(f64__f_long_more_max) ) || ( isnan(std::abs(f64__f_long_more_max)) && isnan(nonstd::cx::abs(f64__f_long_more_max)) ) ));

        REQUIRE(( ( std::abs(f_long__positive)        == nonstd::cx::abs(f_long__positive) )        || ( isnan(std::abs(f_long__positive))        && isnan(nonstd::cx::abs(f_long__positive)) )        ));
        REQUIRE(( ( std::abs(f_long__negative)        == nonstd::cx::abs(f_long__negative) )        || ( isnan(std::abs(f_long__negative))        && isnan(nonstd::cx::abs(f_long__negative)) )        ));
        REQUIRE(( ( std::abs(f_long__f32_min)         == nonstd::cx::abs(f_long__f32_min) )         || ( isnan(std::abs(f_long__f32_min))         && isnan(nonstd::cx::abs(f_long__f32_min)) )         ));
        REQUIRE(( ( std::abs(f_long__f32_max)         == nonstd::cx::abs(f_long__f32_max) )         || ( isnan(std::abs(f_long__f32_max))         && isnan(nonstd::cx::abs(f_long__f32_max)) )         ));
        REQUIRE(( ( std::abs(f_long__f64_min)         == nonstd::cx::abs(f_long__f64_min) )         || ( isnan(std::abs(f_long__f64_min))         && isnan(nonstd::cx::abs(f_long__f64_min)) )         ));
        REQUIRE(( ( std::abs(f_long__f64_max)         == nonstd::cx::abs(f_long__f64_max) )         || ( isnan(std::abs(f_long__f64_max))         && isnan(nonstd::cx::abs(f_long__f64_max)) )         ));
        REQUIRE(( ( std::abs(f_long__f_long_min)      == nonstd::cx::abs(f_long__f_long_min) )      || ( isnan(std::abs(f_long__f_long_min))      && isnan(nonstd::cx::abs(f_long__f_long_min)) )      ));
        REQUIRE(( ( std::abs(f_long__f_long_max)      == nonstd::cx::abs(f_long__f_long_max) )      || ( isnan(std::abs(f_long__f_long_max))      && isnan(nonstd::cx::abs(f_long__f_long_max)) )      ));
        REQUIRE(( ( std::abs(f_long__f32_nan)         == nonstd::cx::abs(f_long__f32_nan) )         || ( isnan(std::abs(f_long__f32_nan))         && isnan(nonstd::cx::abs(f_long__f32_nan)) )         ));
        REQUIRE(( ( std::abs(f_long__f64_nan)         == nonstd::cx::abs(f_long__f64_nan) )         || ( isnan(std::abs(f_long__f64_nan))         && isnan(nonstd::cx::abs(f_long__f64_nan)) )         ));
        REQUIRE(( ( std::abs(f_long__f_long_nan)      == nonstd::cx::abs(f_long__f_long_nan) )      || ( isnan(std::abs(f_long__f_long_nan))      && isnan(nonstd::cx::abs(f_long__f_long_nan)) )      ));
        REQUIRE(( ( std::abs(f_long__f32_inf)         == nonstd::cx::abs(f_long__f32_inf) )         || ( isnan(std::abs(f_long__f32_inf))         && isnan(nonstd::cx::abs(f_long__f32_inf)) )         ));
        REQUIRE(( ( std::abs(f_long__f64_inf)         == nonstd::cx::abs(f_long__f64_inf) )         || ( isnan(std::abs(f_long__f64_inf))         && isnan(nonstd::cx::abs(f_long__f64_inf)) )         ));
        REQUIRE(( ( std::abs(f_long__f_long_inf)      == nonstd::cx::abs(f_long__f_long_inf) )      || ( isnan(std::abs(f_long__f_long_inf))      && isnan(nonstd::cx::abs(f_long__f_long_inf)) )      ));
        REQUIRE(( ( std::abs(f_long__f32_less_min)    == nonstd::cx::abs(f_long__f32_less_min) )    || ( isnan(std::abs(f_long__f32_less_min))    && isnan(nonstd::cx::abs(f_long__f32_less_min)) )    ));
        REQUIRE(( ( std::abs(f_long__f32_more_max)    == nonstd::cx::abs(f_long__f32_more_max) )    || ( isnan(std::abs(f_long__f32_more_max))    && isnan(nonstd::cx::abs(f_long__f32_more_max)) )    ));
        REQUIRE(( ( std::abs(f_long__f64_less_min)    == nonstd::cx::abs(f_long__f64_less_min) )    || ( isnan(std::abs(f_long__f64_less_min))    && isnan(nonstd::cx::abs(f_long__f64_less_min)) )    ));
        REQUIRE(( ( std::abs(f_long__f64_more_max)    == nonstd::cx::abs(f_long__f64_more_max) )    || ( isnan(std::abs(f_long__f64_more_max))    && isnan(nonstd::cx::abs(f_long__f64_more_max)) )    ));
        REQUIRE(( ( std::abs(f_long__f_long_less_min) == nonstd::cx::abs(f_long__f_long_less_min) ) || ( isnan(std::abs(f_long__f_long_less_min)) && isnan(nonstd::cx::abs(f_long__f_long_less_min)) ) ));
        REQUIRE(( ( std::abs(f_long__f_long_more_max) == nonstd::cx::abs(f_long__f_long_more_max) ) || ( isnan(std::abs(f_long__f_long_more_max)) && isnan(nonstd::cx::abs(f_long__f_long_more_max)) ) ));

        // std::abs doesn't hav Integral overloads. Because of this,
        // these functions are ambiguous.

        // REQUIRE(( ( std::abs(u64__positive) == nonstd::cx::abs(u64__positive) ) || ( isnan(std::abs(u64__positive)) && isnan(nonstd::cx::abs(u64__positive)) ) ));
        // REQUIRE(( ( std::abs(u64__zero)     == nonstd::cx::abs(u64__zero) )     || ( isnan(std::abs(u64__zero))     && isnan(nonstd::cx::abs(u64__zero)) )     ));
        // REQUIRE(( ( std::abs(u64__max)      == nonstd::cx::abs(u64__max) )      || ( isnan(std::abs(u64__max))      && isnan(nonstd::cx::abs(u64__max)) )      ));
        // REQUIRE(( ( std::abs(i64__positive) == nonstd::cx::abs(i64__positive) ) || ( isnan(std::abs(i64__positive)) && isnan(nonstd::cx::abs(i64__positive)) ) ));
        // REQUIRE(( ( std::abs(i64__zero)     == nonstd::cx::abs(i64__zero) )     || ( isnan(std::abs(i64__zero))     && isnan(nonstd::cx::abs(i64__zero)) )     ));
        // REQUIRE(( ( std::abs(i64__negative) == nonstd::cx::abs(i64__negative) ) || ( isnan(std::abs(i64__negative)) && isnan(nonstd::cx::abs(i64__negative)) ) ));
        // REQUIRE(( ( std::abs(i64__min)      == nonstd::cx::abs(i64__min) )      || ( isnan(std::abs(i64__min))      && isnan(nonstd::cx::abs(i64__min)) )      ));
        // REQUIRE(( ( std::abs(i64__max)      == nonstd::cx::abs(i64__max) )      || ( isnan(std::abs(i64__max))      && isnan(nonstd::cx::abs(i64__max)) )      ));
    }

    SECTION("fabs") {
        constexpr auto calls_to_fabs_are_constexpr = nonstd::cx::fabs(f32__positive);

        using std::isnan;
        REQUIRE(( ( std::fabs(f32__positive)        == nonstd::cx::fabs(f32__positive) )        || ( isnan(std::fabs(f32__positive))        && isnan(nonstd::cx::fabs(f32__positive)) )        ));
        REQUIRE(( ( std::fabs(f32__negative)        == nonstd::cx::fabs(f32__negative) )        || ( isnan(std::fabs(f32__negative))        && isnan(nonstd::cx::fabs(f32__negative)) )        ));
        REQUIRE(( ( std::fabs(f32__f32_min)         == nonstd::cx::fabs(f32__f32_min) )         || ( isnan(std::fabs(f32__f32_min))         && isnan(nonstd::cx::fabs(f32__f32_min)) )         ));
        REQUIRE(( ( std::fabs(f32__f32_max)         == nonstd::cx::fabs(f32__f32_max) )         || ( isnan(std::fabs(f32__f32_max))         && isnan(nonstd::cx::fabs(f32__f32_max)) )         ));
        REQUIRE(( ( std::fabs(f32__f64_min)         == nonstd::cx::fabs(f32__f64_min) )         || ( isnan(std::fabs(f32__f64_min))         && isnan(nonstd::cx::fabs(f32__f64_min)) )         ));
    //  REQUIRE(( ( std::fabs(f32__f64_max)         == nonstd::cx::fabs(f32__f64_max) )         || ( isnan(std::fabs(f32__f64_max))         && isnan(nonstd::cx::fabs(f32__f64_max)) )         ));
        REQUIRE(( ( std::fabs(f32__f_long_min)      == nonstd::cx::fabs(f32__f_long_min) )      || ( isnan(std::fabs(f32__f_long_min))      && isnan(nonstd::cx::fabs(f32__f_long_min)) )      ));
    //  REQUIRE(( ( std::fabs(f32__f_long_max)      == nonstd::cx::fabs(f32__f_long_max) )      || ( isnan(std::fabs(f32__f_long_max))      && isnan(nonstd::cx::fabs(f32__f_long_max)) )      ));
        REQUIRE(( ( std::fabs(f32__f32_nan)         == nonstd::cx::fabs(f32__f32_nan) )         || ( isnan(std::fabs(f32__f32_nan))         && isnan(nonstd::cx::fabs(f32__f32_nan)) )         ));
        REQUIRE(( ( std::fabs(f32__f64_nan)         == nonstd::cx::fabs(f32__f64_nan) )         || ( isnan(std::fabs(f32__f64_nan))         && isnan(nonstd::cx::fabs(f32__f64_nan)) )         ));
        REQUIRE(( ( std::fabs(f32__f_long_nan)      == nonstd::cx::fabs(f32__f_long_nan) )      || ( isnan(std::fabs(f32__f_long_nan))      && isnan(nonstd::cx::fabs(f32__f_long_nan)) )      ));
        REQUIRE(( ( std::fabs(f32__f32_inf)         == nonstd::cx::fabs(f32__f32_inf) )         || ( isnan(std::fabs(f32__f32_inf))         && isnan(nonstd::cx::fabs(f32__f32_inf)) )         ));
        REQUIRE(( ( std::fabs(f32__f64_inf)         == nonstd::cx::fabs(f32__f64_inf) )         || ( isnan(std::fabs(f32__f64_inf))         && isnan(nonstd::cx::fabs(f32__f64_inf)) )         ));
        REQUIRE(( ( std::fabs(f32__f_long_inf)      == nonstd::cx::fabs(f32__f_long_inf) )      || ( isnan(std::fabs(f32__f_long_inf))      && isnan(nonstd::cx::fabs(f32__f_long_inf)) )      ));
        REQUIRE(( ( std::fabs(f32__f32_less_min)    == nonstd::cx::fabs(f32__f32_less_min) )    || ( isnan(std::fabs(f32__f32_less_min))    && isnan(nonstd::cx::fabs(f32__f32_less_min)) )    ));
        REQUIRE(( ( std::fabs(f32__f32_more_max)    == nonstd::cx::fabs(f32__f32_more_max) )    || ( isnan(std::fabs(f32__f32_more_max))    && isnan(nonstd::cx::fabs(f32__f32_more_max)) )    ));
        REQUIRE(( ( std::fabs(f32__f64_less_min)    == nonstd::cx::fabs(f32__f64_less_min) )    || ( isnan(std::fabs(f32__f64_less_min))    && isnan(nonstd::cx::fabs(f32__f64_less_min)) )    ));
        REQUIRE(( ( std::fabs(f32__f64_more_max)    == nonstd::cx::fabs(f32__f64_more_max) )    || ( isnan(std::fabs(f32__f64_more_max))    && isnan(nonstd::cx::fabs(f32__f64_more_max)) )    ));
        REQUIRE(( ( std::fabs(f32__f_long_less_min) == nonstd::cx::fabs(f32__f_long_less_min) ) || ( isnan(std::fabs(f32__f_long_less_min)) && isnan(nonstd::cx::fabs(f32__f_long_less_min)) ) ));
        REQUIRE(( ( std::fabs(f32__f_long_more_max) == nonstd::cx::fabs(f32__f_long_more_max) ) || ( isnan(std::fabs(f32__f_long_more_max)) && isnan(nonstd::cx::fabs(f32__f_long_more_max)) ) ));

        REQUIRE(( ( std::fabs(f64__positive)        == nonstd::cx::fabs(f64__positive) )        || ( isnan(std::fabs(f64__positive))        && isnan(nonstd::cx::fabs(f64__positive)) )        ));
        REQUIRE(( ( std::fabs(f64__negative)        == nonstd::cx::fabs(f64__negative) )        || ( isnan(std::fabs(f64__negative))        && isnan(nonstd::cx::fabs(f64__negative)) )        ));
        REQUIRE(( ( std::fabs(f64__f32_min)         == nonstd::cx::fabs(f64__f32_min) )         || ( isnan(std::fabs(f64__f32_min))         && isnan(nonstd::cx::fabs(f64__f32_min)) )         ));
        REQUIRE(( ( std::fabs(f64__f32_max)         == nonstd::cx::fabs(f64__f32_max) )         || ( isnan(std::fabs(f64__f32_max))         && isnan(nonstd::cx::fabs(f64__f32_max)) )         ));
        REQUIRE(( ( std::fabs(f64__f64_min)         == nonstd::cx::fabs(f64__f64_min) )         || ( isnan(std::fabs(f64__f64_min))         && isnan(nonstd::cx::fabs(f64__f64_min)) )         ));
        REQUIRE(( ( std::fabs(f64__f64_max)         == nonstd::cx::fabs(f64__f64_max) )         || ( isnan(std::fabs(f64__f64_max))         && isnan(nonstd::cx::fabs(f64__f64_max)) )         ));
        REQUIRE(( ( std::fabs(f64__f_long_min)      == nonstd::cx::fabs(f64__f_long_min) )      || ( isnan(std::fabs(f64__f_long_min))      && isnan(nonstd::cx::fabs(f64__f_long_min)) )      ));
    //  REQUIRE(( ( std::fabs(f64__f_long_max)      == nonstd::cx::fabs(f64__f_long_max) )      || ( isnan(std::fabs(f64__f_long_max))      && isnan(nonstd::cx::fabs(f64__f_long_max)) )      ));
        REQUIRE(( ( std::fabs(f64__f32_nan)         == nonstd::cx::fabs(f64__f32_nan) )         || ( isnan(std::fabs(f64__f32_nan))         && isnan(nonstd::cx::fabs(f64__f32_nan)) )         ));
        REQUIRE(( ( std::fabs(f64__f64_nan)         == nonstd::cx::fabs(f64__f64_nan) )         || ( isnan(std::fabs(f64__f64_nan))         && isnan(nonstd::cx::fabs(f64__f64_nan)) )         ));
        REQUIRE(( ( std::fabs(f64__f_long_nan)      == nonstd::cx::fabs(f64__f_long_nan) )      || ( isnan(std::fabs(f64__f_long_nan))      && isnan(nonstd::cx::fabs(f64__f_long_nan)) )      ));
        REQUIRE(( ( std::fabs(f64__f32_inf)         == nonstd::cx::fabs(f64__f32_inf) )         || ( isnan(std::fabs(f64__f32_inf))         && isnan(nonstd::cx::fabs(f64__f32_inf)) )         ));
        REQUIRE(( ( std::fabs(f64__f64_inf)         == nonstd::cx::fabs(f64__f64_inf) )         || ( isnan(std::fabs(f64__f64_inf))         && isnan(nonstd::cx::fabs(f64__f64_inf)) )         ));
        REQUIRE(( ( std::fabs(f64__f_long_inf)      == nonstd::cx::fabs(f64__f_long_inf) )      || ( isnan(std::fabs(f64__f_long_inf))      && isnan(nonstd::cx::fabs(f64__f_long_inf)) )      ));
        REQUIRE(( ( std::fabs(f64__f32_less_min)    == nonstd::cx::fabs(f64__f32_less_min) )    || ( isnan(std::fabs(f64__f32_less_min))    && isnan(nonstd::cx::fabs(f64__f32_less_min)) )    ));
        REQUIRE(( ( std::fabs(f64__f32_more_max)    == nonstd::cx::fabs(f64__f32_more_max) )    || ( isnan(std::fabs(f64__f32_more_max))    && isnan(nonstd::cx::fabs(f64__f32_more_max)) )    ));
        REQUIRE(( ( std::fabs(f64__f64_less_min)    == nonstd::cx::fabs(f64__f64_less_min) )    || ( isnan(std::fabs(f64__f64_less_min))    && isnan(nonstd::cx::fabs(f64__f64_less_min)) )    ));
        REQUIRE(( ( std::fabs(f64__f64_more_max)    == nonstd::cx::fabs(f64__f64_more_max) )    || ( isnan(std::fabs(f64__f64_more_max))    && isnan(nonstd::cx::fabs(f64__f64_more_max)) )    ));
        REQUIRE(( ( std::fabs(f64__f_long_less_min) == nonstd::cx::fabs(f64__f_long_less_min) ) || ( isnan(std::fabs(f64__f_long_less_min)) && isnan(nonstd::cx::fabs(f64__f_long_less_min)) ) ));
        REQUIRE(( ( std::fabs(f64__f_long_more_max) == nonstd::cx::fabs(f64__f_long_more_max) ) || ( isnan(std::fabs(f64__f_long_more_max)) && isnan(nonstd::cx::fabs(f64__f_long_more_max)) ) ));

        REQUIRE(( ( std::fabs(f_long__positive)        == nonstd::cx::fabs(f_long__positive) )        || ( isnan(std::fabs(f_long__positive))        && isnan(nonstd::cx::fabs(f_long__positive)) )        ));
        REQUIRE(( ( std::fabs(f_long__negative)        == nonstd::cx::fabs(f_long__negative) )        || ( isnan(std::fabs(f_long__negative))        && isnan(nonstd::cx::fabs(f_long__negative)) )        ));
        REQUIRE(( ( std::fabs(f_long__f32_min)         == nonstd::cx::fabs(f_long__f32_min) )         || ( isnan(std::fabs(f_long__f32_min))         && isnan(nonstd::cx::fabs(f_long__f32_min)) )         ));
        REQUIRE(( ( std::fabs(f_long__f32_max)         == nonstd::cx::fabs(f_long__f32_max) )         || ( isnan(std::fabs(f_long__f32_max))         && isnan(nonstd::cx::fabs(f_long__f32_max)) )         ));
        REQUIRE(( ( std::fabs(f_long__f64_min)         == nonstd::cx::fabs(f_long__f64_min) )         || ( isnan(std::fabs(f_long__f64_min))         && isnan(nonstd::cx::fabs(f_long__f64_min)) )         ));
        REQUIRE(( ( std::fabs(f_long__f64_max)         == nonstd::cx::fabs(f_long__f64_max) )         || ( isnan(std::fabs(f_long__f64_max))         && isnan(nonstd::cx::fabs(f_long__f64_max)) )         ));
        REQUIRE(( ( std::fabs(f_long__f_long_min)      == nonstd::cx::fabs(f_long__f_long_min) )      || ( isnan(std::fabs(f_long__f_long_min))      && isnan(nonstd::cx::fabs(f_long__f_long_min)) )      ));
        REQUIRE(( ( std::fabs(f_long__f_long_max)      == nonstd::cx::fabs(f_long__f_long_max) )      || ( isnan(std::fabs(f_long__f_long_max))      && isnan(nonstd::cx::fabs(f_long__f_long_max)) )      ));
        REQUIRE(( ( std::fabs(f_long__f32_nan)         == nonstd::cx::fabs(f_long__f32_nan) )         || ( isnan(std::fabs(f_long__f32_nan))         && isnan(nonstd::cx::fabs(f_long__f32_nan)) )         ));
        REQUIRE(( ( std::fabs(f_long__f64_nan)         == nonstd::cx::fabs(f_long__f64_nan) )         || ( isnan(std::fabs(f_long__f64_nan))         && isnan(nonstd::cx::fabs(f_long__f64_nan)) )         ));
        REQUIRE(( ( std::fabs(f_long__f_long_nan)      == nonstd::cx::fabs(f_long__f_long_nan) )      || ( isnan(std::fabs(f_long__f_long_nan))      && isnan(nonstd::cx::fabs(f_long__f_long_nan)) )      ));
        REQUIRE(( ( std::fabs(f_long__f32_inf)         == nonstd::cx::fabs(f_long__f32_inf) )         || ( isnan(std::fabs(f_long__f32_inf))         && isnan(nonstd::cx::fabs(f_long__f32_inf)) )         ));
        REQUIRE(( ( std::fabs(f_long__f64_inf)         == nonstd::cx::fabs(f_long__f64_inf) )         || ( isnan(std::fabs(f_long__f64_inf))         && isnan(nonstd::cx::fabs(f_long__f64_inf)) )         ));
        REQUIRE(( ( std::fabs(f_long__f_long_inf)      == nonstd::cx::fabs(f_long__f_long_inf) )      || ( isnan(std::fabs(f_long__f_long_inf))      && isnan(nonstd::cx::fabs(f_long__f_long_inf)) )      ));
        REQUIRE(( ( std::fabs(f_long__f32_less_min)    == nonstd::cx::fabs(f_long__f32_less_min) )    || ( isnan(std::fabs(f_long__f32_less_min))    && isnan(nonstd::cx::fabs(f_long__f32_less_min)) )    ));
        REQUIRE(( ( std::fabs(f_long__f32_more_max)    == nonstd::cx::fabs(f_long__f32_more_max) )    || ( isnan(std::fabs(f_long__f32_more_max))    && isnan(nonstd::cx::fabs(f_long__f32_more_max)) )    ));
        REQUIRE(( ( std::fabs(f_long__f64_less_min)    == nonstd::cx::fabs(f_long__f64_less_min) )    || ( isnan(std::fabs(f_long__f64_less_min))    && isnan(nonstd::cx::fabs(f_long__f64_less_min)) )    ));
        REQUIRE(( ( std::fabs(f_long__f64_more_max)    == nonstd::cx::fabs(f_long__f64_more_max) )    || ( isnan(std::fabs(f_long__f64_more_max))    && isnan(nonstd::cx::fabs(f_long__f64_more_max)) )    ));
        REQUIRE(( ( std::fabs(f_long__f_long_less_min) == nonstd::cx::fabs(f_long__f_long_less_min) ) || ( isnan(std::fabs(f_long__f_long_less_min)) && isnan(nonstd::cx::fabs(f_long__f_long_less_min)) ) ));
        REQUIRE(( ( std::fabs(f_long__f_long_more_max) == nonstd::cx::fabs(f_long__f_long_more_max) ) || ( isnan(std::fabs(f_long__f_long_more_max)) && isnan(nonstd::cx::fabs(f_long__f_long_more_max)) ) ));

        REQUIRE(( ( std::fabs(u64__positive) == nonstd::cx::fabs(u64__positive) ) || ( isnan(std::fabs(u64__positive)) && isnan(nonstd::cx::fabs(u64__positive)) ) ));
        REQUIRE(( ( std::fabs(u64__zero)     == nonstd::cx::fabs(u64__zero) )     || ( isnan(std::fabs(u64__zero))     && isnan(nonstd::cx::fabs(u64__zero)) )     ));
        REQUIRE(( ( std::fabs(u64__max)      == nonstd::cx::fabs(u64__max) )      || ( isnan(std::fabs(u64__max))      && isnan(nonstd::cx::fabs(u64__max)) )      ));
        REQUIRE(( ( std::fabs(i64__positive) == nonstd::cx::fabs(i64__positive) ) || ( isnan(std::fabs(i64__positive)) && isnan(nonstd::cx::fabs(i64__positive)) ) ));
        REQUIRE(( ( std::fabs(i64__zero)     == nonstd::cx::fabs(i64__zero) )     || ( isnan(std::fabs(i64__zero))     && isnan(nonstd::cx::fabs(i64__zero)) )     ));
        REQUIRE(( ( std::fabs(i64__negative) == nonstd::cx::fabs(i64__negative) ) || ( isnan(std::fabs(i64__negative)) && isnan(nonstd::cx::fabs(i64__negative)) ) ));
        REQUIRE(( ( std::fabs(i64__min)      == nonstd::cx::fabs(i64__min) )      || ( isnan(std::fabs(i64__min))      && isnan(nonstd::cx::fabs(i64__min)) )      ));
        REQUIRE(( ( std::fabs(i64__max)      == nonstd::cx::fabs(i64__max) )      || ( isnan(std::fabs(i64__max))      && isnan(nonstd::cx::fabs(i64__max)) )      ));
    }

    SECTION("ceil") {
        constexpr auto calls_to_ceil_are_constexpr = nonstd::cx::ceil(f32__positive);

        using std::isnan;
        // ceil and floor don't much benefit from extents testing, and the
        // results from those functions tend to be estimations. It's more
        // important to verify exceptional (INFINITY, NaN) cases are consistent.
        REQUIRE(( ( std::ceil(f32__positive)        == nonstd::cx::ceil(f32__positive) )        || ( isnan(std::ceil(f32__positive))        && isnan(nonstd::cx::ceil(f32__positive)) )        ));
        REQUIRE(( ( std::ceil(f32__negative)        == nonstd::cx::ceil(f32__negative) )        || ( isnan(std::ceil(f32__negative))        && isnan(nonstd::cx::ceil(f32__negative)) )        ));
        // REQUIRE(( ( std::ceil(f32__f32_min)         == nonstd::cx::ceil(f32__f32_min) )         || ( isnan(std::ceil(f32__f32_min))         && isnan(nonstd::cx::ceil(f32__f32_min)) )         ));
        // REQUIRE(( ( std::ceil(f32__f32_max)         == nonstd::cx::ceil(f32__f32_max) )         || ( isnan(std::ceil(f32__f32_max))         && isnan(nonstd::cx::ceil(f32__f32_max)) )         ));
        // REQUIRE(( ( std::ceil(f32__f64_min)         == nonstd::cx::ceil(f32__f64_min) )         || ( isnan(std::ceil(f32__f64_min))         && isnan(nonstd::cx::ceil(f32__f64_min)) )         ));
    //  // REQUIRE(( ( std::ceil(f32__f64_max)         == nonstd::cx::ceil(f32__f64_max) )         || ( isnan(std::ceil(f32__f64_max))         && isnan(nonstd::cx::ceil(f32__f64_max)) )         ));
        // REQUIRE(( ( std::ceil(f32__f_long_min)      == nonstd::cx::ceil(f32__f_long_min) )      || ( isnan(std::ceil(f32__f_long_min))      && isnan(nonstd::cx::ceil(f32__f_long_min)) )      ));
    //  // REQUIRE(( ( std::ceil(f32__f_long_max)      == nonstd::cx::ceil(f32__f_long_max) )      || ( isnan(std::ceil(f32__f_long_max))      && isnan(nonstd::cx::ceil(f32__f_long_max)) )      ));
        REQUIRE(( ( std::ceil(f32__f32_nan)         == nonstd::cx::ceil(f32__f32_nan) )         || ( isnan(std::ceil(f32__f32_nan))         && isnan(nonstd::cx::ceil(f32__f32_nan)) )         ));
        REQUIRE(( ( std::ceil(f32__f64_nan)         == nonstd::cx::ceil(f32__f64_nan) )         || ( isnan(std::ceil(f32__f64_nan))         && isnan(nonstd::cx::ceil(f32__f64_nan)) )         ));
        REQUIRE(( ( std::ceil(f32__f_long_nan)      == nonstd::cx::ceil(f32__f_long_nan) )      || ( isnan(std::ceil(f32__f_long_nan))      && isnan(nonstd::cx::ceil(f32__f_long_nan)) )      ));
        REQUIRE(( ( std::ceil(f32__f32_inf)         == nonstd::cx::ceil(f32__f32_inf) )         || ( isnan(std::ceil(f32__f32_inf))         && isnan(nonstd::cx::ceil(f32__f32_inf)) )         ));
        REQUIRE(( ( std::ceil(f32__f64_inf)         == nonstd::cx::ceil(f32__f64_inf) )         || ( isnan(std::ceil(f32__f64_inf))         && isnan(nonstd::cx::ceil(f32__f64_inf)) )         ));
        REQUIRE(( ( std::ceil(f32__f_long_inf)      == nonstd::cx::ceil(f32__f_long_inf) )      || ( isnan(std::ceil(f32__f_long_inf))      && isnan(nonstd::cx::ceil(f32__f_long_inf)) )      ));
        // REQUIRE(( ( std::ceil(f32__f32_less_min)    == nonstd::cx::ceil(f32__f32_less_min) )    || ( isnan(std::ceil(f32__f32_less_min))    && isnan(nonstd::cx::ceil(f32__f32_less_min)) )    ));
        // REQUIRE(( ( std::ceil(f32__f32_more_max)    == nonstd::cx::ceil(f32__f32_more_max) )    || ( isnan(std::ceil(f32__f32_more_max))    && isnan(nonstd::cx::ceil(f32__f32_more_max)) )    ));
        // REQUIRE(( ( std::ceil(f32__f64_less_min)    == nonstd::cx::ceil(f32__f64_less_min) )    || ( isnan(std::ceil(f32__f64_less_min))    && isnan(nonstd::cx::ceil(f32__f64_less_min)) )    ));
        // REQUIRE(( ( std::ceil(f32__f64_more_max)    == nonstd::cx::ceil(f32__f64_more_max) )    || ( isnan(std::ceil(f32__f64_more_max))    && isnan(nonstd::cx::ceil(f32__f64_more_max)) )    ));
        // REQUIRE(( ( std::ceil(f32__f_long_less_min) == nonstd::cx::ceil(f32__f_long_less_min) ) || ( isnan(std::ceil(f32__f_long_less_min)) && isnan(nonstd::cx::ceil(f32__f_long_less_min)) ) ));
        // REQUIRE(( ( std::ceil(f32__f_long_more_max) == nonstd::cx::ceil(f32__f_long_more_max) ) || ( isnan(std::ceil(f32__f_long_more_max)) && isnan(nonstd::cx::ceil(f32__f_long_more_max)) ) ));

        REQUIRE(( ( std::ceil(f64__positive)        == nonstd::cx::ceil(f64__positive) )        || ( isnan(std::ceil(f64__positive))        && isnan(nonstd::cx::ceil(f64__positive)) )        ));
        REQUIRE(( ( std::ceil(f64__negative)        == nonstd::cx::ceil(f64__negative) )        || ( isnan(std::ceil(f64__negative))        && isnan(nonstd::cx::ceil(f64__negative)) )        ));
        //  REQUIRE(( ( std::ceil(f64__f32_min)         == nonstd::cx::ceil(f64__f32_min) )         || ( isnan(std::ceil(f64__f32_min))         && isnan(nonstd::cx::ceil(f64__f32_min)) )         ));
        //  REQUIRE(( ( std::ceil(f64__f32_max)         == nonstd::cx::ceil(f64__f32_max) )         || ( isnan(std::ceil(f64__f32_max))         && isnan(nonstd::cx::ceil(f64__f32_max)) )         ));
        //  REQUIRE(( ( std::ceil(f64__f64_min)         == nonstd::cx::ceil(f64__f64_min) )         || ( isnan(std::ceil(f64__f64_min))         && isnan(nonstd::cx::ceil(f64__f64_min)) )         ));
        //  REQUIRE(( ( std::ceil(f64__f64_max)         == nonstd::cx::ceil(f64__f64_max) )         || ( isnan(std::ceil(f64__f64_max))         && isnan(nonstd::cx::ceil(f64__f64_max)) )         ));
        //  REQUIRE(( ( std::ceil(f64__f_long_min)      == nonstd::cx::ceil(f64__f_long_min) )      || ( isnan(std::ceil(f64__f_long_min))      && isnan(nonstd::cx::ceil(f64__f_long_min)) )      ));
    //  //  REQUIRE(( ( std::ceil(f64__f_long_max)      == nonstd::cx::ceil(f64__f_long_max) )      || ( isnan(std::ceil(f64__f_long_max))      && isnan(nonstd::cx::ceil(f64__f_long_max)) )      ));
        REQUIRE(( ( std::ceil(f64__f32_nan)         == nonstd::cx::ceil(f64__f32_nan) )         || ( isnan(std::ceil(f64__f32_nan))         && isnan(nonstd::cx::ceil(f64__f32_nan)) )         ));
        REQUIRE(( ( std::ceil(f64__f64_nan)         == nonstd::cx::ceil(f64__f64_nan) )         || ( isnan(std::ceil(f64__f64_nan))         && isnan(nonstd::cx::ceil(f64__f64_nan)) )         ));
        REQUIRE(( ( std::ceil(f64__f_long_nan)      == nonstd::cx::ceil(f64__f_long_nan) )      || ( isnan(std::ceil(f64__f_long_nan))      && isnan(nonstd::cx::ceil(f64__f_long_nan)) )      ));
        REQUIRE(( ( std::ceil(f64__f32_inf)         == nonstd::cx::ceil(f64__f32_inf) )         || ( isnan(std::ceil(f64__f32_inf))         && isnan(nonstd::cx::ceil(f64__f32_inf)) )         ));
        REQUIRE(( ( std::ceil(f64__f64_inf)         == nonstd::cx::ceil(f64__f64_inf) )         || ( isnan(std::ceil(f64__f64_inf))         && isnan(nonstd::cx::ceil(f64__f64_inf)) )         ));
        REQUIRE(( ( std::ceil(f64__f_long_inf)      == nonstd::cx::ceil(f64__f_long_inf) )      || ( isnan(std::ceil(f64__f_long_inf))      && isnan(nonstd::cx::ceil(f64__f_long_inf)) )      ));
        // REQUIRE(( ( std::ceil(f64__f32_less_min)    == nonstd::cx::ceil(f64__f32_less_min) )    || ( isnan(std::ceil(f64__f32_less_min))    && isnan(nonstd::cx::ceil(f64__f32_less_min)) )    ));
        // REQUIRE(( ( std::ceil(f64__f32_more_max)    == nonstd::cx::ceil(f64__f32_more_max) )    || ( isnan(std::ceil(f64__f32_more_max))    && isnan(nonstd::cx::ceil(f64__f32_more_max)) )    ));
        // REQUIRE(( ( std::ceil(f64__f64_less_min)    == nonstd::cx::ceil(f64__f64_less_min) )    || ( isnan(std::ceil(f64__f64_less_min))    && isnan(nonstd::cx::ceil(f64__f64_less_min)) )    ));
        // REQUIRE(( ( std::ceil(f64__f64_more_max)    == nonstd::cx::ceil(f64__f64_more_max) )    || ( isnan(std::ceil(f64__f64_more_max))    && isnan(nonstd::cx::ceil(f64__f64_more_max)) )    ));
        // REQUIRE(( ( std::ceil(f64__f_long_less_min) == nonstd::cx::ceil(f64__f_long_less_min) ) || ( isnan(std::ceil(f64__f_long_less_min)) && isnan(nonstd::cx::ceil(f64__f_long_less_min)) ) ));
        // REQUIRE(( ( std::ceil(f64__f_long_more_max) == nonstd::cx::ceil(f64__f_long_more_max) ) || ( isnan(std::ceil(f64__f_long_more_max)) && isnan(nonstd::cx::ceil(f64__f_long_more_max)) ) ));

        REQUIRE(( ( std::ceil(f_long__positive)        == nonstd::cx::ceil(f_long__positive) )        || ( isnan(std::ceil(f_long__positive))        && isnan(nonstd::cx::ceil(f_long__positive)) )        ));
        REQUIRE(( ( std::ceil(f_long__negative)        == nonstd::cx::ceil(f_long__negative) )        || ( isnan(std::ceil(f_long__negative))        && isnan(nonstd::cx::ceil(f_long__negative)) )        ));
        // REQUIRE(( ( std::ceil(f_long__f32_min)         == nonstd::cx::ceil(f_long__f32_min) )         || ( isnan(std::ceil(f_long__f32_min))         && isnan(nonstd::cx::ceil(f_long__f32_min)) )         ));
        // REQUIRE(( ( std::ceil(f_long__f32_max)         == nonstd::cx::ceil(f_long__f32_max) )         || ( isnan(std::ceil(f_long__f32_max))         && isnan(nonstd::cx::ceil(f_long__f32_max)) )         ));
        // REQUIRE(( ( std::ceil(f_long__f64_min)         == nonstd::cx::ceil(f_long__f64_min) )         || ( isnan(std::ceil(f_long__f64_min))         && isnan(nonstd::cx::ceil(f_long__f64_min)) )         ));
        // REQUIRE(( ( std::ceil(f_long__f64_max)         == nonstd::cx::ceil(f_long__f64_max) )         || ( isnan(std::ceil(f_long__f64_max))         && isnan(nonstd::cx::ceil(f_long__f64_max)) )         ));
        // REQUIRE(( ( std::ceil(f_long__f_long_min)      == nonstd::cx::ceil(f_long__f_long_min) )      || ( isnan(std::ceil(f_long__f_long_min))      && isnan(nonstd::cx::ceil(f_long__f_long_min)) )      ));
        // REQUIRE(( ( std::ceil(f_long__f_long_max)      == nonstd::cx::ceil(f_long__f_long_max) )      || ( isnan(std::ceil(f_long__f_long_max))      && isnan(nonstd::cx::ceil(f_long__f_long_max)) )      ));
        REQUIRE(( ( std::ceil(f_long__f32_nan)         == nonstd::cx::ceil(f_long__f32_nan) )         || ( isnan(std::ceil(f_long__f32_nan))         && isnan(nonstd::cx::ceil(f_long__f32_nan)) )         ));
        REQUIRE(( ( std::ceil(f_long__f64_nan)         == nonstd::cx::ceil(f_long__f64_nan) )         || ( isnan(std::ceil(f_long__f64_nan))         && isnan(nonstd::cx::ceil(f_long__f64_nan)) )         ));
        REQUIRE(( ( std::ceil(f_long__f_long_nan)      == nonstd::cx::ceil(f_long__f_long_nan) )      || ( isnan(std::ceil(f_long__f_long_nan))      && isnan(nonstd::cx::ceil(f_long__f_long_nan)) )      ));
        REQUIRE(( ( std::ceil(f_long__f32_inf)         == nonstd::cx::ceil(f_long__f32_inf) )         || ( isnan(std::ceil(f_long__f32_inf))         && isnan(nonstd::cx::ceil(f_long__f32_inf)) )         ));
        REQUIRE(( ( std::ceil(f_long__f64_inf)         == nonstd::cx::ceil(f_long__f64_inf) )         || ( isnan(std::ceil(f_long__f64_inf))         && isnan(nonstd::cx::ceil(f_long__f64_inf)) )         ));
        REQUIRE(( ( std::ceil(f_long__f_long_inf)      == nonstd::cx::ceil(f_long__f_long_inf) )      || ( isnan(std::ceil(f_long__f_long_inf))      && isnan(nonstd::cx::ceil(f_long__f_long_inf)) )      ));
        // REQUIRE(( ( std::ceil(f_long__f32_less_min)    == nonstd::cx::ceil(f_long__f32_less_min) )    || ( isnan(std::ceil(f_long__f32_less_min))    && isnan(nonstd::cx::ceil(f_long__f32_less_min)) )    ));
        // REQUIRE(( ( std::ceil(f_long__f32_more_max)    == nonstd::cx::ceil(f_long__f32_more_max) )    || ( isnan(std::ceil(f_long__f32_more_max))    && isnan(nonstd::cx::ceil(f_long__f32_more_max)) )    ));
        // REQUIRE(( ( std::ceil(f_long__f64_less_min)    == nonstd::cx::ceil(f_long__f64_less_min) )    || ( isnan(std::ceil(f_long__f64_less_min))    && isnan(nonstd::cx::ceil(f_long__f64_less_min)) )    ));
        // REQUIRE(( ( std::ceil(f_long__f64_more_max)    == nonstd::cx::ceil(f_long__f64_more_max) )    || ( isnan(std::ceil(f_long__f64_more_max))    && isnan(nonstd::cx::ceil(f_long__f64_more_max)) )    ));
        // REQUIRE(( ( std::ceil(f_long__f_long_less_min) == nonstd::cx::ceil(f_long__f_long_less_min) ) || ( isnan(std::ceil(f_long__f_long_less_min)) && isnan(nonstd::cx::ceil(f_long__f_long_less_min)) ) ));
        // REQUIRE(( ( std::ceil(f_long__f_long_more_max) == nonstd::cx::ceil(f_long__f_long_more_max) ) || ( isnan(std::ceil(f_long__f_long_more_max)) && isnan(nonstd::cx::ceil(f_long__f_long_more_max)) ) ));

        // REQUIRE(( ( std::ceil(u64__positive) == nonstd::cx::ceil(u64__positive) ) || ( isnan(std::ceil(u64__positive)) && isnan(nonstd::cx::ceil(u64__positive)) ) ));
        // REQUIRE(( ( std::ceil(u64__zero)     == nonstd::cx::ceil(u64__zero) )     || ( isnan(std::ceil(u64__zero))     && isnan(nonstd::cx::ceil(u64__zero)) )     ));
        // REQUIRE(( ( std::ceil(u64__max)      == nonstd::cx::ceil(u64__max) )      || ( isnan(std::ceil(u64__max))      && isnan(nonstd::cx::ceil(u64__max)) )      ));
        // REQUIRE(( ( std::ceil(i64__positive) == nonstd::cx::ceil(i64__positive) ) || ( isnan(std::ceil(i64__positive)) && isnan(nonstd::cx::ceil(i64__positive)) ) ));
        // REQUIRE(( ( std::ceil(i64__zero)     == nonstd::cx::ceil(i64__zero) )     || ( isnan(std::ceil(i64__zero))     && isnan(nonstd::cx::ceil(i64__zero)) )     ));
        // REQUIRE(( ( std::ceil(i64__negative) == nonstd::cx::ceil(i64__negative) ) || ( isnan(std::ceil(i64__negative)) && isnan(nonstd::cx::ceil(i64__negative)) ) ));
        // REQUIRE(( ( std::ceil(i64__min)      == nonstd::cx::ceil(i64__min) )      || ( isnan(std::ceil(i64__min))      && isnan(nonstd::cx::ceil(i64__min)) )      ));
        // REQUIRE(( ( std::ceil(i64__max)      == nonstd::cx::ceil(i64__max) )      || ( isnan(std::ceil(i64__max))      && isnan(nonstd::cx::ceil(i64__max)) )      ));

        // Make sure cx::ceil is as precise as std::ceil. (Some of these should
        // resolve to 1, rather than 2.)
        f64 a11 = 1.00000000001;         REQUIRE(std::ceil(a11) == nonstd::cx::ceil(a11));
        f64 a12 = 1.000000000001;        REQUIRE(std::ceil(a12) == nonstd::cx::ceil(a12));
        f64 a13 = 1.0000000000001;       REQUIRE(std::ceil(a13) == nonstd::cx::ceil(a13));
        f64 a14 = 1.00000000000001;      REQUIRE(std::ceil(a14) == nonstd::cx::ceil(a14));
        f64 a15 = 1.000000000000001;     REQUIRE(std::ceil(a15) == nonstd::cx::ceil(a15));
        f64 a16 = 1.0000000000000001;    REQUIRE(std::ceil(a16) == nonstd::cx::ceil(a16));
        f64 a17 = 1.00000000000000001;   REQUIRE(std::ceil(a17) == nonstd::cx::ceil(a17));
        f64 a18 = 1.000000000000000001;  REQUIRE(std::ceil(a18) == nonstd::cx::ceil(a18));
        f64 a19 = 1.0000000000000000001; REQUIRE(std::ceil(a19) == nonstd::cx::ceil(a19));
    }

    SECTION("floor") {
        constexpr auto calls_to_floor_are_constexpr = nonstd::cx::floor(f32__positive);

        using std::isnan;
        // floor and floor don't much benefit from extents testing, and the
        // results from those functions tend to be estimations. It's more
        // important to verify exceptional (INFINITY, NaN) cases are consistent.
        REQUIRE(( ( std::floor(f32__positive)        == nonstd::cx::floor(f32__positive) )        || ( isnan(std::floor(f32__positive))        && isnan(nonstd::cx::floor(f32__positive)) )        ));
        REQUIRE(( ( std::floor(f32__negative)        == nonstd::cx::floor(f32__negative) )        || ( isnan(std::floor(f32__negative))        && isnan(nonstd::cx::floor(f32__negative)) )        ));
        // REQUIRE(( ( std::floor(f32__f32_min)         == nonstd::cx::floor(f32__f32_min) )         || ( isnan(std::floor(f32__f32_min))         && isnan(nonstd::cx::floor(f32__f32_min)) )         ));
        // REQUIRE(( ( std::floor(f32__f32_max)         == nonstd::cx::floor(f32__f32_max) )         || ( isnan(std::floor(f32__f32_max))         && isnan(nonstd::cx::floor(f32__f32_max)) )         ));
        // REQUIRE(( ( std::floor(f32__f64_min)         == nonstd::cx::floor(f32__f64_min) )         || ( isnan(std::floor(f32__f64_min))         && isnan(nonstd::cx::floor(f32__f64_min)) )         ));
    //  // REQUIRE(( ( std::floor(f32__f64_max)         == nonstd::cx::floor(f32__f64_max) )         || ( isnan(std::floor(f32__f64_max))         && isnan(nonstd::cx::floor(f32__f64_max)) )         ));
        // REQUIRE(( ( std::floor(f32__f_long_min)      == nonstd::cx::floor(f32__f_long_min) )      || ( isnan(std::floor(f32__f_long_min))      && isnan(nonstd::cx::floor(f32__f_long_min)) )      ));
    //  // REQUIRE(( ( std::floor(f32__f_long_max)      == nonstd::cx::floor(f32__f_long_max) )      || ( isnan(std::floor(f32__f_long_max))      && isnan(nonstd::cx::floor(f32__f_long_max)) )      ));
        REQUIRE(( ( std::floor(f32__f32_nan)         == nonstd::cx::floor(f32__f32_nan) )         || ( isnan(std::floor(f32__f32_nan))         && isnan(nonstd::cx::floor(f32__f32_nan)) )         ));
        REQUIRE(( ( std::floor(f32__f64_nan)         == nonstd::cx::floor(f32__f64_nan) )         || ( isnan(std::floor(f32__f64_nan))         && isnan(nonstd::cx::floor(f32__f64_nan)) )         ));
        REQUIRE(( ( std::floor(f32__f_long_nan)      == nonstd::cx::floor(f32__f_long_nan) )      || ( isnan(std::floor(f32__f_long_nan))      && isnan(nonstd::cx::floor(f32__f_long_nan)) )      ));
        REQUIRE(( ( std::floor(f32__f32_inf)         == nonstd::cx::floor(f32__f32_inf) )         || ( isnan(std::floor(f32__f32_inf))         && isnan(nonstd::cx::floor(f32__f32_inf)) )         ));
        REQUIRE(( ( std::floor(f32__f64_inf)         == nonstd::cx::floor(f32__f64_inf) )         || ( isnan(std::floor(f32__f64_inf))         && isnan(nonstd::cx::floor(f32__f64_inf)) )         ));
        REQUIRE(( ( std::floor(f32__f_long_inf)      == nonstd::cx::floor(f32__f_long_inf) )      || ( isnan(std::floor(f32__f_long_inf))      && isnan(nonstd::cx::floor(f32__f_long_inf)) )      ));
        // REQUIRE(( ( std::floor(f32__f32_less_min)    == nonstd::cx::floor(f32__f32_less_min) )    || ( isnan(std::floor(f32__f32_less_min))    && isnan(nonstd::cx::floor(f32__f32_less_min)) )    ));
        // REQUIRE(( ( std::floor(f32__f32_more_max)    == nonstd::cx::floor(f32__f32_more_max) )    || ( isnan(std::floor(f32__f32_more_max))    && isnan(nonstd::cx::floor(f32__f32_more_max)) )    ));
        // REQUIRE(( ( std::floor(f32__f64_less_min)    == nonstd::cx::floor(f32__f64_less_min) )    || ( isnan(std::floor(f32__f64_less_min))    && isnan(nonstd::cx::floor(f32__f64_less_min)) )    ));
        // REQUIRE(( ( std::floor(f32__f64_more_max)    == nonstd::cx::floor(f32__f64_more_max) )    || ( isnan(std::floor(f32__f64_more_max))    && isnan(nonstd::cx::floor(f32__f64_more_max)) )    ));
        // REQUIRE(( ( std::floor(f32__f_long_less_min) == nonstd::cx::floor(f32__f_long_less_min) ) || ( isnan(std::floor(f32__f_long_less_min)) && isnan(nonstd::cx::floor(f32__f_long_less_min)) ) ));
        // REQUIRE(( ( std::floor(f32__f_long_more_max) == nonstd::cx::floor(f32__f_long_more_max) ) || ( isnan(std::floor(f32__f_long_more_max)) && isnan(nonstd::cx::floor(f32__f_long_more_max)) ) ));

        REQUIRE(( ( std::floor(f64__positive)        == nonstd::cx::floor(f64__positive) )        || ( isnan(std::floor(f64__positive))        && isnan(nonstd::cx::floor(f64__positive)) )        ));
        REQUIRE(( ( std::floor(f64__negative)        == nonstd::cx::floor(f64__negative) )        || ( isnan(std::floor(f64__negative))        && isnan(nonstd::cx::floor(f64__negative)) )        ));
        //  REQUIRE(( ( std::floor(f64__f32_min)         == nonstd::cx::floor(f64__f32_min) )         || ( isnan(std::floor(f64__f32_min))         && isnan(nonstd::cx::floor(f64__f32_min)) )         ));
        //  REQUIRE(( ( std::floor(f64__f32_max)         == nonstd::cx::floor(f64__f32_max) )         || ( isnan(std::floor(f64__f32_max))         && isnan(nonstd::cx::floor(f64__f32_max)) )         ));
        //  REQUIRE(( ( std::floor(f64__f64_min)         == nonstd::cx::floor(f64__f64_min) )         || ( isnan(std::floor(f64__f64_min))         && isnan(nonstd::cx::floor(f64__f64_min)) )         ));
        //  REQUIRE(( ( std::floor(f64__f64_max)         == nonstd::cx::floor(f64__f64_max) )         || ( isnan(std::floor(f64__f64_max))         && isnan(nonstd::cx::floor(f64__f64_max)) )         ));
        //  REQUIRE(( ( std::floor(f64__f_long_min)      == nonstd::cx::floor(f64__f_long_min) )      || ( isnan(std::floor(f64__f_long_min))      && isnan(nonstd::cx::floor(f64__f_long_min)) )      ));
    //  //  REQUIRE(( ( std::floor(f64__f_long_max)      == nonstd::cx::floor(f64__f_long_max) )      || ( isnan(std::floor(f64__f_long_max))      && isnan(nonstd::cx::floor(f64__f_long_max)) )      ));
        REQUIRE(( ( std::floor(f64__f32_nan)         == nonstd::cx::floor(f64__f32_nan) )         || ( isnan(std::floor(f64__f32_nan))         && isnan(nonstd::cx::floor(f64__f32_nan)) )         ));
        REQUIRE(( ( std::floor(f64__f64_nan)         == nonstd::cx::floor(f64__f64_nan) )         || ( isnan(std::floor(f64__f64_nan))         && isnan(nonstd::cx::floor(f64__f64_nan)) )         ));
        REQUIRE(( ( std::floor(f64__f_long_nan)      == nonstd::cx::floor(f64__f_long_nan) )      || ( isnan(std::floor(f64__f_long_nan))      && isnan(nonstd::cx::floor(f64__f_long_nan)) )      ));
        REQUIRE(( ( std::floor(f64__f32_inf)         == nonstd::cx::floor(f64__f32_inf) )         || ( isnan(std::floor(f64__f32_inf))         && isnan(nonstd::cx::floor(f64__f32_inf)) )         ));
        REQUIRE(( ( std::floor(f64__f64_inf)         == nonstd::cx::floor(f64__f64_inf) )         || ( isnan(std::floor(f64__f64_inf))         && isnan(nonstd::cx::floor(f64__f64_inf)) )         ));
        REQUIRE(( ( std::floor(f64__f_long_inf)      == nonstd::cx::floor(f64__f_long_inf) )      || ( isnan(std::floor(f64__f_long_inf))      && isnan(nonstd::cx::floor(f64__f_long_inf)) )      ));
        // REQUIRE(( ( std::floor(f64__f32_less_min)    == nonstd::cx::floor(f64__f32_less_min) )    || ( isnan(std::floor(f64__f32_less_min))    && isnan(nonstd::cx::floor(f64__f32_less_min)) )    ));
        // REQUIRE(( ( std::floor(f64__f32_more_max)    == nonstd::cx::floor(f64__f32_more_max) )    || ( isnan(std::floor(f64__f32_more_max))    && isnan(nonstd::cx::floor(f64__f32_more_max)) )    ));
        // REQUIRE(( ( std::floor(f64__f64_less_min)    == nonstd::cx::floor(f64__f64_less_min) )    || ( isnan(std::floor(f64__f64_less_min))    && isnan(nonstd::cx::floor(f64__f64_less_min)) )    ));
        // REQUIRE(( ( std::floor(f64__f64_more_max)    == nonstd::cx::floor(f64__f64_more_max) )    || ( isnan(std::floor(f64__f64_more_max))    && isnan(nonstd::cx::floor(f64__f64_more_max)) )    ));
        // REQUIRE(( ( std::floor(f64__f_long_less_min) == nonstd::cx::floor(f64__f_long_less_min) ) || ( isnan(std::floor(f64__f_long_less_min)) && isnan(nonstd::cx::floor(f64__f_long_less_min)) ) ));
        // REQUIRE(( ( std::floor(f64__f_long_more_max) == nonstd::cx::floor(f64__f_long_more_max) ) || ( isnan(std::floor(f64__f_long_more_max)) && isnan(nonstd::cx::floor(f64__f_long_more_max)) ) ));

        REQUIRE(( ( std::floor(f_long__positive)        == nonstd::cx::floor(f_long__positive) )        || ( isnan(std::floor(f_long__positive))        && isnan(nonstd::cx::floor(f_long__positive)) )        ));
        REQUIRE(( ( std::floor(f_long__negative)        == nonstd::cx::floor(f_long__negative) )        || ( isnan(std::floor(f_long__negative))        && isnan(nonstd::cx::floor(f_long__negative)) )        ));
        // REQUIRE(( ( std::floor(f_long__f32_min)         == nonstd::cx::floor(f_long__f32_min) )         || ( isnan(std::floor(f_long__f32_min))         && isnan(nonstd::cx::floor(f_long__f32_min)) )         ));
        // REQUIRE(( ( std::floor(f_long__f32_max)         == nonstd::cx::floor(f_long__f32_max) )         || ( isnan(std::floor(f_long__f32_max))         && isnan(nonstd::cx::floor(f_long__f32_max)) )         ));
        // REQUIRE(( ( std::floor(f_long__f64_min)         == nonstd::cx::floor(f_long__f64_min) )         || ( isnan(std::floor(f_long__f64_min))         && isnan(nonstd::cx::floor(f_long__f64_min)) )         ));
        // REQUIRE(( ( std::floor(f_long__f64_max)         == nonstd::cx::floor(f_long__f64_max) )         || ( isnan(std::floor(f_long__f64_max))         && isnan(nonstd::cx::floor(f_long__f64_max)) )         ));
        // REQUIRE(( ( std::floor(f_long__f_long_min)      == nonstd::cx::floor(f_long__f_long_min) )      || ( isnan(std::floor(f_long__f_long_min))      && isnan(nonstd::cx::floor(f_long__f_long_min)) )      ));
        // REQUIRE(( ( std::floor(f_long__f_long_max)      == nonstd::cx::floor(f_long__f_long_max) )      || ( isnan(std::floor(f_long__f_long_max))      && isnan(nonstd::cx::floor(f_long__f_long_max)) )      ));
        REQUIRE(( ( std::floor(f_long__f32_nan)         == nonstd::cx::floor(f_long__f32_nan) )         || ( isnan(std::floor(f_long__f32_nan))         && isnan(nonstd::cx::floor(f_long__f32_nan)) )         ));
        REQUIRE(( ( std::floor(f_long__f64_nan)         == nonstd::cx::floor(f_long__f64_nan) )         || ( isnan(std::floor(f_long__f64_nan))         && isnan(nonstd::cx::floor(f_long__f64_nan)) )         ));
        REQUIRE(( ( std::floor(f_long__f_long_nan)      == nonstd::cx::floor(f_long__f_long_nan) )      || ( isnan(std::floor(f_long__f_long_nan))      && isnan(nonstd::cx::floor(f_long__f_long_nan)) )      ));
        REQUIRE(( ( std::floor(f_long__f32_inf)         == nonstd::cx::floor(f_long__f32_inf) )         || ( isnan(std::floor(f_long__f32_inf))         && isnan(nonstd::cx::floor(f_long__f32_inf)) )         ));
        REQUIRE(( ( std::floor(f_long__f64_inf)         == nonstd::cx::floor(f_long__f64_inf) )         || ( isnan(std::floor(f_long__f64_inf))         && isnan(nonstd::cx::floor(f_long__f64_inf)) )         ));
        REQUIRE(( ( std::floor(f_long__f_long_inf)      == nonstd::cx::floor(f_long__f_long_inf) )      || ( isnan(std::floor(f_long__f_long_inf))      && isnan(nonstd::cx::floor(f_long__f_long_inf)) )      ));
        // REQUIRE(( ( std::floor(f_long__f32_less_min)    == nonstd::cx::floor(f_long__f32_less_min) )    || ( isnan(std::floor(f_long__f32_less_min))    && isnan(nonstd::cx::floor(f_long__f32_less_min)) )    ));
        // REQUIRE(( ( std::floor(f_long__f32_more_max)    == nonstd::cx::floor(f_long__f32_more_max) )    || ( isnan(std::floor(f_long__f32_more_max))    && isnan(nonstd::cx::floor(f_long__f32_more_max)) )    ));
        // REQUIRE(( ( std::floor(f_long__f64_less_min)    == nonstd::cx::floor(f_long__f64_less_min) )    || ( isnan(std::floor(f_long__f64_less_min))    && isnan(nonstd::cx::floor(f_long__f64_less_min)) )    ));
        // REQUIRE(( ( std::floor(f_long__f64_more_max)    == nonstd::cx::floor(f_long__f64_more_max) )    || ( isnan(std::floor(f_long__f64_more_max))    && isnan(nonstd::cx::floor(f_long__f64_more_max)) )    ));
        // REQUIRE(( ( std::floor(f_long__f_long_less_min) == nonstd::cx::floor(f_long__f_long_less_min) ) || ( isnan(std::floor(f_long__f_long_less_min)) && isnan(nonstd::cx::floor(f_long__f_long_less_min)) ) ));
        // REQUIRE(( ( std::floor(f_long__f_long_more_max) == nonstd::cx::floor(f_long__f_long_more_max) ) || ( isnan(std::floor(f_long__f_long_more_max)) && isnan(nonstd::cx::floor(f_long__f_long_more_max)) ) ));

        // REQUIRE(( ( std::floor(u64__positive) == nonstd::cx::floor(u64__positive) ) || ( isnan(std::floor(u64__positive)) && isnan(nonstd::cx::floor(u64__positive)) ) ));
        // REQUIRE(( ( std::floor(u64__zero)     == nonstd::cx::floor(u64__zero) )     || ( isnan(std::floor(u64__zero))     && isnan(nonstd::cx::floor(u64__zero)) )     ));
        // REQUIRE(( ( std::floor(u64__max)      == nonstd::cx::floor(u64__max) )      || ( isnan(std::floor(u64__max))      && isnan(nonstd::cx::floor(u64__max)) )      ));
        // REQUIRE(( ( std::floor(i64__positive) == nonstd::cx::floor(i64__positive) ) || ( isnan(std::floor(i64__positive)) && isnan(nonstd::cx::floor(i64__positive)) ) ));
        // REQUIRE(( ( std::floor(i64__zero)     == nonstd::cx::floor(i64__zero) )     || ( isnan(std::floor(i64__zero))     && isnan(nonstd::cx::floor(i64__zero)) )     ));
        // REQUIRE(( ( std::floor(i64__negative) == nonstd::cx::floor(i64__negative) ) || ( isnan(std::floor(i64__negative)) && isnan(nonstd::cx::floor(i64__negative)) ) ));
        // REQUIRE(( ( std::floor(i64__min)      == nonstd::cx::floor(i64__min) )      || ( isnan(std::floor(i64__min))      && isnan(nonstd::cx::floor(i64__min)) )      ));
        // REQUIRE(( ( std::floor(i64__max)      == nonstd::cx::floor(i64__max) )      || ( isnan(std::floor(i64__max))      && isnan(nonstd::cx::floor(i64__max)) )      ));

        // Make sure cx::floor is as precise as std::floor. (Some of these should
        // resolve to 1, rather than 0.)
        f64 a11 = 0.99999999999;         REQUIRE(std::floor(a11) == nonstd::cx::floor(a11));
        f64 a12 = 0.999999999999;        REQUIRE(std::floor(a12) == nonstd::cx::floor(a12));
        f64 a13 = 0.9999999999999;       REQUIRE(std::floor(a13) == nonstd::cx::floor(a13));
        f64 a14 = 0.99999999999999;      REQUIRE(std::floor(a14) == nonstd::cx::floor(a14));
        f64 a15 = 0.999999999999999;     REQUIRE(std::floor(a15) == nonstd::cx::floor(a15));
        f64 a16 = 0.9999999999999999;    REQUIRE(std::floor(a16) == nonstd::cx::floor(a16));
        f64 a17 = 0.99999999999999999;   REQUIRE(std::floor(a17) == nonstd::cx::floor(a17));
        f64 a18 = 0.999999999999999999;  REQUIRE(std::floor(a18) == nonstd::cx::floor(a18));
        f64 a19 = 0.9999999999999999999; REQUIRE(std::floor(a19) == nonstd::cx::floor(a19));
    }

    SECTION("trunc") {
        constexpr auto calls_to_trunc_are_constexpr = nonstd::cx::trunc(f32__positive);

        using std::isnan;
        // trunc and trunc don't much benefit from extents testing, and the
        // results from those functions tend to be estimations. It's more
        // important to verify exceptional (INFINITY, NaN) cases are consistent.
        REQUIRE(( ( std::trunc(f32__positive)        == nonstd::cx::trunc(f32__positive) )        || ( isnan(std::trunc(f32__positive))        && isnan(nonstd::cx::trunc(f32__positive)) )        ));
        REQUIRE(( ( std::trunc(f32__negative)        == nonstd::cx::trunc(f32__negative) )        || ( isnan(std::trunc(f32__negative))        && isnan(nonstd::cx::trunc(f32__negative)) )        ));
        // REQUIRE(( ( std::trunc(f32__f32_min)         == nonstd::cx::trunc(f32__f32_min) )         || ( isnan(std::trunc(f32__f32_min))         && isnan(nonstd::cx::trunc(f32__f32_min)) )         ));
        // REQUIRE(( ( std::trunc(f32__f32_max)         == nonstd::cx::trunc(f32__f32_max) )         || ( isnan(std::trunc(f32__f32_max))         && isnan(nonstd::cx::trunc(f32__f32_max)) )         ));
        // REQUIRE(( ( std::trunc(f32__f64_min)         == nonstd::cx::trunc(f32__f64_min) )         || ( isnan(std::trunc(f32__f64_min))         && isnan(nonstd::cx::trunc(f32__f64_min)) )         ));
    //  // REQUIRE(( ( std::trunc(f32__f64_max)         == nonstd::cx::trunc(f32__f64_max) )         || ( isnan(std::trunc(f32__f64_max))         && isnan(nonstd::cx::trunc(f32__f64_max)) )         ));
        // REQUIRE(( ( std::trunc(f32__f_long_min)      == nonstd::cx::trunc(f32__f_long_min) )      || ( isnan(std::trunc(f32__f_long_min))      && isnan(nonstd::cx::trunc(f32__f_long_min)) )      ));
    //  // REQUIRE(( ( std::trunc(f32__f_long_max)      == nonstd::cx::trunc(f32__f_long_max) )      || ( isnan(std::trunc(f32__f_long_max))      && isnan(nonstd::cx::trunc(f32__f_long_max)) )      ));
        REQUIRE(( ( std::trunc(f32__f32_nan)         == nonstd::cx::trunc(f32__f32_nan) )         || ( isnan(std::trunc(f32__f32_nan))         && isnan(nonstd::cx::trunc(f32__f32_nan)) )         ));
        REQUIRE(( ( std::trunc(f32__f64_nan)         == nonstd::cx::trunc(f32__f64_nan) )         || ( isnan(std::trunc(f32__f64_nan))         && isnan(nonstd::cx::trunc(f32__f64_nan)) )         ));
        REQUIRE(( ( std::trunc(f32__f_long_nan)      == nonstd::cx::trunc(f32__f_long_nan) )      || ( isnan(std::trunc(f32__f_long_nan))      && isnan(nonstd::cx::trunc(f32__f_long_nan)) )      ));
        REQUIRE(( ( std::trunc(f32__f32_inf)         == nonstd::cx::trunc(f32__f32_inf) )         || ( isnan(std::trunc(f32__f32_inf))         && isnan(nonstd::cx::trunc(f32__f32_inf)) )         ));
        REQUIRE(( ( std::trunc(f32__f64_inf)         == nonstd::cx::trunc(f32__f64_inf) )         || ( isnan(std::trunc(f32__f64_inf))         && isnan(nonstd::cx::trunc(f32__f64_inf)) )         ));
        REQUIRE(( ( std::trunc(f32__f_long_inf)      == nonstd::cx::trunc(f32__f_long_inf) )      || ( isnan(std::trunc(f32__f_long_inf))      && isnan(nonstd::cx::trunc(f32__f_long_inf)) )      ));
        // REQUIRE(( ( std::trunc(f32__f32_less_min)    == nonstd::cx::trunc(f32__f32_less_min) )    || ( isnan(std::trunc(f32__f32_less_min))    && isnan(nonstd::cx::trunc(f32__f32_less_min)) )    ));
        // REQUIRE(( ( std::trunc(f32__f32_more_max)    == nonstd::cx::trunc(f32__f32_more_max) )    || ( isnan(std::trunc(f32__f32_more_max))    && isnan(nonstd::cx::trunc(f32__f32_more_max)) )    ));
        // REQUIRE(( ( std::trunc(f32__f64_less_min)    == nonstd::cx::trunc(f32__f64_less_min) )    || ( isnan(std::trunc(f32__f64_less_min))    && isnan(nonstd::cx::trunc(f32__f64_less_min)) )    ));
        // REQUIRE(( ( std::trunc(f32__f64_more_max)    == nonstd::cx::trunc(f32__f64_more_max) )    || ( isnan(std::trunc(f32__f64_more_max))    && isnan(nonstd::cx::trunc(f32__f64_more_max)) )    ));
        // REQUIRE(( ( std::trunc(f32__f_long_less_min) == nonstd::cx::trunc(f32__f_long_less_min) ) || ( isnan(std::trunc(f32__f_long_less_min)) && isnan(nonstd::cx::trunc(f32__f_long_less_min)) ) ));
        // REQUIRE(( ( std::trunc(f32__f_long_more_max) == nonstd::cx::trunc(f32__f_long_more_max) ) || ( isnan(std::trunc(f32__f_long_more_max)) && isnan(nonstd::cx::trunc(f32__f_long_more_max)) ) ));

        REQUIRE(( ( std::trunc(f64__positive)        == nonstd::cx::trunc(f64__positive) )        || ( isnan(std::trunc(f64__positive))        && isnan(nonstd::cx::trunc(f64__positive)) )        ));
        REQUIRE(( ( std::trunc(f64__negative)        == nonstd::cx::trunc(f64__negative) )        || ( isnan(std::trunc(f64__negative))        && isnan(nonstd::cx::trunc(f64__negative)) )        ));
        //  REQUIRE(( ( std::trunc(f64__f32_min)         == nonstd::cx::trunc(f64__f32_min) )         || ( isnan(std::trunc(f64__f32_min))         && isnan(nonstd::cx::trunc(f64__f32_min)) )         ));
        //  REQUIRE(( ( std::trunc(f64__f32_max)         == nonstd::cx::trunc(f64__f32_max) )         || ( isnan(std::trunc(f64__f32_max))         && isnan(nonstd::cx::trunc(f64__f32_max)) )         ));
        //  REQUIRE(( ( std::trunc(f64__f64_min)         == nonstd::cx::trunc(f64__f64_min) )         || ( isnan(std::trunc(f64__f64_min))         && isnan(nonstd::cx::trunc(f64__f64_min)) )         ));
        //  REQUIRE(( ( std::trunc(f64__f64_max)         == nonstd::cx::trunc(f64__f64_max) )         || ( isnan(std::trunc(f64__f64_max))         && isnan(nonstd::cx::trunc(f64__f64_max)) )         ));
        //  REQUIRE(( ( std::trunc(f64__f_long_min)      == nonstd::cx::trunc(f64__f_long_min) )      || ( isnan(std::trunc(f64__f_long_min))      && isnan(nonstd::cx::trunc(f64__f_long_min)) )      ));
    //  //  REQUIRE(( ( std::trunc(f64__f_long_max)      == nonstd::cx::trunc(f64__f_long_max) )      || ( isnan(std::trunc(f64__f_long_max))      && isnan(nonstd::cx::trunc(f64__f_long_max)) )      ));
        REQUIRE(( ( std::trunc(f64__f32_nan)         == nonstd::cx::trunc(f64__f32_nan) )         || ( isnan(std::trunc(f64__f32_nan))         && isnan(nonstd::cx::trunc(f64__f32_nan)) )         ));
        REQUIRE(( ( std::trunc(f64__f64_nan)         == nonstd::cx::trunc(f64__f64_nan) )         || ( isnan(std::trunc(f64__f64_nan))         && isnan(nonstd::cx::trunc(f64__f64_nan)) )         ));
        REQUIRE(( ( std::trunc(f64__f_long_nan)      == nonstd::cx::trunc(f64__f_long_nan) )      || ( isnan(std::trunc(f64__f_long_nan))      && isnan(nonstd::cx::trunc(f64__f_long_nan)) )      ));
        REQUIRE(( ( std::trunc(f64__f32_inf)         == nonstd::cx::trunc(f64__f32_inf) )         || ( isnan(std::trunc(f64__f32_inf))         && isnan(nonstd::cx::trunc(f64__f32_inf)) )         ));
        REQUIRE(( ( std::trunc(f64__f64_inf)         == nonstd::cx::trunc(f64__f64_inf) )         || ( isnan(std::trunc(f64__f64_inf))         && isnan(nonstd::cx::trunc(f64__f64_inf)) )         ));
        REQUIRE(( ( std::trunc(f64__f_long_inf)      == nonstd::cx::trunc(f64__f_long_inf) )      || ( isnan(std::trunc(f64__f_long_inf))      && isnan(nonstd::cx::trunc(f64__f_long_inf)) )      ));
        // REQUIRE(( ( std::trunc(f64__f32_less_min)    == nonstd::cx::trunc(f64__f32_less_min) )    || ( isnan(std::trunc(f64__f32_less_min))    && isnan(nonstd::cx::trunc(f64__f32_less_min)) )    ));
        // REQUIRE(( ( std::trunc(f64__f32_more_max)    == nonstd::cx::trunc(f64__f32_more_max) )    || ( isnan(std::trunc(f64__f32_more_max))    && isnan(nonstd::cx::trunc(f64__f32_more_max)) )    ));
        // REQUIRE(( ( std::trunc(f64__f64_less_min)    == nonstd::cx::trunc(f64__f64_less_min) )    || ( isnan(std::trunc(f64__f64_less_min))    && isnan(nonstd::cx::trunc(f64__f64_less_min)) )    ));
        // REQUIRE(( ( std::trunc(f64__f64_more_max)    == nonstd::cx::trunc(f64__f64_more_max) )    || ( isnan(std::trunc(f64__f64_more_max))    && isnan(nonstd::cx::trunc(f64__f64_more_max)) )    ));
        // REQUIRE(( ( std::trunc(f64__f_long_less_min) == nonstd::cx::trunc(f64__f_long_less_min) ) || ( isnan(std::trunc(f64__f_long_less_min)) && isnan(nonstd::cx::trunc(f64__f_long_less_min)) ) ));
        // REQUIRE(( ( std::trunc(f64__f_long_more_max) == nonstd::cx::trunc(f64__f_long_more_max) ) || ( isnan(std::trunc(f64__f_long_more_max)) && isnan(nonstd::cx::trunc(f64__f_long_more_max)) ) ));

        REQUIRE(( ( std::trunc(f_long__positive)        == nonstd::cx::trunc(f_long__positive) )        || ( isnan(std::trunc(f_long__positive))        && isnan(nonstd::cx::trunc(f_long__positive)) )        ));
        REQUIRE(( ( std::trunc(f_long__negative)        == nonstd::cx::trunc(f_long__negative) )        || ( isnan(std::trunc(f_long__negative))        && isnan(nonstd::cx::trunc(f_long__negative)) )        ));
        // REQUIRE(( ( std::trunc(f_long__f32_min)         == nonstd::cx::trunc(f_long__f32_min) )         || ( isnan(std::trunc(f_long__f32_min))         && isnan(nonstd::cx::trunc(f_long__f32_min)) )         ));
        // REQUIRE(( ( std::trunc(f_long__f32_max)         == nonstd::cx::trunc(f_long__f32_max) )         || ( isnan(std::trunc(f_long__f32_max))         && isnan(nonstd::cx::trunc(f_long__f32_max)) )         ));
        // REQUIRE(( ( std::trunc(f_long__f64_min)         == nonstd::cx::trunc(f_long__f64_min) )         || ( isnan(std::trunc(f_long__f64_min))         && isnan(nonstd::cx::trunc(f_long__f64_min)) )         ));
        // REQUIRE(( ( std::trunc(f_long__f64_max)         == nonstd::cx::trunc(f_long__f64_max) )         || ( isnan(std::trunc(f_long__f64_max))         && isnan(nonstd::cx::trunc(f_long__f64_max)) )         ));
        // REQUIRE(( ( std::trunc(f_long__f_long_min)      == nonstd::cx::trunc(f_long__f_long_min) )      || ( isnan(std::trunc(f_long__f_long_min))      && isnan(nonstd::cx::trunc(f_long__f_long_min)) )      ));
        // REQUIRE(( ( std::trunc(f_long__f_long_max)      == nonstd::cx::trunc(f_long__f_long_max) )      || ( isnan(std::trunc(f_long__f_long_max))      && isnan(nonstd::cx::trunc(f_long__f_long_max)) )      ));
        REQUIRE(( ( std::trunc(f_long__f32_nan)         == nonstd::cx::trunc(f_long__f32_nan) )         || ( isnan(std::trunc(f_long__f32_nan))         && isnan(nonstd::cx::trunc(f_long__f32_nan)) )         ));
        REQUIRE(( ( std::trunc(f_long__f64_nan)         == nonstd::cx::trunc(f_long__f64_nan) )         || ( isnan(std::trunc(f_long__f64_nan))         && isnan(nonstd::cx::trunc(f_long__f64_nan)) )         ));
        REQUIRE(( ( std::trunc(f_long__f_long_nan)      == nonstd::cx::trunc(f_long__f_long_nan) )      || ( isnan(std::trunc(f_long__f_long_nan))      && isnan(nonstd::cx::trunc(f_long__f_long_nan)) )      ));
        REQUIRE(( ( std::trunc(f_long__f32_inf)         == nonstd::cx::trunc(f_long__f32_inf) )         || ( isnan(std::trunc(f_long__f32_inf))         && isnan(nonstd::cx::trunc(f_long__f32_inf)) )         ));
        REQUIRE(( ( std::trunc(f_long__f64_inf)         == nonstd::cx::trunc(f_long__f64_inf) )         || ( isnan(std::trunc(f_long__f64_inf))         && isnan(nonstd::cx::trunc(f_long__f64_inf)) )         ));
        REQUIRE(( ( std::trunc(f_long__f_long_inf)      == nonstd::cx::trunc(f_long__f_long_inf) )      || ( isnan(std::trunc(f_long__f_long_inf))      && isnan(nonstd::cx::trunc(f_long__f_long_inf)) )      ));
        // REQUIRE(( ( std::trunc(f_long__f32_less_min)    == nonstd::cx::trunc(f_long__f32_less_min) )    || ( isnan(std::trunc(f_long__f32_less_min))    && isnan(nonstd::cx::trunc(f_long__f32_less_min)) )    ));
        // REQUIRE(( ( std::trunc(f_long__f32_more_max)    == nonstd::cx::trunc(f_long__f32_more_max) )    || ( isnan(std::trunc(f_long__f32_more_max))    && isnan(nonstd::cx::trunc(f_long__f32_more_max)) )    ));
        // REQUIRE(( ( std::trunc(f_long__f64_less_min)    == nonstd::cx::trunc(f_long__f64_less_min) )    || ( isnan(std::trunc(f_long__f64_less_min))    && isnan(nonstd::cx::trunc(f_long__f64_less_min)) )    ));
        // REQUIRE(( ( std::trunc(f_long__f64_more_max)    == nonstd::cx::trunc(f_long__f64_more_max) )    || ( isnan(std::trunc(f_long__f64_more_max))    && isnan(nonstd::cx::trunc(f_long__f64_more_max)) )    ));
        // REQUIRE(( ( std::trunc(f_long__f_long_less_min) == nonstd::cx::trunc(f_long__f_long_less_min) ) || ( isnan(std::trunc(f_long__f_long_less_min)) && isnan(nonstd::cx::trunc(f_long__f_long_less_min)) ) ));
        // REQUIRE(( ( std::trunc(f_long__f_long_more_max) == nonstd::cx::trunc(f_long__f_long_more_max) ) || ( isnan(std::trunc(f_long__f_long_more_max)) && isnan(nonstd::cx::trunc(f_long__f_long_more_max)) ) ));

        // REQUIRE(( ( std::trunc(u64__positive) == nonstd::cx::trunc(u64__positive) ) || ( isnan(std::trunc(u64__positive)) && isnan(nonstd::cx::trunc(u64__positive)) ) ));
        // REQUIRE(( ( std::trunc(u64__zero)     == nonstd::cx::trunc(u64__zero) )     || ( isnan(std::trunc(u64__zero))     && isnan(nonstd::cx::trunc(u64__zero)) )     ));
        // REQUIRE(( ( std::trunc(u64__max)      == nonstd::cx::trunc(u64__max) )      || ( isnan(std::trunc(u64__max))      && isnan(nonstd::cx::trunc(u64__max)) )      ));
        // REQUIRE(( ( std::trunc(i64__positive) == nonstd::cx::trunc(i64__positive) ) || ( isnan(std::trunc(i64__positive)) && isnan(nonstd::cx::trunc(i64__positive)) ) ));
        // REQUIRE(( ( std::trunc(i64__zero)     == nonstd::cx::trunc(i64__zero) )     || ( isnan(std::trunc(i64__zero))     && isnan(nonstd::cx::trunc(i64__zero)) )     ));
        // REQUIRE(( ( std::trunc(i64__negative) == nonstd::cx::trunc(i64__negative) ) || ( isnan(std::trunc(i64__negative)) && isnan(nonstd::cx::trunc(i64__negative)) ) ));
        // REQUIRE(( ( std::trunc(i64__min)      == nonstd::cx::trunc(i64__min) )      || ( isnan(std::trunc(i64__min))      && isnan(nonstd::cx::trunc(i64__min)) )      ));
        // REQUIRE(( ( std::trunc(i64__max)      == nonstd::cx::trunc(i64__max) )      || ( isnan(std::trunc(i64__max))      && isnan(nonstd::cx::trunc(i64__max)) )      ));

        // Make sure cx::trunc is as precise as std::trunc. (Some of these should
        // resolve in the direction you wouldn't think is correct.)
        f64 a11 = 0.99999999999;         REQUIRE(std::trunc(a11) == nonstd::cx::trunc(a11));
        f64 a12 = 0.999999999999;        REQUIRE(std::trunc(a12) == nonstd::cx::trunc(a12));
        f64 a13 = 0.9999999999999;       REQUIRE(std::trunc(a13) == nonstd::cx::trunc(a13));
        f64 a14 = 0.99999999999999;      REQUIRE(std::trunc(a14) == nonstd::cx::trunc(a14));
        f64 a15 = 0.999999999999999;     REQUIRE(std::trunc(a15) == nonstd::cx::trunc(a15));
        f64 a16 = 0.9999999999999999;    REQUIRE(std::trunc(a16) == nonstd::cx::trunc(a16));
        f64 a17 = 0.99999999999999999;   REQUIRE(std::trunc(a17) == nonstd::cx::trunc(a17));
        f64 a18 = 0.999999999999999999;  REQUIRE(std::trunc(a18) == nonstd::cx::trunc(a18));
        f64 a19 = 0.9999999999999999999; REQUIRE(std::trunc(a19) == nonstd::cx::trunc(a19));
    }
}

} /* namespace math */
} /* namespace nonstd_test */
