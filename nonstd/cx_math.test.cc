/** Constexpr Math Utility Smoke Tests
 *  =================================
 *  The target is to maintain parity with the functions defined in <cmath>.
 */

#include <nonstd/cx_math.h>
#include <platform/testrunner/testrunner.h>

#include <random>


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
    constexpr f32 f32__positive               =  1.5f;                                      UNUSED(f32__positive);
    constexpr f32 f32__zero                   =  0.0f;                                      UNUSED(f32__zero);
    constexpr f32 f32__negative               = -1.5f;                                      UNUSED(f32__negative);
    constexpr f32 f32__f32_p_smallest         =  std::numeric_limits<f32>::min();           UNUSED(f32__f32_p_smallest);
    constexpr f32 f32__f32_n_smallest         = -std::numeric_limits<f32>::min();           UNUSED(f32__f32_n_smallest);
    constexpr f32 f32__f32_p_largest          =  std::numeric_limits<f32>::max();           UNUSED(f32__f32_p_largest);
    constexpr f32 f32__f32_n_largest          = -std::numeric_limits<f32>::max();           UNUSED(f32__f32_n_largest);
    constexpr f32 f32__f64_p_smallest         =  std::numeric_limits<f64>::min();           UNUSED(f32__f64_p_smallest);
    constexpr f32 f32__f64_n_smallest         = -std::numeric_limits<f64>::min();           UNUSED(f32__f64_n_smallest);
//  constexpr f32 f32__f64_p_largest          =  std::numeric_limits<f64>::max();           UNUSED(f32__f64_p_largest);
//  constexpr f32 f32__f64_n_largest          = -std::numeric_limits<f64>::max();           UNUSED(f32__f64_n_largest);
    constexpr f32 f32__f_long_p_smallest      =  std::numeric_limits<f_long>::min();        UNUSED(f32__f_long_p_smallest);
    constexpr f32 f32__f_long_n_smallest      = -std::numeric_limits<f_long>::min();        UNUSED(f32__f_long_n_smallest);
//  constexpr f32 f32__f_long_p_largest       =  std::numeric_limits<f_long>::max();        UNUSED(f32__f_long_p_largest);
//  constexpr f32 f32__f_long_n_largest       = -std::numeric_limits<f_long>::max();        UNUSED(f32__f_long_n_largest);
    constexpr f32 f32__f32_nan                =  std::numeric_limits<f32>::quiet_NaN();     UNUSED(f32__f32_nan);
    constexpr f32 f32__f64_nan                =  std::numeric_limits<f64>::quiet_NaN();     UNUSED(f32__f64_nan);
    constexpr f32 f32__f_long_nan             =  std::numeric_limits<f_long>::quiet_NaN();  UNUSED(f32__f_long_nan);
    constexpr f32 f32__f32_p_inf              =  std::numeric_limits<f32>::infinity();      UNUSED(f32__f32_p_inf);
    constexpr f32 f32__f32_n_inf              = -std::numeric_limits<f32>::infinity();      UNUSED(f32__f32_n_inf);
    constexpr f32 f32__f64_p_inf              =  std::numeric_limits<f64>::infinity();      UNUSED(f32__f64_p_inf);
    constexpr f32 f32__f64_n_inf              = -std::numeric_limits<f64>::infinity();      UNUSED(f32__f64_n_inf);
    constexpr f32 f32__f_long_p_inf           =  std::numeric_limits<f_long>::infinity();   UNUSED(f32__f_long_p_inf);
    constexpr f32 f32__f_long_n_inf           = -std::numeric_limits<f_long>::infinity();   UNUSED(f32__f_long_n_inf);
              f32 f32__f32_less_p_smallest    =  std::numeric_limits<f32>::min() / 2.0f;    UNUSED(f32__f32_less_p_smallest);
              f32 f32__f32_less_n_smallest    = -std::numeric_limits<f32>::min() / 2.0f;    UNUSED(f32__f32_less_n_smallest);
              f32 f32__f32_more_p_largest     =  std::numeric_limits<f32>::max() * 2.0f;    UNUSED(f32__f32_more_p_largest);
              f32 f32__f32_more_n_largest     = -std::numeric_limits<f32>::max() * 2.0f;    UNUSED(f32__f32_more_n_largest);
              f32 f32__f64_less_p_smallest    =  std::numeric_limits<f64>::min() / 2.0f;    UNUSED(f32__f64_less_p_smallest);
              f32 f32__f64_less_n_smallest    = -std::numeric_limits<f64>::min() / 2.0f;    UNUSED(f32__f64_less_n_smallest);
              f32 f32__f64_more_p_largest     =  std::numeric_limits<f64>::max() * 2.0f;    UNUSED(f32__f64_more_p_largest);
              f32 f32__f64_more_n_largest     = -std::numeric_limits<f64>::max() * 2.0f;    UNUSED(f32__f64_more_n_largest);
              f32 f32__f_long_less_p_smallest =  std::numeric_limits<f_long>::min() / 2.0f; UNUSED(f32__f_long_less_p_smallest);
              f32 f32__f_long_less_n_smallest = -std::numeric_limits<f_long>::min() / 2.0f; UNUSED(f32__f_long_less_n_smallest);
              f32 f32__f_long_more_p_largest  =  std::numeric_limits<f_long>::max() * 2.0f; UNUSED(f32__f_long_more_p_largest);
              f32 f32__f_long_more_n_largest  = -std::numeric_limits<f_long>::max() * 2.0f; UNUSED(f32__f_long_more_n_largest);

    constexpr f64 f64__positive               =  1.5;                                      UNUSED(f64__positive);
    constexpr f64 f64__zero                   =  0.0;                                      UNUSED(f64__zero);
    constexpr f64 f64__negative               = -1.5;                                      UNUSED(f64__negative);
    constexpr f64 f64__f32_p_smallest         =  std::numeric_limits<f32>::min();          UNUSED(f64__f32_p_smallest);
    constexpr f64 f64__f32_n_smallest         = -std::numeric_limits<f32>::min();          UNUSED(f64__f32_n_smallest);
    constexpr f64 f64__f32_p_largest          =  std::numeric_limits<f32>::max();          UNUSED(f64__f32_p_largest);
    constexpr f64 f64__f32_n_largest          = -std::numeric_limits<f32>::max();          UNUSED(f64__f32_n_largest);
    constexpr f64 f64__f64_p_smallest         =  std::numeric_limits<f64>::min();          UNUSED(f64__f64_p_smallest);
    constexpr f64 f64__f64_n_smallest         = -std::numeric_limits<f64>::min();          UNUSED(f64__f64_n_smallest);
    constexpr f64 f64__f64_p_largest          =  std::numeric_limits<f64>::max();          UNUSED(f64__f64_p_largest);
    constexpr f64 f64__f64_n_largest          = -std::numeric_limits<f64>::max();          UNUSED(f64__f64_n_largest);
    constexpr f64 f64__f_long_p_smallest      =  std::numeric_limits<f_long>::min();       UNUSED(f64__f_long_p_smallest);
    constexpr f64 f64__f_long_n_smallest      = -std::numeric_limits<f_long>::min();       UNUSED(f64__f_long_n_smallest);
//  constexpr f64 f64__f_long_p_largest       =  std::numeric_limits<f_long>::max();       UNUSED(f64__f_long_p_largest);
//  constexpr f64 f64__f_long_n_largest       = -std::numeric_limits<f_long>::max();       UNUSED(f64__f_long_n_largest);
    constexpr f64 f64__f32_nan                =  std::numeric_limits<f32>::quiet_NaN();    UNUSED(f64__f32_nan);
    constexpr f64 f64__f64_nan                =  std::numeric_limits<f64>::quiet_NaN();    UNUSED(f64__f64_nan);
    constexpr f64 f64__f_long_nan             =  std::numeric_limits<f_long>::quiet_NaN(); UNUSED(f64__f_long_nan);
    constexpr f64 f64__f32_p_inf              =  std::numeric_limits<f32>::infinity();     UNUSED(f64__f32_p_inf);
    constexpr f64 f64__f32_n_inf              = -std::numeric_limits<f32>::infinity();     UNUSED(f64__f32_n_inf);
    constexpr f64 f64__f64_p_inf              =  std::numeric_limits<f64>::infinity();     UNUSED(f64__f64_p_inf);
    constexpr f64 f64__f64_n_inf              = -std::numeric_limits<f64>::infinity();     UNUSED(f64__f64_n_inf);
    constexpr f64 f64__f_long_p_inf           =  std::numeric_limits<f_long>::infinity();  UNUSED(f64__f_long_p_inf);
    constexpr f64 f64__f_long_n_inf           = -std::numeric_limits<f_long>::infinity();  UNUSED(f64__f_long_n_inf);
              f64 f64__f32_less_p_smallest    =  std::numeric_limits<f32>::min() / 2.0;    UNUSED(f64__f32_less_p_smallest);
              f64 f64__f32_less_n_smallest    = -std::numeric_limits<f32>::min() / 2.0;    UNUSED(f64__f32_less_n_smallest);
              f64 f64__f32_more_p_largest     =  std::numeric_limits<f32>::max() * 2.0;    UNUSED(f64__f32_more_p_largest);
              f64 f64__f32_more_n_largest     = -std::numeric_limits<f32>::max() * 2.0;    UNUSED(f64__f32_more_n_largest);
              f64 f64__f64_less_p_smallest    =  std::numeric_limits<f64>::min() / 2.0;    UNUSED(f64__f64_less_p_smallest);
              f64 f64__f64_less_n_smallest    = -std::numeric_limits<f64>::min() / 2.0;    UNUSED(f64__f64_less_n_smallest);
              f64 f64__f64_more_p_largest     =  std::numeric_limits<f64>::max() * 2.0;    UNUSED(f64__f64_more_p_largest);
              f64 f64__f64_more_n_largest     = -std::numeric_limits<f64>::max() * 2.0;    UNUSED(f64__f64_more_n_largest);
              f64 f64__f_long_less_p_smallest =  std::numeric_limits<f_long>::min() / 2.0; UNUSED(f64__f_long_less_p_smallest);
              f64 f64__f_long_less_n_smallest = -std::numeric_limits<f_long>::min() / 2.0; UNUSED(f64__f_long_less_n_smallest);
              f64 f64__f_long_more_p_largest  =  std::numeric_limits<f_long>::max() * 2.0; UNUSED(f64__f_long_more_p_largest);
              f64 f64__f_long_more_n_largest  = -std::numeric_limits<f_long>::max() * 2.0; UNUSED(f64__f_long_more_n_largest);

    constexpr f_long f_long__positive               =  1.5l;                                      UNUSED(f_long__positive);
    constexpr f_long f_long__zero                   =  0.0l;                                      UNUSED(f_long__zero);
    constexpr f_long f_long__negative               = -1.5l;                                      UNUSED(f_long__negative);
    constexpr f_long f_long__f32_p_smallest         =  std::numeric_limits<f32>::min();           UNUSED(f_long__f32_p_smallest);
    constexpr f_long f_long__f32_n_smallest         = -std::numeric_limits<f32>::min();           UNUSED(f_long__f32_n_smallest);
    constexpr f_long f_long__f32_p_largest          =  std::numeric_limits<f32>::max();           UNUSED(f_long__f32_p_largest);
    constexpr f_long f_long__f32_n_largest          = -std::numeric_limits<f32>::max();           UNUSED(f_long__f32_n_largest);
    constexpr f_long f_long__f64_p_smallest         =  std::numeric_limits<f64>::min();           UNUSED(f_long__f64_p_smallest);
    constexpr f_long f_long__f64_n_smallest         = -std::numeric_limits<f64>::min();           UNUSED(f_long__f64_n_smallest);
    constexpr f_long f_long__f64_p_largest          =  std::numeric_limits<f64>::max();           UNUSED(f_long__f64_p_largest);
    constexpr f_long f_long__f64_n_largest          = -std::numeric_limits<f64>::max();           UNUSED(f_long__f64_n_largest);
    constexpr f_long f_long__f_long_p_smallest      =  std::numeric_limits<f_long>::min();        UNUSED(f_long__f_long_p_smallest);
    constexpr f_long f_long__f_long_n_smallest      = -std::numeric_limits<f_long>::min();        UNUSED(f_long__f_long_n_smallest);
    constexpr f_long f_long__f_long_p_largest       =  std::numeric_limits<f_long>::max();        UNUSED(f_long__f_long_p_largest);
    constexpr f_long f_long__f_long_n_largest       = -std::numeric_limits<f_long>::max();        UNUSED(f_long__f_long_n_largest);
    constexpr f_long f_long__f32_nan                =  std::numeric_limits<f32>::quiet_NaN();     UNUSED(f_long__f32_nan);
    constexpr f_long f_long__f64_nan                =  std::numeric_limits<f64>::quiet_NaN();     UNUSED(f_long__f64_nan);
    constexpr f_long f_long__f_long_nan             =  std::numeric_limits<f_long>::quiet_NaN();  UNUSED(f_long__f_long_nan);
    constexpr f_long f_long__f32_p_inf              =  std::numeric_limits<f32>::infinity();      UNUSED(f_long__f32_p_inf);
    constexpr f_long f_long__f32_n_inf              = -std::numeric_limits<f32>::infinity();      UNUSED(f_long__f32_n_inf);
    constexpr f_long f_long__f64_p_inf              =  std::numeric_limits<f64>::infinity();      UNUSED(f_long__f64_p_inf);
    constexpr f_long f_long__f64_n_inf              = -std::numeric_limits<f64>::infinity();      UNUSED(f_long__f64_n_inf);
    constexpr f_long f_long__f_long_p_inf           =  std::numeric_limits<f_long>::infinity();   UNUSED(f_long__f_long_p_inf);
    constexpr f_long f_long__f_long_n_inf           = -std::numeric_limits<f_long>::infinity();   UNUSED(f_long__f_long_n_inf);
              f_long f_long__f32_less_p_smallest    =  std::numeric_limits<f32>::min() / 2.0l;    UNUSED(f_long__f32_less_p_smallest);
              f_long f_long__f32_less_n_smallest    = -std::numeric_limits<f32>::min() / 2.0l;    UNUSED(f_long__f32_less_n_smallest);
              f_long f_long__f32_more_p_largest     =  std::numeric_limits<f32>::max() * 2.0l;    UNUSED(f_long__f32_more_p_largest);
              f_long f_long__f32_more_n_largest     = -std::numeric_limits<f32>::max() * 2.0l;    UNUSED(f_long__f32_more_n_largest);
              f_long f_long__f64_less_p_smallest    =  std::numeric_limits<f64>::min() / 2.0l;    UNUSED(f_long__f64_less_p_smallest);
              f_long f_long__f64_less_n_smallest    = -std::numeric_limits<f64>::min() / 2.0l;    UNUSED(f_long__f64_less_n_smallest);
              f_long f_long__f64_more_p_largest     =  std::numeric_limits<f64>::max() * 2.0l;    UNUSED(f_long__f64_more_p_largest);
              f_long f_long__f64_more_n_largest     = -std::numeric_limits<f64>::max() * 2.0l;    UNUSED(f_long__f64_more_n_largest);
              f_long f_long__f_long_less_p_smallest =  std::numeric_limits<f_long>::min() / 2.0l; UNUSED(f_long__f_long_less_p_smallest);
              f_long f_long__f_long_less_n_smallest = -std::numeric_limits<f_long>::min() / 2.0l; UNUSED(f_long__f_long_less_n_smallest);
              f_long f_long__f_long_more_p_largest  =  std::numeric_limits<f_long>::max() * 2.0l; UNUSED(f_long__f_long_more_p_largest);
              f_long f_long__f_long_more_n_largest  = -std::numeric_limits<f_long>::max() * 2.0l; UNUSED(f_long__f_long_more_n_largest);

    // You thought we were done? There are Integral overloads for the functions
    // we want to test, too.
    constexpr u64 u64__positive =  1;                               UNUSED(u64__positive);
    constexpr u64 u64__zero     =  0;                               UNUSED(u64__zero);
    constexpr u64 u64__max      =  std::numeric_limits<u64>::max(); UNUSED(u64__max);
    constexpr i64 i64__positive =  1;                               UNUSED(i64__positive);
    constexpr i64 i64__zero     =  0;                               UNUSED(i64__zero);
    constexpr i64 i64__negative = -1;                               UNUSED(i64__negative);
    constexpr i64 i64__min      =  std::numeric_limits<i64>::min(); UNUSED(i64__min); // Note: This is negative.
    constexpr i64 i64__max      =  std::numeric_limits<i64>::max(); UNUSED(i64__max);

    SECTION("f_eq_ulp") {
        constexpr auto calls_to_f_eq_ulp_are_constexpr = nonstd::cx::f_eq_ulp(f32__positive, f32__positive);

        // On platforms where f64 and f_long are identical, the smallest numbers
        // representable will be identical.
        bool f64_and_f_long_are_identical = (sizeof(f64__f64_p_smallest) ==
                                             sizeof(f_long__f_long_p_smallest));

        f64 a = 0.2;
        f64 b = 1 / std::sqrt(5) / std::sqrt(5);
        REQUIRE_FALSE(a == b);
        REQUIRE(nonstd::cx::f_eq_ulp(a, b));

        // Let's make sure we're correct about direct float comparisons
        REQUIRE((f32__f32_nan == f32__f32_nan)       == false);
        REQUIRE((f32__f32_nan == f64__f64_nan)       == false);
        REQUIRE((f32__f32_nan == f_long__f_long_nan) == false);

        REQUIRE((f32__f32_p_inf == f32__f32_p_inf)       == true);
        REQUIRE((f32__f32_p_inf == f64__f64_p_inf)       == true);
        REQUIRE((f32__f32_p_inf == f_long__f_long_p_inf) == true);

        REQUIRE((f32__f32_n_inf       == f32__f32_p_inf)       == false);
        REQUIRE((f64__f64_n_inf       == f64__f64_p_inf)       == false);
        REQUIRE((f_long__f_long_n_inf == f_long__f_long_p_inf) == false);

        REQUIRE((f32__f32_p_smallest == f64__f64_p_smallest)       == false);
        REQUIRE((f64__f64_p_smallest == f_long__f_long_p_smallest) == f64_and_f_long_are_identical);


        // Now let's make sure we match.
        REQUIRE(nonstd::cx::f_eq_ulp(f32__f32_nan, f32__f32_nan)       == false);
        REQUIRE(nonstd::cx::f_eq_ulp(f32__f32_nan, f64__f64_nan)       == false);
        REQUIRE(nonstd::cx::f_eq_ulp(f32__f32_nan, f_long__f_long_nan) == false);

        REQUIRE(nonstd::cx::f_eq_ulp(f32__f32_p_inf, f32__f32_p_inf)       == true);
        REQUIRE(nonstd::cx::f_eq_ulp(f32__f32_p_inf, f64__f64_p_inf)       == true);
        REQUIRE(nonstd::cx::f_eq_ulp(f32__f32_p_inf, f_long__f_long_p_inf) == true);

        REQUIRE(nonstd::cx::f_eq_ulp(f32__f32_n_inf,       f32__f32_p_inf)       == false);
        REQUIRE(nonstd::cx::f_eq_ulp(f64__f64_n_inf,       f64__f64_p_inf)       == false);
        REQUIRE(nonstd::cx::f_eq_ulp(f_long__f_long_n_inf, f_long__f_long_p_inf) == false);

        REQUIRE(nonstd::cx::f_eq_ulp(f32__f32_p_smallest, f64__f64_p_smallest)       == false);
        REQUIRE(nonstd::cx::f_eq_ulp(f64__f64_p_smallest, f_long__f_long_p_smallest) == f64_and_f_long_are_identical);
    }

    SECTION("f_eq_eps") {
        constexpr auto calls_to_f_eq_eps_are_constexpr = nonstd::cx::f_eq_eps(f32__positive, f32__positive);

        // On platforms where f64 and f_long are identical, the smallest numbers
        // representable will be identical.
        bool f64_and_f_long_are_identical = (sizeof(f64__f64_p_smallest) ==
                                             sizeof(f_long__f_long_p_smallest));

        f64 a = 0.2;
        f64 b = 1 / std::sqrt(5) / std::sqrt(5);
        REQUIRE_FALSE(a == b);
        REQUIRE(nonstd::cx::f_eq_eps(a, b));


        // Let's make sure we're correct about direct float comparisons
        REQUIRE((f32__f32_nan == f32__f32_nan)       == false);
        REQUIRE((f32__f32_nan == f64__f64_nan)       == false);
        REQUIRE((f32__f32_nan == f_long__f_long_nan) == false);

        REQUIRE((f32__f32_p_inf == f32__f32_p_inf)       == true);
        REQUIRE((f32__f32_p_inf == f64__f64_p_inf)       == true);
        REQUIRE((f32__f32_p_inf == f_long__f_long_p_inf) == true);

        REQUIRE((f32__f32_n_inf       == f32__f32_p_inf)       == false);
        REQUIRE((f64__f64_n_inf       == f64__f64_p_inf)       == false);
        REQUIRE((f_long__f_long_n_inf == f_long__f_long_p_inf) == false);

        REQUIRE((f32__f32_p_smallest == f64__f64_p_smallest)       == false);
        REQUIRE((f64__f64_p_smallest == f_long__f_long_p_smallest) == f64_and_f_long_are_identical);


        // Now let's make sure we match (mostly).
        REQUIRE(nonstd::cx::f_eq_eps(f32__f32_nan, f32__f32_nan)       == false);
        REQUIRE(nonstd::cx::f_eq_eps(f32__f32_nan, f64__f64_nan)       == false);
        REQUIRE(nonstd::cx::f_eq_eps(f32__f32_nan, f_long__f_long_nan) == false);

        REQUIRE(nonstd::cx::f_eq_eps(f32__f32_p_inf, f32__f32_p_inf)       == true);
        REQUIRE(nonstd::cx::f_eq_eps(f32__f32_p_inf, f64__f64_p_inf)       == true);
        REQUIRE(nonstd::cx::f_eq_eps(f32__f32_p_inf, f_long__f_long_p_inf) == true);

        REQUIRE(nonstd::cx::f_eq_eps(f32__f32_n_inf,       f32__f32_p_inf)       == false);
        REQUIRE(nonstd::cx::f_eq_eps(f64__f64_n_inf,       f64__f64_p_inf)       == false);
        REQUIRE(nonstd::cx::f_eq_eps(f_long__f_long_n_inf, f_long__f_long_p_inf) == false);

        // Note: This will always paper over the differences between the
        //       smallest representable numbers.
        REQUIRE(nonstd::cx::f_eq_eps(f32__f32_p_smallest, f64__f64_p_smallest)       == true);
        REQUIRE(nonstd::cx::f_eq_eps(f64__f64_p_smallest, f_long__f_long_p_smallest) == true);
    }

    SECTION("isinf") {
        constexpr auto calls_to_isinf_are_constexpr = nonstd::cx::isinf(f32__positive);

        REQUIRE( std::isinf(f32__positive)               == nonstd::cx::isinf(f32__positive)               );
        REQUIRE( std::isinf(f32__zero)                   == nonstd::cx::isinf(f32__zero)                   );
        REQUIRE( std::isinf(f32__negative)               == nonstd::cx::isinf(f32__negative)               );
        REQUIRE( std::isinf(f32__f32_p_smallest)         == nonstd::cx::isinf(f32__f32_p_smallest)         );
        REQUIRE( std::isinf(f32__f32_n_smallest)         == nonstd::cx::isinf(f32__f32_n_smallest)         );
        REQUIRE( std::isinf(f32__f32_p_largest)          == nonstd::cx::isinf(f32__f32_p_largest)          );
        REQUIRE( std::isinf(f32__f32_n_largest)          == nonstd::cx::isinf(f32__f32_n_largest)          );
        REQUIRE( std::isinf(f32__f64_p_smallest)         == nonstd::cx::isinf(f32__f64_p_smallest)         );
        REQUIRE( std::isinf(f32__f64_n_smallest)         == nonstd::cx::isinf(f32__f64_n_smallest)         );
    //  REQUIRE( std::isinf(f32__f64_p_largest)          == nonstd::cx::isinf(f32__f64_p_largest)          );
    //  REQUIRE( std::isinf(f32__f64_n_largest)          == nonstd::cx::isinf(f32__f64_n_largest)          );
        REQUIRE( std::isinf(f32__f_long_p_smallest)      == nonstd::cx::isinf(f32__f_long_p_smallest)      );
        REQUIRE( std::isinf(f32__f_long_n_smallest)      == nonstd::cx::isinf(f32__f_long_n_smallest)      );
    //  REQUIRE( std::isinf(f32__f_long_p_largest)       == nonstd::cx::isinf(f32__f_long_p_largest)       );
    //  REQUIRE( std::isinf(f32__f_long_n_largest)       == nonstd::cx::isinf(f32__f_long_n_largest)       );
        REQUIRE( std::isinf(f32__f32_nan)                == nonstd::cx::isinf(f32__f32_nan)                );
        REQUIRE( std::isinf(f32__f64_nan)                == nonstd::cx::isinf(f32__f64_nan)                );
        REQUIRE( std::isinf(f32__f_long_nan)             == nonstd::cx::isinf(f32__f_long_nan)             );
        REQUIRE( std::isinf(f32__f32_p_inf)              == nonstd::cx::isinf(f32__f32_p_inf)              );
        REQUIRE( std::isinf(f32__f32_n_inf)              == nonstd::cx::isinf(f32__f32_n_inf)              );
        REQUIRE( std::isinf(f32__f64_p_inf)              == nonstd::cx::isinf(f32__f64_p_inf)              );
        REQUIRE( std::isinf(f32__f64_n_inf)              == nonstd::cx::isinf(f32__f64_n_inf)              );
        REQUIRE( std::isinf(f32__f_long_p_inf)           == nonstd::cx::isinf(f32__f_long_p_inf)           );
        REQUIRE( std::isinf(f32__f_long_n_inf)           == nonstd::cx::isinf(f32__f_long_n_inf)           );
        REQUIRE( std::isinf(f32__f32_less_p_smallest)    == nonstd::cx::isinf(f32__f32_less_p_smallest)    );
        REQUIRE( std::isinf(f32__f32_less_n_smallest)    == nonstd::cx::isinf(f32__f32_less_n_smallest)    );
        REQUIRE( std::isinf(f32__f32_more_p_largest)     == nonstd::cx::isinf(f32__f32_more_p_largest)     );
        REQUIRE( std::isinf(f32__f32_more_n_largest)     == nonstd::cx::isinf(f32__f32_more_n_largest)     );
        REQUIRE( std::isinf(f32__f64_less_p_smallest)    == nonstd::cx::isinf(f32__f64_less_p_smallest)    );
        REQUIRE( std::isinf(f32__f64_less_n_smallest)    == nonstd::cx::isinf(f32__f64_less_n_smallest)    );
        REQUIRE( std::isinf(f32__f64_more_p_largest)     == nonstd::cx::isinf(f32__f64_more_p_largest)     );
        REQUIRE( std::isinf(f32__f64_more_n_largest)     == nonstd::cx::isinf(f32__f64_more_n_largest)     );
        REQUIRE( std::isinf(f32__f_long_less_p_smallest) == nonstd::cx::isinf(f32__f_long_less_p_smallest) );
        REQUIRE( std::isinf(f32__f_long_less_n_smallest) == nonstd::cx::isinf(f32__f_long_less_n_smallest) );
        REQUIRE( std::isinf(f32__f_long_more_p_largest)  == nonstd::cx::isinf(f32__f_long_more_p_largest)  );
        REQUIRE( std::isinf(f32__f_long_more_n_largest)  == nonstd::cx::isinf(f32__f_long_more_n_largest)  );

        REQUIRE( std::isinf(f64__positive)               == nonstd::cx::isinf(f64__positive)               );
        REQUIRE( std::isinf(f64__zero)                   == nonstd::cx::isinf(f64__zero)                   );
        REQUIRE( std::isinf(f64__negative)               == nonstd::cx::isinf(f64__negative)               );
        REQUIRE( std::isinf(f64__f32_p_smallest)         == nonstd::cx::isinf(f64__f32_p_smallest)         );
        REQUIRE( std::isinf(f64__f32_n_smallest)         == nonstd::cx::isinf(f64__f32_n_smallest)         );
        REQUIRE( std::isinf(f64__f32_p_largest)          == nonstd::cx::isinf(f64__f32_p_largest)          );
        REQUIRE( std::isinf(f64__f32_n_largest)          == nonstd::cx::isinf(f64__f32_n_largest)          );
        REQUIRE( std::isinf(f64__f64_p_smallest)         == nonstd::cx::isinf(f64__f64_p_smallest)         );
        REQUIRE( std::isinf(f64__f64_n_smallest)         == nonstd::cx::isinf(f64__f64_n_smallest)         );
        REQUIRE( std::isinf(f64__f64_p_largest)          == nonstd::cx::isinf(f64__f64_p_largest)          );
        REQUIRE( std::isinf(f64__f64_n_largest)          == nonstd::cx::isinf(f64__f64_n_largest)          );
        REQUIRE( std::isinf(f64__f_long_p_smallest)      == nonstd::cx::isinf(f64__f_long_p_smallest)      );
        REQUIRE( std::isinf(f64__f_long_n_smallest)      == nonstd::cx::isinf(f64__f_long_n_smallest)      );
    //  REQUIRE( std::isinf(f64__f_long_p_largest)       == nonstd::cx::isinf(f64__f_long_p_largest)       );
    //  REQUIRE( std::isinf(f64__f_long_n_largest)       == nonstd::cx::isinf(f64__f_long_n_largest)       );
        REQUIRE( std::isinf(f64__f32_nan)                == nonstd::cx::isinf(f64__f32_nan)                );
        REQUIRE( std::isinf(f64__f64_nan)                == nonstd::cx::isinf(f64__f64_nan)                );
        REQUIRE( std::isinf(f64__f_long_nan)             == nonstd::cx::isinf(f64__f_long_nan)             );
        REQUIRE( std::isinf(f64__f32_p_inf)              == nonstd::cx::isinf(f64__f32_p_inf)              );
        REQUIRE( std::isinf(f64__f32_n_inf)              == nonstd::cx::isinf(f64__f32_n_inf)              );
        REQUIRE( std::isinf(f64__f64_p_inf)              == nonstd::cx::isinf(f64__f64_p_inf)              );
        REQUIRE( std::isinf(f64__f64_n_inf)              == nonstd::cx::isinf(f64__f64_n_inf)              );
        REQUIRE( std::isinf(f64__f_long_p_inf)           == nonstd::cx::isinf(f64__f_long_p_inf)           );
        REQUIRE( std::isinf(f64__f_long_n_inf)           == nonstd::cx::isinf(f64__f_long_n_inf)           );
        REQUIRE( std::isinf(f64__f32_less_p_smallest)    == nonstd::cx::isinf(f64__f32_less_p_smallest)    );
        REQUIRE( std::isinf(f64__f32_less_n_smallest)    == nonstd::cx::isinf(f64__f32_less_n_smallest)    );
        REQUIRE( std::isinf(f64__f32_more_p_largest)     == nonstd::cx::isinf(f64__f32_more_p_largest)     );
        REQUIRE( std::isinf(f64__f32_more_n_largest)     == nonstd::cx::isinf(f64__f32_more_n_largest)     );
        REQUIRE( std::isinf(f64__f64_less_p_smallest)    == nonstd::cx::isinf(f64__f64_less_p_smallest)    );
        REQUIRE( std::isinf(f64__f64_less_n_smallest)    == nonstd::cx::isinf(f64__f64_less_n_smallest)    );
        REQUIRE( std::isinf(f64__f64_more_p_largest)     == nonstd::cx::isinf(f64__f64_more_p_largest)     );
        REQUIRE( std::isinf(f64__f64_more_n_largest)     == nonstd::cx::isinf(f64__f64_more_n_largest)     );
        REQUIRE( std::isinf(f64__f_long_less_p_smallest) == nonstd::cx::isinf(f64__f_long_less_p_smallest) );
        REQUIRE( std::isinf(f64__f_long_less_n_smallest) == nonstd::cx::isinf(f64__f_long_less_n_smallest) );
        REQUIRE( std::isinf(f64__f_long_more_p_largest)  == nonstd::cx::isinf(f64__f_long_more_p_largest)  );
        REQUIRE( std::isinf(f64__f_long_more_n_largest)  == nonstd::cx::isinf(f64__f_long_more_n_largest)  );

        REQUIRE( std::isinf(f_long__positive)               == nonstd::cx::isinf(f_long__positive)               );
        REQUIRE( std::isinf(f_long__zero)                   == nonstd::cx::isinf(f_long__zero)                   );
        REQUIRE( std::isinf(f_long__negative)               == nonstd::cx::isinf(f_long__negative)               );
        REQUIRE( std::isinf(f_long__f32_p_smallest)         == nonstd::cx::isinf(f_long__f32_p_smallest)         );
        REQUIRE( std::isinf(f_long__f32_n_smallest)         == nonstd::cx::isinf(f_long__f32_n_smallest)         );
        REQUIRE( std::isinf(f_long__f32_p_largest)          == nonstd::cx::isinf(f_long__f32_p_largest)          );
        REQUIRE( std::isinf(f_long__f32_n_largest)          == nonstd::cx::isinf(f_long__f32_n_largest)          );
        REQUIRE( std::isinf(f_long__f64_p_smallest)         == nonstd::cx::isinf(f_long__f64_p_smallest)         );
        REQUIRE( std::isinf(f_long__f64_n_smallest)         == nonstd::cx::isinf(f_long__f64_n_smallest)         );
        REQUIRE( std::isinf(f_long__f64_p_largest)          == nonstd::cx::isinf(f_long__f64_p_largest)          );
        REQUIRE( std::isinf(f_long__f64_n_largest)          == nonstd::cx::isinf(f_long__f64_n_largest)          );
        REQUIRE( std::isinf(f_long__f_long_p_smallest)      == nonstd::cx::isinf(f_long__f_long_p_smallest)      );
        REQUIRE( std::isinf(f_long__f_long_n_smallest)      == nonstd::cx::isinf(f_long__f_long_n_smallest)      );
        REQUIRE( std::isinf(f_long__f_long_p_largest)       == nonstd::cx::isinf(f_long__f_long_p_largest)       );
        REQUIRE( std::isinf(f_long__f_long_n_largest)       == nonstd::cx::isinf(f_long__f_long_n_largest)       );
        REQUIRE( std::isinf(f_long__f32_nan)                == nonstd::cx::isinf(f_long__f32_nan)                );
        REQUIRE( std::isinf(f_long__f64_nan)                == nonstd::cx::isinf(f_long__f64_nan)                );
        REQUIRE( std::isinf(f_long__f_long_nan)             == nonstd::cx::isinf(f_long__f_long_nan)             );
        REQUIRE( std::isinf(f_long__f32_p_inf)              == nonstd::cx::isinf(f_long__f32_p_inf)              );
        REQUIRE( std::isinf(f_long__f32_n_inf)              == nonstd::cx::isinf(f_long__f32_n_inf)              );
        REQUIRE( std::isinf(f_long__f64_p_inf)              == nonstd::cx::isinf(f_long__f64_p_inf)              );
        REQUIRE( std::isinf(f_long__f64_n_inf)              == nonstd::cx::isinf(f_long__f64_n_inf)              );
        REQUIRE( std::isinf(f_long__f_long_p_inf)           == nonstd::cx::isinf(f_long__f_long_p_inf)           );
        REQUIRE( std::isinf(f_long__f_long_n_inf)           == nonstd::cx::isinf(f_long__f_long_n_inf)           );
        REQUIRE( std::isinf(f_long__f32_less_p_smallest)    == nonstd::cx::isinf(f_long__f32_less_p_smallest)    );
        REQUIRE( std::isinf(f_long__f32_less_n_smallest)    == nonstd::cx::isinf(f_long__f32_less_n_smallest)    );
        REQUIRE( std::isinf(f_long__f32_more_p_largest)     == nonstd::cx::isinf(f_long__f32_more_p_largest)     );
        REQUIRE( std::isinf(f_long__f32_more_n_largest)     == nonstd::cx::isinf(f_long__f32_more_n_largest)     );
        REQUIRE( std::isinf(f_long__f64_less_p_smallest)    == nonstd::cx::isinf(f_long__f64_less_p_smallest)    );
        REQUIRE( std::isinf(f_long__f64_less_n_smallest)    == nonstd::cx::isinf(f_long__f64_less_n_smallest)    );
        REQUIRE( std::isinf(f_long__f64_more_p_largest)     == nonstd::cx::isinf(f_long__f64_more_p_largest)     );
        REQUIRE( std::isinf(f_long__f64_more_n_largest)     == nonstd::cx::isinf(f_long__f64_more_n_largest)     );
        REQUIRE( std::isinf(f_long__f_long_less_p_smallest) == nonstd::cx::isinf(f_long__f_long_less_p_smallest) );
        REQUIRE( std::isinf(f_long__f_long_less_n_smallest) == nonstd::cx::isinf(f_long__f_long_less_n_smallest) );
        REQUIRE( std::isinf(f_long__f_long_more_p_largest)  == nonstd::cx::isinf(f_long__f_long_more_p_largest)  );
        REQUIRE( std::isinf(f_long__f_long_more_n_largest)  == nonstd::cx::isinf(f_long__f_long_more_n_largest)  );

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

        REQUIRE( std::isnan(f32__positive)               == nonstd::cx::isnan(f32__positive)               );
        REQUIRE( std::isnan(f32__zero)                   == nonstd::cx::isnan(f32__zero)                   );
        REQUIRE( std::isnan(f32__negative)               == nonstd::cx::isnan(f32__negative)               );
        REQUIRE( std::isnan(f32__f32_p_smallest)         == nonstd::cx::isnan(f32__f32_p_smallest)         );
        REQUIRE( std::isnan(f32__f32_n_smallest)         == nonstd::cx::isnan(f32__f32_n_smallest)         );
        REQUIRE( std::isnan(f32__f32_p_largest)          == nonstd::cx::isnan(f32__f32_p_largest)          );
        REQUIRE( std::isnan(f32__f32_n_largest)          == nonstd::cx::isnan(f32__f32_n_largest)          );
        REQUIRE( std::isnan(f32__f64_p_smallest)         == nonstd::cx::isnan(f32__f64_p_smallest)         );
        REQUIRE( std::isnan(f32__f64_n_smallest)         == nonstd::cx::isnan(f32__f64_n_smallest)         );
    //  REQUIRE( std::isnan(f32__f64_p_largest)          == nonstd::cx::isnan(f32__f64_p_largest)          );
    //  REQUIRE( std::isnan(f32__f64_n_largest)          == nonstd::cx::isnan(f32__f64_n_largest)          );
        REQUIRE( std::isnan(f32__f_long_p_smallest)      == nonstd::cx::isnan(f32__f_long_p_smallest)      );
        REQUIRE( std::isnan(f32__f_long_n_smallest)      == nonstd::cx::isnan(f32__f_long_n_smallest)      );
    //  REQUIRE( std::isnan(f32__f_long_p_largest)       == nonstd::cx::isnan(f32__f_long_p_largest)       );
    //  REQUIRE( std::isnan(f32__f_long_n_largest)       == nonstd::cx::isnan(f32__f_long_n_largest)       );
        REQUIRE( std::isnan(f32__f32_nan)                == nonstd::cx::isnan(f32__f32_nan)                );
        REQUIRE( std::isnan(f32__f64_nan)                == nonstd::cx::isnan(f32__f64_nan)                );
        REQUIRE( std::isnan(f32__f_long_nan)             == nonstd::cx::isnan(f32__f_long_nan)             );
        REQUIRE( std::isnan(f32__f32_p_inf)              == nonstd::cx::isnan(f32__f32_p_inf)              );
        REQUIRE( std::isnan(f32__f32_n_inf)              == nonstd::cx::isnan(f32__f32_n_inf)              );
        REQUIRE( std::isnan(f32__f64_p_inf)              == nonstd::cx::isnan(f32__f64_p_inf)              );
        REQUIRE( std::isnan(f32__f64_n_inf)              == nonstd::cx::isnan(f32__f64_n_inf)              );
        REQUIRE( std::isnan(f32__f_long_p_inf)           == nonstd::cx::isnan(f32__f_long_p_inf)           );
        REQUIRE( std::isnan(f32__f_long_n_inf)           == nonstd::cx::isnan(f32__f_long_n_inf)           );
        REQUIRE( std::isnan(f32__f32_less_p_smallest)    == nonstd::cx::isnan(f32__f32_less_p_smallest)    );
        REQUIRE( std::isnan(f32__f32_less_n_smallest)    == nonstd::cx::isnan(f32__f32_less_n_smallest)    );
        REQUIRE( std::isnan(f32__f32_more_p_largest)     == nonstd::cx::isnan(f32__f32_more_p_largest)     );
        REQUIRE( std::isnan(f32__f32_more_n_largest)     == nonstd::cx::isnan(f32__f32_more_n_largest)     );
        REQUIRE( std::isnan(f32__f64_less_p_smallest)    == nonstd::cx::isnan(f32__f64_less_p_smallest)    );
        REQUIRE( std::isnan(f32__f64_less_n_smallest)    == nonstd::cx::isnan(f32__f64_less_n_smallest)    );
        REQUIRE( std::isnan(f32__f64_more_p_largest)     == nonstd::cx::isnan(f32__f64_more_p_largest)     );
        REQUIRE( std::isnan(f32__f64_more_n_largest)     == nonstd::cx::isnan(f32__f64_more_n_largest)     );
        REQUIRE( std::isnan(f32__f_long_less_p_smallest) == nonstd::cx::isnan(f32__f_long_less_p_smallest) );
        REQUIRE( std::isnan(f32__f_long_less_n_smallest) == nonstd::cx::isnan(f32__f_long_less_n_smallest) );
        REQUIRE( std::isnan(f32__f_long_more_p_largest)  == nonstd::cx::isnan(f32__f_long_more_p_largest)  );
        REQUIRE( std::isnan(f32__f_long_more_n_largest)  == nonstd::cx::isnan(f32__f_long_more_n_largest)  );

        REQUIRE( std::isnan(f64__positive)               == nonstd::cx::isnan(f64__positive)               );
        REQUIRE( std::isnan(f64__zero)                   == nonstd::cx::isnan(f64__zero)                   );
        REQUIRE( std::isnan(f64__negative)               == nonstd::cx::isnan(f64__negative)               );
        REQUIRE( std::isnan(f64__f32_p_smallest)         == nonstd::cx::isnan(f64__f32_p_smallest)         );
        REQUIRE( std::isnan(f64__f32_n_smallest)         == nonstd::cx::isnan(f64__f32_n_smallest)         );
        REQUIRE( std::isnan(f64__f32_p_largest)          == nonstd::cx::isnan(f64__f32_p_largest)          );
        REQUIRE( std::isnan(f64__f32_n_largest)          == nonstd::cx::isnan(f64__f32_n_largest)          );
        REQUIRE( std::isnan(f64__f64_p_smallest)         == nonstd::cx::isnan(f64__f64_p_smallest)         );
        REQUIRE( std::isnan(f64__f64_n_smallest)         == nonstd::cx::isnan(f64__f64_n_smallest)         );
        REQUIRE( std::isnan(f64__f64_p_largest)          == nonstd::cx::isnan(f64__f64_p_largest)          );
        REQUIRE( std::isnan(f64__f64_n_largest)          == nonstd::cx::isnan(f64__f64_n_largest)          );
        REQUIRE( std::isnan(f64__f_long_p_smallest)      == nonstd::cx::isnan(f64__f_long_p_smallest)      );
        REQUIRE( std::isnan(f64__f_long_n_smallest)      == nonstd::cx::isnan(f64__f_long_n_smallest)      );
    //  REQUIRE( std::isnan(f64__f_long_p_largest)       == nonstd::cx::isnan(f64__f_long_p_largest)       );
    //  REQUIRE( std::isnan(f64__f_long_n_largest)       == nonstd::cx::isnan(f64__f_long_n_largest)       );
        REQUIRE( std::isnan(f64__f32_nan)                == nonstd::cx::isnan(f64__f32_nan)                );
        REQUIRE( std::isnan(f64__f64_nan)                == nonstd::cx::isnan(f64__f64_nan)                );
        REQUIRE( std::isnan(f64__f_long_nan)             == nonstd::cx::isnan(f64__f_long_nan)             );
        REQUIRE( std::isnan(f64__f32_p_inf)              == nonstd::cx::isnan(f64__f32_p_inf)              );
        REQUIRE( std::isnan(f64__f32_n_inf)              == nonstd::cx::isnan(f64__f32_n_inf)              );
        REQUIRE( std::isnan(f64__f64_p_inf)              == nonstd::cx::isnan(f64__f64_p_inf)              );
        REQUIRE( std::isnan(f64__f64_n_inf)              == nonstd::cx::isnan(f64__f64_n_inf)              );
        REQUIRE( std::isnan(f64__f_long_p_inf)           == nonstd::cx::isnan(f64__f_long_p_inf)           );
        REQUIRE( std::isnan(f64__f_long_n_inf)           == nonstd::cx::isnan(f64__f_long_n_inf)           );
        REQUIRE( std::isnan(f64__f32_less_p_smallest)    == nonstd::cx::isnan(f64__f32_less_p_smallest)    );
        REQUIRE( std::isnan(f64__f32_less_n_smallest)    == nonstd::cx::isnan(f64__f32_less_n_smallest)    );
        REQUIRE( std::isnan(f64__f32_more_p_largest)     == nonstd::cx::isnan(f64__f32_more_p_largest)     );
        REQUIRE( std::isnan(f64__f32_more_n_largest)     == nonstd::cx::isnan(f64__f32_more_n_largest)     );
        REQUIRE( std::isnan(f64__f64_less_p_smallest)    == nonstd::cx::isnan(f64__f64_less_p_smallest)    );
        REQUIRE( std::isnan(f64__f64_less_n_smallest)    == nonstd::cx::isnan(f64__f64_less_n_smallest)    );
        REQUIRE( std::isnan(f64__f64_more_p_largest)     == nonstd::cx::isnan(f64__f64_more_p_largest)     );
        REQUIRE( std::isnan(f64__f64_more_n_largest)     == nonstd::cx::isnan(f64__f64_more_n_largest)     );
        REQUIRE( std::isnan(f64__f_long_less_p_smallest) == nonstd::cx::isnan(f64__f_long_less_p_smallest) );
        REQUIRE( std::isnan(f64__f_long_less_n_smallest) == nonstd::cx::isnan(f64__f_long_less_n_smallest) );
        REQUIRE( std::isnan(f64__f_long_more_p_largest)  == nonstd::cx::isnan(f64__f_long_more_p_largest)  );
        REQUIRE( std::isnan(f64__f_long_more_n_largest)  == nonstd::cx::isnan(f64__f_long_more_n_largest)  );

        REQUIRE( std::isnan(f_long__positive)               == nonstd::cx::isnan(f_long__positive)               );
        REQUIRE( std::isnan(f_long__zero)                   == nonstd::cx::isnan(f_long__zero)                   );
        REQUIRE( std::isnan(f_long__negative)               == nonstd::cx::isnan(f_long__negative)               );
        REQUIRE( std::isnan(f_long__f32_p_smallest)         == nonstd::cx::isnan(f_long__f32_p_smallest)         );
        REQUIRE( std::isnan(f_long__f32_n_smallest)         == nonstd::cx::isnan(f_long__f32_n_smallest)         );
        REQUIRE( std::isnan(f_long__f32_p_largest)          == nonstd::cx::isnan(f_long__f32_p_largest)          );
        REQUIRE( std::isnan(f_long__f32_n_largest)          == nonstd::cx::isnan(f_long__f32_n_largest)          );
        REQUIRE( std::isnan(f_long__f64_p_smallest)         == nonstd::cx::isnan(f_long__f64_p_smallest)         );
        REQUIRE( std::isnan(f_long__f64_n_smallest)         == nonstd::cx::isnan(f_long__f64_n_smallest)         );
        REQUIRE( std::isnan(f_long__f64_p_largest)          == nonstd::cx::isnan(f_long__f64_p_largest)          );
        REQUIRE( std::isnan(f_long__f64_n_largest)          == nonstd::cx::isnan(f_long__f64_n_largest)          );
        REQUIRE( std::isnan(f_long__f_long_p_smallest)      == nonstd::cx::isnan(f_long__f_long_p_smallest)      );
        REQUIRE( std::isnan(f_long__f_long_n_smallest)      == nonstd::cx::isnan(f_long__f_long_n_smallest)      );
        REQUIRE( std::isnan(f_long__f_long_p_largest)       == nonstd::cx::isnan(f_long__f_long_p_largest)       );
        REQUIRE( std::isnan(f_long__f_long_n_largest)       == nonstd::cx::isnan(f_long__f_long_n_largest)       );
        REQUIRE( std::isnan(f_long__f32_nan)                == nonstd::cx::isnan(f_long__f32_nan)                );
        REQUIRE( std::isnan(f_long__f64_nan)                == nonstd::cx::isnan(f_long__f64_nan)                );
        REQUIRE( std::isnan(f_long__f_long_nan)             == nonstd::cx::isnan(f_long__f_long_nan)             );
        REQUIRE( std::isnan(f_long__f32_p_inf)              == nonstd::cx::isnan(f_long__f32_p_inf)              );
        REQUIRE( std::isnan(f_long__f32_n_inf)              == nonstd::cx::isnan(f_long__f32_n_inf)              );
        REQUIRE( std::isnan(f_long__f64_p_inf)              == nonstd::cx::isnan(f_long__f64_p_inf)              );
        REQUIRE( std::isnan(f_long__f64_n_inf)              == nonstd::cx::isnan(f_long__f64_n_inf)              );
        REQUIRE( std::isnan(f_long__f_long_p_inf)           == nonstd::cx::isnan(f_long__f_long_p_inf)           );
        REQUIRE( std::isnan(f_long__f_long_n_inf)           == nonstd::cx::isnan(f_long__f_long_n_inf)           );
        REQUIRE( std::isnan(f_long__f32_less_p_smallest)    == nonstd::cx::isnan(f_long__f32_less_p_smallest)    );
        REQUIRE( std::isnan(f_long__f32_less_n_smallest)    == nonstd::cx::isnan(f_long__f32_less_n_smallest)    );
        REQUIRE( std::isnan(f_long__f32_more_p_largest)     == nonstd::cx::isnan(f_long__f32_more_p_largest)     );
        REQUIRE( std::isnan(f_long__f32_more_n_largest)     == nonstd::cx::isnan(f_long__f32_more_n_largest)     );
        REQUIRE( std::isnan(f_long__f64_less_p_smallest)    == nonstd::cx::isnan(f_long__f64_less_p_smallest)    );
        REQUIRE( std::isnan(f_long__f64_less_n_smallest)    == nonstd::cx::isnan(f_long__f64_less_n_smallest)    );
        REQUIRE( std::isnan(f_long__f64_more_p_largest)     == nonstd::cx::isnan(f_long__f64_more_p_largest)     );
        REQUIRE( std::isnan(f_long__f64_more_n_largest)     == nonstd::cx::isnan(f_long__f64_more_n_largest)     );
        REQUIRE( std::isnan(f_long__f_long_less_p_smallest) == nonstd::cx::isnan(f_long__f_long_less_p_smallest) );
        REQUIRE( std::isnan(f_long__f_long_less_n_smallest) == nonstd::cx::isnan(f_long__f_long_less_n_smallest) );
        REQUIRE( std::isnan(f_long__f_long_more_p_largest)  == nonstd::cx::isnan(f_long__f_long_more_p_largest)  );
        REQUIRE( std::isnan(f_long__f_long_more_n_largest)  == nonstd::cx::isnan(f_long__f_long_more_n_largest)  );

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
        REQUIRE(( ( std::abs(f32__positive)               == nonstd::cx::abs(f32__positive) )               || ( isnan(std::abs(f32__positive))               && isnan(nonstd::cx::abs(f32__positive)) )               ));
        REQUIRE(( ( std::abs(f32__zero)                   == nonstd::cx::abs(f32__zero) )                   || ( isnan(std::abs(f32__zero))                   && isnan(nonstd::cx::abs(f32__zero)) )                   ));
        REQUIRE(( ( std::abs(f32__negative)               == nonstd::cx::abs(f32__negative) )               || ( isnan(std::abs(f32__negative))               && isnan(nonstd::cx::abs(f32__negative)) )               ));
        REQUIRE(( ( std::abs(f32__f32_p_smallest)         == nonstd::cx::abs(f32__f32_p_smallest) )         || ( isnan(std::abs(f32__f32_p_smallest))         && isnan(nonstd::cx::abs(f32__f32_p_smallest)) )         ));
        REQUIRE(( ( std::abs(f32__f32_n_smallest)         == nonstd::cx::abs(f32__f32_n_smallest) )         || ( isnan(std::abs(f32__f32_n_smallest))         && isnan(nonstd::cx::abs(f32__f32_n_smallest)) )         ));
        REQUIRE(( ( std::abs(f32__f32_p_largest)          == nonstd::cx::abs(f32__f32_p_largest) )          || ( isnan(std::abs(f32__f32_p_largest))          && isnan(nonstd::cx::abs(f32__f32_p_largest)) )          ));
        REQUIRE(( ( std::abs(f32__f32_n_largest)          == nonstd::cx::abs(f32__f32_n_largest) )          || ( isnan(std::abs(f32__f32_n_largest))          && isnan(nonstd::cx::abs(f32__f32_n_largest)) )          ));
        REQUIRE(( ( std::abs(f32__f64_p_smallest)         == nonstd::cx::abs(f32__f64_p_smallest) )         || ( isnan(std::abs(f32__f64_p_smallest))         && isnan(nonstd::cx::abs(f32__f64_p_smallest)) )         ));
        REQUIRE(( ( std::abs(f32__f64_n_smallest)         == nonstd::cx::abs(f32__f64_n_smallest) )         || ( isnan(std::abs(f32__f64_n_smallest))         && isnan(nonstd::cx::abs(f32__f64_n_smallest)) )         ));
    //  REQUIRE(( ( std::abs(f32__f64_p_largest)          == nonstd::cx::abs(f32__f64_p_largest) )          || ( isnan(std::abs(f32__f64_p_largest))          && isnan(nonstd::cx::abs(f32__f64_p_largest)) )          ));
    //  REQUIRE(( ( std::abs(f32__f64_n_largest)          == nonstd::cx::abs(f32__f64_n_largest) )          || ( isnan(std::abs(f32__f64_n_largest))          && isnan(nonstd::cx::abs(f32__f64_n_largest)) )          ));
        REQUIRE(( ( std::abs(f32__f_long_p_smallest)      == nonstd::cx::abs(f32__f_long_p_smallest) )      || ( isnan(std::abs(f32__f_long_p_smallest))      && isnan(nonstd::cx::abs(f32__f_long_p_smallest)) )      ));
        REQUIRE(( ( std::abs(f32__f_long_n_smallest)      == nonstd::cx::abs(f32__f_long_n_smallest) )      || ( isnan(std::abs(f32__f_long_n_smallest))      && isnan(nonstd::cx::abs(f32__f_long_n_smallest)) )      ));
    //  REQUIRE(( ( std::abs(f32__f_long_p_largest)       == nonstd::cx::abs(f32__f_long_p_largest) )       || ( isnan(std::abs(f32__f_long_p_largest))       && isnan(nonstd::cx::abs(f32__f_long_p_largest)) )       ));
    //  REQUIRE(( ( std::abs(f32__f_long_n_largest)       == nonstd::cx::abs(f32__f_long_n_largest) )       || ( isnan(std::abs(f32__f_long_n_largest))       && isnan(nonstd::cx::abs(f32__f_long_n_largest)) )       ));
        REQUIRE(( ( std::abs(f32__f32_nan)                == nonstd::cx::abs(f32__f32_nan) )                || ( isnan(std::abs(f32__f32_nan))                && isnan(nonstd::cx::abs(f32__f32_nan)) )                ));
        REQUIRE(( ( std::abs(f32__f64_nan)                == nonstd::cx::abs(f32__f64_nan) )                || ( isnan(std::abs(f32__f64_nan))                && isnan(nonstd::cx::abs(f32__f64_nan)) )                ));
        REQUIRE(( ( std::abs(f32__f_long_nan)             == nonstd::cx::abs(f32__f_long_nan) )             || ( isnan(std::abs(f32__f_long_nan))             && isnan(nonstd::cx::abs(f32__f_long_nan)) )             ));
        REQUIRE(( ( std::abs(f32__f32_p_inf)              == nonstd::cx::abs(f32__f32_p_inf) )              || ( isnan(std::abs(f32__f32_p_inf))              && isnan(nonstd::cx::abs(f32__f32_p_inf)) )              ));
        REQUIRE(( ( std::abs(f32__f32_n_inf)              == nonstd::cx::abs(f32__f32_n_inf) )              || ( isnan(std::abs(f32__f32_n_inf))              && isnan(nonstd::cx::abs(f32__f32_n_inf)) )              ));
        REQUIRE(( ( std::abs(f32__f64_p_inf)              == nonstd::cx::abs(f32__f64_p_inf) )              || ( isnan(std::abs(f32__f64_p_inf))              && isnan(nonstd::cx::abs(f32__f64_p_inf)) )              ));
        REQUIRE(( ( std::abs(f32__f64_n_inf)              == nonstd::cx::abs(f32__f64_n_inf) )              || ( isnan(std::abs(f32__f64_n_inf))              && isnan(nonstd::cx::abs(f32__f64_n_inf)) )              ));
        REQUIRE(( ( std::abs(f32__f_long_p_inf)           == nonstd::cx::abs(f32__f_long_p_inf) )           || ( isnan(std::abs(f32__f_long_p_inf))           && isnan(nonstd::cx::abs(f32__f_long_p_inf)) )           ));
        REQUIRE(( ( std::abs(f32__f_long_n_inf)           == nonstd::cx::abs(f32__f_long_n_inf) )           || ( isnan(std::abs(f32__f_long_n_inf))           && isnan(nonstd::cx::abs(f32__f_long_n_inf)) )           ));
        REQUIRE(( ( std::abs(f32__f32_less_p_smallest)    == nonstd::cx::abs(f32__f32_less_p_smallest) )    || ( isnan(std::abs(f32__f32_less_p_smallest))    && isnan(nonstd::cx::abs(f32__f32_less_p_smallest)) )    ));
        REQUIRE(( ( std::abs(f32__f32_less_n_smallest)    == nonstd::cx::abs(f32__f32_less_n_smallest) )    || ( isnan(std::abs(f32__f32_less_n_smallest))    && isnan(nonstd::cx::abs(f32__f32_less_n_smallest)) )    ));
        REQUIRE(( ( std::abs(f32__f32_more_p_largest)     == nonstd::cx::abs(f32__f32_more_p_largest) )     || ( isnan(std::abs(f32__f32_more_p_largest))     && isnan(nonstd::cx::abs(f32__f32_more_p_largest)) )     ));
        REQUIRE(( ( std::abs(f32__f32_more_n_largest)     == nonstd::cx::abs(f32__f32_more_n_largest) )     || ( isnan(std::abs(f32__f32_more_n_largest))     && isnan(nonstd::cx::abs(f32__f32_more_n_largest)) )     ));
        REQUIRE(( ( std::abs(f32__f64_less_p_smallest)    == nonstd::cx::abs(f32__f64_less_p_smallest) )    || ( isnan(std::abs(f32__f64_less_p_smallest))    && isnan(nonstd::cx::abs(f32__f64_less_p_smallest)) )    ));
        REQUIRE(( ( std::abs(f32__f64_less_n_smallest)    == nonstd::cx::abs(f32__f64_less_n_smallest) )    || ( isnan(std::abs(f32__f64_less_n_smallest))    && isnan(nonstd::cx::abs(f32__f64_less_n_smallest)) )    ));
        REQUIRE(( ( std::abs(f32__f64_more_p_largest)     == nonstd::cx::abs(f32__f64_more_p_largest) )     || ( isnan(std::abs(f32__f64_more_p_largest))     && isnan(nonstd::cx::abs(f32__f64_more_p_largest)) )     ));
        REQUIRE(( ( std::abs(f32__f64_more_n_largest)     == nonstd::cx::abs(f32__f64_more_n_largest) )     || ( isnan(std::abs(f32__f64_more_n_largest))     && isnan(nonstd::cx::abs(f32__f64_more_n_largest)) )     ));
        REQUIRE(( ( std::abs(f32__f_long_less_p_smallest) == nonstd::cx::abs(f32__f_long_less_p_smallest) ) || ( isnan(std::abs(f32__f_long_less_p_smallest)) && isnan(nonstd::cx::abs(f32__f_long_less_p_smallest)) ) ));
        REQUIRE(( ( std::abs(f32__f_long_less_n_smallest) == nonstd::cx::abs(f32__f_long_less_n_smallest) ) || ( isnan(std::abs(f32__f_long_less_n_smallest)) && isnan(nonstd::cx::abs(f32__f_long_less_n_smallest)) ) ));
        REQUIRE(( ( std::abs(f32__f_long_more_p_largest)  == nonstd::cx::abs(f32__f_long_more_p_largest) )  || ( isnan(std::abs(f32__f_long_more_p_largest))  && isnan(nonstd::cx::abs(f32__f_long_more_p_largest)) )  ));
        REQUIRE(( ( std::abs(f32__f_long_more_n_largest)  == nonstd::cx::abs(f32__f_long_more_n_largest) )  || ( isnan(std::abs(f32__f_long_more_n_largest))  && isnan(nonstd::cx::abs(f32__f_long_more_n_largest)) )  ));

        REQUIRE(( ( std::abs(f64__positive)               == nonstd::cx::abs(f64__positive) )               || ( isnan(std::abs(f64__positive))               && isnan(nonstd::cx::abs(f64__positive)) )               ));
        REQUIRE(( ( std::abs(f64__zero)                   == nonstd::cx::abs(f64__zero) )                   || ( isnan(std::abs(f64__zero))                   && isnan(nonstd::cx::abs(f64__zero)) )                   ));
        REQUIRE(( ( std::abs(f64__negative)               == nonstd::cx::abs(f64__negative) )               || ( isnan(std::abs(f64__negative))               && isnan(nonstd::cx::abs(f64__negative)) )               ));
        REQUIRE(( ( std::abs(f64__f32_p_smallest)         == nonstd::cx::abs(f64__f32_p_smallest) )         || ( isnan(std::abs(f64__f32_p_smallest))         && isnan(nonstd::cx::abs(f64__f32_p_smallest)) )         ));
        REQUIRE(( ( std::abs(f64__f32_n_smallest)         == nonstd::cx::abs(f64__f32_n_smallest) )         || ( isnan(std::abs(f64__f32_n_smallest))         && isnan(nonstd::cx::abs(f64__f32_n_smallest)) )         ));
        REQUIRE(( ( std::abs(f64__f32_p_largest)          == nonstd::cx::abs(f64__f32_p_largest) )          || ( isnan(std::abs(f64__f32_p_largest))          && isnan(nonstd::cx::abs(f64__f32_p_largest)) )          ));
        REQUIRE(( ( std::abs(f64__f32_n_largest)          == nonstd::cx::abs(f64__f32_n_largest) )          || ( isnan(std::abs(f64__f32_n_largest))          && isnan(nonstd::cx::abs(f64__f32_n_largest)) )          ));
        REQUIRE(( ( std::abs(f64__f64_p_smallest)         == nonstd::cx::abs(f64__f64_p_smallest) )         || ( isnan(std::abs(f64__f64_p_smallest))         && isnan(nonstd::cx::abs(f64__f64_p_smallest)) )         ));
        REQUIRE(( ( std::abs(f64__f64_n_smallest)         == nonstd::cx::abs(f64__f64_n_smallest) )         || ( isnan(std::abs(f64__f64_n_smallest))         && isnan(nonstd::cx::abs(f64__f64_n_smallest)) )         ));
        REQUIRE(( ( std::abs(f64__f64_p_largest)          == nonstd::cx::abs(f64__f64_p_largest) )          || ( isnan(std::abs(f64__f64_p_largest))          && isnan(nonstd::cx::abs(f64__f64_p_largest)) )          ));
        REQUIRE(( ( std::abs(f64__f64_n_largest)          == nonstd::cx::abs(f64__f64_n_largest) )          || ( isnan(std::abs(f64__f64_n_largest))          && isnan(nonstd::cx::abs(f64__f64_n_largest)) )          ));
        REQUIRE(( ( std::abs(f64__f_long_p_smallest)      == nonstd::cx::abs(f64__f_long_p_smallest) )      || ( isnan(std::abs(f64__f_long_p_smallest))      && isnan(nonstd::cx::abs(f64__f_long_p_smallest)) )      ));
        REQUIRE(( ( std::abs(f64__f_long_n_smallest)      == nonstd::cx::abs(f64__f_long_n_smallest) )      || ( isnan(std::abs(f64__f_long_n_smallest))      && isnan(nonstd::cx::abs(f64__f_long_n_smallest)) )      ));
    //  REQUIRE(( ( std::abs(f64__f_long_p_largest)       == nonstd::cx::abs(f64__f_long_p_largest) )       || ( isnan(std::abs(f64__f_long_p_largest))       && isnan(nonstd::cx::abs(f64__f_long_p_largest)) )       ));
    //  REQUIRE(( ( std::abs(f64__f_long_n_largest)       == nonstd::cx::abs(f64__f_long_n_largest) )       || ( isnan(std::abs(f64__f_long_n_largest))       && isnan(nonstd::cx::abs(f64__f_long_n_largest)) )       ));
        REQUIRE(( ( std::abs(f64__f32_nan)                == nonstd::cx::abs(f64__f32_nan) )                || ( isnan(std::abs(f64__f32_nan))                && isnan(nonstd::cx::abs(f64__f32_nan)) )                ));
        REQUIRE(( ( std::abs(f64__f64_nan)                == nonstd::cx::abs(f64__f64_nan) )                || ( isnan(std::abs(f64__f64_nan))                && isnan(nonstd::cx::abs(f64__f64_nan)) )                ));
        REQUIRE(( ( std::abs(f64__f_long_nan)             == nonstd::cx::abs(f64__f_long_nan) )             || ( isnan(std::abs(f64__f_long_nan))             && isnan(nonstd::cx::abs(f64__f_long_nan)) )             ));
        REQUIRE(( ( std::abs(f64__f32_p_inf)              == nonstd::cx::abs(f64__f32_p_inf) )              || ( isnan(std::abs(f64__f32_p_inf))              && isnan(nonstd::cx::abs(f64__f32_p_inf)) )              ));
        REQUIRE(( ( std::abs(f64__f32_n_inf)              == nonstd::cx::abs(f64__f32_n_inf) )              || ( isnan(std::abs(f64__f32_n_inf))              && isnan(nonstd::cx::abs(f64__f32_n_inf)) )              ));
        REQUIRE(( ( std::abs(f64__f64_p_inf)              == nonstd::cx::abs(f64__f64_p_inf) )              || ( isnan(std::abs(f64__f64_p_inf))              && isnan(nonstd::cx::abs(f64__f64_p_inf)) )              ));
        REQUIRE(( ( std::abs(f64__f64_n_inf)              == nonstd::cx::abs(f64__f64_n_inf) )              || ( isnan(std::abs(f64__f64_n_inf))              && isnan(nonstd::cx::abs(f64__f64_n_inf)) )              ));
        REQUIRE(( ( std::abs(f64__f_long_p_inf)           == nonstd::cx::abs(f64__f_long_p_inf) )           || ( isnan(std::abs(f64__f_long_p_inf))           && isnan(nonstd::cx::abs(f64__f_long_p_inf)) )           ));
        REQUIRE(( ( std::abs(f64__f_long_n_inf)           == nonstd::cx::abs(f64__f_long_n_inf) )           || ( isnan(std::abs(f64__f_long_n_inf))           && isnan(nonstd::cx::abs(f64__f_long_n_inf)) )           ));
        REQUIRE(( ( std::abs(f64__f32_less_p_smallest)    == nonstd::cx::abs(f64__f32_less_p_smallest) )    || ( isnan(std::abs(f64__f32_less_p_smallest))    && isnan(nonstd::cx::abs(f64__f32_less_p_smallest)) )    ));
        REQUIRE(( ( std::abs(f64__f32_less_n_smallest)    == nonstd::cx::abs(f64__f32_less_n_smallest) )    || ( isnan(std::abs(f64__f32_less_n_smallest))    && isnan(nonstd::cx::abs(f64__f32_less_n_smallest)) )    ));
        REQUIRE(( ( std::abs(f64__f32_more_p_largest)     == nonstd::cx::abs(f64__f32_more_p_largest) )     || ( isnan(std::abs(f64__f32_more_p_largest))     && isnan(nonstd::cx::abs(f64__f32_more_p_largest)) )     ));
        REQUIRE(( ( std::abs(f64__f32_more_n_largest)     == nonstd::cx::abs(f64__f32_more_n_largest) )     || ( isnan(std::abs(f64__f32_more_n_largest))     && isnan(nonstd::cx::abs(f64__f32_more_n_largest)) )     ));
        REQUIRE(( ( std::abs(f64__f64_less_p_smallest)    == nonstd::cx::abs(f64__f64_less_p_smallest) )    || ( isnan(std::abs(f64__f64_less_p_smallest))    && isnan(nonstd::cx::abs(f64__f64_less_p_smallest)) )    ));
        REQUIRE(( ( std::abs(f64__f64_less_n_smallest)    == nonstd::cx::abs(f64__f64_less_n_smallest) )    || ( isnan(std::abs(f64__f64_less_n_smallest))    && isnan(nonstd::cx::abs(f64__f64_less_n_smallest)) )    ));
        REQUIRE(( ( std::abs(f64__f64_more_p_largest)     == nonstd::cx::abs(f64__f64_more_p_largest) )     || ( isnan(std::abs(f64__f64_more_p_largest))     && isnan(nonstd::cx::abs(f64__f64_more_p_largest)) )     ));
        REQUIRE(( ( std::abs(f64__f64_more_n_largest)     == nonstd::cx::abs(f64__f64_more_n_largest) )     || ( isnan(std::abs(f64__f64_more_n_largest))     && isnan(nonstd::cx::abs(f64__f64_more_n_largest)) )     ));
        REQUIRE(( ( std::abs(f64__f_long_less_p_smallest) == nonstd::cx::abs(f64__f_long_less_p_smallest) ) || ( isnan(std::abs(f64__f_long_less_p_smallest)) && isnan(nonstd::cx::abs(f64__f_long_less_p_smallest)) ) ));
        REQUIRE(( ( std::abs(f64__f_long_less_n_smallest) == nonstd::cx::abs(f64__f_long_less_n_smallest) ) || ( isnan(std::abs(f64__f_long_less_n_smallest)) && isnan(nonstd::cx::abs(f64__f_long_less_n_smallest)) ) ));
        REQUIRE(( ( std::abs(f64__f_long_more_p_largest)  == nonstd::cx::abs(f64__f_long_more_p_largest) )  || ( isnan(std::abs(f64__f_long_more_p_largest))  && isnan(nonstd::cx::abs(f64__f_long_more_p_largest)) )  ));
        REQUIRE(( ( std::abs(f64__f_long_more_n_largest)  == nonstd::cx::abs(f64__f_long_more_n_largest) )  || ( isnan(std::abs(f64__f_long_more_n_largest))  && isnan(nonstd::cx::abs(f64__f_long_more_n_largest)) )  ));

        REQUIRE(( ( std::abs(f_long__positive)               == nonstd::cx::abs(f_long__positive) )               || ( isnan(std::abs(f_long__positive))               && isnan(nonstd::cx::abs(f_long__positive)) )               ));
        REQUIRE(( ( std::abs(f_long__zero)                   == nonstd::cx::abs(f_long__zero) )                   || ( isnan(std::abs(f_long__zero))                   && isnan(nonstd::cx::abs(f_long__zero)) )                   ));
        REQUIRE(( ( std::abs(f_long__negative)               == nonstd::cx::abs(f_long__negative) )               || ( isnan(std::abs(f_long__negative))               && isnan(nonstd::cx::abs(f_long__negative)) )               ));
        REQUIRE(( ( std::abs(f_long__f32_p_smallest)         == nonstd::cx::abs(f_long__f32_p_smallest) )         || ( isnan(std::abs(f_long__f32_p_smallest))         && isnan(nonstd::cx::abs(f_long__f32_p_smallest)) )         ));
        REQUIRE(( ( std::abs(f_long__f32_n_smallest)         == nonstd::cx::abs(f_long__f32_n_smallest) )         || ( isnan(std::abs(f_long__f32_n_smallest))         && isnan(nonstd::cx::abs(f_long__f32_n_smallest)) )         ));
        REQUIRE(( ( std::abs(f_long__f32_p_largest)          == nonstd::cx::abs(f_long__f32_p_largest) )          || ( isnan(std::abs(f_long__f32_p_largest))          && isnan(nonstd::cx::abs(f_long__f32_p_largest)) )          ));
        REQUIRE(( ( std::abs(f_long__f32_n_largest)          == nonstd::cx::abs(f_long__f32_n_largest) )          || ( isnan(std::abs(f_long__f32_n_largest))          && isnan(nonstd::cx::abs(f_long__f32_n_largest)) )          ));
        REQUIRE(( ( std::abs(f_long__f64_p_smallest)         == nonstd::cx::abs(f_long__f64_p_smallest) )         || ( isnan(std::abs(f_long__f64_p_smallest))         && isnan(nonstd::cx::abs(f_long__f64_p_smallest)) )         ));
        REQUIRE(( ( std::abs(f_long__f64_n_smallest)         == nonstd::cx::abs(f_long__f64_n_smallest) )         || ( isnan(std::abs(f_long__f64_n_smallest))         && isnan(nonstd::cx::abs(f_long__f64_n_smallest)) )         ));
        REQUIRE(( ( std::abs(f_long__f64_p_largest)          == nonstd::cx::abs(f_long__f64_p_largest) )          || ( isnan(std::abs(f_long__f64_p_largest))          && isnan(nonstd::cx::abs(f_long__f64_p_largest)) )          ));
        REQUIRE(( ( std::abs(f_long__f64_n_largest)          == nonstd::cx::abs(f_long__f64_n_largest) )          || ( isnan(std::abs(f_long__f64_n_largest))          && isnan(nonstd::cx::abs(f_long__f64_n_largest)) )          ));
        REQUIRE(( ( std::abs(f_long__f_long_p_smallest)      == nonstd::cx::abs(f_long__f_long_p_smallest) )      || ( isnan(std::abs(f_long__f_long_p_smallest))      && isnan(nonstd::cx::abs(f_long__f_long_p_smallest)) )      ));
        REQUIRE(( ( std::abs(f_long__f_long_n_smallest)      == nonstd::cx::abs(f_long__f_long_n_smallest) )      || ( isnan(std::abs(f_long__f_long_n_smallest))      && isnan(nonstd::cx::abs(f_long__f_long_n_smallest)) )      ));
        REQUIRE(( ( std::abs(f_long__f_long_p_largest)       == nonstd::cx::abs(f_long__f_long_p_largest) )       || ( isnan(std::abs(f_long__f_long_p_largest))       && isnan(nonstd::cx::abs(f_long__f_long_p_largest)) )       ));
        REQUIRE(( ( std::abs(f_long__f_long_n_largest)       == nonstd::cx::abs(f_long__f_long_n_largest) )       || ( isnan(std::abs(f_long__f_long_n_largest))       && isnan(nonstd::cx::abs(f_long__f_long_n_largest)) )       ));
        REQUIRE(( ( std::abs(f_long__f32_nan)                == nonstd::cx::abs(f_long__f32_nan) )                || ( isnan(std::abs(f_long__f32_nan))                && isnan(nonstd::cx::abs(f_long__f32_nan)) )                ));
        REQUIRE(( ( std::abs(f_long__f64_nan)                == nonstd::cx::abs(f_long__f64_nan) )                || ( isnan(std::abs(f_long__f64_nan))                && isnan(nonstd::cx::abs(f_long__f64_nan)) )                ));
        REQUIRE(( ( std::abs(f_long__f_long_nan)             == nonstd::cx::abs(f_long__f_long_nan) )             || ( isnan(std::abs(f_long__f_long_nan))             && isnan(nonstd::cx::abs(f_long__f_long_nan)) )             ));
        REQUIRE(( ( std::abs(f_long__f32_p_inf)              == nonstd::cx::abs(f_long__f32_p_inf) )              || ( isnan(std::abs(f_long__f32_p_inf))              && isnan(nonstd::cx::abs(f_long__f32_p_inf)) )              ));
        REQUIRE(( ( std::abs(f_long__f32_n_inf)              == nonstd::cx::abs(f_long__f32_n_inf) )              || ( isnan(std::abs(f_long__f32_n_inf))              && isnan(nonstd::cx::abs(f_long__f32_n_inf)) )              ));
        REQUIRE(( ( std::abs(f_long__f64_p_inf)              == nonstd::cx::abs(f_long__f64_p_inf) )              || ( isnan(std::abs(f_long__f64_p_inf))              && isnan(nonstd::cx::abs(f_long__f64_p_inf)) )              ));
        REQUIRE(( ( std::abs(f_long__f64_n_inf)              == nonstd::cx::abs(f_long__f64_n_inf) )              || ( isnan(std::abs(f_long__f64_n_inf))              && isnan(nonstd::cx::abs(f_long__f64_n_inf)) )              ));
        REQUIRE(( ( std::abs(f_long__f_long_p_inf)           == nonstd::cx::abs(f_long__f_long_p_inf) )           || ( isnan(std::abs(f_long__f_long_p_inf))           && isnan(nonstd::cx::abs(f_long__f_long_p_inf)) )           ));
        REQUIRE(( ( std::abs(f_long__f_long_n_inf)           == nonstd::cx::abs(f_long__f_long_n_inf) )           || ( isnan(std::abs(f_long__f_long_n_inf))           && isnan(nonstd::cx::abs(f_long__f_long_n_inf)) )           ));
        REQUIRE(( ( std::abs(f_long__f32_less_p_smallest)    == nonstd::cx::abs(f_long__f32_less_p_smallest) )    || ( isnan(std::abs(f_long__f32_less_p_smallest))    && isnan(nonstd::cx::abs(f_long__f32_less_p_smallest)) )    ));
        REQUIRE(( ( std::abs(f_long__f32_less_n_smallest)    == nonstd::cx::abs(f_long__f32_less_n_smallest) )    || ( isnan(std::abs(f_long__f32_less_n_smallest))    && isnan(nonstd::cx::abs(f_long__f32_less_n_smallest)) )    ));
        REQUIRE(( ( std::abs(f_long__f32_more_p_largest)     == nonstd::cx::abs(f_long__f32_more_p_largest) )     || ( isnan(std::abs(f_long__f32_more_p_largest))     && isnan(nonstd::cx::abs(f_long__f32_more_p_largest)) )     ));
        REQUIRE(( ( std::abs(f_long__f32_more_n_largest)     == nonstd::cx::abs(f_long__f32_more_n_largest) )     || ( isnan(std::abs(f_long__f32_more_n_largest))     && isnan(nonstd::cx::abs(f_long__f32_more_n_largest)) )     ));
        REQUIRE(( ( std::abs(f_long__f64_less_p_smallest)    == nonstd::cx::abs(f_long__f64_less_p_smallest) )    || ( isnan(std::abs(f_long__f64_less_p_smallest))    && isnan(nonstd::cx::abs(f_long__f64_less_p_smallest)) )    ));
        REQUIRE(( ( std::abs(f_long__f64_less_n_smallest)    == nonstd::cx::abs(f_long__f64_less_n_smallest) )    || ( isnan(std::abs(f_long__f64_less_n_smallest))    && isnan(nonstd::cx::abs(f_long__f64_less_n_smallest)) )    ));
        REQUIRE(( ( std::abs(f_long__f64_more_p_largest)     == nonstd::cx::abs(f_long__f64_more_p_largest) )     || ( isnan(std::abs(f_long__f64_more_p_largest))     && isnan(nonstd::cx::abs(f_long__f64_more_p_largest)) )     ));
        REQUIRE(( ( std::abs(f_long__f64_more_n_largest)     == nonstd::cx::abs(f_long__f64_more_n_largest) )     || ( isnan(std::abs(f_long__f64_more_n_largest))     && isnan(nonstd::cx::abs(f_long__f64_more_n_largest)) )     ));
        REQUIRE(( ( std::abs(f_long__f_long_less_p_smallest) == nonstd::cx::abs(f_long__f_long_less_p_smallest) ) || ( isnan(std::abs(f_long__f_long_less_p_smallest)) && isnan(nonstd::cx::abs(f_long__f_long_less_p_smallest)) ) ));
        REQUIRE(( ( std::abs(f_long__f_long_less_n_smallest) == nonstd::cx::abs(f_long__f_long_less_n_smallest) ) || ( isnan(std::abs(f_long__f_long_less_n_smallest)) && isnan(nonstd::cx::abs(f_long__f_long_less_n_smallest)) ) ));
        REQUIRE(( ( std::abs(f_long__f_long_more_p_largest)  == nonstd::cx::abs(f_long__f_long_more_p_largest) )  || ( isnan(std::abs(f_long__f_long_more_p_largest))  && isnan(nonstd::cx::abs(f_long__f_long_more_p_largest)) )  ));
        REQUIRE(( ( std::abs(f_long__f_long_more_n_largest)  == nonstd::cx::abs(f_long__f_long_more_n_largest) )  || ( isnan(std::abs(f_long__f_long_more_n_largest))  && isnan(nonstd::cx::abs(f_long__f_long_more_n_largest)) )  ));

        // std::abs doesn't have Integral overloads. so these functions wind up
        // not compiling with ambiguous overload errors.

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
        REQUIRE(( ( std::fabs(f32__positive)               == nonstd::cx::fabs(f32__positive) )               || ( isnan(std::fabs(f32__positive))               && isnan(nonstd::cx::fabs(f32__positive)) )               ));
        REQUIRE(( ( std::fabs(f32__zero)                   == nonstd::cx::fabs(f32__zero) )                   || ( isnan(std::fabs(f32__zero))                   && isnan(nonstd::cx::fabs(f32__zero)) )                   ));
        REQUIRE(( ( std::fabs(f32__negative)               == nonstd::cx::fabs(f32__negative) )               || ( isnan(std::fabs(f32__negative))               && isnan(nonstd::cx::fabs(f32__negative)) )               ));
        REQUIRE(( ( std::fabs(f32__f32_p_smallest)         == nonstd::cx::fabs(f32__f32_p_smallest) )         || ( isnan(std::fabs(f32__f32_p_smallest))         && isnan(nonstd::cx::fabs(f32__f32_p_smallest)) )         ));
        REQUIRE(( ( std::fabs(f32__f32_n_smallest)         == nonstd::cx::fabs(f32__f32_n_smallest) )         || ( isnan(std::fabs(f32__f32_n_smallest))         && isnan(nonstd::cx::fabs(f32__f32_n_smallest)) )         ));
        REQUIRE(( ( std::fabs(f32__f32_p_largest)          == nonstd::cx::fabs(f32__f32_p_largest) )          || ( isnan(std::fabs(f32__f32_p_largest))          && isnan(nonstd::cx::fabs(f32__f32_p_largest)) )          ));
        REQUIRE(( ( std::fabs(f32__f32_n_largest)          == nonstd::cx::fabs(f32__f32_n_largest) )          || ( isnan(std::fabs(f32__f32_n_largest))          && isnan(nonstd::cx::fabs(f32__f32_n_largest)) )          ));
        REQUIRE(( ( std::fabs(f32__f64_p_smallest)         == nonstd::cx::fabs(f32__f64_p_smallest) )         || ( isnan(std::fabs(f32__f64_p_smallest))         && isnan(nonstd::cx::fabs(f32__f64_p_smallest)) )         ));
        REQUIRE(( ( std::fabs(f32__f64_n_smallest)         == nonstd::cx::fabs(f32__f64_n_smallest) )         || ( isnan(std::fabs(f32__f64_n_smallest))         && isnan(nonstd::cx::fabs(f32__f64_n_smallest)) )         ));
    //  REQUIRE(( ( std::fabs(f32__f64_p_largest)          == nonstd::cx::fabs(f32__f64_p_largest) )          || ( isnan(std::fabs(f32__f64_p_largest))          && isnan(nonstd::cx::fabs(f32__f64_p_largest)) )          ));
    //  REQUIRE(( ( std::fabs(f32__f64_n_largest)          == nonstd::cx::fabs(f32__f64_n_largest) )          || ( isnan(std::fabs(f32__f64_n_largest))          && isnan(nonstd::cx::fabs(f32__f64_n_largest)) )          ));
        REQUIRE(( ( std::fabs(f32__f_long_p_smallest)      == nonstd::cx::fabs(f32__f_long_p_smallest) )      || ( isnan(std::fabs(f32__f_long_p_smallest))      && isnan(nonstd::cx::fabs(f32__f_long_p_smallest)) )      ));
        REQUIRE(( ( std::fabs(f32__f_long_n_smallest)      == nonstd::cx::fabs(f32__f_long_n_smallest) )      || ( isnan(std::fabs(f32__f_long_n_smallest))      && isnan(nonstd::cx::fabs(f32__f_long_n_smallest)) )      ));
    //  REQUIRE(( ( std::fabs(f32__f_long_p_largest)       == nonstd::cx::fabs(f32__f_long_p_largest) )       || ( isnan(std::fabs(f32__f_long_p_largest))       && isnan(nonstd::cx::fabs(f32__f_long_p_largest)) )       ));
    //  REQUIRE(( ( std::fabs(f32__f_long_n_largest)       == nonstd::cx::fabs(f32__f_long_n_largest) )       || ( isnan(std::fabs(f32__f_long_n_largest))       && isnan(nonstd::cx::fabs(f32__f_long_n_largest)) )       ));
        REQUIRE(( ( std::fabs(f32__f32_nan)                == nonstd::cx::fabs(f32__f32_nan) )                || ( isnan(std::fabs(f32__f32_nan))                && isnan(nonstd::cx::fabs(f32__f32_nan)) )                ));
        REQUIRE(( ( std::fabs(f32__f64_nan)                == nonstd::cx::fabs(f32__f64_nan) )                || ( isnan(std::fabs(f32__f64_nan))                && isnan(nonstd::cx::fabs(f32__f64_nan)) )                ));
        REQUIRE(( ( std::fabs(f32__f_long_nan)             == nonstd::cx::fabs(f32__f_long_nan) )             || ( isnan(std::fabs(f32__f_long_nan))             && isnan(nonstd::cx::fabs(f32__f_long_nan)) )             ));
        REQUIRE(( ( std::fabs(f32__f32_p_inf)              == nonstd::cx::fabs(f32__f32_p_inf) )              || ( isnan(std::fabs(f32__f32_p_inf))              && isnan(nonstd::cx::fabs(f32__f32_p_inf)) )              ));
        REQUIRE(( ( std::fabs(f32__f32_n_inf)              == nonstd::cx::fabs(f32__f32_n_inf) )              || ( isnan(std::fabs(f32__f32_n_inf))              && isnan(nonstd::cx::fabs(f32__f32_n_inf)) )              ));
        REQUIRE(( ( std::fabs(f32__f64_p_inf)              == nonstd::cx::fabs(f32__f64_p_inf) )              || ( isnan(std::fabs(f32__f64_p_inf))              && isnan(nonstd::cx::fabs(f32__f64_p_inf)) )              ));
        REQUIRE(( ( std::fabs(f32__f64_n_inf)              == nonstd::cx::fabs(f32__f64_n_inf) )              || ( isnan(std::fabs(f32__f64_n_inf))              && isnan(nonstd::cx::fabs(f32__f64_n_inf)) )              ));
        REQUIRE(( ( std::fabs(f32__f_long_p_inf)           == nonstd::cx::fabs(f32__f_long_p_inf) )           || ( isnan(std::fabs(f32__f_long_p_inf))           && isnan(nonstd::cx::fabs(f32__f_long_p_inf)) )           ));
        REQUIRE(( ( std::fabs(f32__f_long_n_inf)           == nonstd::cx::fabs(f32__f_long_n_inf) )           || ( isnan(std::fabs(f32__f_long_n_inf))           && isnan(nonstd::cx::fabs(f32__f_long_n_inf)) )           ));
        REQUIRE(( ( std::fabs(f32__f32_less_p_smallest)    == nonstd::cx::fabs(f32__f32_less_p_smallest) )    || ( isnan(std::fabs(f32__f32_less_p_smallest))    && isnan(nonstd::cx::fabs(f32__f32_less_p_smallest)) )    ));
        REQUIRE(( ( std::fabs(f32__f32_less_n_smallest)    == nonstd::cx::fabs(f32__f32_less_n_smallest) )    || ( isnan(std::fabs(f32__f32_less_n_smallest))    && isnan(nonstd::cx::fabs(f32__f32_less_n_smallest)) )    ));
        REQUIRE(( ( std::fabs(f32__f32_more_p_largest)     == nonstd::cx::fabs(f32__f32_more_p_largest) )     || ( isnan(std::fabs(f32__f32_more_p_largest))     && isnan(nonstd::cx::fabs(f32__f32_more_p_largest)) )     ));
        REQUIRE(( ( std::fabs(f32__f32_more_n_largest)     == nonstd::cx::fabs(f32__f32_more_n_largest) )     || ( isnan(std::fabs(f32__f32_more_n_largest))     && isnan(nonstd::cx::fabs(f32__f32_more_n_largest)) )     ));
        REQUIRE(( ( std::fabs(f32__f64_less_p_smallest)    == nonstd::cx::fabs(f32__f64_less_p_smallest) )    || ( isnan(std::fabs(f32__f64_less_p_smallest))    && isnan(nonstd::cx::fabs(f32__f64_less_p_smallest)) )    ));
        REQUIRE(( ( std::fabs(f32__f64_less_n_smallest)    == nonstd::cx::fabs(f32__f64_less_n_smallest) )    || ( isnan(std::fabs(f32__f64_less_n_smallest))    && isnan(nonstd::cx::fabs(f32__f64_less_n_smallest)) )    ));
        REQUIRE(( ( std::fabs(f32__f64_more_p_largest)     == nonstd::cx::fabs(f32__f64_more_p_largest) )     || ( isnan(std::fabs(f32__f64_more_p_largest))     && isnan(nonstd::cx::fabs(f32__f64_more_p_largest)) )     ));
        REQUIRE(( ( std::fabs(f32__f64_more_n_largest)     == nonstd::cx::fabs(f32__f64_more_n_largest) )     || ( isnan(std::fabs(f32__f64_more_n_largest))     && isnan(nonstd::cx::fabs(f32__f64_more_n_largest)) )     ));
        REQUIRE(( ( std::fabs(f32__f_long_less_p_smallest) == nonstd::cx::fabs(f32__f_long_less_p_smallest) ) || ( isnan(std::fabs(f32__f_long_less_p_smallest)) && isnan(nonstd::cx::fabs(f32__f_long_less_p_smallest)) ) ));
        REQUIRE(( ( std::fabs(f32__f_long_less_n_smallest) == nonstd::cx::fabs(f32__f_long_less_n_smallest) ) || ( isnan(std::fabs(f32__f_long_less_n_smallest)) && isnan(nonstd::cx::fabs(f32__f_long_less_n_smallest)) ) ));
        REQUIRE(( ( std::fabs(f32__f_long_more_p_largest)  == nonstd::cx::fabs(f32__f_long_more_p_largest) )  || ( isnan(std::fabs(f32__f_long_more_p_largest))  && isnan(nonstd::cx::fabs(f32__f_long_more_p_largest)) )  ));
        REQUIRE(( ( std::fabs(f32__f_long_more_n_largest)  == nonstd::cx::fabs(f32__f_long_more_n_largest) )  || ( isnan(std::fabs(f32__f_long_more_n_largest))  && isnan(nonstd::cx::fabs(f32__f_long_more_n_largest)) )  ));

        REQUIRE(( ( std::fabs(f64__positive)               == nonstd::cx::fabs(f64__positive) )               || ( isnan(std::fabs(f64__positive))               && isnan(nonstd::cx::fabs(f64__positive)) )               ));
        REQUIRE(( ( std::fabs(f64__zero)                   == nonstd::cx::fabs(f64__zero) )                   || ( isnan(std::fabs(f64__zero))                   && isnan(nonstd::cx::fabs(f64__zero)) )                   ));
        REQUIRE(( ( std::fabs(f64__negative)               == nonstd::cx::fabs(f64__negative) )               || ( isnan(std::fabs(f64__negative))               && isnan(nonstd::cx::fabs(f64__negative)) )               ));
        REQUIRE(( ( std::fabs(f64__f32_p_smallest)         == nonstd::cx::fabs(f64__f32_p_smallest) )         || ( isnan(std::fabs(f64__f32_p_smallest))         && isnan(nonstd::cx::fabs(f64__f32_p_smallest)) )         ));
        REQUIRE(( ( std::fabs(f64__f32_n_smallest)         == nonstd::cx::fabs(f64__f32_n_smallest) )         || ( isnan(std::fabs(f64__f32_n_smallest))         && isnan(nonstd::cx::fabs(f64__f32_n_smallest)) )         ));
        REQUIRE(( ( std::fabs(f64__f32_p_largest)          == nonstd::cx::fabs(f64__f32_p_largest) )          || ( isnan(std::fabs(f64__f32_p_largest))          && isnan(nonstd::cx::fabs(f64__f32_p_largest)) )          ));
        REQUIRE(( ( std::fabs(f64__f32_n_largest)          == nonstd::cx::fabs(f64__f32_n_largest) )          || ( isnan(std::fabs(f64__f32_n_largest))          && isnan(nonstd::cx::fabs(f64__f32_n_largest)) )          ));
        REQUIRE(( ( std::fabs(f64__f64_p_smallest)         == nonstd::cx::fabs(f64__f64_p_smallest) )         || ( isnan(std::fabs(f64__f64_p_smallest))         && isnan(nonstd::cx::fabs(f64__f64_p_smallest)) )         ));
        REQUIRE(( ( std::fabs(f64__f64_n_smallest)         == nonstd::cx::fabs(f64__f64_n_smallest) )         || ( isnan(std::fabs(f64__f64_n_smallest))         && isnan(nonstd::cx::fabs(f64__f64_n_smallest)) )         ));
        REQUIRE(( ( std::fabs(f64__f64_p_largest)          == nonstd::cx::fabs(f64__f64_p_largest) )          || ( isnan(std::fabs(f64__f64_p_largest))          && isnan(nonstd::cx::fabs(f64__f64_p_largest)) )          ));
        REQUIRE(( ( std::fabs(f64__f64_n_largest)          == nonstd::cx::fabs(f64__f64_n_largest) )          || ( isnan(std::fabs(f64__f64_n_largest))          && isnan(nonstd::cx::fabs(f64__f64_n_largest)) )          ));
        REQUIRE(( ( std::fabs(f64__f_long_p_smallest)      == nonstd::cx::fabs(f64__f_long_p_smallest) )      || ( isnan(std::fabs(f64__f_long_p_smallest))      && isnan(nonstd::cx::fabs(f64__f_long_p_smallest)) )      ));
        REQUIRE(( ( std::fabs(f64__f_long_n_smallest)      == nonstd::cx::fabs(f64__f_long_n_smallest) )      || ( isnan(std::fabs(f64__f_long_n_smallest))      && isnan(nonstd::cx::fabs(f64__f_long_n_smallest)) )      ));
    //  REQUIRE(( ( std::fabs(f64__f_long_p_largest)       == nonstd::cx::fabs(f64__f_long_p_largest) )       || ( isnan(std::fabs(f64__f_long_p_largest))       && isnan(nonstd::cx::fabs(f64__f_long_p_largest)) )       ));
    //  REQUIRE(( ( std::fabs(f64__f_long_n_largest)       == nonstd::cx::fabs(f64__f_long_n_largest) )       || ( isnan(std::fabs(f64__f_long_n_largest))       && isnan(nonstd::cx::fabs(f64__f_long_n_largest)) )       ));
        REQUIRE(( ( std::fabs(f64__f32_nan)                == nonstd::cx::fabs(f64__f32_nan) )                || ( isnan(std::fabs(f64__f32_nan))                && isnan(nonstd::cx::fabs(f64__f32_nan)) )                ));
        REQUIRE(( ( std::fabs(f64__f64_nan)                == nonstd::cx::fabs(f64__f64_nan) )                || ( isnan(std::fabs(f64__f64_nan))                && isnan(nonstd::cx::fabs(f64__f64_nan)) )                ));
        REQUIRE(( ( std::fabs(f64__f_long_nan)             == nonstd::cx::fabs(f64__f_long_nan) )             || ( isnan(std::fabs(f64__f_long_nan))             && isnan(nonstd::cx::fabs(f64__f_long_nan)) )             ));
        REQUIRE(( ( std::fabs(f64__f32_p_inf)              == nonstd::cx::fabs(f64__f32_p_inf) )              || ( isnan(std::fabs(f64__f32_p_inf))              && isnan(nonstd::cx::fabs(f64__f32_p_inf)) )              ));
        REQUIRE(( ( std::fabs(f64__f32_n_inf)              == nonstd::cx::fabs(f64__f32_n_inf) )              || ( isnan(std::fabs(f64__f32_n_inf))              && isnan(nonstd::cx::fabs(f64__f32_n_inf)) )              ));
        REQUIRE(( ( std::fabs(f64__f64_p_inf)              == nonstd::cx::fabs(f64__f64_p_inf) )              || ( isnan(std::fabs(f64__f64_p_inf))              && isnan(nonstd::cx::fabs(f64__f64_p_inf)) )              ));
        REQUIRE(( ( std::fabs(f64__f64_n_inf)              == nonstd::cx::fabs(f64__f64_n_inf) )              || ( isnan(std::fabs(f64__f64_n_inf))              && isnan(nonstd::cx::fabs(f64__f64_n_inf)) )              ));
        REQUIRE(( ( std::fabs(f64__f_long_p_inf)           == nonstd::cx::fabs(f64__f_long_p_inf) )           || ( isnan(std::fabs(f64__f_long_p_inf))           && isnan(nonstd::cx::fabs(f64__f_long_p_inf)) )           ));
        REQUIRE(( ( std::fabs(f64__f_long_n_inf)           == nonstd::cx::fabs(f64__f_long_n_inf) )           || ( isnan(std::fabs(f64__f_long_n_inf))           && isnan(nonstd::cx::fabs(f64__f_long_n_inf)) )           ));
        REQUIRE(( ( std::fabs(f64__f32_less_p_smallest)    == nonstd::cx::fabs(f64__f32_less_p_smallest) )    || ( isnan(std::fabs(f64__f32_less_p_smallest))    && isnan(nonstd::cx::fabs(f64__f32_less_p_smallest)) )    ));
        REQUIRE(( ( std::fabs(f64__f32_less_n_smallest)    == nonstd::cx::fabs(f64__f32_less_n_smallest) )    || ( isnan(std::fabs(f64__f32_less_n_smallest))    && isnan(nonstd::cx::fabs(f64__f32_less_n_smallest)) )    ));
        REQUIRE(( ( std::fabs(f64__f32_more_p_largest)     == nonstd::cx::fabs(f64__f32_more_p_largest) )     || ( isnan(std::fabs(f64__f32_more_p_largest))     && isnan(nonstd::cx::fabs(f64__f32_more_p_largest)) )     ));
        REQUIRE(( ( std::fabs(f64__f32_more_n_largest)     == nonstd::cx::fabs(f64__f32_more_n_largest) )     || ( isnan(std::fabs(f64__f32_more_n_largest))     && isnan(nonstd::cx::fabs(f64__f32_more_n_largest)) )     ));
        REQUIRE(( ( std::fabs(f64__f64_less_p_smallest)    == nonstd::cx::fabs(f64__f64_less_p_smallest) )    || ( isnan(std::fabs(f64__f64_less_p_smallest))    && isnan(nonstd::cx::fabs(f64__f64_less_p_smallest)) )    ));
        REQUIRE(( ( std::fabs(f64__f64_less_n_smallest)    == nonstd::cx::fabs(f64__f64_less_n_smallest) )    || ( isnan(std::fabs(f64__f64_less_n_smallest))    && isnan(nonstd::cx::fabs(f64__f64_less_n_smallest)) )    ));
        REQUIRE(( ( std::fabs(f64__f64_more_p_largest)     == nonstd::cx::fabs(f64__f64_more_p_largest) )     || ( isnan(std::fabs(f64__f64_more_p_largest))     && isnan(nonstd::cx::fabs(f64__f64_more_p_largest)) )     ));
        REQUIRE(( ( std::fabs(f64__f64_more_n_largest)     == nonstd::cx::fabs(f64__f64_more_n_largest) )     || ( isnan(std::fabs(f64__f64_more_n_largest))     && isnan(nonstd::cx::fabs(f64__f64_more_n_largest)) )     ));
        REQUIRE(( ( std::fabs(f64__f_long_less_p_smallest) == nonstd::cx::fabs(f64__f_long_less_p_smallest) ) || ( isnan(std::fabs(f64__f_long_less_p_smallest)) && isnan(nonstd::cx::fabs(f64__f_long_less_p_smallest)) ) ));
        REQUIRE(( ( std::fabs(f64__f_long_less_n_smallest) == nonstd::cx::fabs(f64__f_long_less_n_smallest) ) || ( isnan(std::fabs(f64__f_long_less_n_smallest)) && isnan(nonstd::cx::fabs(f64__f_long_less_n_smallest)) ) ));
        REQUIRE(( ( std::fabs(f64__f_long_more_p_largest)  == nonstd::cx::fabs(f64__f_long_more_p_largest) )  || ( isnan(std::fabs(f64__f_long_more_p_largest))  && isnan(nonstd::cx::fabs(f64__f_long_more_p_largest)) )  ));
        REQUIRE(( ( std::fabs(f64__f_long_more_n_largest)  == nonstd::cx::fabs(f64__f_long_more_n_largest) )  || ( isnan(std::fabs(f64__f_long_more_n_largest))  && isnan(nonstd::cx::fabs(f64__f_long_more_n_largest)) )  ));

        REQUIRE(( ( std::fabs(f_long__positive)               == nonstd::cx::fabs(f_long__positive) )               || ( isnan(std::fabs(f_long__positive))               && isnan(nonstd::cx::fabs(f_long__positive)) )               ));
        REQUIRE(( ( std::fabs(f_long__zero)                   == nonstd::cx::fabs(f_long__zero) )                   || ( isnan(std::fabs(f_long__zero))                   && isnan(nonstd::cx::fabs(f_long__zero)) )                   ));
        REQUIRE(( ( std::fabs(f_long__negative)               == nonstd::cx::fabs(f_long__negative) )               || ( isnan(std::fabs(f_long__negative))               && isnan(nonstd::cx::fabs(f_long__negative)) )               ));
        REQUIRE(( ( std::fabs(f_long__f32_p_smallest)         == nonstd::cx::fabs(f_long__f32_p_smallest) )         || ( isnan(std::fabs(f_long__f32_p_smallest))         && isnan(nonstd::cx::fabs(f_long__f32_p_smallest)) )         ));
        REQUIRE(( ( std::fabs(f_long__f32_n_smallest)         == nonstd::cx::fabs(f_long__f32_n_smallest) )         || ( isnan(std::fabs(f_long__f32_n_smallest))         && isnan(nonstd::cx::fabs(f_long__f32_n_smallest)) )         ));
        REQUIRE(( ( std::fabs(f_long__f32_p_largest)          == nonstd::cx::fabs(f_long__f32_p_largest) )          || ( isnan(std::fabs(f_long__f32_p_largest))          && isnan(nonstd::cx::fabs(f_long__f32_p_largest)) )          ));
        REQUIRE(( ( std::fabs(f_long__f32_n_largest)          == nonstd::cx::fabs(f_long__f32_n_largest) )          || ( isnan(std::fabs(f_long__f32_n_largest))          && isnan(nonstd::cx::fabs(f_long__f32_n_largest)) )          ));
        REQUIRE(( ( std::fabs(f_long__f64_p_smallest)         == nonstd::cx::fabs(f_long__f64_p_smallest) )         || ( isnan(std::fabs(f_long__f64_p_smallest))         && isnan(nonstd::cx::fabs(f_long__f64_p_smallest)) )         ));
        REQUIRE(( ( std::fabs(f_long__f64_n_smallest)         == nonstd::cx::fabs(f_long__f64_n_smallest) )         || ( isnan(std::fabs(f_long__f64_n_smallest))         && isnan(nonstd::cx::fabs(f_long__f64_n_smallest)) )         ));
        REQUIRE(( ( std::fabs(f_long__f64_p_largest)          == nonstd::cx::fabs(f_long__f64_p_largest) )          || ( isnan(std::fabs(f_long__f64_p_largest))          && isnan(nonstd::cx::fabs(f_long__f64_p_largest)) )          ));
        REQUIRE(( ( std::fabs(f_long__f64_n_largest)          == nonstd::cx::fabs(f_long__f64_n_largest) )          || ( isnan(std::fabs(f_long__f64_n_largest))          && isnan(nonstd::cx::fabs(f_long__f64_n_largest)) )          ));
        REQUIRE(( ( std::fabs(f_long__f_long_p_smallest)      == nonstd::cx::fabs(f_long__f_long_p_smallest) )      || ( isnan(std::fabs(f_long__f_long_p_smallest))      && isnan(nonstd::cx::fabs(f_long__f_long_p_smallest)) )      ));
        REQUIRE(( ( std::fabs(f_long__f_long_n_smallest)      == nonstd::cx::fabs(f_long__f_long_n_smallest) )      || ( isnan(std::fabs(f_long__f_long_n_smallest))      && isnan(nonstd::cx::fabs(f_long__f_long_n_smallest)) )      ));
        REQUIRE(( ( std::fabs(f_long__f_long_p_largest)       == nonstd::cx::fabs(f_long__f_long_p_largest) )       || ( isnan(std::fabs(f_long__f_long_p_largest))       && isnan(nonstd::cx::fabs(f_long__f_long_p_largest)) )       ));
        REQUIRE(( ( std::fabs(f_long__f_long_n_largest)       == nonstd::cx::fabs(f_long__f_long_n_largest) )       || ( isnan(std::fabs(f_long__f_long_n_largest))       && isnan(nonstd::cx::fabs(f_long__f_long_n_largest)) )       ));
        REQUIRE(( ( std::fabs(f_long__f32_nan)                == nonstd::cx::fabs(f_long__f32_nan) )                || ( isnan(std::fabs(f_long__f32_nan))                && isnan(nonstd::cx::fabs(f_long__f32_nan)) )                ));
        REQUIRE(( ( std::fabs(f_long__f64_nan)                == nonstd::cx::fabs(f_long__f64_nan) )                || ( isnan(std::fabs(f_long__f64_nan))                && isnan(nonstd::cx::fabs(f_long__f64_nan)) )                ));
        REQUIRE(( ( std::fabs(f_long__f_long_nan)             == nonstd::cx::fabs(f_long__f_long_nan) )             || ( isnan(std::fabs(f_long__f_long_nan))             && isnan(nonstd::cx::fabs(f_long__f_long_nan)) )             ));
        REQUIRE(( ( std::fabs(f_long__f32_p_inf)              == nonstd::cx::fabs(f_long__f32_p_inf) )              || ( isnan(std::fabs(f_long__f32_p_inf))              && isnan(nonstd::cx::fabs(f_long__f32_p_inf)) )              ));
        REQUIRE(( ( std::fabs(f_long__f32_n_inf)              == nonstd::cx::fabs(f_long__f32_n_inf) )              || ( isnan(std::fabs(f_long__f32_n_inf))              && isnan(nonstd::cx::fabs(f_long__f32_n_inf)) )              ));
        REQUIRE(( ( std::fabs(f_long__f64_p_inf)              == nonstd::cx::fabs(f_long__f64_p_inf) )              || ( isnan(std::fabs(f_long__f64_p_inf))              && isnan(nonstd::cx::fabs(f_long__f64_p_inf)) )              ));
        REQUIRE(( ( std::fabs(f_long__f64_n_inf)              == nonstd::cx::fabs(f_long__f64_n_inf) )              || ( isnan(std::fabs(f_long__f64_n_inf))              && isnan(nonstd::cx::fabs(f_long__f64_n_inf)) )              ));
        REQUIRE(( ( std::fabs(f_long__f_long_p_inf)           == nonstd::cx::fabs(f_long__f_long_p_inf) )           || ( isnan(std::fabs(f_long__f_long_p_inf))           && isnan(nonstd::cx::fabs(f_long__f_long_p_inf)) )           ));
        REQUIRE(( ( std::fabs(f_long__f_long_n_inf)           == nonstd::cx::fabs(f_long__f_long_n_inf) )           || ( isnan(std::fabs(f_long__f_long_n_inf))           && isnan(nonstd::cx::fabs(f_long__f_long_n_inf)) )           ));
        REQUIRE(( ( std::fabs(f_long__f32_less_p_smallest)    == nonstd::cx::fabs(f_long__f32_less_p_smallest) )    || ( isnan(std::fabs(f_long__f32_less_p_smallest))    && isnan(nonstd::cx::fabs(f_long__f32_less_p_smallest)) )    ));
        REQUIRE(( ( std::fabs(f_long__f32_less_n_smallest)    == nonstd::cx::fabs(f_long__f32_less_n_smallest) )    || ( isnan(std::fabs(f_long__f32_less_n_smallest))    && isnan(nonstd::cx::fabs(f_long__f32_less_n_smallest)) )    ));
        REQUIRE(( ( std::fabs(f_long__f32_more_p_largest)     == nonstd::cx::fabs(f_long__f32_more_p_largest) )     || ( isnan(std::fabs(f_long__f32_more_p_largest))     && isnan(nonstd::cx::fabs(f_long__f32_more_p_largest)) )     ));
        REQUIRE(( ( std::fabs(f_long__f32_more_n_largest)     == nonstd::cx::fabs(f_long__f32_more_n_largest) )     || ( isnan(std::fabs(f_long__f32_more_n_largest))     && isnan(nonstd::cx::fabs(f_long__f32_more_n_largest)) )     ));
        REQUIRE(( ( std::fabs(f_long__f64_less_p_smallest)    == nonstd::cx::fabs(f_long__f64_less_p_smallest) )    || ( isnan(std::fabs(f_long__f64_less_p_smallest))    && isnan(nonstd::cx::fabs(f_long__f64_less_p_smallest)) )    ));
        REQUIRE(( ( std::fabs(f_long__f64_less_n_smallest)    == nonstd::cx::fabs(f_long__f64_less_n_smallest) )    || ( isnan(std::fabs(f_long__f64_less_n_smallest))    && isnan(nonstd::cx::fabs(f_long__f64_less_n_smallest)) )    ));
        REQUIRE(( ( std::fabs(f_long__f64_more_p_largest)     == nonstd::cx::fabs(f_long__f64_more_p_largest) )     || ( isnan(std::fabs(f_long__f64_more_p_largest))     && isnan(nonstd::cx::fabs(f_long__f64_more_p_largest)) )     ));
        REQUIRE(( ( std::fabs(f_long__f64_more_n_largest)     == nonstd::cx::fabs(f_long__f64_more_n_largest) )     || ( isnan(std::fabs(f_long__f64_more_n_largest))     && isnan(nonstd::cx::fabs(f_long__f64_more_n_largest)) )     ));
        REQUIRE(( ( std::fabs(f_long__f_long_less_p_smallest) == nonstd::cx::fabs(f_long__f_long_less_p_smallest) ) || ( isnan(std::fabs(f_long__f_long_less_p_smallest)) && isnan(nonstd::cx::fabs(f_long__f_long_less_p_smallest)) ) ));
        REQUIRE(( ( std::fabs(f_long__f_long_less_n_smallest) == nonstd::cx::fabs(f_long__f_long_less_n_smallest) ) || ( isnan(std::fabs(f_long__f_long_less_n_smallest)) && isnan(nonstd::cx::fabs(f_long__f_long_less_n_smallest)) ) ));
        REQUIRE(( ( std::fabs(f_long__f_long_more_p_largest)  == nonstd::cx::fabs(f_long__f_long_more_p_largest) )  || ( isnan(std::fabs(f_long__f_long_more_p_largest))  && isnan(nonstd::cx::fabs(f_long__f_long_more_p_largest)) )  ));
        REQUIRE(( ( std::fabs(f_long__f_long_more_n_largest)  == nonstd::cx::fabs(f_long__f_long_more_n_largest) )  || ( isnan(std::fabs(f_long__f_long_more_n_largest))  && isnan(nonstd::cx::fabs(f_long__f_long_more_n_largest)) )  ));

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
        REQUIRE(( ( std::ceil(f32__positive)               == nonstd::cx::ceil(f32__positive) )               || ( isnan(std::ceil(f32__positive))               && isnan(nonstd::cx::ceil(f32__positive)) )               ));
        REQUIRE(( ( std::ceil(f32__zero)                   == nonstd::cx::ceil(f32__zero) )                   || ( isnan(std::ceil(f32__zero))                   && isnan(nonstd::cx::ceil(f32__zero)) )                   ));
        REQUIRE(( ( std::ceil(f32__negative)               == nonstd::cx::ceil(f32__negative) )               || ( isnan(std::ceil(f32__negative))               && isnan(nonstd::cx::ceil(f32__negative)) )               ));
        REQUIRE(( ( std::ceil(f32__f32_p_smallest)         == nonstd::cx::ceil(f32__f32_p_smallest) )         || ( isnan(std::ceil(f32__f32_p_smallest))         && isnan(nonstd::cx::ceil(f32__f32_p_smallest)) )         ));
        REQUIRE(( ( std::ceil(f32__f32_n_smallest)         == nonstd::cx::ceil(f32__f32_n_smallest) )         || ( isnan(std::ceil(f32__f32_n_smallest))         && isnan(nonstd::cx::ceil(f32__f32_n_smallest)) )         ));
        REQUIRE(( ( std::ceil(f32__f32_p_largest)          == nonstd::cx::ceil(f32__f32_p_largest) )          || ( isnan(std::ceil(f32__f32_p_largest))          && isnan(nonstd::cx::ceil(f32__f32_p_largest)) )          ));
        REQUIRE(( ( std::ceil(f32__f32_n_largest)          == nonstd::cx::ceil(f32__f32_n_largest) )          || ( isnan(std::ceil(f32__f32_n_largest))          && isnan(nonstd::cx::ceil(f32__f32_n_largest)) )          ));
        REQUIRE(( ( std::ceil(f32__f64_p_smallest)         == nonstd::cx::ceil(f32__f64_p_smallest) )         || ( isnan(std::ceil(f32__f64_p_smallest))         && isnan(nonstd::cx::ceil(f32__f64_p_smallest)) )         ));
        REQUIRE(( ( std::ceil(f32__f64_n_smallest)         == nonstd::cx::ceil(f32__f64_n_smallest) )         || ( isnan(std::ceil(f32__f64_n_smallest))         && isnan(nonstd::cx::ceil(f32__f64_n_smallest)) )         ));
    //  REQUIRE(( ( std::ceil(f32__f64_p_largest)          == nonstd::cx::ceil(f32__f64_p_largest) )          || ( isnan(std::ceil(f32__f64_p_largest))          && isnan(nonstd::cx::ceil(f32__f64_p_largest)) )          ));
    //  REQUIRE(( ( std::ceil(f32__f64_n_largest)          == nonstd::cx::ceil(f32__f64_n_largest) )          || ( isnan(std::ceil(f32__f64_n_largest))          && isnan(nonstd::cx::ceil(f32__f64_n_largest)) )          ));
        REQUIRE(( ( std::ceil(f32__f_long_p_smallest)      == nonstd::cx::ceil(f32__f_long_p_smallest) )      || ( isnan(std::ceil(f32__f_long_p_smallest))      && isnan(nonstd::cx::ceil(f32__f_long_p_smallest)) )      ));
        REQUIRE(( ( std::ceil(f32__f_long_n_smallest)      == nonstd::cx::ceil(f32__f_long_n_smallest) )      || ( isnan(std::ceil(f32__f_long_n_smallest))      && isnan(nonstd::cx::ceil(f32__f_long_n_smallest)) )      ));
    //  REQUIRE(( ( std::ceil(f32__f_long_p_largest)       == nonstd::cx::ceil(f32__f_long_p_largest) )       || ( isnan(std::ceil(f32__f_long_p_largest))       && isnan(nonstd::cx::ceil(f32__f_long_p_largest)) )       ));
    //  REQUIRE(( ( std::ceil(f32__f_long_n_largest)       == nonstd::cx::ceil(f32__f_long_n_largest) )       || ( isnan(std::ceil(f32__f_long_n_largest))       && isnan(nonstd::cx::ceil(f32__f_long_n_largest)) )       ));
        REQUIRE(( ( std::ceil(f32__f32_nan)                == nonstd::cx::ceil(f32__f32_nan) )                || ( isnan(std::ceil(f32__f32_nan))                && isnan(nonstd::cx::ceil(f32__f32_nan)) )                ));
        REQUIRE(( ( std::ceil(f32__f64_nan)                == nonstd::cx::ceil(f32__f64_nan) )                || ( isnan(std::ceil(f32__f64_nan))                && isnan(nonstd::cx::ceil(f32__f64_nan)) )                ));
        REQUIRE(( ( std::ceil(f32__f_long_nan)             == nonstd::cx::ceil(f32__f_long_nan) )             || ( isnan(std::ceil(f32__f_long_nan))             && isnan(nonstd::cx::ceil(f32__f_long_nan)) )             ));
        REQUIRE(( ( std::ceil(f32__f32_p_inf)              == nonstd::cx::ceil(f32__f32_p_inf) )              || ( isnan(std::ceil(f32__f32_p_inf))              && isnan(nonstd::cx::ceil(f32__f32_p_inf)) )              ));
        REQUIRE(( ( std::ceil(f32__f32_n_inf)              == nonstd::cx::ceil(f32__f32_n_inf) )              || ( isnan(std::ceil(f32__f32_n_inf))              && isnan(nonstd::cx::ceil(f32__f32_n_inf)) )              ));
        REQUIRE(( ( std::ceil(f32__f64_p_inf)              == nonstd::cx::ceil(f32__f64_p_inf) )              || ( isnan(std::ceil(f32__f64_p_inf))              && isnan(nonstd::cx::ceil(f32__f64_p_inf)) )              ));
        REQUIRE(( ( std::ceil(f32__f64_n_inf)              == nonstd::cx::ceil(f32__f64_n_inf) )              || ( isnan(std::ceil(f32__f64_n_inf))              && isnan(nonstd::cx::ceil(f32__f64_n_inf)) )              ));
        REQUIRE(( ( std::ceil(f32__f_long_p_inf)           == nonstd::cx::ceil(f32__f_long_p_inf) )           || ( isnan(std::ceil(f32__f_long_p_inf))           && isnan(nonstd::cx::ceil(f32__f_long_p_inf)) )           ));
        REQUIRE(( ( std::ceil(f32__f_long_n_inf)           == nonstd::cx::ceil(f32__f_long_n_inf) )           || ( isnan(std::ceil(f32__f_long_n_inf))           && isnan(nonstd::cx::ceil(f32__f_long_n_inf)) )           ));
        REQUIRE(( ( std::ceil(f32__f32_less_p_smallest)    == nonstd::cx::ceil(f32__f32_less_p_smallest) )    || ( isnan(std::ceil(f32__f32_less_p_smallest))    && isnan(nonstd::cx::ceil(f32__f32_less_p_smallest)) )    ));
        REQUIRE(( ( std::ceil(f32__f32_less_n_smallest)    == nonstd::cx::ceil(f32__f32_less_n_smallest) )    || ( isnan(std::ceil(f32__f32_less_n_smallest))    && isnan(nonstd::cx::ceil(f32__f32_less_n_smallest)) )    ));
        REQUIRE(( ( std::ceil(f32__f32_more_p_largest)     == nonstd::cx::ceil(f32__f32_more_p_largest) )     || ( isnan(std::ceil(f32__f32_more_p_largest))     && isnan(nonstd::cx::ceil(f32__f32_more_p_largest)) )     ));
        REQUIRE(( ( std::ceil(f32__f32_more_n_largest)     == nonstd::cx::ceil(f32__f32_more_n_largest) )     || ( isnan(std::ceil(f32__f32_more_n_largest))     && isnan(nonstd::cx::ceil(f32__f32_more_n_largest)) )     ));
        REQUIRE(( ( std::ceil(f32__f64_less_p_smallest)    == nonstd::cx::ceil(f32__f64_less_p_smallest) )    || ( isnan(std::ceil(f32__f64_less_p_smallest))    && isnan(nonstd::cx::ceil(f32__f64_less_p_smallest)) )    ));
        REQUIRE(( ( std::ceil(f32__f64_less_n_smallest)    == nonstd::cx::ceil(f32__f64_less_n_smallest) )    || ( isnan(std::ceil(f32__f64_less_n_smallest))    && isnan(nonstd::cx::ceil(f32__f64_less_n_smallest)) )    ));
        REQUIRE(( ( std::ceil(f32__f64_more_p_largest)     == nonstd::cx::ceil(f32__f64_more_p_largest) )     || ( isnan(std::ceil(f32__f64_more_p_largest))     && isnan(nonstd::cx::ceil(f32__f64_more_p_largest)) )     ));
        REQUIRE(( ( std::ceil(f32__f64_more_n_largest)     == nonstd::cx::ceil(f32__f64_more_n_largest) )     || ( isnan(std::ceil(f32__f64_more_n_largest))     && isnan(nonstd::cx::ceil(f32__f64_more_n_largest)) )     ));
        REQUIRE(( ( std::ceil(f32__f_long_less_p_smallest) == nonstd::cx::ceil(f32__f_long_less_p_smallest) ) || ( isnan(std::ceil(f32__f_long_less_p_smallest)) && isnan(nonstd::cx::ceil(f32__f_long_less_p_smallest)) ) ));
        REQUIRE(( ( std::ceil(f32__f_long_less_n_smallest) == nonstd::cx::ceil(f32__f_long_less_n_smallest) ) || ( isnan(std::ceil(f32__f_long_less_n_smallest)) && isnan(nonstd::cx::ceil(f32__f_long_less_n_smallest)) ) ));
        REQUIRE(( ( std::ceil(f32__f_long_more_p_largest)  == nonstd::cx::ceil(f32__f_long_more_p_largest) )  || ( isnan(std::ceil(f32__f_long_more_p_largest))  && isnan(nonstd::cx::ceil(f32__f_long_more_p_largest)) )  ));
        REQUIRE(( ( std::ceil(f32__f_long_more_n_largest)  == nonstd::cx::ceil(f32__f_long_more_n_largest) )  || ( isnan(std::ceil(f32__f_long_more_n_largest))  && isnan(nonstd::cx::ceil(f32__f_long_more_n_largest)) )  ));

        REQUIRE(( ( std::ceil(f64__positive)               == nonstd::cx::ceil(f64__positive) )               || ( isnan(std::ceil(f64__positive))               && isnan(nonstd::cx::ceil(f64__positive)) )               ));
        REQUIRE(( ( std::ceil(f64__zero)                   == nonstd::cx::ceil(f64__zero) )                   || ( isnan(std::ceil(f64__zero))                   && isnan(nonstd::cx::ceil(f64__zero)) )                   ));
        REQUIRE(( ( std::ceil(f64__negative)               == nonstd::cx::ceil(f64__negative) )               || ( isnan(std::ceil(f64__negative))               && isnan(nonstd::cx::ceil(f64__negative)) )               ));
        REQUIRE(( ( std::ceil(f64__f32_p_smallest)         == nonstd::cx::ceil(f64__f32_p_smallest) )         || ( isnan(std::ceil(f64__f32_p_smallest))         && isnan(nonstd::cx::ceil(f64__f32_p_smallest)) )         ));
        REQUIRE(( ( std::ceil(f64__f32_n_smallest)         == nonstd::cx::ceil(f64__f32_n_smallest) )         || ( isnan(std::ceil(f64__f32_n_smallest))         && isnan(nonstd::cx::ceil(f64__f32_n_smallest)) )         ));
        REQUIRE(( ( std::ceil(f64__f32_p_largest)          == nonstd::cx::ceil(f64__f32_p_largest) )          || ( isnan(std::ceil(f64__f32_p_largest))          && isnan(nonstd::cx::ceil(f64__f32_p_largest)) )          ));
        REQUIRE(( ( std::ceil(f64__f32_n_largest)          == nonstd::cx::ceil(f64__f32_n_largest) )          || ( isnan(std::ceil(f64__f32_n_largest))          && isnan(nonstd::cx::ceil(f64__f32_n_largest)) )          ));
        REQUIRE(( ( std::ceil(f64__f64_p_smallest)         == nonstd::cx::ceil(f64__f64_p_smallest) )         || ( isnan(std::ceil(f64__f64_p_smallest))         && isnan(nonstd::cx::ceil(f64__f64_p_smallest)) )         ));
        REQUIRE(( ( std::ceil(f64__f64_n_smallest)         == nonstd::cx::ceil(f64__f64_n_smallest) )         || ( isnan(std::ceil(f64__f64_n_smallest))         && isnan(nonstd::cx::ceil(f64__f64_n_smallest)) )         ));
        REQUIRE(( ( std::ceil(f64__f64_p_largest)          == nonstd::cx::ceil(f64__f64_p_largest) )          || ( isnan(std::ceil(f64__f64_p_largest))          && isnan(nonstd::cx::ceil(f64__f64_p_largest)) )          ));
        REQUIRE(( ( std::ceil(f64__f64_n_largest)          == nonstd::cx::ceil(f64__f64_n_largest) )          || ( isnan(std::ceil(f64__f64_n_largest))          && isnan(nonstd::cx::ceil(f64__f64_n_largest)) )          ));
        REQUIRE(( ( std::ceil(f64__f_long_p_smallest)      == nonstd::cx::ceil(f64__f_long_p_smallest) )      || ( isnan(std::ceil(f64__f_long_p_smallest))      && isnan(nonstd::cx::ceil(f64__f_long_p_smallest)) )      ));
        REQUIRE(( ( std::ceil(f64__f_long_n_smallest)      == nonstd::cx::ceil(f64__f_long_n_smallest) )      || ( isnan(std::ceil(f64__f_long_n_smallest))      && isnan(nonstd::cx::ceil(f64__f_long_n_smallest)) )      ));
    //  REQUIRE(( ( std::ceil(f64__f_long_p_largest)       == nonstd::cx::ceil(f64__f_long_p_largest) )       || ( isnan(std::ceil(f64__f_long_p_largest))       && isnan(nonstd::cx::ceil(f64__f_long_p_largest)) )       ));
    //  REQUIRE(( ( std::ceil(f64__f_long_n_largest)       == nonstd::cx::ceil(f64__f_long_n_largest) )       || ( isnan(std::ceil(f64__f_long_n_largest))       && isnan(nonstd::cx::ceil(f64__f_long_n_largest)) )       ));
        REQUIRE(( ( std::ceil(f64__f32_nan)                == nonstd::cx::ceil(f64__f32_nan) )                || ( isnan(std::ceil(f64__f32_nan))                && isnan(nonstd::cx::ceil(f64__f32_nan)) )                ));
        REQUIRE(( ( std::ceil(f64__f64_nan)                == nonstd::cx::ceil(f64__f64_nan) )                || ( isnan(std::ceil(f64__f64_nan))                && isnan(nonstd::cx::ceil(f64__f64_nan)) )                ));
        REQUIRE(( ( std::ceil(f64__f_long_nan)             == nonstd::cx::ceil(f64__f_long_nan) )             || ( isnan(std::ceil(f64__f_long_nan))             && isnan(nonstd::cx::ceil(f64__f_long_nan)) )             ));
        REQUIRE(( ( std::ceil(f64__f32_p_inf)              == nonstd::cx::ceil(f64__f32_p_inf) )              || ( isnan(std::ceil(f64__f32_p_inf))              && isnan(nonstd::cx::ceil(f64__f32_p_inf)) )              ));
        REQUIRE(( ( std::ceil(f64__f32_n_inf)              == nonstd::cx::ceil(f64__f32_n_inf) )              || ( isnan(std::ceil(f64__f32_n_inf))              && isnan(nonstd::cx::ceil(f64__f32_n_inf)) )              ));
        REQUIRE(( ( std::ceil(f64__f64_p_inf)              == nonstd::cx::ceil(f64__f64_p_inf) )              || ( isnan(std::ceil(f64__f64_p_inf))              && isnan(nonstd::cx::ceil(f64__f64_p_inf)) )              ));
        REQUIRE(( ( std::ceil(f64__f64_n_inf)              == nonstd::cx::ceil(f64__f64_n_inf) )              || ( isnan(std::ceil(f64__f64_n_inf))              && isnan(nonstd::cx::ceil(f64__f64_n_inf)) )              ));
        REQUIRE(( ( std::ceil(f64__f_long_p_inf)           == nonstd::cx::ceil(f64__f_long_p_inf) )           || ( isnan(std::ceil(f64__f_long_p_inf))           && isnan(nonstd::cx::ceil(f64__f_long_p_inf)) )           ));
        REQUIRE(( ( std::ceil(f64__f_long_n_inf)           == nonstd::cx::ceil(f64__f_long_n_inf) )           || ( isnan(std::ceil(f64__f_long_n_inf))           && isnan(nonstd::cx::ceil(f64__f_long_n_inf)) )           ));
        REQUIRE(( ( std::ceil(f64__f32_less_p_smallest)    == nonstd::cx::ceil(f64__f32_less_p_smallest) )    || ( isnan(std::ceil(f64__f32_less_p_smallest))    && isnan(nonstd::cx::ceil(f64__f32_less_p_smallest)) )    ));
        REQUIRE(( ( std::ceil(f64__f32_less_n_smallest)    == nonstd::cx::ceil(f64__f32_less_n_smallest) )    || ( isnan(std::ceil(f64__f32_less_n_smallest))    && isnan(nonstd::cx::ceil(f64__f32_less_n_smallest)) )    ));
        REQUIRE(( ( std::ceil(f64__f32_more_p_largest)     == nonstd::cx::ceil(f64__f32_more_p_largest) )     || ( isnan(std::ceil(f64__f32_more_p_largest))     && isnan(nonstd::cx::ceil(f64__f32_more_p_largest)) )     ));
        REQUIRE(( ( std::ceil(f64__f32_more_n_largest)     == nonstd::cx::ceil(f64__f32_more_n_largest) )     || ( isnan(std::ceil(f64__f32_more_n_largest))     && isnan(nonstd::cx::ceil(f64__f32_more_n_largest)) )     ));
        REQUIRE(( ( std::ceil(f64__f64_less_p_smallest)    == nonstd::cx::ceil(f64__f64_less_p_smallest) )    || ( isnan(std::ceil(f64__f64_less_p_smallest))    && isnan(nonstd::cx::ceil(f64__f64_less_p_smallest)) )    ));
        REQUIRE(( ( std::ceil(f64__f64_less_n_smallest)    == nonstd::cx::ceil(f64__f64_less_n_smallest) )    || ( isnan(std::ceil(f64__f64_less_n_smallest))    && isnan(nonstd::cx::ceil(f64__f64_less_n_smallest)) )    ));
        REQUIRE(( ( std::ceil(f64__f64_more_p_largest)     == nonstd::cx::ceil(f64__f64_more_p_largest) )     || ( isnan(std::ceil(f64__f64_more_p_largest))     && isnan(nonstd::cx::ceil(f64__f64_more_p_largest)) )     ));
        REQUIRE(( ( std::ceil(f64__f64_more_n_largest)     == nonstd::cx::ceil(f64__f64_more_n_largest) )     || ( isnan(std::ceil(f64__f64_more_n_largest))     && isnan(nonstd::cx::ceil(f64__f64_more_n_largest)) )     ));
        REQUIRE(( ( std::ceil(f64__f_long_less_p_smallest) == nonstd::cx::ceil(f64__f_long_less_p_smallest) ) || ( isnan(std::ceil(f64__f_long_less_p_smallest)) && isnan(nonstd::cx::ceil(f64__f_long_less_p_smallest)) ) ));
        REQUIRE(( ( std::ceil(f64__f_long_less_n_smallest) == nonstd::cx::ceil(f64__f_long_less_n_smallest) ) || ( isnan(std::ceil(f64__f_long_less_n_smallest)) && isnan(nonstd::cx::ceil(f64__f_long_less_n_smallest)) ) ));
        REQUIRE(( ( std::ceil(f64__f_long_more_p_largest)  == nonstd::cx::ceil(f64__f_long_more_p_largest) )  || ( isnan(std::ceil(f64__f_long_more_p_largest))  && isnan(nonstd::cx::ceil(f64__f_long_more_p_largest)) )  ));
        REQUIRE(( ( std::ceil(f64__f_long_more_n_largest)  == nonstd::cx::ceil(f64__f_long_more_n_largest) )  || ( isnan(std::ceil(f64__f_long_more_n_largest))  && isnan(nonstd::cx::ceil(f64__f_long_more_n_largest)) )  ));

        REQUIRE(( ( std::ceil(f_long__positive)               == nonstd::cx::ceil(f_long__positive) )               || ( isnan(std::ceil(f_long__positive))               && isnan(nonstd::cx::ceil(f_long__positive)) )               ));
        REQUIRE(( ( std::ceil(f_long__zero)                   == nonstd::cx::ceil(f_long__zero) )                   || ( isnan(std::ceil(f_long__zero))                   && isnan(nonstd::cx::ceil(f_long__zero)) )                   ));
        REQUIRE(( ( std::ceil(f_long__negative)               == nonstd::cx::ceil(f_long__negative) )               || ( isnan(std::ceil(f_long__negative))               && isnan(nonstd::cx::ceil(f_long__negative)) )               ));
        REQUIRE(( ( std::ceil(f_long__f32_p_smallest)         == nonstd::cx::ceil(f_long__f32_p_smallest) )         || ( isnan(std::ceil(f_long__f32_p_smallest))         && isnan(nonstd::cx::ceil(f_long__f32_p_smallest)) )         ));
        REQUIRE(( ( std::ceil(f_long__f32_n_smallest)         == nonstd::cx::ceil(f_long__f32_n_smallest) )         || ( isnan(std::ceil(f_long__f32_n_smallest))         && isnan(nonstd::cx::ceil(f_long__f32_n_smallest)) )         ));
        REQUIRE(( ( std::ceil(f_long__f32_p_largest)          == nonstd::cx::ceil(f_long__f32_p_largest) )          || ( isnan(std::ceil(f_long__f32_p_largest))          && isnan(nonstd::cx::ceil(f_long__f32_p_largest)) )          ));
        REQUIRE(( ( std::ceil(f_long__f32_n_largest)          == nonstd::cx::ceil(f_long__f32_n_largest) )          || ( isnan(std::ceil(f_long__f32_n_largest))          && isnan(nonstd::cx::ceil(f_long__f32_n_largest)) )          ));
        REQUIRE(( ( std::ceil(f_long__f64_p_smallest)         == nonstd::cx::ceil(f_long__f64_p_smallest) )         || ( isnan(std::ceil(f_long__f64_p_smallest))         && isnan(nonstd::cx::ceil(f_long__f64_p_smallest)) )         ));
        REQUIRE(( ( std::ceil(f_long__f64_n_smallest)         == nonstd::cx::ceil(f_long__f64_n_smallest) )         || ( isnan(std::ceil(f_long__f64_n_smallest))         && isnan(nonstd::cx::ceil(f_long__f64_n_smallest)) )         ));
        REQUIRE(( ( std::ceil(f_long__f64_p_largest)          == nonstd::cx::ceil(f_long__f64_p_largest) )          || ( isnan(std::ceil(f_long__f64_p_largest))          && isnan(nonstd::cx::ceil(f_long__f64_p_largest)) )          ));
        REQUIRE(( ( std::ceil(f_long__f64_n_largest)          == nonstd::cx::ceil(f_long__f64_n_largest) )          || ( isnan(std::ceil(f_long__f64_n_largest))          && isnan(nonstd::cx::ceil(f_long__f64_n_largest)) )          ));
        REQUIRE(( ( std::ceil(f_long__f_long_p_smallest)      == nonstd::cx::ceil(f_long__f_long_p_smallest) )      || ( isnan(std::ceil(f_long__f_long_p_smallest))      && isnan(nonstd::cx::ceil(f_long__f_long_p_smallest)) )      ));
        REQUIRE(( ( std::ceil(f_long__f_long_n_smallest)      == nonstd::cx::ceil(f_long__f_long_n_smallest) )      || ( isnan(std::ceil(f_long__f_long_n_smallest))      && isnan(nonstd::cx::ceil(f_long__f_long_n_smallest)) )      ));
        REQUIRE(( ( std::ceil(f_long__f_long_p_largest)       == nonstd::cx::ceil(f_long__f_long_p_largest) )       || ( isnan(std::ceil(f_long__f_long_p_largest))       && isnan(nonstd::cx::ceil(f_long__f_long_p_largest)) )       ));
        REQUIRE(( ( std::ceil(f_long__f_long_n_largest)       == nonstd::cx::ceil(f_long__f_long_n_largest) )       || ( isnan(std::ceil(f_long__f_long_n_largest))       && isnan(nonstd::cx::ceil(f_long__f_long_n_largest)) )       ));
        REQUIRE(( ( std::ceil(f_long__f32_nan)                == nonstd::cx::ceil(f_long__f32_nan) )                || ( isnan(std::ceil(f_long__f32_nan))                && isnan(nonstd::cx::ceil(f_long__f32_nan)) )                ));
        REQUIRE(( ( std::ceil(f_long__f64_nan)                == nonstd::cx::ceil(f_long__f64_nan) )                || ( isnan(std::ceil(f_long__f64_nan))                && isnan(nonstd::cx::ceil(f_long__f64_nan)) )                ));
        REQUIRE(( ( std::ceil(f_long__f_long_nan)             == nonstd::cx::ceil(f_long__f_long_nan) )             || ( isnan(std::ceil(f_long__f_long_nan))             && isnan(nonstd::cx::ceil(f_long__f_long_nan)) )             ));
        REQUIRE(( ( std::ceil(f_long__f32_p_inf)              == nonstd::cx::ceil(f_long__f32_p_inf) )              || ( isnan(std::ceil(f_long__f32_p_inf))              && isnan(nonstd::cx::ceil(f_long__f32_p_inf)) )              ));
        REQUIRE(( ( std::ceil(f_long__f32_n_inf)              == nonstd::cx::ceil(f_long__f32_n_inf) )              || ( isnan(std::ceil(f_long__f32_n_inf))              && isnan(nonstd::cx::ceil(f_long__f32_n_inf)) )              ));
        REQUIRE(( ( std::ceil(f_long__f64_p_inf)              == nonstd::cx::ceil(f_long__f64_p_inf) )              || ( isnan(std::ceil(f_long__f64_p_inf))              && isnan(nonstd::cx::ceil(f_long__f64_p_inf)) )              ));
        REQUIRE(( ( std::ceil(f_long__f64_n_inf)              == nonstd::cx::ceil(f_long__f64_n_inf) )              || ( isnan(std::ceil(f_long__f64_n_inf))              && isnan(nonstd::cx::ceil(f_long__f64_n_inf)) )              ));
        REQUIRE(( ( std::ceil(f_long__f_long_p_inf)           == nonstd::cx::ceil(f_long__f_long_p_inf) )           || ( isnan(std::ceil(f_long__f_long_p_inf))           && isnan(nonstd::cx::ceil(f_long__f_long_p_inf)) )           ));
        REQUIRE(( ( std::ceil(f_long__f_long_n_inf)           == nonstd::cx::ceil(f_long__f_long_n_inf) )           || ( isnan(std::ceil(f_long__f_long_n_inf))           && isnan(nonstd::cx::ceil(f_long__f_long_n_inf)) )           ));
        REQUIRE(( ( std::ceil(f_long__f32_less_p_smallest)    == nonstd::cx::ceil(f_long__f32_less_p_smallest) )    || ( isnan(std::ceil(f_long__f32_less_p_smallest))    && isnan(nonstd::cx::ceil(f_long__f32_less_p_smallest)) )    ));
        REQUIRE(( ( std::ceil(f_long__f32_less_n_smallest)    == nonstd::cx::ceil(f_long__f32_less_n_smallest) )    || ( isnan(std::ceil(f_long__f32_less_n_smallest))    && isnan(nonstd::cx::ceil(f_long__f32_less_n_smallest)) )    ));
        REQUIRE(( ( std::ceil(f_long__f32_more_p_largest)     == nonstd::cx::ceil(f_long__f32_more_p_largest) )     || ( isnan(std::ceil(f_long__f32_more_p_largest))     && isnan(nonstd::cx::ceil(f_long__f32_more_p_largest)) )     ));
        REQUIRE(( ( std::ceil(f_long__f32_more_n_largest)     == nonstd::cx::ceil(f_long__f32_more_n_largest) )     || ( isnan(std::ceil(f_long__f32_more_n_largest))     && isnan(nonstd::cx::ceil(f_long__f32_more_n_largest)) )     ));
        REQUIRE(( ( std::ceil(f_long__f64_less_p_smallest)    == nonstd::cx::ceil(f_long__f64_less_p_smallest) )    || ( isnan(std::ceil(f_long__f64_less_p_smallest))    && isnan(nonstd::cx::ceil(f_long__f64_less_p_smallest)) )    ));
        REQUIRE(( ( std::ceil(f_long__f64_less_n_smallest)    == nonstd::cx::ceil(f_long__f64_less_n_smallest) )    || ( isnan(std::ceil(f_long__f64_less_n_smallest))    && isnan(nonstd::cx::ceil(f_long__f64_less_n_smallest)) )    ));
        REQUIRE(( ( std::ceil(f_long__f64_more_p_largest)     == nonstd::cx::ceil(f_long__f64_more_p_largest) )     || ( isnan(std::ceil(f_long__f64_more_p_largest))     && isnan(nonstd::cx::ceil(f_long__f64_more_p_largest)) )     ));
        REQUIRE(( ( std::ceil(f_long__f64_more_n_largest)     == nonstd::cx::ceil(f_long__f64_more_n_largest) )     || ( isnan(std::ceil(f_long__f64_more_n_largest))     && isnan(nonstd::cx::ceil(f_long__f64_more_n_largest)) )     ));
        REQUIRE(( ( std::ceil(f_long__f_long_less_p_smallest) == nonstd::cx::ceil(f_long__f_long_less_p_smallest) ) || ( isnan(std::ceil(f_long__f_long_less_p_smallest)) && isnan(nonstd::cx::ceil(f_long__f_long_less_p_smallest)) ) ));
        REQUIRE(( ( std::ceil(f_long__f_long_less_n_smallest) == nonstd::cx::ceil(f_long__f_long_less_n_smallest) ) || ( isnan(std::ceil(f_long__f_long_less_n_smallest)) && isnan(nonstd::cx::ceil(f_long__f_long_less_n_smallest)) ) ));
        REQUIRE(( ( std::ceil(f_long__f_long_more_p_largest)  == nonstd::cx::ceil(f_long__f_long_more_p_largest) )  || ( isnan(std::ceil(f_long__f_long_more_p_largest))  && isnan(nonstd::cx::ceil(f_long__f_long_more_p_largest)) )  ));
        REQUIRE(( ( std::ceil(f_long__f_long_more_n_largest)  == nonstd::cx::ceil(f_long__f_long_more_n_largest) )  || ( isnan(std::ceil(f_long__f_long_more_n_largest))  && isnan(nonstd::cx::ceil(f_long__f_long_more_n_largest)) )  ));

        REQUIRE(( ( std::ceil(u64__positive) == nonstd::cx::ceil(u64__positive) ) || ( isnan(std::ceil(u64__positive)) && isnan(nonstd::cx::ceil(u64__positive)) ) ));
        REQUIRE(( ( std::ceil(u64__zero)     == nonstd::cx::ceil(u64__zero) )     || ( isnan(std::ceil(u64__zero))     && isnan(nonstd::cx::ceil(u64__zero)) )     ));
        REQUIRE(( ( std::ceil(u64__max)      == nonstd::cx::ceil(u64__max) )      || ( isnan(std::ceil(u64__max))      && isnan(nonstd::cx::ceil(u64__max)) )      ));
        REQUIRE(( ( std::ceil(i64__positive) == nonstd::cx::ceil(i64__positive) ) || ( isnan(std::ceil(i64__positive)) && isnan(nonstd::cx::ceil(i64__positive)) ) ));
        REQUIRE(( ( std::ceil(i64__zero)     == nonstd::cx::ceil(i64__zero) )     || ( isnan(std::ceil(i64__zero))     && isnan(nonstd::cx::ceil(i64__zero)) )     ));
        REQUIRE(( ( std::ceil(i64__negative) == nonstd::cx::ceil(i64__negative) ) || ( isnan(std::ceil(i64__negative)) && isnan(nonstd::cx::ceil(i64__negative)) ) ));
        REQUIRE(( ( std::ceil(i64__min)      == nonstd::cx::ceil(i64__min) )      || ( isnan(std::ceil(i64__min))      && isnan(nonstd::cx::ceil(i64__min)) )      ));
        REQUIRE(( ( std::ceil(i64__max)      == nonstd::cx::ceil(i64__max) )      || ( isnan(std::ceil(i64__max))      && isnan(nonstd::cx::ceil(i64__max)) )      ));

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

        // For this function, it's worth doing some additional testing in ranges
        // we might see in the real world. Or just random ranges. Pulled from a
        // random number generator. Either one.
        std::random_device rd;
        std::mt19937 re(rd());

        std::uniform_real_distribution<float> rnd_float_range(
            -std::numeric_limits<float>::max(),
             std::numeric_limits<float>::max());
        std::uniform_real_distribution<double> rnd_double_range(
            -std::numeric_limits<double>::max(),
             std::numeric_limits<double>::max());
        std::uniform_real_distribution<long double> rnd_long_double_range(
            -std::numeric_limits<long double>::max(),
             std::numeric_limits<long double>::max());

        constexpr int iterations = 10000;
        for (int i = 0; i < iterations; i++) {
            float x = rnd_float_range(re);
            REQUIRE(( ( std::ceil(x)  == nonstd::cx::ceil(x) ) || ( isnan(std::ceil(x)) && isnan(nonstd::cx::ceil(x)) ) ));
        }
        for (int i = 0; i < iterations; i++) {
            double x = rnd_double_range(re);
            REQUIRE(( ( std::ceil(x)  == nonstd::cx::ceil(x) ) || ( isnan(std::ceil(x)) && isnan(nonstd::cx::ceil(x)) ) ));
        }
        for (int i = 0; i < iterations; i++) {
            long double x = rnd_long_double_range(re);
            REQUIRE(( ( std::ceil(x)  == nonstd::cx::ceil(x) ) || ( isnan(std::ceil(x)) && isnan(nonstd::cx::ceil(x)) ) ));
        }
    }

    SECTION("floor") {
        constexpr auto calls_to_floor_are_constexpr = nonstd::cx::floor(f32__positive);

        using std::isnan;
        REQUIRE(( ( std::floor(f32__positive)               == nonstd::cx::floor(f32__positive) )               || ( isnan(std::floor(f32__positive))               && isnan(nonstd::cx::floor(f32__positive)) )               ));
        REQUIRE(( ( std::floor(f32__zero)                   == nonstd::cx::floor(f32__zero) )                   || ( isnan(std::floor(f32__zero))                   && isnan(nonstd::cx::floor(f32__zero)) )                   ));
        REQUIRE(( ( std::floor(f32__negative)               == nonstd::cx::floor(f32__negative) )               || ( isnan(std::floor(f32__negative))               && isnan(nonstd::cx::floor(f32__negative)) )               ));
        REQUIRE(( ( std::floor(f32__f32_p_smallest)         == nonstd::cx::floor(f32__f32_p_smallest) )         || ( isnan(std::floor(f32__f32_p_smallest))         && isnan(nonstd::cx::floor(f32__f32_p_smallest)) )         ));
        REQUIRE(( ( std::floor(f32__f32_n_smallest)         == nonstd::cx::floor(f32__f32_n_smallest) )         || ( isnan(std::floor(f32__f32_n_smallest))         && isnan(nonstd::cx::floor(f32__f32_n_smallest)) )         ));
        REQUIRE(( ( std::floor(f32__f32_p_largest)          == nonstd::cx::floor(f32__f32_p_largest) )          || ( isnan(std::floor(f32__f32_p_largest))          && isnan(nonstd::cx::floor(f32__f32_p_largest)) )          ));
        REQUIRE(( ( std::floor(f32__f32_n_largest)          == nonstd::cx::floor(f32__f32_n_largest) )          || ( isnan(std::floor(f32__f32_n_largest))          && isnan(nonstd::cx::floor(f32__f32_n_largest)) )          ));
        REQUIRE(( ( std::floor(f32__f64_p_smallest)         == nonstd::cx::floor(f32__f64_p_smallest) )         || ( isnan(std::floor(f32__f64_p_smallest))         && isnan(nonstd::cx::floor(f32__f64_p_smallest)) )         ));
        REQUIRE(( ( std::floor(f32__f64_n_smallest)         == nonstd::cx::floor(f32__f64_n_smallest) )         || ( isnan(std::floor(f32__f64_n_smallest))         && isnan(nonstd::cx::floor(f32__f64_n_smallest)) )         ));
    //  REQUIRE(( ( std::floor(f32__f64_p_largest)          == nonstd::cx::floor(f32__f64_p_largest) )          || ( isnan(std::floor(f32__f64_p_largest))          && isnan(nonstd::cx::floor(f32__f64_p_largest)) )          ));
    //  REQUIRE(( ( std::floor(f32__f64_n_largest)          == nonstd::cx::floor(f32__f64_n_largest) )          || ( isnan(std::floor(f32__f64_n_largest))          && isnan(nonstd::cx::floor(f32__f64_n_largest)) )          ));
        REQUIRE(( ( std::floor(f32__f_long_p_smallest)      == nonstd::cx::floor(f32__f_long_p_smallest) )      || ( isnan(std::floor(f32__f_long_p_smallest))      && isnan(nonstd::cx::floor(f32__f_long_p_smallest)) )      ));
        REQUIRE(( ( std::floor(f32__f_long_n_smallest)      == nonstd::cx::floor(f32__f_long_n_smallest) )      || ( isnan(std::floor(f32__f_long_n_smallest))      && isnan(nonstd::cx::floor(f32__f_long_n_smallest)) )      ));
    //  REQUIRE(( ( std::floor(f32__f_long_p_largest)       == nonstd::cx::floor(f32__f_long_p_largest) )       || ( isnan(std::floor(f32__f_long_p_largest))       && isnan(nonstd::cx::floor(f32__f_long_p_largest)) )       ));
    //  REQUIRE(( ( std::floor(f32__f_long_n_largest)       == nonstd::cx::floor(f32__f_long_n_largest) )       || ( isnan(std::floor(f32__f_long_n_largest))       && isnan(nonstd::cx::floor(f32__f_long_n_largest)) )       ));
        REQUIRE(( ( std::floor(f32__f32_nan)                == nonstd::cx::floor(f32__f32_nan) )                || ( isnan(std::floor(f32__f32_nan))                && isnan(nonstd::cx::floor(f32__f32_nan)) )                ));
        REQUIRE(( ( std::floor(f32__f64_nan)                == nonstd::cx::floor(f32__f64_nan) )                || ( isnan(std::floor(f32__f64_nan))                && isnan(nonstd::cx::floor(f32__f64_nan)) )                ));
        REQUIRE(( ( std::floor(f32__f_long_nan)             == nonstd::cx::floor(f32__f_long_nan) )             || ( isnan(std::floor(f32__f_long_nan))             && isnan(nonstd::cx::floor(f32__f_long_nan)) )             ));
        REQUIRE(( ( std::floor(f32__f32_p_inf)              == nonstd::cx::floor(f32__f32_p_inf) )              || ( isnan(std::floor(f32__f32_p_inf))              && isnan(nonstd::cx::floor(f32__f32_p_inf)) )              ));
        REQUIRE(( ( std::floor(f32__f32_n_inf)              == nonstd::cx::floor(f32__f32_n_inf) )              || ( isnan(std::floor(f32__f32_n_inf))              && isnan(nonstd::cx::floor(f32__f32_n_inf)) )              ));
        REQUIRE(( ( std::floor(f32__f64_p_inf)              == nonstd::cx::floor(f32__f64_p_inf) )              || ( isnan(std::floor(f32__f64_p_inf))              && isnan(nonstd::cx::floor(f32__f64_p_inf)) )              ));
        REQUIRE(( ( std::floor(f32__f64_n_inf)              == nonstd::cx::floor(f32__f64_n_inf) )              || ( isnan(std::floor(f32__f64_n_inf))              && isnan(nonstd::cx::floor(f32__f64_n_inf)) )              ));
        REQUIRE(( ( std::floor(f32__f_long_p_inf)           == nonstd::cx::floor(f32__f_long_p_inf) )           || ( isnan(std::floor(f32__f_long_p_inf))           && isnan(nonstd::cx::floor(f32__f_long_p_inf)) )           ));
        REQUIRE(( ( std::floor(f32__f_long_n_inf)           == nonstd::cx::floor(f32__f_long_n_inf) )           || ( isnan(std::floor(f32__f_long_n_inf))           && isnan(nonstd::cx::floor(f32__f_long_n_inf)) )           ));
        REQUIRE(( ( std::floor(f32__f32_less_p_smallest)    == nonstd::cx::floor(f32__f32_less_p_smallest) )    || ( isnan(std::floor(f32__f32_less_p_smallest))    && isnan(nonstd::cx::floor(f32__f32_less_p_smallest)) )    ));
        REQUIRE(( ( std::floor(f32__f32_less_n_smallest)    == nonstd::cx::floor(f32__f32_less_n_smallest) )    || ( isnan(std::floor(f32__f32_less_n_smallest))    && isnan(nonstd::cx::floor(f32__f32_less_n_smallest)) )    ));
        REQUIRE(( ( std::floor(f32__f32_more_p_largest)     == nonstd::cx::floor(f32__f32_more_p_largest) )     || ( isnan(std::floor(f32__f32_more_p_largest))     && isnan(nonstd::cx::floor(f32__f32_more_p_largest)) )     ));
        REQUIRE(( ( std::floor(f32__f32_more_n_largest)     == nonstd::cx::floor(f32__f32_more_n_largest) )     || ( isnan(std::floor(f32__f32_more_n_largest))     && isnan(nonstd::cx::floor(f32__f32_more_n_largest)) )     ));
        REQUIRE(( ( std::floor(f32__f64_less_p_smallest)    == nonstd::cx::floor(f32__f64_less_p_smallest) )    || ( isnan(std::floor(f32__f64_less_p_smallest))    && isnan(nonstd::cx::floor(f32__f64_less_p_smallest)) )    ));
        REQUIRE(( ( std::floor(f32__f64_less_n_smallest)    == nonstd::cx::floor(f32__f64_less_n_smallest) )    || ( isnan(std::floor(f32__f64_less_n_smallest))    && isnan(nonstd::cx::floor(f32__f64_less_n_smallest)) )    ));
        REQUIRE(( ( std::floor(f32__f64_more_p_largest)     == nonstd::cx::floor(f32__f64_more_p_largest) )     || ( isnan(std::floor(f32__f64_more_p_largest))     && isnan(nonstd::cx::floor(f32__f64_more_p_largest)) )     ));
        REQUIRE(( ( std::floor(f32__f64_more_n_largest)     == nonstd::cx::floor(f32__f64_more_n_largest) )     || ( isnan(std::floor(f32__f64_more_n_largest))     && isnan(nonstd::cx::floor(f32__f64_more_n_largest)) )     ));
        REQUIRE(( ( std::floor(f32__f_long_less_p_smallest) == nonstd::cx::floor(f32__f_long_less_p_smallest) ) || ( isnan(std::floor(f32__f_long_less_p_smallest)) && isnan(nonstd::cx::floor(f32__f_long_less_p_smallest)) ) ));
        REQUIRE(( ( std::floor(f32__f_long_less_n_smallest) == nonstd::cx::floor(f32__f_long_less_n_smallest) ) || ( isnan(std::floor(f32__f_long_less_n_smallest)) && isnan(nonstd::cx::floor(f32__f_long_less_n_smallest)) ) ));
        REQUIRE(( ( std::floor(f32__f_long_more_p_largest)  == nonstd::cx::floor(f32__f_long_more_p_largest) )  || ( isnan(std::floor(f32__f_long_more_p_largest))  && isnan(nonstd::cx::floor(f32__f_long_more_p_largest)) )  ));
        REQUIRE(( ( std::floor(f32__f_long_more_n_largest)  == nonstd::cx::floor(f32__f_long_more_n_largest) )  || ( isnan(std::floor(f32__f_long_more_n_largest))  && isnan(nonstd::cx::floor(f32__f_long_more_n_largest)) )  ));

        REQUIRE(( ( std::floor(f64__positive)               == nonstd::cx::floor(f64__positive) )               || ( isnan(std::floor(f64__positive))               && isnan(nonstd::cx::floor(f64__positive)) )               ));
        REQUIRE(( ( std::floor(f64__zero)                   == nonstd::cx::floor(f64__zero) )                   || ( isnan(std::floor(f64__zero))                   && isnan(nonstd::cx::floor(f64__zero)) )                   ));
        REQUIRE(( ( std::floor(f64__negative)               == nonstd::cx::floor(f64__negative) )               || ( isnan(std::floor(f64__negative))               && isnan(nonstd::cx::floor(f64__negative)) )               ));
        REQUIRE(( ( std::floor(f64__f32_p_smallest)         == nonstd::cx::floor(f64__f32_p_smallest) )         || ( isnan(std::floor(f64__f32_p_smallest))         && isnan(nonstd::cx::floor(f64__f32_p_smallest)) )         ));
        REQUIRE(( ( std::floor(f64__f32_n_smallest)         == nonstd::cx::floor(f64__f32_n_smallest) )         || ( isnan(std::floor(f64__f32_n_smallest))         && isnan(nonstd::cx::floor(f64__f32_n_smallest)) )         ));
        REQUIRE(( ( std::floor(f64__f32_p_largest)          == nonstd::cx::floor(f64__f32_p_largest) )          || ( isnan(std::floor(f64__f32_p_largest))          && isnan(nonstd::cx::floor(f64__f32_p_largest)) )          ));
        REQUIRE(( ( std::floor(f64__f32_n_largest)          == nonstd::cx::floor(f64__f32_n_largest) )          || ( isnan(std::floor(f64__f32_n_largest))          && isnan(nonstd::cx::floor(f64__f32_n_largest)) )          ));
        REQUIRE(( ( std::floor(f64__f64_p_smallest)         == nonstd::cx::floor(f64__f64_p_smallest) )         || ( isnan(std::floor(f64__f64_p_smallest))         && isnan(nonstd::cx::floor(f64__f64_p_smallest)) )         ));
        REQUIRE(( ( std::floor(f64__f64_n_smallest)         == nonstd::cx::floor(f64__f64_n_smallest) )         || ( isnan(std::floor(f64__f64_n_smallest))         && isnan(nonstd::cx::floor(f64__f64_n_smallest)) )         ));
        REQUIRE(( ( std::floor(f64__f64_p_largest)          == nonstd::cx::floor(f64__f64_p_largest) )          || ( isnan(std::floor(f64__f64_p_largest))          && isnan(nonstd::cx::floor(f64__f64_p_largest)) )          ));
        REQUIRE(( ( std::floor(f64__f64_n_largest)          == nonstd::cx::floor(f64__f64_n_largest) )          || ( isnan(std::floor(f64__f64_n_largest))          && isnan(nonstd::cx::floor(f64__f64_n_largest)) )          ));
        REQUIRE(( ( std::floor(f64__f_long_p_smallest)      == nonstd::cx::floor(f64__f_long_p_smallest) )      || ( isnan(std::floor(f64__f_long_p_smallest))      && isnan(nonstd::cx::floor(f64__f_long_p_smallest)) )      ));
        REQUIRE(( ( std::floor(f64__f_long_n_smallest)      == nonstd::cx::floor(f64__f_long_n_smallest) )      || ( isnan(std::floor(f64__f_long_n_smallest))      && isnan(nonstd::cx::floor(f64__f_long_n_smallest)) )      ));
    //  REQUIRE(( ( std::floor(f64__f_long_p_largest)       == nonstd::cx::floor(f64__f_long_p_largest) )       || ( isnan(std::floor(f64__f_long_p_largest))       && isnan(nonstd::cx::floor(f64__f_long_p_largest)) )       ));
    //  REQUIRE(( ( std::floor(f64__f_long_n_largest)       == nonstd::cx::floor(f64__f_long_n_largest) )       || ( isnan(std::floor(f64__f_long_n_largest))       && isnan(nonstd::cx::floor(f64__f_long_n_largest)) )       ));
        REQUIRE(( ( std::floor(f64__f32_nan)                == nonstd::cx::floor(f64__f32_nan) )                || ( isnan(std::floor(f64__f32_nan))                && isnan(nonstd::cx::floor(f64__f32_nan)) )                ));
        REQUIRE(( ( std::floor(f64__f64_nan)                == nonstd::cx::floor(f64__f64_nan) )                || ( isnan(std::floor(f64__f64_nan))                && isnan(nonstd::cx::floor(f64__f64_nan)) )                ));
        REQUIRE(( ( std::floor(f64__f_long_nan)             == nonstd::cx::floor(f64__f_long_nan) )             || ( isnan(std::floor(f64__f_long_nan))             && isnan(nonstd::cx::floor(f64__f_long_nan)) )             ));
        REQUIRE(( ( std::floor(f64__f32_p_inf)              == nonstd::cx::floor(f64__f32_p_inf) )              || ( isnan(std::floor(f64__f32_p_inf))              && isnan(nonstd::cx::floor(f64__f32_p_inf)) )              ));
        REQUIRE(( ( std::floor(f64__f32_n_inf)              == nonstd::cx::floor(f64__f32_n_inf) )              || ( isnan(std::floor(f64__f32_n_inf))              && isnan(nonstd::cx::floor(f64__f32_n_inf)) )              ));
        REQUIRE(( ( std::floor(f64__f64_p_inf)              == nonstd::cx::floor(f64__f64_p_inf) )              || ( isnan(std::floor(f64__f64_p_inf))              && isnan(nonstd::cx::floor(f64__f64_p_inf)) )              ));
        REQUIRE(( ( std::floor(f64__f64_n_inf)              == nonstd::cx::floor(f64__f64_n_inf) )              || ( isnan(std::floor(f64__f64_n_inf))              && isnan(nonstd::cx::floor(f64__f64_n_inf)) )              ));
        REQUIRE(( ( std::floor(f64__f_long_p_inf)           == nonstd::cx::floor(f64__f_long_p_inf) )           || ( isnan(std::floor(f64__f_long_p_inf))           && isnan(nonstd::cx::floor(f64__f_long_p_inf)) )           ));
        REQUIRE(( ( std::floor(f64__f_long_n_inf)           == nonstd::cx::floor(f64__f_long_n_inf) )           || ( isnan(std::floor(f64__f_long_n_inf))           && isnan(nonstd::cx::floor(f64__f_long_n_inf)) )           ));
        REQUIRE(( ( std::floor(f64__f32_less_p_smallest)    == nonstd::cx::floor(f64__f32_less_p_smallest) )    || ( isnan(std::floor(f64__f32_less_p_smallest))    && isnan(nonstd::cx::floor(f64__f32_less_p_smallest)) )    ));
        REQUIRE(( ( std::floor(f64__f32_less_n_smallest)    == nonstd::cx::floor(f64__f32_less_n_smallest) )    || ( isnan(std::floor(f64__f32_less_n_smallest))    && isnan(nonstd::cx::floor(f64__f32_less_n_smallest)) )    ));
        REQUIRE(( ( std::floor(f64__f32_more_p_largest)     == nonstd::cx::floor(f64__f32_more_p_largest) )     || ( isnan(std::floor(f64__f32_more_p_largest))     && isnan(nonstd::cx::floor(f64__f32_more_p_largest)) )     ));
        REQUIRE(( ( std::floor(f64__f32_more_n_largest)     == nonstd::cx::floor(f64__f32_more_n_largest) )     || ( isnan(std::floor(f64__f32_more_n_largest))     && isnan(nonstd::cx::floor(f64__f32_more_n_largest)) )     ));
        REQUIRE(( ( std::floor(f64__f64_less_p_smallest)    == nonstd::cx::floor(f64__f64_less_p_smallest) )    || ( isnan(std::floor(f64__f64_less_p_smallest))    && isnan(nonstd::cx::floor(f64__f64_less_p_smallest)) )    ));
        REQUIRE(( ( std::floor(f64__f64_less_n_smallest)    == nonstd::cx::floor(f64__f64_less_n_smallest) )    || ( isnan(std::floor(f64__f64_less_n_smallest))    && isnan(nonstd::cx::floor(f64__f64_less_n_smallest)) )    ));
        REQUIRE(( ( std::floor(f64__f64_more_p_largest)     == nonstd::cx::floor(f64__f64_more_p_largest) )     || ( isnan(std::floor(f64__f64_more_p_largest))     && isnan(nonstd::cx::floor(f64__f64_more_p_largest)) )     ));
        REQUIRE(( ( std::floor(f64__f64_more_n_largest)     == nonstd::cx::floor(f64__f64_more_n_largest) )     || ( isnan(std::floor(f64__f64_more_n_largest))     && isnan(nonstd::cx::floor(f64__f64_more_n_largest)) )     ));
        REQUIRE(( ( std::floor(f64__f_long_less_p_smallest) == nonstd::cx::floor(f64__f_long_less_p_smallest) ) || ( isnan(std::floor(f64__f_long_less_p_smallest)) && isnan(nonstd::cx::floor(f64__f_long_less_p_smallest)) ) ));
        REQUIRE(( ( std::floor(f64__f_long_less_n_smallest) == nonstd::cx::floor(f64__f_long_less_n_smallest) ) || ( isnan(std::floor(f64__f_long_less_n_smallest)) && isnan(nonstd::cx::floor(f64__f_long_less_n_smallest)) ) ));
        REQUIRE(( ( std::floor(f64__f_long_more_p_largest)  == nonstd::cx::floor(f64__f_long_more_p_largest) )  || ( isnan(std::floor(f64__f_long_more_p_largest))  && isnan(nonstd::cx::floor(f64__f_long_more_p_largest)) )  ));
        REQUIRE(( ( std::floor(f64__f_long_more_n_largest)  == nonstd::cx::floor(f64__f_long_more_n_largest) )  || ( isnan(std::floor(f64__f_long_more_n_largest))  && isnan(nonstd::cx::floor(f64__f_long_more_n_largest)) )  ));

        REQUIRE(( ( std::floor(f_long__positive)               == nonstd::cx::floor(f_long__positive) )               || ( isnan(std::floor(f_long__positive))               && isnan(nonstd::cx::floor(f_long__positive)) )               ));
        REQUIRE(( ( std::floor(f_long__zero)                   == nonstd::cx::floor(f_long__zero) )                   || ( isnan(std::floor(f_long__zero))                   && isnan(nonstd::cx::floor(f_long__zero)) )                   ));
        REQUIRE(( ( std::floor(f_long__negative)               == nonstd::cx::floor(f_long__negative) )               || ( isnan(std::floor(f_long__negative))               && isnan(nonstd::cx::floor(f_long__negative)) )               ));
        REQUIRE(( ( std::floor(f_long__f32_p_smallest)         == nonstd::cx::floor(f_long__f32_p_smallest) )         || ( isnan(std::floor(f_long__f32_p_smallest))         && isnan(nonstd::cx::floor(f_long__f32_p_smallest)) )         ));
        REQUIRE(( ( std::floor(f_long__f32_n_smallest)         == nonstd::cx::floor(f_long__f32_n_smallest) )         || ( isnan(std::floor(f_long__f32_n_smallest))         && isnan(nonstd::cx::floor(f_long__f32_n_smallest)) )         ));
        REQUIRE(( ( std::floor(f_long__f32_p_largest)          == nonstd::cx::floor(f_long__f32_p_largest) )          || ( isnan(std::floor(f_long__f32_p_largest))          && isnan(nonstd::cx::floor(f_long__f32_p_largest)) )          ));
        REQUIRE(( ( std::floor(f_long__f32_n_largest)          == nonstd::cx::floor(f_long__f32_n_largest) )          || ( isnan(std::floor(f_long__f32_n_largest))          && isnan(nonstd::cx::floor(f_long__f32_n_largest)) )          ));
        REQUIRE(( ( std::floor(f_long__f64_p_smallest)         == nonstd::cx::floor(f_long__f64_p_smallest) )         || ( isnan(std::floor(f_long__f64_p_smallest))         && isnan(nonstd::cx::floor(f_long__f64_p_smallest)) )         ));
        REQUIRE(( ( std::floor(f_long__f64_n_smallest)         == nonstd::cx::floor(f_long__f64_n_smallest) )         || ( isnan(std::floor(f_long__f64_n_smallest))         && isnan(nonstd::cx::floor(f_long__f64_n_smallest)) )         ));
        REQUIRE(( ( std::floor(f_long__f64_p_largest)          == nonstd::cx::floor(f_long__f64_p_largest) )          || ( isnan(std::floor(f_long__f64_p_largest))          && isnan(nonstd::cx::floor(f_long__f64_p_largest)) )          ));
        REQUIRE(( ( std::floor(f_long__f64_n_largest)          == nonstd::cx::floor(f_long__f64_n_largest) )          || ( isnan(std::floor(f_long__f64_n_largest))          && isnan(nonstd::cx::floor(f_long__f64_n_largest)) )          ));
        REQUIRE(( ( std::floor(f_long__f_long_p_smallest)      == nonstd::cx::floor(f_long__f_long_p_smallest) )      || ( isnan(std::floor(f_long__f_long_p_smallest))      && isnan(nonstd::cx::floor(f_long__f_long_p_smallest)) )      ));
        REQUIRE(( ( std::floor(f_long__f_long_n_smallest)      == nonstd::cx::floor(f_long__f_long_n_smallest) )      || ( isnan(std::floor(f_long__f_long_n_smallest))      && isnan(nonstd::cx::floor(f_long__f_long_n_smallest)) )      ));
        REQUIRE(( ( std::floor(f_long__f_long_p_largest)       == nonstd::cx::floor(f_long__f_long_p_largest) )       || ( isnan(std::floor(f_long__f_long_p_largest))       && isnan(nonstd::cx::floor(f_long__f_long_p_largest)) )       ));
        REQUIRE(( ( std::floor(f_long__f_long_n_largest)       == nonstd::cx::floor(f_long__f_long_n_largest) )       || ( isnan(std::floor(f_long__f_long_n_largest))       && isnan(nonstd::cx::floor(f_long__f_long_n_largest)) )       ));
        REQUIRE(( ( std::floor(f_long__f32_nan)                == nonstd::cx::floor(f_long__f32_nan) )                || ( isnan(std::floor(f_long__f32_nan))                && isnan(nonstd::cx::floor(f_long__f32_nan)) )                ));
        REQUIRE(( ( std::floor(f_long__f64_nan)                == nonstd::cx::floor(f_long__f64_nan) )                || ( isnan(std::floor(f_long__f64_nan))                && isnan(nonstd::cx::floor(f_long__f64_nan)) )                ));
        REQUIRE(( ( std::floor(f_long__f_long_nan)             == nonstd::cx::floor(f_long__f_long_nan) )             || ( isnan(std::floor(f_long__f_long_nan))             && isnan(nonstd::cx::floor(f_long__f_long_nan)) )             ));
        REQUIRE(( ( std::floor(f_long__f32_p_inf)              == nonstd::cx::floor(f_long__f32_p_inf) )              || ( isnan(std::floor(f_long__f32_p_inf))              && isnan(nonstd::cx::floor(f_long__f32_p_inf)) )              ));
        REQUIRE(( ( std::floor(f_long__f32_n_inf)              == nonstd::cx::floor(f_long__f32_n_inf) )              || ( isnan(std::floor(f_long__f32_n_inf))              && isnan(nonstd::cx::floor(f_long__f32_n_inf)) )              ));
        REQUIRE(( ( std::floor(f_long__f64_p_inf)              == nonstd::cx::floor(f_long__f64_p_inf) )              || ( isnan(std::floor(f_long__f64_p_inf))              && isnan(nonstd::cx::floor(f_long__f64_p_inf)) )              ));
        REQUIRE(( ( std::floor(f_long__f64_n_inf)              == nonstd::cx::floor(f_long__f64_n_inf) )              || ( isnan(std::floor(f_long__f64_n_inf))              && isnan(nonstd::cx::floor(f_long__f64_n_inf)) )              ));
        REQUIRE(( ( std::floor(f_long__f_long_p_inf)           == nonstd::cx::floor(f_long__f_long_p_inf) )           || ( isnan(std::floor(f_long__f_long_p_inf))           && isnan(nonstd::cx::floor(f_long__f_long_p_inf)) )           ));
        REQUIRE(( ( std::floor(f_long__f_long_n_inf)           == nonstd::cx::floor(f_long__f_long_n_inf) )           || ( isnan(std::floor(f_long__f_long_n_inf))           && isnan(nonstd::cx::floor(f_long__f_long_n_inf)) )           ));
        REQUIRE(( ( std::floor(f_long__f32_less_p_smallest)    == nonstd::cx::floor(f_long__f32_less_p_smallest) )    || ( isnan(std::floor(f_long__f32_less_p_smallest))    && isnan(nonstd::cx::floor(f_long__f32_less_p_smallest)) )    ));
        REQUIRE(( ( std::floor(f_long__f32_less_n_smallest)    == nonstd::cx::floor(f_long__f32_less_n_smallest) )    || ( isnan(std::floor(f_long__f32_less_n_smallest))    && isnan(nonstd::cx::floor(f_long__f32_less_n_smallest)) )    ));
        REQUIRE(( ( std::floor(f_long__f32_more_p_largest)     == nonstd::cx::floor(f_long__f32_more_p_largest) )     || ( isnan(std::floor(f_long__f32_more_p_largest))     && isnan(nonstd::cx::floor(f_long__f32_more_p_largest)) )     ));
        REQUIRE(( ( std::floor(f_long__f32_more_n_largest)     == nonstd::cx::floor(f_long__f32_more_n_largest) )     || ( isnan(std::floor(f_long__f32_more_n_largest))     && isnan(nonstd::cx::floor(f_long__f32_more_n_largest)) )     ));
        REQUIRE(( ( std::floor(f_long__f64_less_p_smallest)    == nonstd::cx::floor(f_long__f64_less_p_smallest) )    || ( isnan(std::floor(f_long__f64_less_p_smallest))    && isnan(nonstd::cx::floor(f_long__f64_less_p_smallest)) )    ));
        REQUIRE(( ( std::floor(f_long__f64_less_n_smallest)    == nonstd::cx::floor(f_long__f64_less_n_smallest) )    || ( isnan(std::floor(f_long__f64_less_n_smallest))    && isnan(nonstd::cx::floor(f_long__f64_less_n_smallest)) )    ));
        REQUIRE(( ( std::floor(f_long__f64_more_p_largest)     == nonstd::cx::floor(f_long__f64_more_p_largest) )     || ( isnan(std::floor(f_long__f64_more_p_largest))     && isnan(nonstd::cx::floor(f_long__f64_more_p_largest)) )     ));
        REQUIRE(( ( std::floor(f_long__f64_more_n_largest)     == nonstd::cx::floor(f_long__f64_more_n_largest) )     || ( isnan(std::floor(f_long__f64_more_n_largest))     && isnan(nonstd::cx::floor(f_long__f64_more_n_largest)) )     ));
        REQUIRE(( ( std::floor(f_long__f_long_less_p_smallest) == nonstd::cx::floor(f_long__f_long_less_p_smallest) ) || ( isnan(std::floor(f_long__f_long_less_p_smallest)) && isnan(nonstd::cx::floor(f_long__f_long_less_p_smallest)) ) ));
        REQUIRE(( ( std::floor(f_long__f_long_less_n_smallest) == nonstd::cx::floor(f_long__f_long_less_n_smallest) ) || ( isnan(std::floor(f_long__f_long_less_n_smallest)) && isnan(nonstd::cx::floor(f_long__f_long_less_n_smallest)) ) ));
        REQUIRE(( ( std::floor(f_long__f_long_more_p_largest)  == nonstd::cx::floor(f_long__f_long_more_p_largest) )  || ( isnan(std::floor(f_long__f_long_more_p_largest))  && isnan(nonstd::cx::floor(f_long__f_long_more_p_largest)) )  ));
        REQUIRE(( ( std::floor(f_long__f_long_more_n_largest)  == nonstd::cx::floor(f_long__f_long_more_n_largest) )  || ( isnan(std::floor(f_long__f_long_more_n_largest))  && isnan(nonstd::cx::floor(f_long__f_long_more_n_largest)) )  ));

        REQUIRE(( ( std::floor(u64__positive) == nonstd::cx::floor(u64__positive) ) || ( isnan(std::floor(u64__positive)) && isnan(nonstd::cx::floor(u64__positive)) ) ));
        REQUIRE(( ( std::floor(u64__zero)     == nonstd::cx::floor(u64__zero) )     || ( isnan(std::floor(u64__zero))     && isnan(nonstd::cx::floor(u64__zero)) )     ));
        REQUIRE(( ( std::floor(u64__max)      == nonstd::cx::floor(u64__max) )      || ( isnan(std::floor(u64__max))      && isnan(nonstd::cx::floor(u64__max)) )      ));
        REQUIRE(( ( std::floor(i64__positive) == nonstd::cx::floor(i64__positive) ) || ( isnan(std::floor(i64__positive)) && isnan(nonstd::cx::floor(i64__positive)) ) ));
        REQUIRE(( ( std::floor(i64__zero)     == nonstd::cx::floor(i64__zero) )     || ( isnan(std::floor(i64__zero))     && isnan(nonstd::cx::floor(i64__zero)) )     ));
        REQUIRE(( ( std::floor(i64__negative) == nonstd::cx::floor(i64__negative) ) || ( isnan(std::floor(i64__negative)) && isnan(nonstd::cx::floor(i64__negative)) ) ));
        REQUIRE(( ( std::floor(i64__min)      == nonstd::cx::floor(i64__min) )      || ( isnan(std::floor(i64__min))      && isnan(nonstd::cx::floor(i64__min)) )      ));
        REQUIRE(( ( std::floor(i64__max)      == nonstd::cx::floor(i64__max) )      || ( isnan(std::floor(i64__max))      && isnan(nonstd::cx::floor(i64__max)) )      ));

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

        // For this function, it's worth doing some additional testing in ranges
        // we might see in the real world. Or just random ranges. Pulled from a
        // random number generator. Either one.
        std::random_device rd;
        std::mt19937 re(rd());

        std::uniform_real_distribution<float> rnd_float_range(
            -std::numeric_limits<float>::max(),
             std::numeric_limits<float>::max());
        std::uniform_real_distribution<double> rnd_double_range(
            -std::numeric_limits<double>::max(),
             std::numeric_limits<double>::max());
        std::uniform_real_distribution<long double> rnd_long_double_range(
            -std::numeric_limits<long double>::max(),
             std::numeric_limits<long double>::max());

        constexpr int iterations = 10000;
        for (int i = 0; i < iterations; i++) {
            float x = rnd_float_range(re);
            REQUIRE(( ( std::floor(x)  == nonstd::cx::floor(x) ) || ( isnan(std::floor(x)) && isnan(nonstd::cx::floor(x)) ) ));
        }
        for (int i = 0; i < iterations; i++) {
            double x = rnd_double_range(re);
            REQUIRE(( ( std::floor(x)  == nonstd::cx::floor(x) ) || ( isnan(std::floor(x)) && isnan(nonstd::cx::floor(x)) ) ));
        }
        for (int i = 0; i < iterations; i++) {
            long double x = rnd_long_double_range(re);
            REQUIRE(( ( std::floor(x)  == nonstd::cx::floor(x) ) || ( isnan(std::floor(x)) && isnan(nonstd::cx::floor(x)) ) ));
        }
    }

    SECTION("trunc") {
        constexpr auto calls_to_trunc_are_constexpr = nonstd::cx::trunc(f32__positive);

        using std::isnan;
        REQUIRE(( ( std::trunc(f32__positive)               == nonstd::cx::trunc(f32__positive) )               || ( isnan(std::trunc(f32__positive))               && isnan(nonstd::cx::trunc(f32__positive)) )               ));
        REQUIRE(( ( std::trunc(f32__zero)                   == nonstd::cx::trunc(f32__zero) )                   || ( isnan(std::trunc(f32__zero))                   && isnan(nonstd::cx::trunc(f32__zero)) )                   ));
        REQUIRE(( ( std::trunc(f32__negative)               == nonstd::cx::trunc(f32__negative) )               || ( isnan(std::trunc(f32__negative))               && isnan(nonstd::cx::trunc(f32__negative)) )               ));
        REQUIRE(( ( std::trunc(f32__f32_p_smallest)         == nonstd::cx::trunc(f32__f32_p_smallest) )         || ( isnan(std::trunc(f32__f32_p_smallest))         && isnan(nonstd::cx::trunc(f32__f32_p_smallest)) )         ));
        REQUIRE(( ( std::trunc(f32__f32_n_smallest)         == nonstd::cx::trunc(f32__f32_n_smallest) )         || ( isnan(std::trunc(f32__f32_n_smallest))         && isnan(nonstd::cx::trunc(f32__f32_n_smallest)) )         ));
        REQUIRE(( ( std::trunc(f32__f32_p_largest)          == nonstd::cx::trunc(f32__f32_p_largest) )          || ( isnan(std::trunc(f32__f32_p_largest))          && isnan(nonstd::cx::trunc(f32__f32_p_largest)) )          ));
        REQUIRE(( ( std::trunc(f32__f32_n_largest)          == nonstd::cx::trunc(f32__f32_n_largest) )          || ( isnan(std::trunc(f32__f32_n_largest))          && isnan(nonstd::cx::trunc(f32__f32_n_largest)) )          ));
        REQUIRE(( ( std::trunc(f32__f64_p_smallest)         == nonstd::cx::trunc(f32__f64_p_smallest) )         || ( isnan(std::trunc(f32__f64_p_smallest))         && isnan(nonstd::cx::trunc(f32__f64_p_smallest)) )         ));
        REQUIRE(( ( std::trunc(f32__f64_n_smallest)         == nonstd::cx::trunc(f32__f64_n_smallest) )         || ( isnan(std::trunc(f32__f64_n_smallest))         && isnan(nonstd::cx::trunc(f32__f64_n_smallest)) )         ));
    //  REQUIRE(( ( std::trunc(f32__f64_p_largest)          == nonstd::cx::trunc(f32__f64_p_largest) )          || ( isnan(std::trunc(f32__f64_p_largest))          && isnan(nonstd::cx::trunc(f32__f64_p_largest)) )          ));
    //  REQUIRE(( ( std::trunc(f32__f64_n_largest)          == nonstd::cx::trunc(f32__f64_n_largest) )          || ( isnan(std::trunc(f32__f64_n_largest))          && isnan(nonstd::cx::trunc(f32__f64_n_largest)) )          ));
        REQUIRE(( ( std::trunc(f32__f_long_p_smallest)      == nonstd::cx::trunc(f32__f_long_p_smallest) )      || ( isnan(std::trunc(f32__f_long_p_smallest))      && isnan(nonstd::cx::trunc(f32__f_long_p_smallest)) )      ));
        REQUIRE(( ( std::trunc(f32__f_long_n_smallest)      == nonstd::cx::trunc(f32__f_long_n_smallest) )      || ( isnan(std::trunc(f32__f_long_n_smallest))      && isnan(nonstd::cx::trunc(f32__f_long_n_smallest)) )      ));
    //  REQUIRE(( ( std::trunc(f32__f_long_p_largest)       == nonstd::cx::trunc(f32__f_long_p_largest) )       || ( isnan(std::trunc(f32__f_long_p_largest))       && isnan(nonstd::cx::trunc(f32__f_long_p_largest)) )       ));
    //  REQUIRE(( ( std::trunc(f32__f_long_n_largest)       == nonstd::cx::trunc(f32__f_long_n_largest) )       || ( isnan(std::trunc(f32__f_long_n_largest))       && isnan(nonstd::cx::trunc(f32__f_long_n_largest)) )       ));
        REQUIRE(( ( std::trunc(f32__f32_nan)                == nonstd::cx::trunc(f32__f32_nan) )                || ( isnan(std::trunc(f32__f32_nan))                && isnan(nonstd::cx::trunc(f32__f32_nan)) )                ));
        REQUIRE(( ( std::trunc(f32__f64_nan)                == nonstd::cx::trunc(f32__f64_nan) )                || ( isnan(std::trunc(f32__f64_nan))                && isnan(nonstd::cx::trunc(f32__f64_nan)) )                ));
        REQUIRE(( ( std::trunc(f32__f_long_nan)             == nonstd::cx::trunc(f32__f_long_nan) )             || ( isnan(std::trunc(f32__f_long_nan))             && isnan(nonstd::cx::trunc(f32__f_long_nan)) )             ));
        REQUIRE(( ( std::trunc(f32__f32_p_inf)              == nonstd::cx::trunc(f32__f32_p_inf) )              || ( isnan(std::trunc(f32__f32_p_inf))              && isnan(nonstd::cx::trunc(f32__f32_p_inf)) )              ));
        REQUIRE(( ( std::trunc(f32__f32_n_inf)              == nonstd::cx::trunc(f32__f32_n_inf) )              || ( isnan(std::trunc(f32__f32_n_inf))              && isnan(nonstd::cx::trunc(f32__f32_n_inf)) )              ));
        REQUIRE(( ( std::trunc(f32__f64_p_inf)              == nonstd::cx::trunc(f32__f64_p_inf) )              || ( isnan(std::trunc(f32__f64_p_inf))              && isnan(nonstd::cx::trunc(f32__f64_p_inf)) )              ));
        REQUIRE(( ( std::trunc(f32__f64_n_inf)              == nonstd::cx::trunc(f32__f64_n_inf) )              || ( isnan(std::trunc(f32__f64_n_inf))              && isnan(nonstd::cx::trunc(f32__f64_n_inf)) )              ));
        REQUIRE(( ( std::trunc(f32__f_long_p_inf)           == nonstd::cx::trunc(f32__f_long_p_inf) )           || ( isnan(std::trunc(f32__f_long_p_inf))           && isnan(nonstd::cx::trunc(f32__f_long_p_inf)) )           ));
        REQUIRE(( ( std::trunc(f32__f_long_n_inf)           == nonstd::cx::trunc(f32__f_long_n_inf) )           || ( isnan(std::trunc(f32__f_long_n_inf))           && isnan(nonstd::cx::trunc(f32__f_long_n_inf)) )           ));
        REQUIRE(( ( std::trunc(f32__f32_less_p_smallest)    == nonstd::cx::trunc(f32__f32_less_p_smallest) )    || ( isnan(std::trunc(f32__f32_less_p_smallest))    && isnan(nonstd::cx::trunc(f32__f32_less_p_smallest)) )    ));
        REQUIRE(( ( std::trunc(f32__f32_less_n_smallest)    == nonstd::cx::trunc(f32__f32_less_n_smallest) )    || ( isnan(std::trunc(f32__f32_less_n_smallest))    && isnan(nonstd::cx::trunc(f32__f32_less_n_smallest)) )    ));
        REQUIRE(( ( std::trunc(f32__f32_more_p_largest)     == nonstd::cx::trunc(f32__f32_more_p_largest) )     || ( isnan(std::trunc(f32__f32_more_p_largest))     && isnan(nonstd::cx::trunc(f32__f32_more_p_largest)) )     ));
        REQUIRE(( ( std::trunc(f32__f32_more_n_largest)     == nonstd::cx::trunc(f32__f32_more_n_largest) )     || ( isnan(std::trunc(f32__f32_more_n_largest))     && isnan(nonstd::cx::trunc(f32__f32_more_n_largest)) )     ));
        REQUIRE(( ( std::trunc(f32__f64_less_p_smallest)    == nonstd::cx::trunc(f32__f64_less_p_smallest) )    || ( isnan(std::trunc(f32__f64_less_p_smallest))    && isnan(nonstd::cx::trunc(f32__f64_less_p_smallest)) )    ));
        REQUIRE(( ( std::trunc(f32__f64_less_n_smallest)    == nonstd::cx::trunc(f32__f64_less_n_smallest) )    || ( isnan(std::trunc(f32__f64_less_n_smallest))    && isnan(nonstd::cx::trunc(f32__f64_less_n_smallest)) )    ));
        REQUIRE(( ( std::trunc(f32__f64_more_p_largest)     == nonstd::cx::trunc(f32__f64_more_p_largest) )     || ( isnan(std::trunc(f32__f64_more_p_largest))     && isnan(nonstd::cx::trunc(f32__f64_more_p_largest)) )     ));
        REQUIRE(( ( std::trunc(f32__f64_more_n_largest)     == nonstd::cx::trunc(f32__f64_more_n_largest) )     || ( isnan(std::trunc(f32__f64_more_n_largest))     && isnan(nonstd::cx::trunc(f32__f64_more_n_largest)) )     ));
        REQUIRE(( ( std::trunc(f32__f_long_less_p_smallest) == nonstd::cx::trunc(f32__f_long_less_p_smallest) ) || ( isnan(std::trunc(f32__f_long_less_p_smallest)) && isnan(nonstd::cx::trunc(f32__f_long_less_p_smallest)) ) ));
        REQUIRE(( ( std::trunc(f32__f_long_less_n_smallest) == nonstd::cx::trunc(f32__f_long_less_n_smallest) ) || ( isnan(std::trunc(f32__f_long_less_n_smallest)) && isnan(nonstd::cx::trunc(f32__f_long_less_n_smallest)) ) ));
        REQUIRE(( ( std::trunc(f32__f_long_more_p_largest)  == nonstd::cx::trunc(f32__f_long_more_p_largest) )  || ( isnan(std::trunc(f32__f_long_more_p_largest))  && isnan(nonstd::cx::trunc(f32__f_long_more_p_largest)) )  ));
        REQUIRE(( ( std::trunc(f32__f_long_more_n_largest)  == nonstd::cx::trunc(f32__f_long_more_n_largest) )  || ( isnan(std::trunc(f32__f_long_more_n_largest))  && isnan(nonstd::cx::trunc(f32__f_long_more_n_largest)) )  ));

        REQUIRE(( ( std::trunc(f64__positive)               == nonstd::cx::trunc(f64__positive) )               || ( isnan(std::trunc(f64__positive))               && isnan(nonstd::cx::trunc(f64__positive)) )               ));
        REQUIRE(( ( std::trunc(f64__zero)                   == nonstd::cx::trunc(f64__zero) )                   || ( isnan(std::trunc(f64__zero))                   && isnan(nonstd::cx::trunc(f64__zero)) )                   ));
        REQUIRE(( ( std::trunc(f64__negative)               == nonstd::cx::trunc(f64__negative) )               || ( isnan(std::trunc(f64__negative))               && isnan(nonstd::cx::trunc(f64__negative)) )               ));
        REQUIRE(( ( std::trunc(f64__f32_p_smallest)         == nonstd::cx::trunc(f64__f32_p_smallest) )         || ( isnan(std::trunc(f64__f32_p_smallest))         && isnan(nonstd::cx::trunc(f64__f32_p_smallest)) )         ));
        REQUIRE(( ( std::trunc(f64__f32_n_smallest)         == nonstd::cx::trunc(f64__f32_n_smallest) )         || ( isnan(std::trunc(f64__f32_n_smallest))         && isnan(nonstd::cx::trunc(f64__f32_n_smallest)) )         ));
        REQUIRE(( ( std::trunc(f64__f32_p_largest)          == nonstd::cx::trunc(f64__f32_p_largest) )          || ( isnan(std::trunc(f64__f32_p_largest))          && isnan(nonstd::cx::trunc(f64__f32_p_largest)) )          ));
        REQUIRE(( ( std::trunc(f64__f32_n_largest)          == nonstd::cx::trunc(f64__f32_n_largest) )          || ( isnan(std::trunc(f64__f32_n_largest))          && isnan(nonstd::cx::trunc(f64__f32_n_largest)) )          ));
        REQUIRE(( ( std::trunc(f64__f64_p_smallest)         == nonstd::cx::trunc(f64__f64_p_smallest) )         || ( isnan(std::trunc(f64__f64_p_smallest))         && isnan(nonstd::cx::trunc(f64__f64_p_smallest)) )         ));
        REQUIRE(( ( std::trunc(f64__f64_n_smallest)         == nonstd::cx::trunc(f64__f64_n_smallest) )         || ( isnan(std::trunc(f64__f64_n_smallest))         && isnan(nonstd::cx::trunc(f64__f64_n_smallest)) )         ));
        REQUIRE(( ( std::trunc(f64__f64_p_largest)          == nonstd::cx::trunc(f64__f64_p_largest) )          || ( isnan(std::trunc(f64__f64_p_largest))          && isnan(nonstd::cx::trunc(f64__f64_p_largest)) )          ));
        REQUIRE(( ( std::trunc(f64__f64_n_largest)          == nonstd::cx::trunc(f64__f64_n_largest) )          || ( isnan(std::trunc(f64__f64_n_largest))          && isnan(nonstd::cx::trunc(f64__f64_n_largest)) )          ));
        REQUIRE(( ( std::trunc(f64__f_long_p_smallest)      == nonstd::cx::trunc(f64__f_long_p_smallest) )      || ( isnan(std::trunc(f64__f_long_p_smallest))      && isnan(nonstd::cx::trunc(f64__f_long_p_smallest)) )      ));
        REQUIRE(( ( std::trunc(f64__f_long_n_smallest)      == nonstd::cx::trunc(f64__f_long_n_smallest) )      || ( isnan(std::trunc(f64__f_long_n_smallest))      && isnan(nonstd::cx::trunc(f64__f_long_n_smallest)) )      ));
    //  REQUIRE(( ( std::trunc(f64__f_long_p_largest)       == nonstd::cx::trunc(f64__f_long_p_largest) )       || ( isnan(std::trunc(f64__f_long_p_largest))       && isnan(nonstd::cx::trunc(f64__f_long_p_largest)) )       ));
    //  REQUIRE(( ( std::trunc(f64__f_long_n_largest)       == nonstd::cx::trunc(f64__f_long_n_largest) )       || ( isnan(std::trunc(f64__f_long_n_largest))       && isnan(nonstd::cx::trunc(f64__f_long_n_largest)) )       ));
        REQUIRE(( ( std::trunc(f64__f32_nan)                == nonstd::cx::trunc(f64__f32_nan) )                || ( isnan(std::trunc(f64__f32_nan))                && isnan(nonstd::cx::trunc(f64__f32_nan)) )                ));
        REQUIRE(( ( std::trunc(f64__f64_nan)                == nonstd::cx::trunc(f64__f64_nan) )                || ( isnan(std::trunc(f64__f64_nan))                && isnan(nonstd::cx::trunc(f64__f64_nan)) )                ));
        REQUIRE(( ( std::trunc(f64__f_long_nan)             == nonstd::cx::trunc(f64__f_long_nan) )             || ( isnan(std::trunc(f64__f_long_nan))             && isnan(nonstd::cx::trunc(f64__f_long_nan)) )             ));
        REQUIRE(( ( std::trunc(f64__f32_p_inf)              == nonstd::cx::trunc(f64__f32_p_inf) )              || ( isnan(std::trunc(f64__f32_p_inf))              && isnan(nonstd::cx::trunc(f64__f32_p_inf)) )              ));
        REQUIRE(( ( std::trunc(f64__f32_n_inf)              == nonstd::cx::trunc(f64__f32_n_inf) )              || ( isnan(std::trunc(f64__f32_n_inf))              && isnan(nonstd::cx::trunc(f64__f32_n_inf)) )              ));
        REQUIRE(( ( std::trunc(f64__f64_p_inf)              == nonstd::cx::trunc(f64__f64_p_inf) )              || ( isnan(std::trunc(f64__f64_p_inf))              && isnan(nonstd::cx::trunc(f64__f64_p_inf)) )              ));
        REQUIRE(( ( std::trunc(f64__f64_n_inf)              == nonstd::cx::trunc(f64__f64_n_inf) )              || ( isnan(std::trunc(f64__f64_n_inf))              && isnan(nonstd::cx::trunc(f64__f64_n_inf)) )              ));
        REQUIRE(( ( std::trunc(f64__f_long_p_inf)           == nonstd::cx::trunc(f64__f_long_p_inf) )           || ( isnan(std::trunc(f64__f_long_p_inf))           && isnan(nonstd::cx::trunc(f64__f_long_p_inf)) )           ));
        REQUIRE(( ( std::trunc(f64__f_long_n_inf)           == nonstd::cx::trunc(f64__f_long_n_inf) )           || ( isnan(std::trunc(f64__f_long_n_inf))           && isnan(nonstd::cx::trunc(f64__f_long_n_inf)) )           ));
        REQUIRE(( ( std::trunc(f64__f32_less_p_smallest)    == nonstd::cx::trunc(f64__f32_less_p_smallest) )    || ( isnan(std::trunc(f64__f32_less_p_smallest))    && isnan(nonstd::cx::trunc(f64__f32_less_p_smallest)) )    ));
        REQUIRE(( ( std::trunc(f64__f32_less_n_smallest)    == nonstd::cx::trunc(f64__f32_less_n_smallest) )    || ( isnan(std::trunc(f64__f32_less_n_smallest))    && isnan(nonstd::cx::trunc(f64__f32_less_n_smallest)) )    ));
        REQUIRE(( ( std::trunc(f64__f32_more_p_largest)     == nonstd::cx::trunc(f64__f32_more_p_largest) )     || ( isnan(std::trunc(f64__f32_more_p_largest))     && isnan(nonstd::cx::trunc(f64__f32_more_p_largest)) )     ));
        REQUIRE(( ( std::trunc(f64__f32_more_n_largest)     == nonstd::cx::trunc(f64__f32_more_n_largest) )     || ( isnan(std::trunc(f64__f32_more_n_largest))     && isnan(nonstd::cx::trunc(f64__f32_more_n_largest)) )     ));
        REQUIRE(( ( std::trunc(f64__f64_less_p_smallest)    == nonstd::cx::trunc(f64__f64_less_p_smallest) )    || ( isnan(std::trunc(f64__f64_less_p_smallest))    && isnan(nonstd::cx::trunc(f64__f64_less_p_smallest)) )    ));
        REQUIRE(( ( std::trunc(f64__f64_less_n_smallest)    == nonstd::cx::trunc(f64__f64_less_n_smallest) )    || ( isnan(std::trunc(f64__f64_less_n_smallest))    && isnan(nonstd::cx::trunc(f64__f64_less_n_smallest)) )    ));
        REQUIRE(( ( std::trunc(f64__f64_more_p_largest)     == nonstd::cx::trunc(f64__f64_more_p_largest) )     || ( isnan(std::trunc(f64__f64_more_p_largest))     && isnan(nonstd::cx::trunc(f64__f64_more_p_largest)) )     ));
        REQUIRE(( ( std::trunc(f64__f64_more_n_largest)     == nonstd::cx::trunc(f64__f64_more_n_largest) )     || ( isnan(std::trunc(f64__f64_more_n_largest))     && isnan(nonstd::cx::trunc(f64__f64_more_n_largest)) )     ));
        REQUIRE(( ( std::trunc(f64__f_long_less_p_smallest) == nonstd::cx::trunc(f64__f_long_less_p_smallest) ) || ( isnan(std::trunc(f64__f_long_less_p_smallest)) && isnan(nonstd::cx::trunc(f64__f_long_less_p_smallest)) ) ));
        REQUIRE(( ( std::trunc(f64__f_long_less_n_smallest) == nonstd::cx::trunc(f64__f_long_less_n_smallest) ) || ( isnan(std::trunc(f64__f_long_less_n_smallest)) && isnan(nonstd::cx::trunc(f64__f_long_less_n_smallest)) ) ));
        REQUIRE(( ( std::trunc(f64__f_long_more_p_largest)  == nonstd::cx::trunc(f64__f_long_more_p_largest) )  || ( isnan(std::trunc(f64__f_long_more_p_largest))  && isnan(nonstd::cx::trunc(f64__f_long_more_p_largest)) )  ));
        REQUIRE(( ( std::trunc(f64__f_long_more_n_largest)  == nonstd::cx::trunc(f64__f_long_more_n_largest) )  || ( isnan(std::trunc(f64__f_long_more_n_largest))  && isnan(nonstd::cx::trunc(f64__f_long_more_n_largest)) )  ));

        REQUIRE(( ( std::trunc(f_long__positive)               == nonstd::cx::trunc(f_long__positive) )               || ( isnan(std::trunc(f_long__positive))               && isnan(nonstd::cx::trunc(f_long__positive)) )               ));
        REQUIRE(( ( std::trunc(f_long__zero)                   == nonstd::cx::trunc(f_long__zero) )                   || ( isnan(std::trunc(f_long__zero))                   && isnan(nonstd::cx::trunc(f_long__zero)) )                   ));
        REQUIRE(( ( std::trunc(f_long__negative)               == nonstd::cx::trunc(f_long__negative) )               || ( isnan(std::trunc(f_long__negative))               && isnan(nonstd::cx::trunc(f_long__negative)) )               ));
        REQUIRE(( ( std::trunc(f_long__f32_p_smallest)         == nonstd::cx::trunc(f_long__f32_p_smallest) )         || ( isnan(std::trunc(f_long__f32_p_smallest))         && isnan(nonstd::cx::trunc(f_long__f32_p_smallest)) )         ));
        REQUIRE(( ( std::trunc(f_long__f32_n_smallest)         == nonstd::cx::trunc(f_long__f32_n_smallest) )         || ( isnan(std::trunc(f_long__f32_n_smallest))         && isnan(nonstd::cx::trunc(f_long__f32_n_smallest)) )         ));
        REQUIRE(( ( std::trunc(f_long__f32_p_largest)          == nonstd::cx::trunc(f_long__f32_p_largest) )          || ( isnan(std::trunc(f_long__f32_p_largest))          && isnan(nonstd::cx::trunc(f_long__f32_p_largest)) )          ));
        REQUIRE(( ( std::trunc(f_long__f32_n_largest)          == nonstd::cx::trunc(f_long__f32_n_largest) )          || ( isnan(std::trunc(f_long__f32_n_largest))          && isnan(nonstd::cx::trunc(f_long__f32_n_largest)) )          ));
        REQUIRE(( ( std::trunc(f_long__f64_p_smallest)         == nonstd::cx::trunc(f_long__f64_p_smallest) )         || ( isnan(std::trunc(f_long__f64_p_smallest))         && isnan(nonstd::cx::trunc(f_long__f64_p_smallest)) )         ));
        REQUIRE(( ( std::trunc(f_long__f64_n_smallest)         == nonstd::cx::trunc(f_long__f64_n_smallest) )         || ( isnan(std::trunc(f_long__f64_n_smallest))         && isnan(nonstd::cx::trunc(f_long__f64_n_smallest)) )         ));
        REQUIRE(( ( std::trunc(f_long__f64_p_largest)          == nonstd::cx::trunc(f_long__f64_p_largest) )          || ( isnan(std::trunc(f_long__f64_p_largest))          && isnan(nonstd::cx::trunc(f_long__f64_p_largest)) )          ));
        REQUIRE(( ( std::trunc(f_long__f64_n_largest)          == nonstd::cx::trunc(f_long__f64_n_largest) )          || ( isnan(std::trunc(f_long__f64_n_largest))          && isnan(nonstd::cx::trunc(f_long__f64_n_largest)) )          ));
        REQUIRE(( ( std::trunc(f_long__f_long_p_smallest)      == nonstd::cx::trunc(f_long__f_long_p_smallest) )      || ( isnan(std::trunc(f_long__f_long_p_smallest))      && isnan(nonstd::cx::trunc(f_long__f_long_p_smallest)) )      ));
        REQUIRE(( ( std::trunc(f_long__f_long_n_smallest)      == nonstd::cx::trunc(f_long__f_long_n_smallest) )      || ( isnan(std::trunc(f_long__f_long_n_smallest))      && isnan(nonstd::cx::trunc(f_long__f_long_n_smallest)) )      ));
        REQUIRE(( ( std::trunc(f_long__f_long_p_largest)       == nonstd::cx::trunc(f_long__f_long_p_largest) )       || ( isnan(std::trunc(f_long__f_long_p_largest))       && isnan(nonstd::cx::trunc(f_long__f_long_p_largest)) )       ));
        REQUIRE(( ( std::trunc(f_long__f_long_n_largest)       == nonstd::cx::trunc(f_long__f_long_n_largest) )       || ( isnan(std::trunc(f_long__f_long_n_largest))       && isnan(nonstd::cx::trunc(f_long__f_long_n_largest)) )       ));
        REQUIRE(( ( std::trunc(f_long__f32_nan)                == nonstd::cx::trunc(f_long__f32_nan) )                || ( isnan(std::trunc(f_long__f32_nan))                && isnan(nonstd::cx::trunc(f_long__f32_nan)) )                ));
        REQUIRE(( ( std::trunc(f_long__f64_nan)                == nonstd::cx::trunc(f_long__f64_nan) )                || ( isnan(std::trunc(f_long__f64_nan))                && isnan(nonstd::cx::trunc(f_long__f64_nan)) )                ));
        REQUIRE(( ( std::trunc(f_long__f_long_nan)             == nonstd::cx::trunc(f_long__f_long_nan) )             || ( isnan(std::trunc(f_long__f_long_nan))             && isnan(nonstd::cx::trunc(f_long__f_long_nan)) )             ));
        REQUIRE(( ( std::trunc(f_long__f32_p_inf)              == nonstd::cx::trunc(f_long__f32_p_inf) )              || ( isnan(std::trunc(f_long__f32_p_inf))              && isnan(nonstd::cx::trunc(f_long__f32_p_inf)) )              ));
        REQUIRE(( ( std::trunc(f_long__f32_n_inf)              == nonstd::cx::trunc(f_long__f32_n_inf) )              || ( isnan(std::trunc(f_long__f32_n_inf))              && isnan(nonstd::cx::trunc(f_long__f32_n_inf)) )              ));
        REQUIRE(( ( std::trunc(f_long__f64_p_inf)              == nonstd::cx::trunc(f_long__f64_p_inf) )              || ( isnan(std::trunc(f_long__f64_p_inf))              && isnan(nonstd::cx::trunc(f_long__f64_p_inf)) )              ));
        REQUIRE(( ( std::trunc(f_long__f64_n_inf)              == nonstd::cx::trunc(f_long__f64_n_inf) )              || ( isnan(std::trunc(f_long__f64_n_inf))              && isnan(nonstd::cx::trunc(f_long__f64_n_inf)) )              ));
        REQUIRE(( ( std::trunc(f_long__f_long_p_inf)           == nonstd::cx::trunc(f_long__f_long_p_inf) )           || ( isnan(std::trunc(f_long__f_long_p_inf))           && isnan(nonstd::cx::trunc(f_long__f_long_p_inf)) )           ));
        REQUIRE(( ( std::trunc(f_long__f_long_n_inf)           == nonstd::cx::trunc(f_long__f_long_n_inf) )           || ( isnan(std::trunc(f_long__f_long_n_inf))           && isnan(nonstd::cx::trunc(f_long__f_long_n_inf)) )           ));
        REQUIRE(( ( std::trunc(f_long__f32_less_p_smallest)    == nonstd::cx::trunc(f_long__f32_less_p_smallest) )    || ( isnan(std::trunc(f_long__f32_less_p_smallest))    && isnan(nonstd::cx::trunc(f_long__f32_less_p_smallest)) )    ));
        REQUIRE(( ( std::trunc(f_long__f32_less_n_smallest)    == nonstd::cx::trunc(f_long__f32_less_n_smallest) )    || ( isnan(std::trunc(f_long__f32_less_n_smallest))    && isnan(nonstd::cx::trunc(f_long__f32_less_n_smallest)) )    ));
        REQUIRE(( ( std::trunc(f_long__f32_more_p_largest)     == nonstd::cx::trunc(f_long__f32_more_p_largest) )     || ( isnan(std::trunc(f_long__f32_more_p_largest))     && isnan(nonstd::cx::trunc(f_long__f32_more_p_largest)) )     ));
        REQUIRE(( ( std::trunc(f_long__f32_more_n_largest)     == nonstd::cx::trunc(f_long__f32_more_n_largest) )     || ( isnan(std::trunc(f_long__f32_more_n_largest))     && isnan(nonstd::cx::trunc(f_long__f32_more_n_largest)) )     ));
        REQUIRE(( ( std::trunc(f_long__f64_less_p_smallest)    == nonstd::cx::trunc(f_long__f64_less_p_smallest) )    || ( isnan(std::trunc(f_long__f64_less_p_smallest))    && isnan(nonstd::cx::trunc(f_long__f64_less_p_smallest)) )    ));
        REQUIRE(( ( std::trunc(f_long__f64_less_n_smallest)    == nonstd::cx::trunc(f_long__f64_less_n_smallest) )    || ( isnan(std::trunc(f_long__f64_less_n_smallest))    && isnan(nonstd::cx::trunc(f_long__f64_less_n_smallest)) )    ));
        REQUIRE(( ( std::trunc(f_long__f64_more_p_largest)     == nonstd::cx::trunc(f_long__f64_more_p_largest) )     || ( isnan(std::trunc(f_long__f64_more_p_largest))     && isnan(nonstd::cx::trunc(f_long__f64_more_p_largest)) )     ));
        REQUIRE(( ( std::trunc(f_long__f64_more_n_largest)     == nonstd::cx::trunc(f_long__f64_more_n_largest) )     || ( isnan(std::trunc(f_long__f64_more_n_largest))     && isnan(nonstd::cx::trunc(f_long__f64_more_n_largest)) )     ));
        REQUIRE(( ( std::trunc(f_long__f_long_less_p_smallest) == nonstd::cx::trunc(f_long__f_long_less_p_smallest) ) || ( isnan(std::trunc(f_long__f_long_less_p_smallest)) && isnan(nonstd::cx::trunc(f_long__f_long_less_p_smallest)) ) ));
        REQUIRE(( ( std::trunc(f_long__f_long_less_n_smallest) == nonstd::cx::trunc(f_long__f_long_less_n_smallest) ) || ( isnan(std::trunc(f_long__f_long_less_n_smallest)) && isnan(nonstd::cx::trunc(f_long__f_long_less_n_smallest)) ) ));
        REQUIRE(( ( std::trunc(f_long__f_long_more_p_largest)  == nonstd::cx::trunc(f_long__f_long_more_p_largest) )  || ( isnan(std::trunc(f_long__f_long_more_p_largest))  && isnan(nonstd::cx::trunc(f_long__f_long_more_p_largest)) )  ));
        REQUIRE(( ( std::trunc(f_long__f_long_more_n_largest)  == nonstd::cx::trunc(f_long__f_long_more_n_largest) )  || ( isnan(std::trunc(f_long__f_long_more_n_largest))  && isnan(nonstd::cx::trunc(f_long__f_long_more_n_largest)) )  ));

        REQUIRE(( ( std::trunc(u64__positive) == nonstd::cx::trunc(u64__positive) ) || ( isnan(std::trunc(u64__positive)) && isnan(nonstd::cx::trunc(u64__positive)) ) ));
        REQUIRE(( ( std::trunc(u64__zero)     == nonstd::cx::trunc(u64__zero) )     || ( isnan(std::trunc(u64__zero))     && isnan(nonstd::cx::trunc(u64__zero)) )     ));
        REQUIRE(( ( std::trunc(u64__max)      == nonstd::cx::trunc(u64__max) )      || ( isnan(std::trunc(u64__max))      && isnan(nonstd::cx::trunc(u64__max)) )      ));
        REQUIRE(( ( std::trunc(i64__positive) == nonstd::cx::trunc(i64__positive) ) || ( isnan(std::trunc(i64__positive)) && isnan(nonstd::cx::trunc(i64__positive)) ) ));
        REQUIRE(( ( std::trunc(i64__zero)     == nonstd::cx::trunc(i64__zero) )     || ( isnan(std::trunc(i64__zero))     && isnan(nonstd::cx::trunc(i64__zero)) )     ));
        REQUIRE(( ( std::trunc(i64__negative) == nonstd::cx::trunc(i64__negative) ) || ( isnan(std::trunc(i64__negative)) && isnan(nonstd::cx::trunc(i64__negative)) ) ));
        REQUIRE(( ( std::trunc(i64__min)      == nonstd::cx::trunc(i64__min) )      || ( isnan(std::trunc(i64__min))      && isnan(nonstd::cx::trunc(i64__min)) )      ));
        REQUIRE(( ( std::trunc(i64__max)      == nonstd::cx::trunc(i64__max) )      || ( isnan(std::trunc(i64__max))      && isnan(nonstd::cx::trunc(i64__max)) )      ));

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

        // For this function, it's worth doing some additional testing in ranges
        // we might see in the real world. Or just random ranges. Pulled from a
        // random number generator. Either one.
        std::random_device rd;
        std::mt19937 re(rd());

        std::uniform_real_distribution<float> rnd_float_range(
            -std::numeric_limits<float>::max(),
             std::numeric_limits<float>::max());
        std::uniform_real_distribution<double> rnd_double_range(
            -std::numeric_limits<double>::max(),
             std::numeric_limits<double>::max());
        std::uniform_real_distribution<long double> rnd_long_double_range(
            -std::numeric_limits<long double>::max(),
             std::numeric_limits<long double>::max());

        constexpr int iterations = 10000;
        for (int i = 0; i < iterations; i++) {
            float x = rnd_float_range(re);
            REQUIRE(( ( std::trunc(x)  == nonstd::cx::trunc(x) ) || ( isnan(std::trunc(x)) && isnan(nonstd::cx::trunc(x)) ) ));
        }
        for (int i = 0; i < iterations; i++) {
            double x = rnd_double_range(re);
            REQUIRE(( ( std::trunc(x)  == nonstd::cx::trunc(x) ) || ( isnan(std::trunc(x)) && isnan(nonstd::cx::trunc(x)) ) ));
        }
        for (int i = 0; i < iterations; i++) {
            long double x = rnd_long_double_range(re);
            REQUIRE(( ( std::trunc(x)  == nonstd::cx::trunc(x) ) || ( isnan(std::trunc(x)) && isnan(nonstd::cx::trunc(x)) ) ));
        }
    }

    SECTION("fmod") {
        constexpr auto calls_to_fmod_are_constexpr = nonstd::cx::fmod(f32__positive, 1.0f);

        using std::isnan;

        // Test basic sign combinations
        REQUIRE(( ( std::fmod(f32__positive,  1.0f) == nonstd::cx::fmod(f32__positive,  1.0f) ) || ( isnan(std::fmod(f32__positive,  1.0f)) && isnan(nonstd::cx::fmod(f32__positive,  1.0f)) ) ));
        REQUIRE(( ( std::fmod(f32__positive, -1.0f) == nonstd::cx::fmod(f32__positive, -1.0f) ) || ( isnan(std::fmod(f32__positive, -1.0f)) && isnan(nonstd::cx::fmod(f32__positive, -1.0f)) ) ));
        REQUIRE(( ( std::fmod(f32__zero,      1.0f) == nonstd::cx::fmod(f32__zero,      1.0f) ) || ( isnan(std::fmod(f32__zero,      1.0f)) && isnan(nonstd::cx::fmod(f32__zero,      1.0f)) ) ));
        REQUIRE(( ( std::fmod(f32__zero,     -1.0f) == nonstd::cx::fmod(f32__zero,     -1.0f) ) || ( isnan(std::fmod(f32__zero,     -1.0f)) && isnan(nonstd::cx::fmod(f32__zero,     -1.0f)) ) ));
        REQUIRE(( ( std::fmod(f32__negative,  1.0f) == nonstd::cx::fmod(f32__negative,  1.0f) ) || ( isnan(std::fmod(f32__negative,  1.0f)) && isnan(nonstd::cx::fmod(f32__negative,  1.0f)) ) ));
        REQUIRE(( ( std::fmod(f32__negative, -1.0f) == nonstd::cx::fmod(f32__negative, -1.0f) ) || ( isnan(std::fmod(f32__negative, -1.0f)) && isnan(nonstd::cx::fmod(f32__negative, -1.0f)) ) ));


        REQUIRE(( ( std::fmod(f64__positive,  1.0) == nonstd::cx::fmod(f64__positive,  1.0) ) || ( isnan(std::fmod(f64__positive,  1.0)) && isnan(nonstd::cx::fmod(f64__positive,  1.0)) ) ));
        REQUIRE(( ( std::fmod(f64__positive, -1.0) == nonstd::cx::fmod(f64__positive, -1.0) ) || ( isnan(std::fmod(f64__positive, -1.0)) && isnan(nonstd::cx::fmod(f64__positive, -1.0)) ) ));
        REQUIRE(( ( std::fmod(f64__zero,      1.0) == nonstd::cx::fmod(f64__zero,      1.0) ) || ( isnan(std::fmod(f64__zero,      1.0)) && isnan(nonstd::cx::fmod(f64__zero,      1.0)) ) ));
        REQUIRE(( ( std::fmod(f64__zero,     -1.0) == nonstd::cx::fmod(f64__zero,     -1.0) ) || ( isnan(std::fmod(f64__zero,     -1.0)) && isnan(nonstd::cx::fmod(f64__zero,     -1.0)) ) ));
        REQUIRE(( ( std::fmod(f64__negative,  1.0) == nonstd::cx::fmod(f64__negative,  1.0) ) || ( isnan(std::fmod(f64__negative,  1.0)) && isnan(nonstd::cx::fmod(f64__negative,  1.0)) ) ));
        REQUIRE(( ( std::fmod(f64__negative, -1.0) == nonstd::cx::fmod(f64__negative, -1.0) ) || ( isnan(std::fmod(f64__negative, -1.0)) && isnan(nonstd::cx::fmod(f64__negative, -1.0)) ) ));


        REQUIRE(( ( std::fmod(f_long__positive,  1.0l) == nonstd::cx::fmod(f_long__positive,  1.0l) ) || ( isnan(std::fmod(f_long__positive,  1.0l)) && isnan(nonstd::cx::fmod(f_long__positive,  1.0l)) ) ));
        REQUIRE(( ( std::fmod(f_long__positive, -1.0l) == nonstd::cx::fmod(f_long__positive, -1.0l) ) || ( isnan(std::fmod(f_long__positive, -1.0l)) && isnan(nonstd::cx::fmod(f_long__positive, -1.0l)) ) ));
        REQUIRE(( ( std::fmod(f_long__zero,      1.0l) == nonstd::cx::fmod(f_long__zero,      1.0l) ) || ( isnan(std::fmod(f_long__zero,      1.0l)) && isnan(nonstd::cx::fmod(f_long__zero,      1.0l)) ) ));
        REQUIRE(( ( std::fmod(f_long__zero,     -1.0l) == nonstd::cx::fmod(f_long__zero,     -1.0l) ) || ( isnan(std::fmod(f_long__zero,     -1.0l)) && isnan(nonstd::cx::fmod(f_long__zero,     -1.0l)) ) ));
        REQUIRE(( ( std::fmod(f_long__negative,  1.0l) == nonstd::cx::fmod(f_long__negative,  1.0l) ) || ( isnan(std::fmod(f_long__negative,  1.0l)) && isnan(nonstd::cx::fmod(f_long__negative,  1.0l)) ) ));
        REQUIRE(( ( std::fmod(f_long__negative, -1.0l) == nonstd::cx::fmod(f_long__negative, -1.0l) ) || ( isnan(std::fmod(f_long__negative, -1.0l)) && isnan(nonstd::cx::fmod(f_long__negative, -1.0l)) ) ));


        // Test numeric extents
        REQUIRE(( ( std::fmod(f32__f32_p_smallest,  1.0f) == nonstd::cx::fmod(f32__f32_p_smallest,  1.0f) ) || ( isnan(std::fmod(f32__f32_p_smallest,  1.0f)) && isnan(nonstd::cx::fmod(f32__f32_p_smallest,  1.0f)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_p_smallest, -1.0f) == nonstd::cx::fmod(f32__f32_p_smallest, -1.0f) ) || ( isnan(std::fmod(f32__f32_p_smallest, -1.0f)) && isnan(nonstd::cx::fmod(f32__f32_p_smallest, -1.0f)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_n_smallest,  1.0f) == nonstd::cx::fmod(f32__f32_n_smallest,  1.0f) ) || ( isnan(std::fmod(f32__f32_n_smallest,  1.0f)) && isnan(nonstd::cx::fmod(f32__f32_n_smallest,  1.0f)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_n_smallest, -1.0f) == nonstd::cx::fmod(f32__f32_n_smallest, -1.0f) ) || ( isnan(std::fmod(f32__f32_n_smallest, -1.0f)) && isnan(nonstd::cx::fmod(f32__f32_n_smallest, -1.0f)) ) ));
        REQUIRE(( ( std::fmod( 1.0f, f32__f32_p_smallest) == nonstd::cx::fmod( 1.0f, f32__f32_p_smallest) ) || ( isnan(std::fmod( 1.0f, f32__f32_p_smallest)) && isnan(nonstd::cx::fmod( 1.0f, f32__f32_p_smallest)) ) ));
        REQUIRE(( ( std::fmod(-1.0f, f32__f32_p_smallest) == nonstd::cx::fmod(-1.0f, f32__f32_p_smallest) ) || ( isnan(std::fmod(-1.0f, f32__f32_p_smallest)) && isnan(nonstd::cx::fmod(-1.0f, f32__f32_p_smallest)) ) ));
        REQUIRE(( ( std::fmod( 1.0f, f32__f32_n_smallest) == nonstd::cx::fmod( 1.0f, f32__f32_n_smallest) ) || ( isnan(std::fmod( 1.0f, f32__f32_n_smallest)) && isnan(nonstd::cx::fmod( 1.0f, f32__f32_n_smallest)) ) ));
        REQUIRE(( ( std::fmod(-1.0f, f32__f32_n_smallest) == nonstd::cx::fmod(-1.0f, f32__f32_n_smallest) ) || ( isnan(std::fmod(-1.0f, f32__f32_n_smallest)) && isnan(nonstd::cx::fmod(-1.0f, f32__f32_n_smallest)) ) ));

        REQUIRE(( ( std::fmod(f32__f32_p_largest,  1.0f) == nonstd::cx::fmod(f32__f32_p_largest,  1.0f) ) || ( isnan(std::fmod(f32__f32_p_largest,  1.0f)) && isnan(nonstd::cx::fmod(f32__f32_p_largest,  1.0f)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_p_largest, -1.0f) == nonstd::cx::fmod(f32__f32_p_largest, -1.0f) ) || ( isnan(std::fmod(f32__f32_p_largest, -1.0f)) && isnan(nonstd::cx::fmod(f32__f32_p_largest, -1.0f)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_n_largest,  1.0f) == nonstd::cx::fmod(f32__f32_n_largest,  1.0f) ) || ( isnan(std::fmod(f32__f32_n_largest,  1.0f)) && isnan(nonstd::cx::fmod(f32__f32_n_largest,  1.0f)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_n_largest, -1.0f) == nonstd::cx::fmod(f32__f32_n_largest, -1.0f) ) || ( isnan(std::fmod(f32__f32_n_largest, -1.0f)) && isnan(nonstd::cx::fmod(f32__f32_n_largest, -1.0f)) ) ));
        REQUIRE(( ( std::fmod( 1.0f, f32__f32_p_largest) == nonstd::cx::fmod( 1.0f, f32__f32_p_largest) ) || ( isnan(std::fmod( 1.0f, f32__f32_p_largest)) && isnan(nonstd::cx::fmod( 1.0f, f32__f32_p_largest)) ) ));
        REQUIRE(( ( std::fmod(-1.0f, f32__f32_p_largest) == nonstd::cx::fmod(-1.0f, f32__f32_p_largest) ) || ( isnan(std::fmod(-1.0f, f32__f32_p_largest)) && isnan(nonstd::cx::fmod(-1.0f, f32__f32_p_largest)) ) ));
        REQUIRE(( ( std::fmod( 1.0f, f32__f32_n_largest) == nonstd::cx::fmod( 1.0f, f32__f32_n_largest) ) || ( isnan(std::fmod( 1.0f, f32__f32_n_largest)) && isnan(nonstd::cx::fmod( 1.0f, f32__f32_n_largest)) ) ));
        REQUIRE(( ( std::fmod(-1.0f, f32__f32_n_largest) == nonstd::cx::fmod(-1.0f, f32__f32_n_largest) ) || ( isnan(std::fmod(-1.0f, f32__f32_n_largest)) && isnan(nonstd::cx::fmod(-1.0f, f32__f32_n_largest)) ) ));


        REQUIRE(( ( std::fmod(f64__f64_p_smallest,  1.0) == nonstd::cx::fmod(f64__f64_p_smallest,  1.0) ) || ( isnan(std::fmod(f64__f64_p_smallest,  1.0)) && isnan(nonstd::cx::fmod(f64__f64_p_smallest,  1.0)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_p_smallest, -1.0) == nonstd::cx::fmod(f64__f64_p_smallest, -1.0) ) || ( isnan(std::fmod(f64__f64_p_smallest, -1.0)) && isnan(nonstd::cx::fmod(f64__f64_p_smallest, -1.0)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_n_smallest,  1.0) == nonstd::cx::fmod(f64__f64_n_smallest,  1.0) ) || ( isnan(std::fmod(f64__f64_n_smallest,  1.0)) && isnan(nonstd::cx::fmod(f64__f64_n_smallest,  1.0)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_n_smallest, -1.0) == nonstd::cx::fmod(f64__f64_n_smallest, -1.0) ) || ( isnan(std::fmod(f64__f64_n_smallest, -1.0)) && isnan(nonstd::cx::fmod(f64__f64_n_smallest, -1.0)) ) ));
        REQUIRE(( ( std::fmod( 1.0, f64__f64_p_smallest) == nonstd::cx::fmod( 1.0, f64__f64_p_smallest) ) || ( isnan(std::fmod( 1.0, f64__f64_p_smallest)) && isnan(nonstd::cx::fmod( 1.0, f64__f64_p_smallest)) ) ));
        REQUIRE(( ( std::fmod(-1.0, f64__f64_p_smallest) == nonstd::cx::fmod(-1.0, f64__f64_p_smallest) ) || ( isnan(std::fmod(-1.0, f64__f64_p_smallest)) && isnan(nonstd::cx::fmod(-1.0, f64__f64_p_smallest)) ) ));
        REQUIRE(( ( std::fmod( 1.0, f64__f64_n_smallest) == nonstd::cx::fmod( 1.0, f64__f64_n_smallest) ) || ( isnan(std::fmod( 1.0, f64__f64_n_smallest)) && isnan(nonstd::cx::fmod( 1.0, f64__f64_n_smallest)) ) ));
        REQUIRE(( ( std::fmod(-1.0, f64__f64_n_smallest) == nonstd::cx::fmod(-1.0, f64__f64_n_smallest) ) || ( isnan(std::fmod(-1.0, f64__f64_n_smallest)) && isnan(nonstd::cx::fmod(-1.0, f64__f64_n_smallest)) ) ));

        REQUIRE(( ( std::fmod(f64__f64_p_largest,  1.0) == nonstd::cx::fmod(f64__f64_p_largest,  1.0) ) || ( isnan(std::fmod(f64__f64_p_largest,  1.0)) && isnan(nonstd::cx::fmod(f64__f64_p_largest,  1.0)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_p_largest, -1.0) == nonstd::cx::fmod(f64__f64_p_largest, -1.0) ) || ( isnan(std::fmod(f64__f64_p_largest, -1.0)) && isnan(nonstd::cx::fmod(f64__f64_p_largest, -1.0)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_n_largest,  1.0) == nonstd::cx::fmod(f64__f64_n_largest,  1.0) ) || ( isnan(std::fmod(f64__f64_n_largest,  1.0)) && isnan(nonstd::cx::fmod(f64__f64_n_largest,  1.0)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_n_largest, -1.0) == nonstd::cx::fmod(f64__f64_n_largest, -1.0) ) || ( isnan(std::fmod(f64__f64_n_largest, -1.0)) && isnan(nonstd::cx::fmod(f64__f64_n_largest, -1.0)) ) ));
        REQUIRE(( ( std::fmod( 1.0, f64__f64_p_largest) == nonstd::cx::fmod( 1.0, f64__f64_p_largest) ) || ( isnan(std::fmod( 1.0, f64__f64_p_largest)) && isnan(nonstd::cx::fmod( 1.0, f64__f64_p_largest)) ) ));
        REQUIRE(( ( std::fmod(-1.0, f64__f64_p_largest) == nonstd::cx::fmod(-1.0, f64__f64_p_largest) ) || ( isnan(std::fmod(-1.0, f64__f64_p_largest)) && isnan(nonstd::cx::fmod(-1.0, f64__f64_p_largest)) ) ));
        REQUIRE(( ( std::fmod( 1.0, f64__f64_n_largest) == nonstd::cx::fmod( 1.0, f64__f64_n_largest) ) || ( isnan(std::fmod( 1.0, f64__f64_n_largest)) && isnan(nonstd::cx::fmod( 1.0, f64__f64_n_largest)) ) ));
        REQUIRE(( ( std::fmod(-1.0, f64__f64_n_largest) == nonstd::cx::fmod(-1.0, f64__f64_n_largest) ) || ( isnan(std::fmod(-1.0, f64__f64_n_largest)) && isnan(nonstd::cx::fmod(-1.0, f64__f64_n_largest)) ) ));


        REQUIRE(( ( std::fmod(f_long__f_long_p_smallest,  1.0l) == nonstd::cx::fmod(f_long__f_long_p_smallest,  1.0l) ) || ( isnan(std::fmod(f_long__f_long_p_smallest,  1.0l)) && isnan(nonstd::cx::fmod(f_long__f_long_p_smallest,  1.0l)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_p_smallest, -1.0l) == nonstd::cx::fmod(f_long__f_long_p_smallest, -1.0l) ) || ( isnan(std::fmod(f_long__f_long_p_smallest, -1.0l)) && isnan(nonstd::cx::fmod(f_long__f_long_p_smallest, -1.0l)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_n_smallest,  1.0l) == nonstd::cx::fmod(f_long__f_long_n_smallest,  1.0l) ) || ( isnan(std::fmod(f_long__f_long_n_smallest,  1.0l)) && isnan(nonstd::cx::fmod(f_long__f_long_n_smallest,  1.0l)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_n_smallest, -1.0l) == nonstd::cx::fmod(f_long__f_long_n_smallest, -1.0l) ) || ( isnan(std::fmod(f_long__f_long_n_smallest, -1.0l)) && isnan(nonstd::cx::fmod(f_long__f_long_n_smallest, -1.0l)) ) ));
        REQUIRE(( ( std::fmod( 1.0l, f_long__f_long_p_smallest) == nonstd::cx::fmod( 1.0l, f_long__f_long_p_smallest) ) || ( isnan(std::fmod( 1.0l, f_long__f_long_p_smallest)) && isnan(nonstd::cx::fmod( 1.0l, f_long__f_long_p_smallest)) ) ));
        REQUIRE(( ( std::fmod(-1.0l, f_long__f_long_p_smallest) == nonstd::cx::fmod(-1.0l, f_long__f_long_p_smallest) ) || ( isnan(std::fmod(-1.0l, f_long__f_long_p_smallest)) && isnan(nonstd::cx::fmod(-1.0l, f_long__f_long_p_smallest)) ) ));
        REQUIRE(( ( std::fmod( 1.0l, f_long__f_long_n_smallest) == nonstd::cx::fmod( 1.0l, f_long__f_long_n_smallest) ) || ( isnan(std::fmod( 1.0l, f_long__f_long_n_smallest)) && isnan(nonstd::cx::fmod( 1.0l, f_long__f_long_n_smallest)) ) ));
        REQUIRE(( ( std::fmod(-1.0l, f_long__f_long_n_smallest) == nonstd::cx::fmod(-1.0l, f_long__f_long_n_smallest) ) || ( isnan(std::fmod(-1.0l, f_long__f_long_n_smallest)) && isnan(nonstd::cx::fmod(-1.0l, f_long__f_long_n_smallest)) ) ));

        REQUIRE(( ( std::fmod(f_long__f_long_p_largest,  1.0l) == nonstd::cx::fmod(f_long__f_long_p_largest,  1.0l) ) || ( isnan(std::fmod(f_long__f_long_p_largest,  1.0l)) && isnan(nonstd::cx::fmod(f_long__f_long_p_largest,  1.0l)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_p_largest, -1.0l) == nonstd::cx::fmod(f_long__f_long_p_largest, -1.0l) ) || ( isnan(std::fmod(f_long__f_long_p_largest, -1.0l)) && isnan(nonstd::cx::fmod(f_long__f_long_p_largest, -1.0l)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_n_largest,  1.0l) == nonstd::cx::fmod(f_long__f_long_n_largest,  1.0l) ) || ( isnan(std::fmod(f_long__f_long_n_largest,  1.0l)) && isnan(nonstd::cx::fmod(f_long__f_long_n_largest,  1.0l)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_n_largest, -1.0l) == nonstd::cx::fmod(f_long__f_long_n_largest, -1.0l) ) || ( isnan(std::fmod(f_long__f_long_n_largest, -1.0l)) && isnan(nonstd::cx::fmod(f_long__f_long_n_largest, -1.0l)) ) ));
        REQUIRE(( ( std::fmod( 1.0l, f_long__f_long_p_largest) == nonstd::cx::fmod( 1.0l, f_long__f_long_p_largest) ) || ( isnan(std::fmod( 1.0l, f_long__f_long_p_largest)) && isnan(nonstd::cx::fmod( 1.0l, f_long__f_long_p_largest)) ) ));
        REQUIRE(( ( std::fmod(-1.0l, f_long__f_long_p_largest) == nonstd::cx::fmod(-1.0l, f_long__f_long_p_largest) ) || ( isnan(std::fmod(-1.0l, f_long__f_long_p_largest)) && isnan(nonstd::cx::fmod(-1.0l, f_long__f_long_p_largest)) ) ));
        REQUIRE(( ( std::fmod( 1.0l, f_long__f_long_n_largest) == nonstd::cx::fmod( 1.0l, f_long__f_long_n_largest) ) || ( isnan(std::fmod( 1.0l, f_long__f_long_n_largest)) && isnan(nonstd::cx::fmod( 1.0l, f_long__f_long_n_largest)) ) ));
        REQUIRE(( ( std::fmod(-1.0l, f_long__f_long_n_largest) == nonstd::cx::fmod(-1.0l, f_long__f_long_n_largest) ) || ( isnan(std::fmod(-1.0l, f_long__f_long_n_largest)) && isnan(nonstd::cx::fmod(-1.0l, f_long__f_long_n_largest)) ) ));

        // Test INF
        REQUIRE(( ( std::fmod(f32__f32_p_smallest, f32__f32_p_inf) == nonstd::cx::fmod(f32__f32_p_smallest, f32__f32_p_inf) ) || ( isnan(std::fmod(f32__f32_p_smallest, f32__f32_p_inf)) && isnan(nonstd::cx::fmod(f32__f32_p_smallest, f32__f32_p_inf)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_p_smallest, f32__f32_n_inf) == nonstd::cx::fmod(f32__f32_p_smallest, f32__f32_n_inf) ) || ( isnan(std::fmod(f32__f32_p_smallest, f32__f32_n_inf)) && isnan(nonstd::cx::fmod(f32__f32_p_smallest, f32__f32_n_inf)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_n_smallest, f32__f32_p_inf) == nonstd::cx::fmod(f32__f32_n_smallest, f32__f32_p_inf) ) || ( isnan(std::fmod(f32__f32_n_smallest, f32__f32_p_inf)) && isnan(nonstd::cx::fmod(f32__f32_n_smallest, f32__f32_p_inf)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_n_smallest, f32__f32_n_inf) == nonstd::cx::fmod(f32__f32_n_smallest, f32__f32_n_inf) ) || ( isnan(std::fmod(f32__f32_n_smallest, f32__f32_n_inf)) && isnan(nonstd::cx::fmod(f32__f32_n_smallest, f32__f32_n_inf)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_p_inf, f32__f32_p_smallest) == nonstd::cx::fmod(f32__f32_p_inf, f32__f32_p_smallest) ) || ( isnan(std::fmod(f32__f32_p_inf, f32__f32_p_smallest)) && isnan(nonstd::cx::fmod(f32__f32_p_inf, f32__f32_p_smallest)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_n_inf, f32__f32_p_smallest) == nonstd::cx::fmod(f32__f32_n_inf, f32__f32_p_smallest) ) || ( isnan(std::fmod(f32__f32_n_inf, f32__f32_p_smallest)) && isnan(nonstd::cx::fmod(f32__f32_n_inf, f32__f32_p_smallest)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_p_inf, f32__f32_n_smallest) == nonstd::cx::fmod(f32__f32_p_inf, f32__f32_n_smallest) ) || ( isnan(std::fmod(f32__f32_p_inf, f32__f32_n_smallest)) && isnan(nonstd::cx::fmod(f32__f32_p_inf, f32__f32_n_smallest)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_n_inf, f32__f32_n_smallest) == nonstd::cx::fmod(f32__f32_n_inf, f32__f32_n_smallest) ) || ( isnan(std::fmod(f32__f32_n_inf, f32__f32_n_smallest)) && isnan(nonstd::cx::fmod(f32__f32_n_inf, f32__f32_n_smallest)) ) ));

        REQUIRE(( ( std::fmod(f32__f32_p_largest, f32__f32_p_inf) == nonstd::cx::fmod(f32__f32_p_largest, f32__f32_p_inf) ) || ( isnan(std::fmod(f32__f32_p_largest, f32__f32_p_inf)) && isnan(nonstd::cx::fmod(f32__f32_p_largest, f32__f32_p_inf)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_p_largest, f32__f32_n_inf) == nonstd::cx::fmod(f32__f32_p_largest, f32__f32_n_inf) ) || ( isnan(std::fmod(f32__f32_p_largest, f32__f32_n_inf)) && isnan(nonstd::cx::fmod(f32__f32_p_largest, f32__f32_n_inf)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_n_largest, f32__f32_p_inf) == nonstd::cx::fmod(f32__f32_n_largest, f32__f32_p_inf) ) || ( isnan(std::fmod(f32__f32_n_largest, f32__f32_p_inf)) && isnan(nonstd::cx::fmod(f32__f32_n_largest, f32__f32_p_inf)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_n_largest, f32__f32_n_inf) == nonstd::cx::fmod(f32__f32_n_largest, f32__f32_n_inf) ) || ( isnan(std::fmod(f32__f32_n_largest, f32__f32_n_inf)) && isnan(nonstd::cx::fmod(f32__f32_n_largest, f32__f32_n_inf)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_p_inf, f32__f32_p_largest) == nonstd::cx::fmod(f32__f32_p_inf, f32__f32_p_largest) ) || ( isnan(std::fmod(f32__f32_p_inf, f32__f32_p_largest)) && isnan(nonstd::cx::fmod(f32__f32_p_inf, f32__f32_p_largest)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_n_inf, f32__f32_p_largest) == nonstd::cx::fmod(f32__f32_n_inf, f32__f32_p_largest) ) || ( isnan(std::fmod(f32__f32_n_inf, f32__f32_p_largest)) && isnan(nonstd::cx::fmod(f32__f32_n_inf, f32__f32_p_largest)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_p_inf, f32__f32_n_largest) == nonstd::cx::fmod(f32__f32_p_inf, f32__f32_n_largest) ) || ( isnan(std::fmod(f32__f32_p_inf, f32__f32_n_largest)) && isnan(nonstd::cx::fmod(f32__f32_p_inf, f32__f32_n_largest)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_n_inf, f32__f32_n_largest) == nonstd::cx::fmod(f32__f32_n_inf, f32__f32_n_largest) ) || ( isnan(std::fmod(f32__f32_n_inf, f32__f32_n_largest)) && isnan(nonstd::cx::fmod(f32__f32_n_inf, f32__f32_n_largest)) ) ));


        REQUIRE(( ( std::fmod(f64__f64_p_smallest, f64__f64_p_inf) == nonstd::cx::fmod(f64__f64_p_smallest, f64__f64_p_inf) ) || ( isnan(std::fmod(f64__f64_p_smallest, f64__f64_p_inf)) && isnan(nonstd::cx::fmod(f64__f64_p_smallest, f64__f64_p_inf)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_p_smallest, f64__f64_n_inf) == nonstd::cx::fmod(f64__f64_p_smallest, f64__f64_n_inf) ) || ( isnan(std::fmod(f64__f64_p_smallest, f64__f64_n_inf)) && isnan(nonstd::cx::fmod(f64__f64_p_smallest, f64__f64_n_inf)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_n_smallest, f64__f64_p_inf) == nonstd::cx::fmod(f64__f64_n_smallest, f64__f64_p_inf) ) || ( isnan(std::fmod(f64__f64_n_smallest, f64__f64_p_inf)) && isnan(nonstd::cx::fmod(f64__f64_n_smallest, f64__f64_p_inf)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_n_smallest, f64__f64_n_inf) == nonstd::cx::fmod(f64__f64_n_smallest, f64__f64_n_inf) ) || ( isnan(std::fmod(f64__f64_n_smallest, f64__f64_n_inf)) && isnan(nonstd::cx::fmod(f64__f64_n_smallest, f64__f64_n_inf)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_p_inf, f64__f64_p_smallest) == nonstd::cx::fmod(f64__f64_p_inf, f64__f64_p_smallest) ) || ( isnan(std::fmod(f64__f64_p_inf, f64__f64_p_smallest)) && isnan(nonstd::cx::fmod(f64__f64_p_inf, f64__f64_p_smallest)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_n_inf, f64__f64_p_smallest) == nonstd::cx::fmod(f64__f64_n_inf, f64__f64_p_smallest) ) || ( isnan(std::fmod(f64__f64_n_inf, f64__f64_p_smallest)) && isnan(nonstd::cx::fmod(f64__f64_n_inf, f64__f64_p_smallest)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_p_inf, f64__f64_n_smallest) == nonstd::cx::fmod(f64__f64_p_inf, f64__f64_n_smallest) ) || ( isnan(std::fmod(f64__f64_p_inf, f64__f64_n_smallest)) && isnan(nonstd::cx::fmod(f64__f64_p_inf, f64__f64_n_smallest)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_n_inf, f64__f64_n_smallest) == nonstd::cx::fmod(f64__f64_n_inf, f64__f64_n_smallest) ) || ( isnan(std::fmod(f64__f64_n_inf, f64__f64_n_smallest)) && isnan(nonstd::cx::fmod(f64__f64_n_inf, f64__f64_n_smallest)) ) ));

        REQUIRE(( ( std::fmod(f64__f64_p_largest, f64__f64_p_inf) == nonstd::cx::fmod(f64__f64_p_largest, f64__f64_p_inf) ) || ( isnan(std::fmod(f64__f64_p_largest, f64__f64_p_inf)) && isnan(nonstd::cx::fmod(f64__f64_p_largest, f64__f64_p_inf)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_p_largest, f64__f64_n_inf) == nonstd::cx::fmod(f64__f64_p_largest, f64__f64_n_inf) ) || ( isnan(std::fmod(f64__f64_p_largest, f64__f64_n_inf)) && isnan(nonstd::cx::fmod(f64__f64_p_largest, f64__f64_n_inf)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_n_largest, f64__f64_p_inf) == nonstd::cx::fmod(f64__f64_n_largest, f64__f64_p_inf) ) || ( isnan(std::fmod(f64__f64_n_largest, f64__f64_p_inf)) && isnan(nonstd::cx::fmod(f64__f64_n_largest, f64__f64_p_inf)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_n_largest, f64__f64_n_inf) == nonstd::cx::fmod(f64__f64_n_largest, f64__f64_n_inf) ) || ( isnan(std::fmod(f64__f64_n_largest, f64__f64_n_inf)) && isnan(nonstd::cx::fmod(f64__f64_n_largest, f64__f64_n_inf)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_p_inf, f64__f64_p_largest) == nonstd::cx::fmod(f64__f64_p_inf, f64__f64_p_largest) ) || ( isnan(std::fmod(f64__f64_p_inf, f64__f64_p_largest)) && isnan(nonstd::cx::fmod(f64__f64_p_inf, f64__f64_p_largest)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_n_inf, f64__f64_p_largest) == nonstd::cx::fmod(f64__f64_n_inf, f64__f64_p_largest) ) || ( isnan(std::fmod(f64__f64_n_inf, f64__f64_p_largest)) && isnan(nonstd::cx::fmod(f64__f64_n_inf, f64__f64_p_largest)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_p_inf, f64__f64_n_largest) == nonstd::cx::fmod(f64__f64_p_inf, f64__f64_n_largest) ) || ( isnan(std::fmod(f64__f64_p_inf, f64__f64_n_largest)) && isnan(nonstd::cx::fmod(f64__f64_p_inf, f64__f64_n_largest)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_n_inf, f64__f64_n_largest) == nonstd::cx::fmod(f64__f64_n_inf, f64__f64_n_largest) ) || ( isnan(std::fmod(f64__f64_n_inf, f64__f64_n_largest)) && isnan(nonstd::cx::fmod(f64__f64_n_inf, f64__f64_n_largest)) ) ));


        REQUIRE(( ( std::fmod(f_long__f_long_p_smallest, f_long__f_long_p_inf) == nonstd::cx::fmod(f_long__f_long_p_smallest, f_long__f_long_p_inf) ) || ( isnan(std::fmod(f_long__f_long_p_smallest, f_long__f_long_p_inf)) && isnan(nonstd::cx::fmod(f_long__f_long_p_smallest, f_long__f_long_p_inf)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_p_smallest, f_long__f_long_n_inf) == nonstd::cx::fmod(f_long__f_long_p_smallest, f_long__f_long_n_inf) ) || ( isnan(std::fmod(f_long__f_long_p_smallest, f_long__f_long_n_inf)) && isnan(nonstd::cx::fmod(f_long__f_long_p_smallest, f_long__f_long_n_inf)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_n_smallest, f_long__f_long_p_inf) == nonstd::cx::fmod(f_long__f_long_n_smallest, f_long__f_long_p_inf) ) || ( isnan(std::fmod(f_long__f_long_n_smallest, f_long__f_long_p_inf)) && isnan(nonstd::cx::fmod(f_long__f_long_n_smallest, f_long__f_long_p_inf)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_n_smallest, f_long__f_long_n_inf) == nonstd::cx::fmod(f_long__f_long_n_smallest, f_long__f_long_n_inf) ) || ( isnan(std::fmod(f_long__f_long_n_smallest, f_long__f_long_n_inf)) && isnan(nonstd::cx::fmod(f_long__f_long_n_smallest, f_long__f_long_n_inf)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_p_inf, f_long__f_long_p_smallest) == nonstd::cx::fmod(f_long__f_long_p_inf, f_long__f_long_p_smallest) ) || ( isnan(std::fmod(f_long__f_long_p_inf, f_long__f_long_p_smallest)) && isnan(nonstd::cx::fmod(f_long__f_long_p_inf, f_long__f_long_p_smallest)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_n_inf, f_long__f_long_p_smallest) == nonstd::cx::fmod(f_long__f_long_n_inf, f_long__f_long_p_smallest) ) || ( isnan(std::fmod(f_long__f_long_n_inf, f_long__f_long_p_smallest)) && isnan(nonstd::cx::fmod(f_long__f_long_n_inf, f_long__f_long_p_smallest)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_p_inf, f_long__f_long_n_smallest) == nonstd::cx::fmod(f_long__f_long_p_inf, f_long__f_long_n_smallest) ) || ( isnan(std::fmod(f_long__f_long_p_inf, f_long__f_long_n_smallest)) && isnan(nonstd::cx::fmod(f_long__f_long_p_inf, f_long__f_long_n_smallest)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_n_inf, f_long__f_long_n_smallest) == nonstd::cx::fmod(f_long__f_long_n_inf, f_long__f_long_n_smallest) ) || ( isnan(std::fmod(f_long__f_long_n_inf, f_long__f_long_n_smallest)) && isnan(nonstd::cx::fmod(f_long__f_long_n_inf, f_long__f_long_n_smallest)) ) ));

        REQUIRE(( ( std::fmod(f_long__f_long_p_largest, f_long__f_long_p_inf) == nonstd::cx::fmod(f_long__f_long_p_largest, f_long__f_long_p_inf) ) || ( isnan(std::fmod(f_long__f_long_p_largest, f_long__f_long_p_inf)) && isnan(nonstd::cx::fmod(f_long__f_long_p_largest, f_long__f_long_p_inf)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_p_largest, f_long__f_long_n_inf) == nonstd::cx::fmod(f_long__f_long_p_largest, f_long__f_long_n_inf) ) || ( isnan(std::fmod(f_long__f_long_p_largest, f_long__f_long_n_inf)) && isnan(nonstd::cx::fmod(f_long__f_long_p_largest, f_long__f_long_n_inf)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_n_largest, f_long__f_long_p_inf) == nonstd::cx::fmod(f_long__f_long_n_largest, f_long__f_long_p_inf) ) || ( isnan(std::fmod(f_long__f_long_n_largest, f_long__f_long_p_inf)) && isnan(nonstd::cx::fmod(f_long__f_long_n_largest, f_long__f_long_p_inf)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_n_largest, f_long__f_long_n_inf) == nonstd::cx::fmod(f_long__f_long_n_largest, f_long__f_long_n_inf) ) || ( isnan(std::fmod(f_long__f_long_n_largest, f_long__f_long_n_inf)) && isnan(nonstd::cx::fmod(f_long__f_long_n_largest, f_long__f_long_n_inf)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_p_inf, f_long__f_long_p_largest) == nonstd::cx::fmod(f_long__f_long_p_inf, f_long__f_long_p_largest) ) || ( isnan(std::fmod(f_long__f_long_p_inf, f_long__f_long_p_largest)) && isnan(nonstd::cx::fmod(f_long__f_long_p_inf, f_long__f_long_p_largest)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_n_inf, f_long__f_long_p_largest) == nonstd::cx::fmod(f_long__f_long_n_inf, f_long__f_long_p_largest) ) || ( isnan(std::fmod(f_long__f_long_n_inf, f_long__f_long_p_largest)) && isnan(nonstd::cx::fmod(f_long__f_long_n_inf, f_long__f_long_p_largest)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_p_inf, f_long__f_long_n_largest) == nonstd::cx::fmod(f_long__f_long_p_inf, f_long__f_long_n_largest) ) || ( isnan(std::fmod(f_long__f_long_p_inf, f_long__f_long_n_largest)) && isnan(nonstd::cx::fmod(f_long__f_long_p_inf, f_long__f_long_n_largest)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_n_inf, f_long__f_long_n_largest) == nonstd::cx::fmod(f_long__f_long_n_inf, f_long__f_long_n_largest) ) || ( isnan(std::fmod(f_long__f_long_n_inf, f_long__f_long_n_largest)) && isnan(nonstd::cx::fmod(f_long__f_long_n_inf, f_long__f_long_n_largest)) ) ));


        // Test NaN propagation
        REQUIRE(( ( std::fmod(f32__f32_nan,  1.0f) == nonstd::cx::fmod(f32__f32_nan,  1.0f) ) || ( isnan(std::fmod(f32__f32_nan,  1.0f)) && isnan(nonstd::cx::fmod(f32__f32_nan,  1.0f)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_nan, -1.0f) == nonstd::cx::fmod(f32__f32_nan, -1.0f) ) || ( isnan(std::fmod(f32__f32_nan, -1.0f)) && isnan(nonstd::cx::fmod(f32__f32_nan, -1.0f)) ) ));
        REQUIRE(( ( std::fmod( 1.0f, f32__f32_nan) == nonstd::cx::fmod( 1.0f, f32__f32_nan) ) || ( isnan(std::fmod( 1.0f, f32__f32_nan)) && isnan(nonstd::cx::fmod( 1.0f, f32__f32_nan)) ) ));
        REQUIRE(( ( std::fmod(-1.0f, f32__f32_nan) == nonstd::cx::fmod(-1.0f, f32__f32_nan) ) || ( isnan(std::fmod(-1.0f, f32__f32_nan)) && isnan(nonstd::cx::fmod(-1.0f, f32__f32_nan)) ) ));

        REQUIRE(( ( std::fmod(f32__f32_nan, f32__f32_n_inf) == nonstd::cx::fmod(f32__f32_nan, f32__f32_n_inf) ) || ( isnan(std::fmod(f32__f32_nan, f32__f32_n_inf)) && isnan(nonstd::cx::fmod(f32__f32_nan, f32__f32_n_inf)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_nan, f32__f32_p_inf) == nonstd::cx::fmod(f32__f32_nan, f32__f32_p_inf) ) || ( isnan(std::fmod(f32__f32_nan, f32__f32_p_inf)) && isnan(nonstd::cx::fmod(f32__f32_nan, f32__f32_p_inf)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_n_inf, f32__f32_nan) == nonstd::cx::fmod(f32__f32_n_inf, f32__f32_nan) ) || ( isnan(std::fmod(f32__f32_n_inf, f32__f32_nan)) && isnan(nonstd::cx::fmod(f32__f32_n_inf, f32__f32_nan)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_p_inf, f32__f32_nan) == nonstd::cx::fmod(f32__f32_p_inf, f32__f32_nan) ) || ( isnan(std::fmod(f32__f32_p_inf, f32__f32_nan)) && isnan(nonstd::cx::fmod(f32__f32_p_inf, f32__f32_nan)) ) ));

        // For this function, it's worth doing some additional testing in ranges
        // we might see in the real world. Or just random ranges. Pulled from a
        // random number generator. Either one.
        std::random_device rd;
        std::mt19937 re(rd());

        std::uniform_real_distribution<float> rnd_float_range(
            -std::numeric_limits<float>::max(),
             std::numeric_limits<float>::max());
        std::uniform_real_distribution<double> rnd_double_range(
            -std::numeric_limits<double>::max(),
             std::numeric_limits<double>::max());
        std::uniform_real_distribution<long double> rnd_long_double_range(
            -std::numeric_limits<long double>::max(),
             std::numeric_limits<long double>::max());

        constexpr int iterations = 10000;
        for (int i = 0; i < iterations; i++) {
            float x = rnd_float_range(re);
            float y = rnd_float_range(re);
            REQUIRE(( ( std::fmod(x, y)  == nonstd::cx::fmod(x, y) ) || ( isnan(std::fmod(x, y)) && isnan(nonstd::cx::fmod(x, y)) ) ));
        }
        for (int i = 0; i < iterations; i++) {
            double x = rnd_double_range(re);
            double y = rnd_double_range(re);
            REQUIRE(( ( std::fmod(x, y)  == nonstd::cx::fmod(x, y) ) || ( isnan(std::fmod(x, y)) && isnan(nonstd::cx::fmod(x, y)) ) ));
        }
        for (int i = 0; i < iterations; i++) {
            long double x = rnd_long_double_range(re);
            long double y = rnd_long_double_range(re);
            REQUIRE(( ( std::fmod(x, y)  == nonstd::cx::fmod(x, y) ) || ( isnan(std::fmod(x, y)) && isnan(nonstd::cx::fmod(x, y)) ) ));
        }
    }

}

} /* namespace math */
} /* namespace nonstd_test */
