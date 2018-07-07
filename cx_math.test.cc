/** Constexpr Math Utility Smoke Tests
 *  =================================
 *  The target is to maintain parity with the functions defined in <cmath>.
 */

#include <nonstd/cx_math.h>
#include <platform/testrunner/testrunner.h>

#include <random>
#include <cmath>


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
    constexpr f32 f32__positive                     =  1.5f;                                         UNUSED(f32__positive);
    constexpr f32 f32__zero                         =  0.0f;                                         UNUSED(f32__zero);
    constexpr f32 f32__negative                     = -1.5f;                                         UNUSED(f32__negative);
    constexpr f32 f32__f32_smallest_p               =  std::numeric_limits<f32>::min();              UNUSED(f32__f32_smallest_p);
    constexpr f32 f32__f32_smallest_n               = -std::numeric_limits<f32>::min();              UNUSED(f32__f32_smallest_n);
    constexpr f32 f32__f32_highest                  =  std::numeric_limits<f32>::max();              UNUSED(f32__f32_highest);
    constexpr f32 f32__f32_lowest                   =  std::numeric_limits<f32>::lowest();           UNUSED(f32__f32_lowest);
    constexpr f32 f32__f64_smallest_p               =  std::numeric_limits<f64>::min();              UNUSED(f32__f64_smallest_p);
    constexpr f32 f32__f64_smallest_n               = -std::numeric_limits<f64>::min();              UNUSED(f32__f64_smallest_n);
//  constexpr f32 f32__f64_highest                  =  std::numeric_limits<f64>::max();              UNUSED(f32__f64_highest);
//  constexpr f32 f32__f64_lowest                   =  std::numeric_limits<f64>::lowest();           UNUSED(f32__f64_lowest);
    constexpr f32 f32__f_long_smallest_p            =  std::numeric_limits<f_long>::min();           UNUSED(f32__f_long_smallest_p);
    constexpr f32 f32__f_long_smallest_n            = -std::numeric_limits<f_long>::min();           UNUSED(f32__f_long_smallest_n);
//  constexpr f32 f32__f_long_highest               =  std::numeric_limits<f_long>::max();           UNUSED(f32__f_long_highest);
//  constexpr f32 f32__f_long_lowest                =  std::numeric_limits<f_long>::lowest();        UNUSED(f32__f_long_lowest);
    constexpr f32 f32__f32_nan                      =  std::numeric_limits<f32>::quiet_NaN();        UNUSED(f32__f32_nan);
    constexpr f32 f32__f64_nan                      =  std::numeric_limits<f64>::quiet_NaN();        UNUSED(f32__f64_nan);
    constexpr f32 f32__f_long_nan                   =  std::numeric_limits<f_long>::quiet_NaN();     UNUSED(f32__f_long_nan);
    constexpr f32 f32__f32_p_inf                    =  std::numeric_limits<f32>::infinity();         UNUSED(f32__f32_p_inf);
    constexpr f32 f32__f32_n_inf                    = -std::numeric_limits<f32>::infinity();         UNUSED(f32__f32_n_inf);
    constexpr f32 f32__f64_p_inf                    =  std::numeric_limits<f64>::infinity();         UNUSED(f32__f64_p_inf);
    constexpr f32 f32__f64_n_inf                    = -std::numeric_limits<f64>::infinity();         UNUSED(f32__f64_n_inf);
    constexpr f32 f32__f_long_p_inf                 =  std::numeric_limits<f_long>::infinity();      UNUSED(f32__f_long_p_inf);
    constexpr f32 f32__f_long_n_inf                 = -std::numeric_limits<f_long>::infinity();      UNUSED(f32__f_long_n_inf);
              f32 f32__f32_less_smallest_p          =  std::numeric_limits<f32>::min() / 2.0f;       UNUSED(f32__f32_less_smallest_p);
              f32 f32__f32_less_smallest_n          = -std::numeric_limits<f32>::min() / 2.0f;       UNUSED(f32__f32_less_smallest_n);
              f32 f32__f32_more_highest             =  std::numeric_limits<f32>::max() * 2.0f;       UNUSED(f32__f32_more_highest);
              f32 f32__f32_more_lowest              =  std::numeric_limits<f32>::lowest() * 2.0f;    UNUSED(f32__f32_more_lowest);
              f32 f32__f64_less_smallest_p          =  std::numeric_limits<f64>::min() / 2.0f;       UNUSED(f32__f64_less_smallest_p);
              f32 f32__f64_less_smallest_n          = -std::numeric_limits<f64>::min() / 2.0f;       UNUSED(f32__f64_less_smallest_n);
              f32 f32__f64_more_highest             =  std::numeric_limits<f64>::max() * 2.0f;       UNUSED(f32__f64_more_highest);
              f32 f32__f64_more_lowest              =  std::numeric_limits<f64>::lowest() * 2.0f;    UNUSED(f32__f64_more_lowest);
              f32 f32__f_long_less_smallest_p       =  std::numeric_limits<f_long>::min() / 2.0f;    UNUSED(f32__f_long_less_smallest_p);
              f32 f32__f_long_less_smallest_n       = -std::numeric_limits<f_long>::min() / 2.0f;    UNUSED(f32__f_long_less_smallest_n);
              f32 f32__f_long_more_highest          =  std::numeric_limits<f_long>::max() * 2.0f;    UNUSED(f32__f_long_more_highest);
              f32 f32__f_long_more_lowest           =  std::numeric_limits<f_long>::lowest() * 2.0f; UNUSED(f32__f_long_more_lowest);

    constexpr f64 f64__positive                     =  1.5;                                         UNUSED(f64__positive);
    constexpr f64 f64__zero                         =  0.0;                                         UNUSED(f64__zero);
    constexpr f64 f64__negative                     = -1.5;                                         UNUSED(f64__negative);
    constexpr f64 f64__f32_smallest_p               =  std::numeric_limits<f32>::min();             UNUSED(f64__f32_smallest_p);
    constexpr f64 f64__f32_smallest_n               = -std::numeric_limits<f32>::min();             UNUSED(f64__f32_smallest_n);
    constexpr f64 f64__f32_highest                  =  std::numeric_limits<f32>::max();             UNUSED(f64__f32_highest);
    constexpr f64 f64__f32_lowest                   =  std::numeric_limits<f32>::lowest();          UNUSED(f64__f32_lowest);
    constexpr f64 f64__f64_smallest_p               =  std::numeric_limits<f64>::min();             UNUSED(f64__f64_smallest_p);
    constexpr f64 f64__f64_smallest_n               = -std::numeric_limits<f64>::min();             UNUSED(f64__f64_smallest_n);
    constexpr f64 f64__f64_highest                  =  std::numeric_limits<f64>::max();             UNUSED(f64__f64_highest);
    constexpr f64 f64__f64_lowest                   =  std::numeric_limits<f64>::lowest();          UNUSED(f64__f64_lowest);
    constexpr f64 f64__f_long_smallest_p            =  std::numeric_limits<f_long>::min();          UNUSED(f64__f_long_smallest_p);
    constexpr f64 f64__f_long_smallest_n            = -std::numeric_limits<f_long>::min();          UNUSED(f64__f_long_smallest_n);
//  constexpr f64 f64__f_long_highest               =  std::numeric_limits<f_long>::max();          UNUSED(f64__f_long_highest);
//  constexpr f64 f64__f_long_lowest                =  std::numeric_limits<f_long>::lowest();       UNUSED(f64__f_long_lowest);
    constexpr f64 f64__f32_nan                      =  std::numeric_limits<f32>::quiet_NaN();       UNUSED(f64__f32_nan);
    constexpr f64 f64__f64_nan                      =  std::numeric_limits<f64>::quiet_NaN();       UNUSED(f64__f64_nan);
    constexpr f64 f64__f_long_nan                   =  std::numeric_limits<f_long>::quiet_NaN();    UNUSED(f64__f_long_nan);
    constexpr f64 f64__f32_p_inf                    =  std::numeric_limits<f32>::infinity();        UNUSED(f64__f32_p_inf);
    constexpr f64 f64__f32_n_inf                    = -std::numeric_limits<f32>::infinity();        UNUSED(f64__f32_n_inf);
    constexpr f64 f64__f64_p_inf                    =  std::numeric_limits<f64>::infinity();        UNUSED(f64__f64_p_inf);
    constexpr f64 f64__f64_n_inf                    = -std::numeric_limits<f64>::infinity();        UNUSED(f64__f64_n_inf);
    constexpr f64 f64__f_long_p_inf                 =  std::numeric_limits<f_long>::infinity();     UNUSED(f64__f_long_p_inf);
    constexpr f64 f64__f_long_n_inf                 = -std::numeric_limits<f_long>::infinity();     UNUSED(f64__f_long_n_inf);
              f64 f64__f32_less_smallest_p          =  std::numeric_limits<f32>::min() / 2.0;       UNUSED(f64__f32_less_smallest_p);
              f64 f64__f32_less_smallest_n          = -std::numeric_limits<f32>::min() / 2.0;       UNUSED(f64__f32_less_smallest_n);
              f64 f64__f32_more_highest             =  std::numeric_limits<f32>::max() * 2.0;       UNUSED(f64__f32_more_highest);
              f64 f64__f32_more_lowest              =  std::numeric_limits<f32>::lowest() * 2.0;    UNUSED(f64__f32_more_lowest);
              f64 f64__f64_less_smallest_p          =  std::numeric_limits<f64>::min() / 2.0;       UNUSED(f64__f64_less_smallest_p);
              f64 f64__f64_less_smallest_n          = -std::numeric_limits<f64>::min() / 2.0;       UNUSED(f64__f64_less_smallest_n);
              f64 f64__f64_more_highest             =  std::numeric_limits<f64>::max() * 2.0;       UNUSED(f64__f64_more_highest);
              f64 f64__f64_more_lowest              =  std::numeric_limits<f64>::lowest() * 2.0;    UNUSED(f64__f64_more_lowest);
              f64 f64__f_long_less_smallest_p       =  std::numeric_limits<f_long>::min() / 2.0;    UNUSED(f64__f_long_less_smallest_p);
              f64 f64__f_long_less_smallest_n       = -std::numeric_limits<f_long>::min() / 2.0;    UNUSED(f64__f_long_less_smallest_n);
              f64 f64__f_long_more_highest          =  std::numeric_limits<f_long>::max() * 2.0;    UNUSED(f64__f_long_more_highest);
              f64 f64__f_long_more_lowest           =  std::numeric_limits<f_long>::lowest() * 2.0; UNUSED(f64__f_long_more_lowest);

    constexpr f_long f_long__positive               =  1.5l;                                         UNUSED(f_long__positive);
    constexpr f_long f_long__zero                   =  0.0l;                                         UNUSED(f_long__zero);
    constexpr f_long f_long__negative               = -1.5l;                                         UNUSED(f_long__negative);
    constexpr f_long f_long__f32_smallest_p         =  std::numeric_limits<f32>::min();              UNUSED(f_long__f32_smallest_p);
    constexpr f_long f_long__f32_smallest_n         = -std::numeric_limits<f32>::min();              UNUSED(f_long__f32_smallest_n);
    constexpr f_long f_long__f32_highest            =  std::numeric_limits<f32>::max();              UNUSED(f_long__f32_highest);
    constexpr f_long f_long__f32_lowest             =  std::numeric_limits<f32>::lowest();           UNUSED(f_long__f32_lowest);
    constexpr f_long f_long__f64_smallest_p         =  std::numeric_limits<f64>::min();              UNUSED(f_long__f64_smallest_p);
    constexpr f_long f_long__f64_smallest_n         = -std::numeric_limits<f64>::min();              UNUSED(f_long__f64_smallest_n);
    constexpr f_long f_long__f64_highest            =  std::numeric_limits<f64>::max();              UNUSED(f_long__f64_highest);
    constexpr f_long f_long__f64_lowest             =  std::numeric_limits<f64>::lowest();           UNUSED(f_long__f64_lowest);
    constexpr f_long f_long__f_long_smallest_p      =  std::numeric_limits<f_long>::min();           UNUSED(f_long__f_long_smallest_p);
    constexpr f_long f_long__f_long_smallest_n      = -std::numeric_limits<f_long>::min();           UNUSED(f_long__f_long_smallest_n);
    constexpr f_long f_long__f_long_highest         =  std::numeric_limits<f_long>::max();           UNUSED(f_long__f_long_highest);
    constexpr f_long f_long__f_long_lowest          =  std::numeric_limits<f_long>::lowest();        UNUSED(f_long__f_long_lowest);
    constexpr f_long f_long__f32_nan                =  std::numeric_limits<f32>::quiet_NaN();        UNUSED(f_long__f32_nan);
    constexpr f_long f_long__f64_nan                =  std::numeric_limits<f64>::quiet_NaN();        UNUSED(f_long__f64_nan);
    constexpr f_long f_long__f_long_nan             =  std::numeric_limits<f_long>::quiet_NaN();     UNUSED(f_long__f_long_nan);
    constexpr f_long f_long__f32_p_inf              =  std::numeric_limits<f32>::infinity();         UNUSED(f_long__f32_p_inf);
    constexpr f_long f_long__f32_n_inf              = -std::numeric_limits<f32>::infinity();         UNUSED(f_long__f32_n_inf);
    constexpr f_long f_long__f64_p_inf              =  std::numeric_limits<f64>::infinity();         UNUSED(f_long__f64_p_inf);
    constexpr f_long f_long__f64_n_inf              = -std::numeric_limits<f64>::infinity();         UNUSED(f_long__f64_n_inf);
    constexpr f_long f_long__f_long_p_inf           =  std::numeric_limits<f_long>::infinity();      UNUSED(f_long__f_long_p_inf);
    constexpr f_long f_long__f_long_n_inf           = -std::numeric_limits<f_long>::infinity();      UNUSED(f_long__f_long_n_inf);
              f_long f_long__f32_less_smallest_p    =  std::numeric_limits<f32>::min() / 2.0l;       UNUSED(f_long__f32_less_smallest_p);
              f_long f_long__f32_less_smallest_n    = -std::numeric_limits<f32>::min() / 2.0l;       UNUSED(f_long__f32_less_smallest_n);
              f_long f_long__f32_more_highest       =  std::numeric_limits<f32>::max() * 2.0l;       UNUSED(f_long__f32_more_highest);
              f_long f_long__f32_more_lowest        =  std::numeric_limits<f32>::lowest() * 2.0l;    UNUSED(f_long__f32_more_lowest);
              f_long f_long__f64_less_smallest_p    =  std::numeric_limits<f64>::min() / 2.0l;       UNUSED(f_long__f64_less_smallest_p);
              f_long f_long__f64_less_smallest_n    = -std::numeric_limits<f64>::min() / 2.0l;       UNUSED(f_long__f64_less_smallest_n);
              f_long f_long__f64_more_highest       =  std::numeric_limits<f64>::max() * 2.0l;       UNUSED(f_long__f64_more_highest);
              f_long f_long__f64_more_lowest        =  std::numeric_limits<f64>::lowest() * 2.0l;    UNUSED(f_long__f64_more_lowest);
              f_long f_long__f_long_less_smallest_p =  std::numeric_limits<f_long>::min() / 2.0l;    UNUSED(f_long__f_long_less_smallest_p);
              f_long f_long__f_long_less_smallest_n = -std::numeric_limits<f_long>::min() / 2.0l;    UNUSED(f_long__f_long_less_smallest_n);
              f_long f_long__f_long_more_highest    =  std::numeric_limits<f_long>::max() * 2.0l;    UNUSED(f_long__f_long_more_highest);
              f_long f_long__f_long_more_lowest     =  std::numeric_limits<f_long>::lowest() * 2.0l; UNUSED(f_long__f_long_more_lowest);

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
        bool f64_and_f_long_are_identical = (sizeof(f64__f64_smallest_p) ==
                                             sizeof(f_long__f_long_smallest_p));

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

        REQUIRE((f32__f32_smallest_p == f64__f64_smallest_p)       == false);
        REQUIRE((f64__f64_smallest_p == f_long__f_long_smallest_p) == f64_and_f_long_are_identical);


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

        REQUIRE(nonstd::cx::f_eq_ulp(f32__f32_smallest_p, f64__f64_smallest_p)       == false);
        REQUIRE(nonstd::cx::f_eq_ulp(f64__f64_smallest_p, f_long__f_long_smallest_p) == f64_and_f_long_are_identical);
    }

    SECTION("f_eq_eps") {
        constexpr auto calls_to_f_eq_eps_are_constexpr = nonstd::cx::f_eq_eps(f32__positive, f32__positive);

        // On platforms where f64 and f_long are identical, the smallest numbers
        // representable will be identical.
        bool f64_and_f_long_are_identical = (sizeof(f64__f64_smallest_p) ==
                                             sizeof(f_long__f_long_smallest_p));

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

        REQUIRE((f32__f32_smallest_p == f64__f64_smallest_p)       == false);
        REQUIRE((f64__f64_smallest_p == f_long__f_long_smallest_p) == f64_and_f_long_are_identical);


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
        REQUIRE(nonstd::cx::f_eq_eps(f32__f32_smallest_p, f64__f64_smallest_p)       == true);
        REQUIRE(nonstd::cx::f_eq_eps(f64__f64_smallest_p, f_long__f_long_smallest_p) == true);
    }

    SECTION("isinf") {
        constexpr auto calls_to_isinf_are_constexpr = nonstd::cx::isinf(f32__positive);

        REQUIRE( std::isinf(f32__positive)                  == nonstd::cx::isinf(f32__positive)               );
        REQUIRE( std::isinf(f32__zero)                      == nonstd::cx::isinf(f32__zero)                   );
        REQUIRE( std::isinf(f32__negative)                  == nonstd::cx::isinf(f32__negative)               );
        REQUIRE( std::isinf(f32__f32_smallest_p)            == nonstd::cx::isinf(f32__f32_smallest_p)         );
        REQUIRE( std::isinf(f32__f32_smallest_n)            == nonstd::cx::isinf(f32__f32_smallest_n)         );
        REQUIRE( std::isinf(f32__f32_highest)               == nonstd::cx::isinf(f32__f32_highest)            );
        REQUIRE( std::isinf(f32__f32_lowest)                == nonstd::cx::isinf(f32__f32_lowest)             );
        REQUIRE( std::isinf(f32__f64_smallest_p)            == nonstd::cx::isinf(f32__f64_smallest_p)         );
        REQUIRE( std::isinf(f32__f64_smallest_n)            == nonstd::cx::isinf(f32__f64_smallest_n)         );
    //  REQUIRE( std::isinf(f32__f64_highest)               == nonstd::cx::isinf(f32__f64_highest)            );
    //  REQUIRE( std::isinf(f32__f64_lowest)                == nonstd::cx::isinf(f32__f64_lowest)             );
        REQUIRE( std::isinf(f32__f_long_smallest_p)         == nonstd::cx::isinf(f32__f_long_smallest_p)      );
        REQUIRE( std::isinf(f32__f_long_smallest_n)         == nonstd::cx::isinf(f32__f_long_smallest_n)      );
    //  REQUIRE( std::isinf(f32__f_long_highest)            == nonstd::cx::isinf(f32__f_long_highest)         );
    //  REQUIRE( std::isinf(f32__f_long_lowest)             == nonstd::cx::isinf(f32__f_long_lowest)          );
        REQUIRE( std::isinf(f32__f32_nan)                   == nonstd::cx::isinf(f32__f32_nan)                );
        REQUIRE( std::isinf(f32__f64_nan)                   == nonstd::cx::isinf(f32__f64_nan)                );
        REQUIRE( std::isinf(f32__f_long_nan)                == nonstd::cx::isinf(f32__f_long_nan)             );
        REQUIRE( std::isinf(f32__f32_p_inf)                 == nonstd::cx::isinf(f32__f32_p_inf)              );
        REQUIRE( std::isinf(f32__f32_n_inf)                 == nonstd::cx::isinf(f32__f32_n_inf)              );
        REQUIRE( std::isinf(f32__f64_p_inf)                 == nonstd::cx::isinf(f32__f64_p_inf)              );
        REQUIRE( std::isinf(f32__f64_n_inf)                 == nonstd::cx::isinf(f32__f64_n_inf)              );
        REQUIRE( std::isinf(f32__f_long_p_inf)              == nonstd::cx::isinf(f32__f_long_p_inf)           );
        REQUIRE( std::isinf(f32__f_long_n_inf)              == nonstd::cx::isinf(f32__f_long_n_inf)           );
        REQUIRE( std::isinf(f32__f32_less_smallest_p)       == nonstd::cx::isinf(f32__f32_less_smallest_p)    );
        REQUIRE( std::isinf(f32__f32_less_smallest_n)       == nonstd::cx::isinf(f32__f32_less_smallest_n)    );
        REQUIRE( std::isinf(f32__f32_more_highest)          == nonstd::cx::isinf(f32__f32_more_highest)       );
        REQUIRE( std::isinf(f32__f32_more_lowest)           == nonstd::cx::isinf(f32__f32_more_lowest)        );
        REQUIRE( std::isinf(f32__f64_less_smallest_p)       == nonstd::cx::isinf(f32__f64_less_smallest_p)    );
        REQUIRE( std::isinf(f32__f64_less_smallest_n)       == nonstd::cx::isinf(f32__f64_less_smallest_n)    );
        REQUIRE( std::isinf(f32__f64_more_highest)          == nonstd::cx::isinf(f32__f64_more_highest)       );
        REQUIRE( std::isinf(f32__f64_more_lowest)           == nonstd::cx::isinf(f32__f64_more_lowest)        );
        REQUIRE( std::isinf(f32__f_long_less_smallest_p)    == nonstd::cx::isinf(f32__f_long_less_smallest_p) );
        REQUIRE( std::isinf(f32__f_long_less_smallest_n)    == nonstd::cx::isinf(f32__f_long_less_smallest_n) );
        REQUIRE( std::isinf(f32__f_long_more_highest)       == nonstd::cx::isinf(f32__f_long_more_highest)    );
        REQUIRE( std::isinf(f32__f_long_more_lowest)        == nonstd::cx::isinf(f32__f_long_more_lowest)     );

        REQUIRE( std::isinf(f64__positive)                  == nonstd::cx::isinf(f64__positive)               );
        REQUIRE( std::isinf(f64__zero)                      == nonstd::cx::isinf(f64__zero)                   );
        REQUIRE( std::isinf(f64__negative)                  == nonstd::cx::isinf(f64__negative)               );
        REQUIRE( std::isinf(f64__f32_smallest_p)            == nonstd::cx::isinf(f64__f32_smallest_p)         );
        REQUIRE( std::isinf(f64__f32_smallest_n)            == nonstd::cx::isinf(f64__f32_smallest_n)         );
        REQUIRE( std::isinf(f64__f32_highest)               == nonstd::cx::isinf(f64__f32_highest)            );
        REQUIRE( std::isinf(f64__f32_lowest)                == nonstd::cx::isinf(f64__f32_lowest)             );
        REQUIRE( std::isinf(f64__f64_smallest_p)            == nonstd::cx::isinf(f64__f64_smallest_p)         );
        REQUIRE( std::isinf(f64__f64_smallest_n)            == nonstd::cx::isinf(f64__f64_smallest_n)         );
        REQUIRE( std::isinf(f64__f64_highest)               == nonstd::cx::isinf(f64__f64_highest)            );
        REQUIRE( std::isinf(f64__f64_lowest)                == nonstd::cx::isinf(f64__f64_lowest)             );
        REQUIRE( std::isinf(f64__f_long_smallest_p)         == nonstd::cx::isinf(f64__f_long_smallest_p)      );
        REQUIRE( std::isinf(f64__f_long_smallest_n)         == nonstd::cx::isinf(f64__f_long_smallest_n)      );
    //  REQUIRE( std::isinf(f64__f_long_highest)            == nonstd::cx::isinf(f64__f_long_highest)         );
    //  REQUIRE( std::isinf(f64__f_long_lowest)             == nonstd::cx::isinf(f64__f_long_lowest)          );
        REQUIRE( std::isinf(f64__f32_nan)                   == nonstd::cx::isinf(f64__f32_nan)                );
        REQUIRE( std::isinf(f64__f64_nan)                   == nonstd::cx::isinf(f64__f64_nan)                );
        REQUIRE( std::isinf(f64__f_long_nan)                == nonstd::cx::isinf(f64__f_long_nan)             );
        REQUIRE( std::isinf(f64__f32_p_inf)                 == nonstd::cx::isinf(f64__f32_p_inf)              );
        REQUIRE( std::isinf(f64__f32_n_inf)                 == nonstd::cx::isinf(f64__f32_n_inf)              );
        REQUIRE( std::isinf(f64__f64_p_inf)                 == nonstd::cx::isinf(f64__f64_p_inf)              );
        REQUIRE( std::isinf(f64__f64_n_inf)                 == nonstd::cx::isinf(f64__f64_n_inf)              );
        REQUIRE( std::isinf(f64__f_long_p_inf)              == nonstd::cx::isinf(f64__f_long_p_inf)           );
        REQUIRE( std::isinf(f64__f_long_n_inf)              == nonstd::cx::isinf(f64__f_long_n_inf)           );
        REQUIRE( std::isinf(f64__f32_less_smallest_p)       == nonstd::cx::isinf(f64__f32_less_smallest_p)    );
        REQUIRE( std::isinf(f64__f32_less_smallest_n)       == nonstd::cx::isinf(f64__f32_less_smallest_n)    );
        REQUIRE( std::isinf(f64__f32_more_highest)          == nonstd::cx::isinf(f64__f32_more_highest)       );
        REQUIRE( std::isinf(f64__f32_more_lowest)           == nonstd::cx::isinf(f64__f32_more_lowest)        );
        REQUIRE( std::isinf(f64__f64_less_smallest_p)       == nonstd::cx::isinf(f64__f64_less_smallest_p)    );
        REQUIRE( std::isinf(f64__f64_less_smallest_n)       == nonstd::cx::isinf(f64__f64_less_smallest_n)    );
        REQUIRE( std::isinf(f64__f64_more_highest)          == nonstd::cx::isinf(f64__f64_more_highest)       );
        REQUIRE( std::isinf(f64__f64_more_lowest)           == nonstd::cx::isinf(f64__f64_more_lowest)        );
        REQUIRE( std::isinf(f64__f_long_less_smallest_p)    == nonstd::cx::isinf(f64__f_long_less_smallest_p) );
        REQUIRE( std::isinf(f64__f_long_less_smallest_n)    == nonstd::cx::isinf(f64__f_long_less_smallest_n) );
        REQUIRE( std::isinf(f64__f_long_more_highest)       == nonstd::cx::isinf(f64__f_long_more_highest)    );
        REQUIRE( std::isinf(f64__f_long_more_lowest)        == nonstd::cx::isinf(f64__f_long_more_lowest)     );

        REQUIRE( std::isinf(f_long__positive)               == nonstd::cx::isinf(f_long__positive)               );
        REQUIRE( std::isinf(f_long__zero)                   == nonstd::cx::isinf(f_long__zero)                   );
        REQUIRE( std::isinf(f_long__negative)               == nonstd::cx::isinf(f_long__negative)               );
        REQUIRE( std::isinf(f_long__f32_smallest_p)         == nonstd::cx::isinf(f_long__f32_smallest_p)         );
        REQUIRE( std::isinf(f_long__f32_smallest_n)         == nonstd::cx::isinf(f_long__f32_smallest_n)         );
        REQUIRE( std::isinf(f_long__f32_highest)            == nonstd::cx::isinf(f_long__f32_highest)            );
        REQUIRE( std::isinf(f_long__f32_lowest)             == nonstd::cx::isinf(f_long__f32_lowest)             );
        REQUIRE( std::isinf(f_long__f64_smallest_p)         == nonstd::cx::isinf(f_long__f64_smallest_p)         );
        REQUIRE( std::isinf(f_long__f64_smallest_n)         == nonstd::cx::isinf(f_long__f64_smallest_n)         );
        REQUIRE( std::isinf(f_long__f64_highest)            == nonstd::cx::isinf(f_long__f64_highest)            );
        REQUIRE( std::isinf(f_long__f64_lowest)             == nonstd::cx::isinf(f_long__f64_lowest)             );
        REQUIRE( std::isinf(f_long__f_long_smallest_p)      == nonstd::cx::isinf(f_long__f_long_smallest_p)      );
        REQUIRE( std::isinf(f_long__f_long_smallest_n)      == nonstd::cx::isinf(f_long__f_long_smallest_n)      );
        REQUIRE( std::isinf(f_long__f_long_highest)         == nonstd::cx::isinf(f_long__f_long_highest)         );
        REQUIRE( std::isinf(f_long__f_long_lowest)          == nonstd::cx::isinf(f_long__f_long_lowest)          );
        REQUIRE( std::isinf(f_long__f32_nan)                == nonstd::cx::isinf(f_long__f32_nan)                );
        REQUIRE( std::isinf(f_long__f64_nan)                == nonstd::cx::isinf(f_long__f64_nan)                );
        REQUIRE( std::isinf(f_long__f_long_nan)             == nonstd::cx::isinf(f_long__f_long_nan)             );
        REQUIRE( std::isinf(f_long__f32_p_inf)              == nonstd::cx::isinf(f_long__f32_p_inf)              );
        REQUIRE( std::isinf(f_long__f32_n_inf)              == nonstd::cx::isinf(f_long__f32_n_inf)              );
        REQUIRE( std::isinf(f_long__f64_p_inf)              == nonstd::cx::isinf(f_long__f64_p_inf)              );
        REQUIRE( std::isinf(f_long__f64_n_inf)              == nonstd::cx::isinf(f_long__f64_n_inf)              );
        REQUIRE( std::isinf(f_long__f_long_p_inf)           == nonstd::cx::isinf(f_long__f_long_p_inf)           );
        REQUIRE( std::isinf(f_long__f_long_n_inf)           == nonstd::cx::isinf(f_long__f_long_n_inf)           );
        REQUIRE( std::isinf(f_long__f32_less_smallest_p)    == nonstd::cx::isinf(f_long__f32_less_smallest_p)    );
        REQUIRE( std::isinf(f_long__f32_less_smallest_n)    == nonstd::cx::isinf(f_long__f32_less_smallest_n)    );
        REQUIRE( std::isinf(f_long__f32_more_highest)       == nonstd::cx::isinf(f_long__f32_more_highest)       );
        REQUIRE( std::isinf(f_long__f32_more_lowest)        == nonstd::cx::isinf(f_long__f32_more_lowest)        );
        REQUIRE( std::isinf(f_long__f64_less_smallest_p)    == nonstd::cx::isinf(f_long__f64_less_smallest_p)    );
        REQUIRE( std::isinf(f_long__f64_less_smallest_n)    == nonstd::cx::isinf(f_long__f64_less_smallest_n)    );
        REQUIRE( std::isinf(f_long__f64_more_highest)       == nonstd::cx::isinf(f_long__f64_more_highest)       );
        REQUIRE( std::isinf(f_long__f64_more_lowest)        == nonstd::cx::isinf(f_long__f64_more_lowest)        );
        REQUIRE( std::isinf(f_long__f_long_less_smallest_p) == nonstd::cx::isinf(f_long__f_long_less_smallest_p) );
        REQUIRE( std::isinf(f_long__f_long_less_smallest_n) == nonstd::cx::isinf(f_long__f_long_less_smallest_n) );
        REQUIRE( std::isinf(f_long__f_long_more_highest)    == nonstd::cx::isinf(f_long__f_long_more_highest)    );
        REQUIRE( std::isinf(f_long__f_long_more_lowest)     == nonstd::cx::isinf(f_long__f_long_more_lowest)     );

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
        REQUIRE( std::isnan(f32__f32_smallest_p)         == nonstd::cx::isnan(f32__f32_smallest_p)         );
        REQUIRE( std::isnan(f32__f32_smallest_n)         == nonstd::cx::isnan(f32__f32_smallest_n)         );
        REQUIRE( std::isnan(f32__f32_highest)            == nonstd::cx::isnan(f32__f32_highest)            );
        REQUIRE( std::isnan(f32__f32_lowest)             == nonstd::cx::isnan(f32__f32_lowest)             );
        REQUIRE( std::isnan(f32__f64_smallest_p)         == nonstd::cx::isnan(f32__f64_smallest_p)         );
        REQUIRE( std::isnan(f32__f64_smallest_n)         == nonstd::cx::isnan(f32__f64_smallest_n)         );
    //  REQUIRE( std::isnan(f32__f64_highest)            == nonstd::cx::isnan(f32__f64_highest)            );
    //  REQUIRE( std::isnan(f32__f64_lowest)             == nonstd::cx::isnan(f32__f64_lowest)             );
        REQUIRE( std::isnan(f32__f_long_smallest_p)      == nonstd::cx::isnan(f32__f_long_smallest_p)      );
        REQUIRE( std::isnan(f32__f_long_smallest_n)      == nonstd::cx::isnan(f32__f_long_smallest_n)      );
    //  REQUIRE( std::isnan(f32__f_long_highest)         == nonstd::cx::isnan(f32__f_long_highest)         );
    //  REQUIRE( std::isnan(f32__f_long_lowest)          == nonstd::cx::isnan(f32__f_long_lowest)          );
        REQUIRE( std::isnan(f32__f32_nan)                == nonstd::cx::isnan(f32__f32_nan)                );
        REQUIRE( std::isnan(f32__f64_nan)                == nonstd::cx::isnan(f32__f64_nan)                );
        REQUIRE( std::isnan(f32__f_long_nan)             == nonstd::cx::isnan(f32__f_long_nan)             );
        REQUIRE( std::isnan(f32__f32_p_inf)              == nonstd::cx::isnan(f32__f32_p_inf)              );
        REQUIRE( std::isnan(f32__f32_n_inf)              == nonstd::cx::isnan(f32__f32_n_inf)              );
        REQUIRE( std::isnan(f32__f64_p_inf)              == nonstd::cx::isnan(f32__f64_p_inf)              );
        REQUIRE( std::isnan(f32__f64_n_inf)              == nonstd::cx::isnan(f32__f64_n_inf)              );
        REQUIRE( std::isnan(f32__f_long_p_inf)           == nonstd::cx::isnan(f32__f_long_p_inf)           );
        REQUIRE( std::isnan(f32__f_long_n_inf)           == nonstd::cx::isnan(f32__f_long_n_inf)           );
        REQUIRE( std::isnan(f32__f32_less_smallest_p)    == nonstd::cx::isnan(f32__f32_less_smallest_p)    );
        REQUIRE( std::isnan(f32__f32_less_smallest_n)    == nonstd::cx::isnan(f32__f32_less_smallest_n)    );
        REQUIRE( std::isnan(f32__f32_more_highest)       == nonstd::cx::isnan(f32__f32_more_highest)       );
        REQUIRE( std::isnan(f32__f32_more_lowest)        == nonstd::cx::isnan(f32__f32_more_lowest)        );
        REQUIRE( std::isnan(f32__f64_less_smallest_p)    == nonstd::cx::isnan(f32__f64_less_smallest_p)    );
        REQUIRE( std::isnan(f32__f64_less_smallest_n)    == nonstd::cx::isnan(f32__f64_less_smallest_n)    );
        REQUIRE( std::isnan(f32__f64_more_highest)       == nonstd::cx::isnan(f32__f64_more_highest)       );
        REQUIRE( std::isnan(f32__f64_more_lowest)        == nonstd::cx::isnan(f32__f64_more_lowest)        );
        REQUIRE( std::isnan(f32__f_long_less_smallest_p) == nonstd::cx::isnan(f32__f_long_less_smallest_p) );
        REQUIRE( std::isnan(f32__f_long_less_smallest_n) == nonstd::cx::isnan(f32__f_long_less_smallest_n) );
        REQUIRE( std::isnan(f32__f_long_more_highest)    == nonstd::cx::isnan(f32__f_long_more_highest)    );
        REQUIRE( std::isnan(f32__f_long_more_lowest)     == nonstd::cx::isnan(f32__f_long_more_lowest)     );

        REQUIRE( std::isnan(f64__positive)               == nonstd::cx::isnan(f64__positive)               );
        REQUIRE( std::isnan(f64__zero)                   == nonstd::cx::isnan(f64__zero)                   );
        REQUIRE( std::isnan(f64__negative)               == nonstd::cx::isnan(f64__negative)               );
        REQUIRE( std::isnan(f64__f32_smallest_p)         == nonstd::cx::isnan(f64__f32_smallest_p)         );
        REQUIRE( std::isnan(f64__f32_smallest_n)         == nonstd::cx::isnan(f64__f32_smallest_n)         );
        REQUIRE( std::isnan(f64__f32_highest)            == nonstd::cx::isnan(f64__f32_highest)            );
        REQUIRE( std::isnan(f64__f32_lowest)             == nonstd::cx::isnan(f64__f32_lowest)             );
        REQUIRE( std::isnan(f64__f64_smallest_p)         == nonstd::cx::isnan(f64__f64_smallest_p)         );
        REQUIRE( std::isnan(f64__f64_smallest_n)         == nonstd::cx::isnan(f64__f64_smallest_n)         );
        REQUIRE( std::isnan(f64__f64_highest)            == nonstd::cx::isnan(f64__f64_highest)            );
        REQUIRE( std::isnan(f64__f64_lowest)             == nonstd::cx::isnan(f64__f64_lowest)             );
        REQUIRE( std::isnan(f64__f_long_smallest_p)      == nonstd::cx::isnan(f64__f_long_smallest_p)      );
        REQUIRE( std::isnan(f64__f_long_smallest_n)      == nonstd::cx::isnan(f64__f_long_smallest_n)      );
    //  REQUIRE( std::isnan(f64__f_long_highest)         == nonstd::cx::isnan(f64__f_long_highest)         );
    //  REQUIRE( std::isnan(f64__f_long_lowest)          == nonstd::cx::isnan(f64__f_long_lowest)          );
        REQUIRE( std::isnan(f64__f32_nan)                == nonstd::cx::isnan(f64__f32_nan)                );
        REQUIRE( std::isnan(f64__f64_nan)                == nonstd::cx::isnan(f64__f64_nan)                );
        REQUIRE( std::isnan(f64__f_long_nan)             == nonstd::cx::isnan(f64__f_long_nan)             );
        REQUIRE( std::isnan(f64__f32_p_inf)              == nonstd::cx::isnan(f64__f32_p_inf)              );
        REQUIRE( std::isnan(f64__f32_n_inf)              == nonstd::cx::isnan(f64__f32_n_inf)              );
        REQUIRE( std::isnan(f64__f64_p_inf)              == nonstd::cx::isnan(f64__f64_p_inf)              );
        REQUIRE( std::isnan(f64__f64_n_inf)              == nonstd::cx::isnan(f64__f64_n_inf)              );
        REQUIRE( std::isnan(f64__f_long_p_inf)           == nonstd::cx::isnan(f64__f_long_p_inf)           );
        REQUIRE( std::isnan(f64__f_long_n_inf)           == nonstd::cx::isnan(f64__f_long_n_inf)           );
        REQUIRE( std::isnan(f64__f32_less_smallest_p)    == nonstd::cx::isnan(f64__f32_less_smallest_p)    );
        REQUIRE( std::isnan(f64__f32_less_smallest_n)    == nonstd::cx::isnan(f64__f32_less_smallest_n)    );
        REQUIRE( std::isnan(f64__f32_more_highest)       == nonstd::cx::isnan(f64__f32_more_highest)       );
        REQUIRE( std::isnan(f64__f32_more_lowest)        == nonstd::cx::isnan(f64__f32_more_lowest)        );
        REQUIRE( std::isnan(f64__f64_less_smallest_p)    == nonstd::cx::isnan(f64__f64_less_smallest_p)    );
        REQUIRE( std::isnan(f64__f64_less_smallest_n)    == nonstd::cx::isnan(f64__f64_less_smallest_n)    );
        REQUIRE( std::isnan(f64__f64_more_highest)       == nonstd::cx::isnan(f64__f64_more_highest)       );
        REQUIRE( std::isnan(f64__f64_more_lowest)        == nonstd::cx::isnan(f64__f64_more_lowest)        );
        REQUIRE( std::isnan(f64__f_long_less_smallest_p) == nonstd::cx::isnan(f64__f_long_less_smallest_p) );
        REQUIRE( std::isnan(f64__f_long_less_smallest_n) == nonstd::cx::isnan(f64__f_long_less_smallest_n) );
        REQUIRE( std::isnan(f64__f_long_more_highest)    == nonstd::cx::isnan(f64__f_long_more_highest)    );
        REQUIRE( std::isnan(f64__f_long_more_lowest)    == nonstd::cx::isnan(f64__f_long_more_lowest)      );

        REQUIRE( std::isnan(f_long__positive)               == nonstd::cx::isnan(f_long__positive)               );
        REQUIRE( std::isnan(f_long__zero)                   == nonstd::cx::isnan(f_long__zero)                   );
        REQUIRE( std::isnan(f_long__negative)               == nonstd::cx::isnan(f_long__negative)               );
        REQUIRE( std::isnan(f_long__f32_smallest_p)         == nonstd::cx::isnan(f_long__f32_smallest_p)         );
        REQUIRE( std::isnan(f_long__f32_smallest_n)         == nonstd::cx::isnan(f_long__f32_smallest_n)         );
        REQUIRE( std::isnan(f_long__f32_highest)            == nonstd::cx::isnan(f_long__f32_highest)            );
        REQUIRE( std::isnan(f_long__f32_lowest)             == nonstd::cx::isnan(f_long__f32_lowest)             );
        REQUIRE( std::isnan(f_long__f64_smallest_p)         == nonstd::cx::isnan(f_long__f64_smallest_p)         );
        REQUIRE( std::isnan(f_long__f64_smallest_n)         == nonstd::cx::isnan(f_long__f64_smallest_n)         );
        REQUIRE( std::isnan(f_long__f64_highest)            == nonstd::cx::isnan(f_long__f64_highest)            );
        REQUIRE( std::isnan(f_long__f64_lowest)             == nonstd::cx::isnan(f_long__f64_lowest)             );
        REQUIRE( std::isnan(f_long__f_long_smallest_p)      == nonstd::cx::isnan(f_long__f_long_smallest_p)      );
        REQUIRE( std::isnan(f_long__f_long_smallest_n)      == nonstd::cx::isnan(f_long__f_long_smallest_n)      );
        REQUIRE( std::isnan(f_long__f_long_highest)         == nonstd::cx::isnan(f_long__f_long_highest)         );
        REQUIRE( std::isnan(f_long__f_long_lowest)          == nonstd::cx::isnan(f_long__f_long_lowest)          );
        REQUIRE( std::isnan(f_long__f32_nan)                == nonstd::cx::isnan(f_long__f32_nan)                );
        REQUIRE( std::isnan(f_long__f64_nan)                == nonstd::cx::isnan(f_long__f64_nan)                );
        REQUIRE( std::isnan(f_long__f_long_nan)             == nonstd::cx::isnan(f_long__f_long_nan)             );
        REQUIRE( std::isnan(f_long__f32_p_inf)              == nonstd::cx::isnan(f_long__f32_p_inf)              );
        REQUIRE( std::isnan(f_long__f32_n_inf)              == nonstd::cx::isnan(f_long__f32_n_inf)              );
        REQUIRE( std::isnan(f_long__f64_p_inf)              == nonstd::cx::isnan(f_long__f64_p_inf)              );
        REQUIRE( std::isnan(f_long__f64_n_inf)              == nonstd::cx::isnan(f_long__f64_n_inf)              );
        REQUIRE( std::isnan(f_long__f_long_p_inf)           == nonstd::cx::isnan(f_long__f_long_p_inf)           );
        REQUIRE( std::isnan(f_long__f_long_n_inf)           == nonstd::cx::isnan(f_long__f_long_n_inf)           );
        REQUIRE( std::isnan(f_long__f32_less_smallest_p)    == nonstd::cx::isnan(f_long__f32_less_smallest_p)    );
        REQUIRE( std::isnan(f_long__f32_less_smallest_n)    == nonstd::cx::isnan(f_long__f32_less_smallest_n)    );
        REQUIRE( std::isnan(f_long__f32_more_highest)       == nonstd::cx::isnan(f_long__f32_more_highest)       );
        REQUIRE( std::isnan(f_long__f32_more_lowest)        == nonstd::cx::isnan(f_long__f32_more_lowest)        );
        REQUIRE( std::isnan(f_long__f64_less_smallest_p)    == nonstd::cx::isnan(f_long__f64_less_smallest_p)    );
        REQUIRE( std::isnan(f_long__f64_less_smallest_n)    == nonstd::cx::isnan(f_long__f64_less_smallest_n)    );
        REQUIRE( std::isnan(f_long__f64_more_highest)       == nonstd::cx::isnan(f_long__f64_more_highest)       );
        REQUIRE( std::isnan(f_long__f64_more_lowest)        == nonstd::cx::isnan(f_long__f64_more_lowest)        );
        REQUIRE( std::isnan(f_long__f_long_less_smallest_p) == nonstd::cx::isnan(f_long__f_long_less_smallest_p) );
        REQUIRE( std::isnan(f_long__f_long_less_smallest_n) == nonstd::cx::isnan(f_long__f_long_less_smallest_n) );
        REQUIRE( std::isnan(f_long__f_long_more_highest)    == nonstd::cx::isnan(f_long__f_long_more_highest)    );
        REQUIRE( std::isnan(f_long__f_long_more_lowest)     == nonstd::cx::isnan(f_long__f_long_more_lowest)     );

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
        REQUIRE(( ( std::abs(f32__f32_smallest_p)         == nonstd::cx::abs(f32__f32_smallest_p) )         || ( isnan(std::abs(f32__f32_smallest_p))         && isnan(nonstd::cx::abs(f32__f32_smallest_p)) )         ));
        REQUIRE(( ( std::abs(f32__f32_smallest_n)         == nonstd::cx::abs(f32__f32_smallest_n) )         || ( isnan(std::abs(f32__f32_smallest_n))         && isnan(nonstd::cx::abs(f32__f32_smallest_n)) )         ));
        REQUIRE(( ( std::abs(f32__f32_highest)            == nonstd::cx::abs(f32__f32_highest) )            || ( isnan(std::abs(f32__f32_highest))            && isnan(nonstd::cx::abs(f32__f32_highest)) )            ));
        REQUIRE(( ( std::abs(f32__f32_lowest)             == nonstd::cx::abs(f32__f32_lowest) )             || ( isnan(std::abs(f32__f32_lowest))             && isnan(nonstd::cx::abs(f32__f32_lowest)) )             ));
        REQUIRE(( ( std::abs(f32__f64_smallest_p)         == nonstd::cx::abs(f32__f64_smallest_p) )         || ( isnan(std::abs(f32__f64_smallest_p))         && isnan(nonstd::cx::abs(f32__f64_smallest_p)) )         ));
        REQUIRE(( ( std::abs(f32__f64_smallest_n)         == nonstd::cx::abs(f32__f64_smallest_n) )         || ( isnan(std::abs(f32__f64_smallest_n))         && isnan(nonstd::cx::abs(f32__f64_smallest_n)) )         ));
    //  REQUIRE(( ( std::abs(f32__f64_highest)            == nonstd::cx::abs(f32__f64_highest) )            || ( isnan(std::abs(f32__f64_highest))            && isnan(nonstd::cx::abs(f32__f64_highest)) )            ));
    //  REQUIRE(( ( std::abs(f32__f64_lowest)             == nonstd::cx::abs(f32__f64_lowest) )             || ( isnan(std::abs(f32__f64_lowest))             && isnan(nonstd::cx::abs(f32__f64_lowest)) )             ));
        REQUIRE(( ( std::abs(f32__f_long_smallest_p)      == nonstd::cx::abs(f32__f_long_smallest_p) )      || ( isnan(std::abs(f32__f_long_smallest_p))      && isnan(nonstd::cx::abs(f32__f_long_smallest_p)) )      ));
        REQUIRE(( ( std::abs(f32__f_long_smallest_n)      == nonstd::cx::abs(f32__f_long_smallest_n) )      || ( isnan(std::abs(f32__f_long_smallest_n))      && isnan(nonstd::cx::abs(f32__f_long_smallest_n)) )      ));
    //  REQUIRE(( ( std::abs(f32__f_long_highest)         == nonstd::cx::abs(f32__f_long_highest) )         || ( isnan(std::abs(f32__f_long_highest))         && isnan(nonstd::cx::abs(f32__f_long_highest)) )         ));
    //  REQUIRE(( ( std::abs(f32__f_long_lowest)          == nonstd::cx::abs(f32__f_long_lowest) )          || ( isnan(std::abs(f32__f_long_lowest))          && isnan(nonstd::cx::abs(f32__f_long_lowest)) )          ));
        REQUIRE(( ( std::abs(f32__f32_nan)                == nonstd::cx::abs(f32__f32_nan) )                || ( isnan(std::abs(f32__f32_nan))                && isnan(nonstd::cx::abs(f32__f32_nan)) )                ));
        REQUIRE(( ( std::abs(f32__f64_nan)                == nonstd::cx::abs(f32__f64_nan) )                || ( isnan(std::abs(f32__f64_nan))                && isnan(nonstd::cx::abs(f32__f64_nan)) )                ));
        REQUIRE(( ( std::abs(f32__f_long_nan)             == nonstd::cx::abs(f32__f_long_nan) )             || ( isnan(std::abs(f32__f_long_nan))             && isnan(nonstd::cx::abs(f32__f_long_nan)) )             ));
        REQUIRE(( ( std::abs(f32__f32_p_inf)              == nonstd::cx::abs(f32__f32_p_inf) )              || ( isnan(std::abs(f32__f32_p_inf))              && isnan(nonstd::cx::abs(f32__f32_p_inf)) )              ));
        REQUIRE(( ( std::abs(f32__f32_n_inf)              == nonstd::cx::abs(f32__f32_n_inf) )              || ( isnan(std::abs(f32__f32_n_inf))              && isnan(nonstd::cx::abs(f32__f32_n_inf)) )              ));
        REQUIRE(( ( std::abs(f32__f64_p_inf)              == nonstd::cx::abs(f32__f64_p_inf) )              || ( isnan(std::abs(f32__f64_p_inf))              && isnan(nonstd::cx::abs(f32__f64_p_inf)) )              ));
        REQUIRE(( ( std::abs(f32__f64_n_inf)              == nonstd::cx::abs(f32__f64_n_inf) )              || ( isnan(std::abs(f32__f64_n_inf))              && isnan(nonstd::cx::abs(f32__f64_n_inf)) )              ));
        REQUIRE(( ( std::abs(f32__f_long_p_inf)           == nonstd::cx::abs(f32__f_long_p_inf) )           || ( isnan(std::abs(f32__f_long_p_inf))           && isnan(nonstd::cx::abs(f32__f_long_p_inf)) )           ));
        REQUIRE(( ( std::abs(f32__f_long_n_inf)           == nonstd::cx::abs(f32__f_long_n_inf) )           || ( isnan(std::abs(f32__f_long_n_inf))           && isnan(nonstd::cx::abs(f32__f_long_n_inf)) )           ));
        REQUIRE(( ( std::abs(f32__f32_less_smallest_p)    == nonstd::cx::abs(f32__f32_less_smallest_p) )    || ( isnan(std::abs(f32__f32_less_smallest_p))    && isnan(nonstd::cx::abs(f32__f32_less_smallest_p)) )    ));
        REQUIRE(( ( std::abs(f32__f32_less_smallest_n)    == nonstd::cx::abs(f32__f32_less_smallest_n) )    || ( isnan(std::abs(f32__f32_less_smallest_n))    && isnan(nonstd::cx::abs(f32__f32_less_smallest_n)) )    ));
        REQUIRE(( ( std::abs(f32__f32_more_highest)       == nonstd::cx::abs(f32__f32_more_highest) )       || ( isnan(std::abs(f32__f32_more_highest))       && isnan(nonstd::cx::abs(f32__f32_more_highest)) )       ));
        REQUIRE(( ( std::abs(f32__f32_more_lowest)        == nonstd::cx::abs(f32__f32_more_lowest) )        || ( isnan(std::abs(f32__f32_more_lowest))        && isnan(nonstd::cx::abs(f32__f32_more_lowest)) )        ));
        REQUIRE(( ( std::abs(f32__f64_less_smallest_p)    == nonstd::cx::abs(f32__f64_less_smallest_p) )    || ( isnan(std::abs(f32__f64_less_smallest_p))    && isnan(nonstd::cx::abs(f32__f64_less_smallest_p)) )    ));
        REQUIRE(( ( std::abs(f32__f64_less_smallest_n)    == nonstd::cx::abs(f32__f64_less_smallest_n) )    || ( isnan(std::abs(f32__f64_less_smallest_n))    && isnan(nonstd::cx::abs(f32__f64_less_smallest_n)) )    ));
        REQUIRE(( ( std::abs(f32__f64_more_highest)       == nonstd::cx::abs(f32__f64_more_highest) )       || ( isnan(std::abs(f32__f64_more_highest))       && isnan(nonstd::cx::abs(f32__f64_more_highest)) )       ));
        REQUIRE(( ( std::abs(f32__f64_more_lowest)        == nonstd::cx::abs(f32__f64_more_lowest) )        || ( isnan(std::abs(f32__f64_more_lowest))        && isnan(nonstd::cx::abs(f32__f64_more_lowest)) )        ));
        REQUIRE(( ( std::abs(f32__f_long_less_smallest_p) == nonstd::cx::abs(f32__f_long_less_smallest_p) ) || ( isnan(std::abs(f32__f_long_less_smallest_p)) && isnan(nonstd::cx::abs(f32__f_long_less_smallest_p)) ) ));
        REQUIRE(( ( std::abs(f32__f_long_less_smallest_n) == nonstd::cx::abs(f32__f_long_less_smallest_n) ) || ( isnan(std::abs(f32__f_long_less_smallest_n)) && isnan(nonstd::cx::abs(f32__f_long_less_smallest_n)) ) ));
        REQUIRE(( ( std::abs(f32__f_long_more_highest)    == nonstd::cx::abs(f32__f_long_more_highest) )    || ( isnan(std::abs(f32__f_long_more_highest))    && isnan(nonstd::cx::abs(f32__f_long_more_highest)) )    ));
        REQUIRE(( ( std::abs(f32__f_long_more_lowest)     == nonstd::cx::abs(f32__f_long_more_lowest) )     || ( isnan(std::abs(f32__f_long_more_lowest))     && isnan(nonstd::cx::abs(f32__f_long_more_lowest)) )     ));

        REQUIRE(( ( std::abs(f64__positive)               == nonstd::cx::abs(f64__positive) )               || ( isnan(std::abs(f64__positive))               && isnan(nonstd::cx::abs(f64__positive)) )               ));
        REQUIRE(( ( std::abs(f64__zero)                   == nonstd::cx::abs(f64__zero) )                   || ( isnan(std::abs(f64__zero))                   && isnan(nonstd::cx::abs(f64__zero)) )                   ));
        REQUIRE(( ( std::abs(f64__negative)               == nonstd::cx::abs(f64__negative) )               || ( isnan(std::abs(f64__negative))               && isnan(nonstd::cx::abs(f64__negative)) )               ));
        REQUIRE(( ( std::abs(f64__f32_smallest_p)         == nonstd::cx::abs(f64__f32_smallest_p) )         || ( isnan(std::abs(f64__f32_smallest_p))         && isnan(nonstd::cx::abs(f64__f32_smallest_p)) )         ));
        REQUIRE(( ( std::abs(f64__f32_smallest_n)         == nonstd::cx::abs(f64__f32_smallest_n) )         || ( isnan(std::abs(f64__f32_smallest_n))         && isnan(nonstd::cx::abs(f64__f32_smallest_n)) )         ));
        REQUIRE(( ( std::abs(f64__f32_highest)            == nonstd::cx::abs(f64__f32_highest) )            || ( isnan(std::abs(f64__f32_highest))            && isnan(nonstd::cx::abs(f64__f32_highest)) )            ));
        REQUIRE(( ( std::abs(f64__f32_lowest)             == nonstd::cx::abs(f64__f32_lowest) )             || ( isnan(std::abs(f64__f32_lowest))             && isnan(nonstd::cx::abs(f64__f32_lowest)) )             ));
        REQUIRE(( ( std::abs(f64__f64_smallest_p)         == nonstd::cx::abs(f64__f64_smallest_p) )         || ( isnan(std::abs(f64__f64_smallest_p))         && isnan(nonstd::cx::abs(f64__f64_smallest_p)) )         ));
        REQUIRE(( ( std::abs(f64__f64_smallest_n)         == nonstd::cx::abs(f64__f64_smallest_n) )         || ( isnan(std::abs(f64__f64_smallest_n))         && isnan(nonstd::cx::abs(f64__f64_smallest_n)) )         ));
        REQUIRE(( ( std::abs(f64__f64_highest)            == nonstd::cx::abs(f64__f64_highest) )            || ( isnan(std::abs(f64__f64_highest))            && isnan(nonstd::cx::abs(f64__f64_highest)) )            ));
        REQUIRE(( ( std::abs(f64__f64_lowest)             == nonstd::cx::abs(f64__f64_lowest) )             || ( isnan(std::abs(f64__f64_lowest))             && isnan(nonstd::cx::abs(f64__f64_lowest)) )             ));
        REQUIRE(( ( std::abs(f64__f_long_smallest_p)      == nonstd::cx::abs(f64__f_long_smallest_p) )      || ( isnan(std::abs(f64__f_long_smallest_p))      && isnan(nonstd::cx::abs(f64__f_long_smallest_p)) )      ));
        REQUIRE(( ( std::abs(f64__f_long_smallest_n)      == nonstd::cx::abs(f64__f_long_smallest_n) )      || ( isnan(std::abs(f64__f_long_smallest_n))      && isnan(nonstd::cx::abs(f64__f_long_smallest_n)) )      ));
    //  REQUIRE(( ( std::abs(f64__f_long_highest)         == nonstd::cx::abs(f64__f_long_highest) )         || ( isnan(std::abs(f64__f_long_highest))         && isnan(nonstd::cx::abs(f64__f_long_highest)) )         ));
    //  REQUIRE(( ( std::abs(f64__f_long_lowest)          == nonstd::cx::abs(f64__f_long_lowest) )          || ( isnan(std::abs(f64__f_long_lowest))          && isnan(nonstd::cx::abs(f64__f_long_lowest)) )          ));
        REQUIRE(( ( std::abs(f64__f32_nan)                == nonstd::cx::abs(f64__f32_nan) )                || ( isnan(std::abs(f64__f32_nan))                && isnan(nonstd::cx::abs(f64__f32_nan)) )                ));
        REQUIRE(( ( std::abs(f64__f64_nan)                == nonstd::cx::abs(f64__f64_nan) )                || ( isnan(std::abs(f64__f64_nan))                && isnan(nonstd::cx::abs(f64__f64_nan)) )                ));
        REQUIRE(( ( std::abs(f64__f_long_nan)             == nonstd::cx::abs(f64__f_long_nan) )             || ( isnan(std::abs(f64__f_long_nan))             && isnan(nonstd::cx::abs(f64__f_long_nan)) )             ));
        REQUIRE(( ( std::abs(f64__f32_p_inf)              == nonstd::cx::abs(f64__f32_p_inf) )              || ( isnan(std::abs(f64__f32_p_inf))              && isnan(nonstd::cx::abs(f64__f32_p_inf)) )              ));
        REQUIRE(( ( std::abs(f64__f32_n_inf)              == nonstd::cx::abs(f64__f32_n_inf) )              || ( isnan(std::abs(f64__f32_n_inf))              && isnan(nonstd::cx::abs(f64__f32_n_inf)) )              ));
        REQUIRE(( ( std::abs(f64__f64_p_inf)              == nonstd::cx::abs(f64__f64_p_inf) )              || ( isnan(std::abs(f64__f64_p_inf))              && isnan(nonstd::cx::abs(f64__f64_p_inf)) )              ));
        REQUIRE(( ( std::abs(f64__f64_n_inf)              == nonstd::cx::abs(f64__f64_n_inf) )              || ( isnan(std::abs(f64__f64_n_inf))              && isnan(nonstd::cx::abs(f64__f64_n_inf)) )              ));
        REQUIRE(( ( std::abs(f64__f_long_p_inf)           == nonstd::cx::abs(f64__f_long_p_inf) )           || ( isnan(std::abs(f64__f_long_p_inf))           && isnan(nonstd::cx::abs(f64__f_long_p_inf)) )           ));
        REQUIRE(( ( std::abs(f64__f_long_n_inf)           == nonstd::cx::abs(f64__f_long_n_inf) )           || ( isnan(std::abs(f64__f_long_n_inf))           && isnan(nonstd::cx::abs(f64__f_long_n_inf)) )           ));
        REQUIRE(( ( std::abs(f64__f32_less_smallest_p)    == nonstd::cx::abs(f64__f32_less_smallest_p) )    || ( isnan(std::abs(f64__f32_less_smallest_p))    && isnan(nonstd::cx::abs(f64__f32_less_smallest_p)) )    ));
        REQUIRE(( ( std::abs(f64__f32_less_smallest_n)    == nonstd::cx::abs(f64__f32_less_smallest_n) )    || ( isnan(std::abs(f64__f32_less_smallest_n))    && isnan(nonstd::cx::abs(f64__f32_less_smallest_n)) )    ));
        REQUIRE(( ( std::abs(f64__f32_more_highest)       == nonstd::cx::abs(f64__f32_more_highest) )       || ( isnan(std::abs(f64__f32_more_highest))       && isnan(nonstd::cx::abs(f64__f32_more_highest)) )       ));
        REQUIRE(( ( std::abs(f64__f32_more_lowest)        == nonstd::cx::abs(f64__f32_more_lowest) )        || ( isnan(std::abs(f64__f32_more_lowest))        && isnan(nonstd::cx::abs(f64__f32_more_lowest)) )        ));
        REQUIRE(( ( std::abs(f64__f64_less_smallest_p)    == nonstd::cx::abs(f64__f64_less_smallest_p) )    || ( isnan(std::abs(f64__f64_less_smallest_p))    && isnan(nonstd::cx::abs(f64__f64_less_smallest_p)) )    ));
        REQUIRE(( ( std::abs(f64__f64_less_smallest_n)    == nonstd::cx::abs(f64__f64_less_smallest_n) )    || ( isnan(std::abs(f64__f64_less_smallest_n))    && isnan(nonstd::cx::abs(f64__f64_less_smallest_n)) )    ));
        REQUIRE(( ( std::abs(f64__f64_more_highest)       == nonstd::cx::abs(f64__f64_more_highest) )       || ( isnan(std::abs(f64__f64_more_highest))       && isnan(nonstd::cx::abs(f64__f64_more_highest)) )       ));
        REQUIRE(( ( std::abs(f64__f64_more_lowest)        == nonstd::cx::abs(f64__f64_more_lowest) )        || ( isnan(std::abs(f64__f64_more_lowest))        && isnan(nonstd::cx::abs(f64__f64_more_lowest)) )        ));
        REQUIRE(( ( std::abs(f64__f_long_less_smallest_p) == nonstd::cx::abs(f64__f_long_less_smallest_p) ) || ( isnan(std::abs(f64__f_long_less_smallest_p)) && isnan(nonstd::cx::abs(f64__f_long_less_smallest_p)) ) ));
        REQUIRE(( ( std::abs(f64__f_long_less_smallest_n) == nonstd::cx::abs(f64__f_long_less_smallest_n) ) || ( isnan(std::abs(f64__f_long_less_smallest_n)) && isnan(nonstd::cx::abs(f64__f_long_less_smallest_n)) ) ));
        REQUIRE(( ( std::abs(f64__f_long_more_highest)    == nonstd::cx::abs(f64__f_long_more_highest) )    || ( isnan(std::abs(f64__f_long_more_highest))    && isnan(nonstd::cx::abs(f64__f_long_more_highest)) )    ));
        REQUIRE(( ( std::abs(f64__f_long_more_lowest)     == nonstd::cx::abs(f64__f_long_more_lowest) )     || ( isnan(std::abs(f64__f_long_more_lowest))     && isnan(nonstd::cx::abs(f64__f_long_more_lowest)) )     ));

        REQUIRE(( ( std::abs(f_long__positive)               == nonstd::cx::abs(f_long__positive) )               || ( isnan(std::abs(f_long__positive))               && isnan(nonstd::cx::abs(f_long__positive)) )               ));
        REQUIRE(( ( std::abs(f_long__zero)                   == nonstd::cx::abs(f_long__zero) )                   || ( isnan(std::abs(f_long__zero))                   && isnan(nonstd::cx::abs(f_long__zero)) )                   ));
        REQUIRE(( ( std::abs(f_long__negative)               == nonstd::cx::abs(f_long__negative) )               || ( isnan(std::abs(f_long__negative))               && isnan(nonstd::cx::abs(f_long__negative)) )               ));
        REQUIRE(( ( std::abs(f_long__f32_smallest_p)         == nonstd::cx::abs(f_long__f32_smallest_p) )         || ( isnan(std::abs(f_long__f32_smallest_p))         && isnan(nonstd::cx::abs(f_long__f32_smallest_p)) )         ));
        REQUIRE(( ( std::abs(f_long__f32_smallest_n)         == nonstd::cx::abs(f_long__f32_smallest_n) )         || ( isnan(std::abs(f_long__f32_smallest_n))         && isnan(nonstd::cx::abs(f_long__f32_smallest_n)) )         ));
        REQUIRE(( ( std::abs(f_long__f32_highest)            == nonstd::cx::abs(f_long__f32_highest) )            || ( isnan(std::abs(f_long__f32_highest))            && isnan(nonstd::cx::abs(f_long__f32_highest)) )            ));
        REQUIRE(( ( std::abs(f_long__f32_lowest)             == nonstd::cx::abs(f_long__f32_lowest) )             || ( isnan(std::abs(f_long__f32_lowest))             && isnan(nonstd::cx::abs(f_long__f32_lowest)) )             ));
        REQUIRE(( ( std::abs(f_long__f64_smallest_p)         == nonstd::cx::abs(f_long__f64_smallest_p) )         || ( isnan(std::abs(f_long__f64_smallest_p))         && isnan(nonstd::cx::abs(f_long__f64_smallest_p)) )         ));
        REQUIRE(( ( std::abs(f_long__f64_smallest_n)         == nonstd::cx::abs(f_long__f64_smallest_n) )         || ( isnan(std::abs(f_long__f64_smallest_n))         && isnan(nonstd::cx::abs(f_long__f64_smallest_n)) )         ));
        REQUIRE(( ( std::abs(f_long__f64_highest)            == nonstd::cx::abs(f_long__f64_highest) )            || ( isnan(std::abs(f_long__f64_highest))            && isnan(nonstd::cx::abs(f_long__f64_highest)) )            ));
        REQUIRE(( ( std::abs(f_long__f64_lowest)             == nonstd::cx::abs(f_long__f64_lowest) )             || ( isnan(std::abs(f_long__f64_lowest))             && isnan(nonstd::cx::abs(f_long__f64_lowest)) )             ));
        REQUIRE(( ( std::abs(f_long__f_long_smallest_p)      == nonstd::cx::abs(f_long__f_long_smallest_p) )      || ( isnan(std::abs(f_long__f_long_smallest_p))      && isnan(nonstd::cx::abs(f_long__f_long_smallest_p)) )      ));
        REQUIRE(( ( std::abs(f_long__f_long_smallest_n)      == nonstd::cx::abs(f_long__f_long_smallest_n) )      || ( isnan(std::abs(f_long__f_long_smallest_n))      && isnan(nonstd::cx::abs(f_long__f_long_smallest_n)) )      ));
        REQUIRE(( ( std::abs(f_long__f_long_highest)         == nonstd::cx::abs(f_long__f_long_highest) )         || ( isnan(std::abs(f_long__f_long_highest))         && isnan(nonstd::cx::abs(f_long__f_long_highest)) )         ));
        REQUIRE(( ( std::abs(f_long__f_long_lowest)          == nonstd::cx::abs(f_long__f_long_lowest) )          || ( isnan(std::abs(f_long__f_long_lowest))          && isnan(nonstd::cx::abs(f_long__f_long_lowest)) )          ));
        REQUIRE(( ( std::abs(f_long__f32_nan)                == nonstd::cx::abs(f_long__f32_nan) )                || ( isnan(std::abs(f_long__f32_nan))                && isnan(nonstd::cx::abs(f_long__f32_nan)) )                ));
        REQUIRE(( ( std::abs(f_long__f64_nan)                == nonstd::cx::abs(f_long__f64_nan) )                || ( isnan(std::abs(f_long__f64_nan))                && isnan(nonstd::cx::abs(f_long__f64_nan)) )                ));
        REQUIRE(( ( std::abs(f_long__f_long_nan)             == nonstd::cx::abs(f_long__f_long_nan) )             || ( isnan(std::abs(f_long__f_long_nan))             && isnan(nonstd::cx::abs(f_long__f_long_nan)) )             ));
        REQUIRE(( ( std::abs(f_long__f32_p_inf)              == nonstd::cx::abs(f_long__f32_p_inf) )              || ( isnan(std::abs(f_long__f32_p_inf))              && isnan(nonstd::cx::abs(f_long__f32_p_inf)) )              ));
        REQUIRE(( ( std::abs(f_long__f32_n_inf)              == nonstd::cx::abs(f_long__f32_n_inf) )              || ( isnan(std::abs(f_long__f32_n_inf))              && isnan(nonstd::cx::abs(f_long__f32_n_inf)) )              ));
        REQUIRE(( ( std::abs(f_long__f64_p_inf)              == nonstd::cx::abs(f_long__f64_p_inf) )              || ( isnan(std::abs(f_long__f64_p_inf))              && isnan(nonstd::cx::abs(f_long__f64_p_inf)) )              ));
        REQUIRE(( ( std::abs(f_long__f64_n_inf)              == nonstd::cx::abs(f_long__f64_n_inf) )              || ( isnan(std::abs(f_long__f64_n_inf))              && isnan(nonstd::cx::abs(f_long__f64_n_inf)) )              ));
        REQUIRE(( ( std::abs(f_long__f_long_p_inf)           == nonstd::cx::abs(f_long__f_long_p_inf) )           || ( isnan(std::abs(f_long__f_long_p_inf))           && isnan(nonstd::cx::abs(f_long__f_long_p_inf)) )           ));
        REQUIRE(( ( std::abs(f_long__f_long_n_inf)           == nonstd::cx::abs(f_long__f_long_n_inf) )           || ( isnan(std::abs(f_long__f_long_n_inf))           && isnan(nonstd::cx::abs(f_long__f_long_n_inf)) )           ));
        REQUIRE(( ( std::abs(f_long__f32_less_smallest_p)    == nonstd::cx::abs(f_long__f32_less_smallest_p) )    || ( isnan(std::abs(f_long__f32_less_smallest_p))    && isnan(nonstd::cx::abs(f_long__f32_less_smallest_p)) )    ));
        REQUIRE(( ( std::abs(f_long__f32_less_smallest_n)    == nonstd::cx::abs(f_long__f32_less_smallest_n) )    || ( isnan(std::abs(f_long__f32_less_smallest_n))    && isnan(nonstd::cx::abs(f_long__f32_less_smallest_n)) )    ));
        REQUIRE(( ( std::abs(f_long__f32_more_highest)       == nonstd::cx::abs(f_long__f32_more_highest) )       || ( isnan(std::abs(f_long__f32_more_highest))       && isnan(nonstd::cx::abs(f_long__f32_more_highest)) )       ));
        REQUIRE(( ( std::abs(f_long__f32_more_lowest)        == nonstd::cx::abs(f_long__f32_more_lowest) )        || ( isnan(std::abs(f_long__f32_more_lowest))        && isnan(nonstd::cx::abs(f_long__f32_more_lowest)) )        ));
        REQUIRE(( ( std::abs(f_long__f64_less_smallest_p)    == nonstd::cx::abs(f_long__f64_less_smallest_p) )    || ( isnan(std::abs(f_long__f64_less_smallest_p))    && isnan(nonstd::cx::abs(f_long__f64_less_smallest_p)) )    ));
        REQUIRE(( ( std::abs(f_long__f64_less_smallest_n)    == nonstd::cx::abs(f_long__f64_less_smallest_n) )    || ( isnan(std::abs(f_long__f64_less_smallest_n))    && isnan(nonstd::cx::abs(f_long__f64_less_smallest_n)) )    ));
        REQUIRE(( ( std::abs(f_long__f64_more_highest)       == nonstd::cx::abs(f_long__f64_more_highest) )       || ( isnan(std::abs(f_long__f64_more_highest))       && isnan(nonstd::cx::abs(f_long__f64_more_highest)) )       ));
        REQUIRE(( ( std::abs(f_long__f64_more_lowest)        == nonstd::cx::abs(f_long__f64_more_lowest) )        || ( isnan(std::abs(f_long__f64_more_lowest))        && isnan(nonstd::cx::abs(f_long__f64_more_lowest)) )        ));
        REQUIRE(( ( std::abs(f_long__f_long_less_smallest_p) == nonstd::cx::abs(f_long__f_long_less_smallest_p) ) || ( isnan(std::abs(f_long__f_long_less_smallest_p)) && isnan(nonstd::cx::abs(f_long__f_long_less_smallest_p)) ) ));
        REQUIRE(( ( std::abs(f_long__f_long_less_smallest_n) == nonstd::cx::abs(f_long__f_long_less_smallest_n) ) || ( isnan(std::abs(f_long__f_long_less_smallest_n)) && isnan(nonstd::cx::abs(f_long__f_long_less_smallest_n)) ) ));
        REQUIRE(( ( std::abs(f_long__f_long_more_highest)    == nonstd::cx::abs(f_long__f_long_more_highest) )    || ( isnan(std::abs(f_long__f_long_more_highest))    && isnan(nonstd::cx::abs(f_long__f_long_more_highest)) )    ));
        REQUIRE(( ( std::abs(f_long__f_long_more_lowest)     == nonstd::cx::abs(f_long__f_long_more_lowest) )     || ( isnan(std::abs(f_long__f_long_more_lowest))     && isnan(nonstd::cx::abs(f_long__f_long_more_lowest)) )     ));

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
        REQUIRE(( ( std::fabs(f32__f32_smallest_p)         == nonstd::cx::fabs(f32__f32_smallest_p) )         || ( isnan(std::fabs(f32__f32_smallest_p))         && isnan(nonstd::cx::fabs(f32__f32_smallest_p)) )         ));
        REQUIRE(( ( std::fabs(f32__f32_smallest_n)         == nonstd::cx::fabs(f32__f32_smallest_n) )         || ( isnan(std::fabs(f32__f32_smallest_n))         && isnan(nonstd::cx::fabs(f32__f32_smallest_n)) )         ));
        REQUIRE(( ( std::fabs(f32__f32_highest)            == nonstd::cx::fabs(f32__f32_highest) )            || ( isnan(std::fabs(f32__f32_highest))            && isnan(nonstd::cx::fabs(f32__f32_highest)) )            ));
        REQUIRE(( ( std::fabs(f32__f32_lowest)             == nonstd::cx::fabs(f32__f32_lowest) )             || ( isnan(std::fabs(f32__f32_lowest))             && isnan(nonstd::cx::fabs(f32__f32_lowest)) )             ));
        REQUIRE(( ( std::fabs(f32__f64_smallest_p)         == nonstd::cx::fabs(f32__f64_smallest_p) )         || ( isnan(std::fabs(f32__f64_smallest_p))         && isnan(nonstd::cx::fabs(f32__f64_smallest_p)) )         ));
        REQUIRE(( ( std::fabs(f32__f64_smallest_n)         == nonstd::cx::fabs(f32__f64_smallest_n) )         || ( isnan(std::fabs(f32__f64_smallest_n))         && isnan(nonstd::cx::fabs(f32__f64_smallest_n)) )         ));
    //  REQUIRE(( ( std::fabs(f32__f64_highest)            == nonstd::cx::fabs(f32__f64_highest) )            || ( isnan(std::fabs(f32__f64_highest))            && isnan(nonstd::cx::fabs(f32__f64_highest)) )            ));
    //  REQUIRE(( ( std::fabs(f32__f64_lowest)             == nonstd::cx::fabs(f32__f64_lowest) )             || ( isnan(std::fabs(f32__f64_lowest))             && isnan(nonstd::cx::fabs(f32__f64_lowest)) )             ));
        REQUIRE(( ( std::fabs(f32__f_long_smallest_p)      == nonstd::cx::fabs(f32__f_long_smallest_p) )      || ( isnan(std::fabs(f32__f_long_smallest_p))      && isnan(nonstd::cx::fabs(f32__f_long_smallest_p)) )      ));
        REQUIRE(( ( std::fabs(f32__f_long_smallest_n)      == nonstd::cx::fabs(f32__f_long_smallest_n) )      || ( isnan(std::fabs(f32__f_long_smallest_n))      && isnan(nonstd::cx::fabs(f32__f_long_smallest_n)) )      ));
    //  REQUIRE(( ( std::fabs(f32__f_long_highest)         == nonstd::cx::fabs(f32__f_long_highest) )         || ( isnan(std::fabs(f32__f_long_highest))         && isnan(nonstd::cx::fabs(f32__f_long_highest)) )         ));
    //  REQUIRE(( ( std::fabs(f32__f_long_lowest)          == nonstd::cx::fabs(f32__f_long_lowest) )          || ( isnan(std::fabs(f32__f_long_lowest))          && isnan(nonstd::cx::fabs(f32__f_long_lowest)) )          ));
        REQUIRE(( ( std::fabs(f32__f32_nan)                == nonstd::cx::fabs(f32__f32_nan) )                || ( isnan(std::fabs(f32__f32_nan))                && isnan(nonstd::cx::fabs(f32__f32_nan)) )                ));
        REQUIRE(( ( std::fabs(f32__f64_nan)                == nonstd::cx::fabs(f32__f64_nan) )                || ( isnan(std::fabs(f32__f64_nan))                && isnan(nonstd::cx::fabs(f32__f64_nan)) )                ));
        REQUIRE(( ( std::fabs(f32__f_long_nan)             == nonstd::cx::fabs(f32__f_long_nan) )             || ( isnan(std::fabs(f32__f_long_nan))             && isnan(nonstd::cx::fabs(f32__f_long_nan)) )             ));
        REQUIRE(( ( std::fabs(f32__f32_p_inf)              == nonstd::cx::fabs(f32__f32_p_inf) )              || ( isnan(std::fabs(f32__f32_p_inf))              && isnan(nonstd::cx::fabs(f32__f32_p_inf)) )              ));
        REQUIRE(( ( std::fabs(f32__f32_n_inf)              == nonstd::cx::fabs(f32__f32_n_inf) )              || ( isnan(std::fabs(f32__f32_n_inf))              && isnan(nonstd::cx::fabs(f32__f32_n_inf)) )              ));
        REQUIRE(( ( std::fabs(f32__f64_p_inf)              == nonstd::cx::fabs(f32__f64_p_inf) )              || ( isnan(std::fabs(f32__f64_p_inf))              && isnan(nonstd::cx::fabs(f32__f64_p_inf)) )              ));
        REQUIRE(( ( std::fabs(f32__f64_n_inf)              == nonstd::cx::fabs(f32__f64_n_inf) )              || ( isnan(std::fabs(f32__f64_n_inf))              && isnan(nonstd::cx::fabs(f32__f64_n_inf)) )              ));
        REQUIRE(( ( std::fabs(f32__f_long_p_inf)           == nonstd::cx::fabs(f32__f_long_p_inf) )           || ( isnan(std::fabs(f32__f_long_p_inf))           && isnan(nonstd::cx::fabs(f32__f_long_p_inf)) )           ));
        REQUIRE(( ( std::fabs(f32__f_long_n_inf)           == nonstd::cx::fabs(f32__f_long_n_inf) )           || ( isnan(std::fabs(f32__f_long_n_inf))           && isnan(nonstd::cx::fabs(f32__f_long_n_inf)) )           ));
        REQUIRE(( ( std::fabs(f32__f32_less_smallest_p)    == nonstd::cx::fabs(f32__f32_less_smallest_p) )    || ( isnan(std::fabs(f32__f32_less_smallest_p))    && isnan(nonstd::cx::fabs(f32__f32_less_smallest_p)) )    ));
        REQUIRE(( ( std::fabs(f32__f32_less_smallest_n)    == nonstd::cx::fabs(f32__f32_less_smallest_n) )    || ( isnan(std::fabs(f32__f32_less_smallest_n))    && isnan(nonstd::cx::fabs(f32__f32_less_smallest_n)) )    ));
        REQUIRE(( ( std::fabs(f32__f32_more_highest)       == nonstd::cx::fabs(f32__f32_more_highest) )       || ( isnan(std::fabs(f32__f32_more_highest))       && isnan(nonstd::cx::fabs(f32__f32_more_highest)) )       ));
        REQUIRE(( ( std::fabs(f32__f32_more_lowest)        == nonstd::cx::fabs(f32__f32_more_lowest) )        || ( isnan(std::fabs(f32__f32_more_lowest))        && isnan(nonstd::cx::fabs(f32__f32_more_lowest)) )        ));
        REQUIRE(( ( std::fabs(f32__f64_less_smallest_p)    == nonstd::cx::fabs(f32__f64_less_smallest_p) )    || ( isnan(std::fabs(f32__f64_less_smallest_p))    && isnan(nonstd::cx::fabs(f32__f64_less_smallest_p)) )    ));
        REQUIRE(( ( std::fabs(f32__f64_less_smallest_n)    == nonstd::cx::fabs(f32__f64_less_smallest_n) )    || ( isnan(std::fabs(f32__f64_less_smallest_n))    && isnan(nonstd::cx::fabs(f32__f64_less_smallest_n)) )    ));
        REQUIRE(( ( std::fabs(f32__f64_more_highest)       == nonstd::cx::fabs(f32__f64_more_highest) )       || ( isnan(std::fabs(f32__f64_more_highest))       && isnan(nonstd::cx::fabs(f32__f64_more_highest)) )       ));
        REQUIRE(( ( std::fabs(f32__f64_more_lowest)        == nonstd::cx::fabs(f32__f64_more_lowest) )        || ( isnan(std::fabs(f32__f64_more_lowest))        && isnan(nonstd::cx::fabs(f32__f64_more_lowest)) )        ));
        REQUIRE(( ( std::fabs(f32__f_long_less_smallest_p) == nonstd::cx::fabs(f32__f_long_less_smallest_p) ) || ( isnan(std::fabs(f32__f_long_less_smallest_p)) && isnan(nonstd::cx::fabs(f32__f_long_less_smallest_p)) ) ));
        REQUIRE(( ( std::fabs(f32__f_long_less_smallest_n) == nonstd::cx::fabs(f32__f_long_less_smallest_n) ) || ( isnan(std::fabs(f32__f_long_less_smallest_n)) && isnan(nonstd::cx::fabs(f32__f_long_less_smallest_n)) ) ));
        REQUIRE(( ( std::fabs(f32__f_long_more_highest)    == nonstd::cx::fabs(f32__f_long_more_highest) )    || ( isnan(std::fabs(f32__f_long_more_highest))    && isnan(nonstd::cx::fabs(f32__f_long_more_highest)) )    ));
        REQUIRE(( ( std::fabs(f32__f_long_more_lowest)     == nonstd::cx::fabs(f32__f_long_more_lowest) )     || ( isnan(std::fabs(f32__f_long_more_lowest))     && isnan(nonstd::cx::fabs(f32__f_long_more_lowest)) )     ));

        REQUIRE(( ( std::fabs(f64__positive)               == nonstd::cx::fabs(f64__positive) )               || ( isnan(std::fabs(f64__positive))               && isnan(nonstd::cx::fabs(f64__positive)) )               ));
        REQUIRE(( ( std::fabs(f64__zero)                   == nonstd::cx::fabs(f64__zero) )                   || ( isnan(std::fabs(f64__zero))                   && isnan(nonstd::cx::fabs(f64__zero)) )                   ));
        REQUIRE(( ( std::fabs(f64__negative)               == nonstd::cx::fabs(f64__negative) )               || ( isnan(std::fabs(f64__negative))               && isnan(nonstd::cx::fabs(f64__negative)) )               ));
        REQUIRE(( ( std::fabs(f64__f32_smallest_p)         == nonstd::cx::fabs(f64__f32_smallest_p) )         || ( isnan(std::fabs(f64__f32_smallest_p))         && isnan(nonstd::cx::fabs(f64__f32_smallest_p)) )         ));
        REQUIRE(( ( std::fabs(f64__f32_smallest_n)         == nonstd::cx::fabs(f64__f32_smallest_n) )         || ( isnan(std::fabs(f64__f32_smallest_n))         && isnan(nonstd::cx::fabs(f64__f32_smallest_n)) )         ));
        REQUIRE(( ( std::fabs(f64__f32_highest)            == nonstd::cx::fabs(f64__f32_highest) )            || ( isnan(std::fabs(f64__f32_highest))            && isnan(nonstd::cx::fabs(f64__f32_highest)) )            ));
        REQUIRE(( ( std::fabs(f64__f32_lowest)             == nonstd::cx::fabs(f64__f32_lowest) )             || ( isnan(std::fabs(f64__f32_lowest))             && isnan(nonstd::cx::fabs(f64__f32_lowest)) )             ));
        REQUIRE(( ( std::fabs(f64__f64_smallest_p)         == nonstd::cx::fabs(f64__f64_smallest_p) )         || ( isnan(std::fabs(f64__f64_smallest_p))         && isnan(nonstd::cx::fabs(f64__f64_smallest_p)) )         ));
        REQUIRE(( ( std::fabs(f64__f64_smallest_n)         == nonstd::cx::fabs(f64__f64_smallest_n) )         || ( isnan(std::fabs(f64__f64_smallest_n))         && isnan(nonstd::cx::fabs(f64__f64_smallest_n)) )         ));
        REQUIRE(( ( std::fabs(f64__f64_highest)            == nonstd::cx::fabs(f64__f64_highest) )            || ( isnan(std::fabs(f64__f64_highest))            && isnan(nonstd::cx::fabs(f64__f64_highest)) )            ));
        REQUIRE(( ( std::fabs(f64__f64_lowest)             == nonstd::cx::fabs(f64__f64_lowest) )             || ( isnan(std::fabs(f64__f64_lowest))             && isnan(nonstd::cx::fabs(f64__f64_lowest)) )             ));
        REQUIRE(( ( std::fabs(f64__f_long_smallest_p)      == nonstd::cx::fabs(f64__f_long_smallest_p) )      || ( isnan(std::fabs(f64__f_long_smallest_p))      && isnan(nonstd::cx::fabs(f64__f_long_smallest_p)) )      ));
        REQUIRE(( ( std::fabs(f64__f_long_smallest_n)      == nonstd::cx::fabs(f64__f_long_smallest_n) )      || ( isnan(std::fabs(f64__f_long_smallest_n))      && isnan(nonstd::cx::fabs(f64__f_long_smallest_n)) )      ));
    //  REQUIRE(( ( std::fabs(f64__f_long_highest)         == nonstd::cx::fabs(f64__f_long_highest) )         || ( isnan(std::fabs(f64__f_long_highest))         && isnan(nonstd::cx::fabs(f64__f_long_highest)) )         ));
    //  REQUIRE(( ( std::fabs(f64__f_long_lowest)          == nonstd::cx::fabs(f64__f_long_lowest) )          || ( isnan(std::fabs(f64__f_long_lowest))          && isnan(nonstd::cx::fabs(f64__f_long_lowest)) )          ));
        REQUIRE(( ( std::fabs(f64__f32_nan)                == nonstd::cx::fabs(f64__f32_nan) )                || ( isnan(std::fabs(f64__f32_nan))                && isnan(nonstd::cx::fabs(f64__f32_nan)) )                ));
        REQUIRE(( ( std::fabs(f64__f64_nan)                == nonstd::cx::fabs(f64__f64_nan) )                || ( isnan(std::fabs(f64__f64_nan))                && isnan(nonstd::cx::fabs(f64__f64_nan)) )                ));
        REQUIRE(( ( std::fabs(f64__f_long_nan)             == nonstd::cx::fabs(f64__f_long_nan) )             || ( isnan(std::fabs(f64__f_long_nan))             && isnan(nonstd::cx::fabs(f64__f_long_nan)) )             ));
        REQUIRE(( ( std::fabs(f64__f32_p_inf)              == nonstd::cx::fabs(f64__f32_p_inf) )              || ( isnan(std::fabs(f64__f32_p_inf))              && isnan(nonstd::cx::fabs(f64__f32_p_inf)) )              ));
        REQUIRE(( ( std::fabs(f64__f32_n_inf)              == nonstd::cx::fabs(f64__f32_n_inf) )              || ( isnan(std::fabs(f64__f32_n_inf))              && isnan(nonstd::cx::fabs(f64__f32_n_inf)) )              ));
        REQUIRE(( ( std::fabs(f64__f64_p_inf)              == nonstd::cx::fabs(f64__f64_p_inf) )              || ( isnan(std::fabs(f64__f64_p_inf))              && isnan(nonstd::cx::fabs(f64__f64_p_inf)) )              ));
        REQUIRE(( ( std::fabs(f64__f64_n_inf)              == nonstd::cx::fabs(f64__f64_n_inf) )              || ( isnan(std::fabs(f64__f64_n_inf))              && isnan(nonstd::cx::fabs(f64__f64_n_inf)) )              ));
        REQUIRE(( ( std::fabs(f64__f_long_p_inf)           == nonstd::cx::fabs(f64__f_long_p_inf) )           || ( isnan(std::fabs(f64__f_long_p_inf))           && isnan(nonstd::cx::fabs(f64__f_long_p_inf)) )           ));
        REQUIRE(( ( std::fabs(f64__f_long_n_inf)           == nonstd::cx::fabs(f64__f_long_n_inf) )           || ( isnan(std::fabs(f64__f_long_n_inf))           && isnan(nonstd::cx::fabs(f64__f_long_n_inf)) )           ));
        REQUIRE(( ( std::fabs(f64__f32_less_smallest_p)    == nonstd::cx::fabs(f64__f32_less_smallest_p) )    || ( isnan(std::fabs(f64__f32_less_smallest_p))    && isnan(nonstd::cx::fabs(f64__f32_less_smallest_p)) )    ));
        REQUIRE(( ( std::fabs(f64__f32_less_smallest_n)    == nonstd::cx::fabs(f64__f32_less_smallest_n) )    || ( isnan(std::fabs(f64__f32_less_smallest_n))    && isnan(nonstd::cx::fabs(f64__f32_less_smallest_n)) )    ));
        REQUIRE(( ( std::fabs(f64__f32_more_highest)       == nonstd::cx::fabs(f64__f32_more_highest) )       || ( isnan(std::fabs(f64__f32_more_highest))       && isnan(nonstd::cx::fabs(f64__f32_more_highest)) )       ));
        REQUIRE(( ( std::fabs(f64__f32_more_lowest)        == nonstd::cx::fabs(f64__f32_more_lowest) )        || ( isnan(std::fabs(f64__f32_more_lowest))        && isnan(nonstd::cx::fabs(f64__f32_more_lowest)) )        ));
        REQUIRE(( ( std::fabs(f64__f64_less_smallest_p)    == nonstd::cx::fabs(f64__f64_less_smallest_p) )    || ( isnan(std::fabs(f64__f64_less_smallest_p))    && isnan(nonstd::cx::fabs(f64__f64_less_smallest_p)) )    ));
        REQUIRE(( ( std::fabs(f64__f64_less_smallest_n)    == nonstd::cx::fabs(f64__f64_less_smallest_n) )    || ( isnan(std::fabs(f64__f64_less_smallest_n))    && isnan(nonstd::cx::fabs(f64__f64_less_smallest_n)) )    ));
        REQUIRE(( ( std::fabs(f64__f64_more_highest)       == nonstd::cx::fabs(f64__f64_more_highest) )       || ( isnan(std::fabs(f64__f64_more_highest))       && isnan(nonstd::cx::fabs(f64__f64_more_highest)) )       ));
        REQUIRE(( ( std::fabs(f64__f64_more_lowest)        == nonstd::cx::fabs(f64__f64_more_lowest) )        || ( isnan(std::fabs(f64__f64_more_lowest))        && isnan(nonstd::cx::fabs(f64__f64_more_lowest)) )        ));
        REQUIRE(( ( std::fabs(f64__f_long_less_smallest_p) == nonstd::cx::fabs(f64__f_long_less_smallest_p) ) || ( isnan(std::fabs(f64__f_long_less_smallest_p)) && isnan(nonstd::cx::fabs(f64__f_long_less_smallest_p)) ) ));
        REQUIRE(( ( std::fabs(f64__f_long_less_smallest_n) == nonstd::cx::fabs(f64__f_long_less_smallest_n) ) || ( isnan(std::fabs(f64__f_long_less_smallest_n)) && isnan(nonstd::cx::fabs(f64__f_long_less_smallest_n)) ) ));
        REQUIRE(( ( std::fabs(f64__f_long_more_highest)    == nonstd::cx::fabs(f64__f_long_more_highest) )    || ( isnan(std::fabs(f64__f_long_more_highest))    && isnan(nonstd::cx::fabs(f64__f_long_more_highest)) )    ));
        REQUIRE(( ( std::fabs(f64__f_long_more_lowest)     == nonstd::cx::fabs(f64__f_long_more_lowest) )     || ( isnan(std::fabs(f64__f_long_more_lowest))     && isnan(nonstd::cx::fabs(f64__f_long_more_lowest)) )     ));

        REQUIRE(( ( std::fabs(f_long__positive)               == nonstd::cx::fabs(f_long__positive) )               || ( isnan(std::fabs(f_long__positive))               && isnan(nonstd::cx::fabs(f_long__positive)) )               ));
        REQUIRE(( ( std::fabs(f_long__zero)                   == nonstd::cx::fabs(f_long__zero) )                   || ( isnan(std::fabs(f_long__zero))                   && isnan(nonstd::cx::fabs(f_long__zero)) )                   ));
        REQUIRE(( ( std::fabs(f_long__negative)               == nonstd::cx::fabs(f_long__negative) )               || ( isnan(std::fabs(f_long__negative))               && isnan(nonstd::cx::fabs(f_long__negative)) )               ));
        REQUIRE(( ( std::fabs(f_long__f32_smallest_p)         == nonstd::cx::fabs(f_long__f32_smallest_p) )         || ( isnan(std::fabs(f_long__f32_smallest_p))         && isnan(nonstd::cx::fabs(f_long__f32_smallest_p)) )         ));
        REQUIRE(( ( std::fabs(f_long__f32_smallest_n)         == nonstd::cx::fabs(f_long__f32_smallest_n) )         || ( isnan(std::fabs(f_long__f32_smallest_n))         && isnan(nonstd::cx::fabs(f_long__f32_smallest_n)) )         ));
        REQUIRE(( ( std::fabs(f_long__f32_highest)            == nonstd::cx::fabs(f_long__f32_highest) )            || ( isnan(std::fabs(f_long__f32_highest))            && isnan(nonstd::cx::fabs(f_long__f32_highest)) )            ));
        REQUIRE(( ( std::fabs(f_long__f32_lowest)             == nonstd::cx::fabs(f_long__f32_lowest) )             || ( isnan(std::fabs(f_long__f32_lowest))             && isnan(nonstd::cx::fabs(f_long__f32_lowest)) )             ));
        REQUIRE(( ( std::fabs(f_long__f64_smallest_p)         == nonstd::cx::fabs(f_long__f64_smallest_p) )         || ( isnan(std::fabs(f_long__f64_smallest_p))         && isnan(nonstd::cx::fabs(f_long__f64_smallest_p)) )         ));
        REQUIRE(( ( std::fabs(f_long__f64_smallest_n)         == nonstd::cx::fabs(f_long__f64_smallest_n) )         || ( isnan(std::fabs(f_long__f64_smallest_n))         && isnan(nonstd::cx::fabs(f_long__f64_smallest_n)) )         ));
        REQUIRE(( ( std::fabs(f_long__f64_highest)            == nonstd::cx::fabs(f_long__f64_highest) )            || ( isnan(std::fabs(f_long__f64_highest))            && isnan(nonstd::cx::fabs(f_long__f64_highest)) )            ));
        REQUIRE(( ( std::fabs(f_long__f64_lowest)             == nonstd::cx::fabs(f_long__f64_lowest) )             || ( isnan(std::fabs(f_long__f64_lowest))             && isnan(nonstd::cx::fabs(f_long__f64_lowest)) )             ));
        REQUIRE(( ( std::fabs(f_long__f_long_smallest_p)      == nonstd::cx::fabs(f_long__f_long_smallest_p) )      || ( isnan(std::fabs(f_long__f_long_smallest_p))      && isnan(nonstd::cx::fabs(f_long__f_long_smallest_p)) )      ));
        REQUIRE(( ( std::fabs(f_long__f_long_smallest_n)      == nonstd::cx::fabs(f_long__f_long_smallest_n) )      || ( isnan(std::fabs(f_long__f_long_smallest_n))      && isnan(nonstd::cx::fabs(f_long__f_long_smallest_n)) )      ));
        REQUIRE(( ( std::fabs(f_long__f_long_highest)         == nonstd::cx::fabs(f_long__f_long_highest) )         || ( isnan(std::fabs(f_long__f_long_highest))         && isnan(nonstd::cx::fabs(f_long__f_long_highest)) )         ));
        REQUIRE(( ( std::fabs(f_long__f_long_lowest)          == nonstd::cx::fabs(f_long__f_long_lowest) )          || ( isnan(std::fabs(f_long__f_long_lowest))          && isnan(nonstd::cx::fabs(f_long__f_long_lowest)) )          ));
        REQUIRE(( ( std::fabs(f_long__f32_nan)                == nonstd::cx::fabs(f_long__f32_nan) )                || ( isnan(std::fabs(f_long__f32_nan))                && isnan(nonstd::cx::fabs(f_long__f32_nan)) )                ));
        REQUIRE(( ( std::fabs(f_long__f64_nan)                == nonstd::cx::fabs(f_long__f64_nan) )                || ( isnan(std::fabs(f_long__f64_nan))                && isnan(nonstd::cx::fabs(f_long__f64_nan)) )                ));
        REQUIRE(( ( std::fabs(f_long__f_long_nan)             == nonstd::cx::fabs(f_long__f_long_nan) )             || ( isnan(std::fabs(f_long__f_long_nan))             && isnan(nonstd::cx::fabs(f_long__f_long_nan)) )             ));
        REQUIRE(( ( std::fabs(f_long__f32_p_inf)              == nonstd::cx::fabs(f_long__f32_p_inf) )              || ( isnan(std::fabs(f_long__f32_p_inf))              && isnan(nonstd::cx::fabs(f_long__f32_p_inf)) )              ));
        REQUIRE(( ( std::fabs(f_long__f32_n_inf)              == nonstd::cx::fabs(f_long__f32_n_inf) )              || ( isnan(std::fabs(f_long__f32_n_inf))              && isnan(nonstd::cx::fabs(f_long__f32_n_inf)) )              ));
        REQUIRE(( ( std::fabs(f_long__f64_p_inf)              == nonstd::cx::fabs(f_long__f64_p_inf) )              || ( isnan(std::fabs(f_long__f64_p_inf))              && isnan(nonstd::cx::fabs(f_long__f64_p_inf)) )              ));
        REQUIRE(( ( std::fabs(f_long__f64_n_inf)              == nonstd::cx::fabs(f_long__f64_n_inf) )              || ( isnan(std::fabs(f_long__f64_n_inf))              && isnan(nonstd::cx::fabs(f_long__f64_n_inf)) )              ));
        REQUIRE(( ( std::fabs(f_long__f_long_p_inf)           == nonstd::cx::fabs(f_long__f_long_p_inf) )           || ( isnan(std::fabs(f_long__f_long_p_inf))           && isnan(nonstd::cx::fabs(f_long__f_long_p_inf)) )           ));
        REQUIRE(( ( std::fabs(f_long__f_long_n_inf)           == nonstd::cx::fabs(f_long__f_long_n_inf) )           || ( isnan(std::fabs(f_long__f_long_n_inf))           && isnan(nonstd::cx::fabs(f_long__f_long_n_inf)) )           ));
        REQUIRE(( ( std::fabs(f_long__f32_less_smallest_p)    == nonstd::cx::fabs(f_long__f32_less_smallest_p) )    || ( isnan(std::fabs(f_long__f32_less_smallest_p))    && isnan(nonstd::cx::fabs(f_long__f32_less_smallest_p)) )    ));
        REQUIRE(( ( std::fabs(f_long__f32_less_smallest_n)    == nonstd::cx::fabs(f_long__f32_less_smallest_n) )    || ( isnan(std::fabs(f_long__f32_less_smallest_n))    && isnan(nonstd::cx::fabs(f_long__f32_less_smallest_n)) )    ));
        REQUIRE(( ( std::fabs(f_long__f32_more_highest)       == nonstd::cx::fabs(f_long__f32_more_highest) )       || ( isnan(std::fabs(f_long__f32_more_highest))       && isnan(nonstd::cx::fabs(f_long__f32_more_highest)) )       ));
        REQUIRE(( ( std::fabs(f_long__f32_more_lowest)        == nonstd::cx::fabs(f_long__f32_more_lowest) )        || ( isnan(std::fabs(f_long__f32_more_lowest))        && isnan(nonstd::cx::fabs(f_long__f32_more_lowest)) )        ));
        REQUIRE(( ( std::fabs(f_long__f64_less_smallest_p)    == nonstd::cx::fabs(f_long__f64_less_smallest_p) )    || ( isnan(std::fabs(f_long__f64_less_smallest_p))    && isnan(nonstd::cx::fabs(f_long__f64_less_smallest_p)) )    ));
        REQUIRE(( ( std::fabs(f_long__f64_less_smallest_n)    == nonstd::cx::fabs(f_long__f64_less_smallest_n) )    || ( isnan(std::fabs(f_long__f64_less_smallest_n))    && isnan(nonstd::cx::fabs(f_long__f64_less_smallest_n)) )    ));
        REQUIRE(( ( std::fabs(f_long__f64_more_highest)       == nonstd::cx::fabs(f_long__f64_more_highest) )       || ( isnan(std::fabs(f_long__f64_more_highest))       && isnan(nonstd::cx::fabs(f_long__f64_more_highest)) )       ));
        REQUIRE(( ( std::fabs(f_long__f64_more_lowest)        == nonstd::cx::fabs(f_long__f64_more_lowest) )        || ( isnan(std::fabs(f_long__f64_more_lowest))        && isnan(nonstd::cx::fabs(f_long__f64_more_lowest)) )        ));
        REQUIRE(( ( std::fabs(f_long__f_long_less_smallest_p) == nonstd::cx::fabs(f_long__f_long_less_smallest_p) ) || ( isnan(std::fabs(f_long__f_long_less_smallest_p)) && isnan(nonstd::cx::fabs(f_long__f_long_less_smallest_p)) ) ));
        REQUIRE(( ( std::fabs(f_long__f_long_less_smallest_n) == nonstd::cx::fabs(f_long__f_long_less_smallest_n) ) || ( isnan(std::fabs(f_long__f_long_less_smallest_n)) && isnan(nonstd::cx::fabs(f_long__f_long_less_smallest_n)) ) ));
        REQUIRE(( ( std::fabs(f_long__f_long_more_highest)    == nonstd::cx::fabs(f_long__f_long_more_highest) )    || ( isnan(std::fabs(f_long__f_long_more_highest))    && isnan(nonstd::cx::fabs(f_long__f_long_more_highest)) )    ));
        REQUIRE(( ( std::fabs(f_long__f_long_more_lowest)     == nonstd::cx::fabs(f_long__f_long_more_lowest) )     || ( isnan(std::fabs(f_long__f_long_more_lowest))     && isnan(nonstd::cx::fabs(f_long__f_long_more_lowest)) )     ));

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
        REQUIRE(( ( std::ceil(f32__f32_smallest_p)         == nonstd::cx::ceil(f32__f32_smallest_p) )         || ( isnan(std::ceil(f32__f32_smallest_p))         && isnan(nonstd::cx::ceil(f32__f32_smallest_p)) )         ));
        REQUIRE(( ( std::ceil(f32__f32_smallest_n)         == nonstd::cx::ceil(f32__f32_smallest_n) )         || ( isnan(std::ceil(f32__f32_smallest_n))         && isnan(nonstd::cx::ceil(f32__f32_smallest_n)) )         ));
        REQUIRE(( ( std::ceil(f32__f32_highest)            == nonstd::cx::ceil(f32__f32_highest) )            || ( isnan(std::ceil(f32__f32_highest))            && isnan(nonstd::cx::ceil(f32__f32_highest)) )            ));
        REQUIRE(( ( std::ceil(f32__f32_lowest)             == nonstd::cx::ceil(f32__f32_lowest) )             || ( isnan(std::ceil(f32__f32_lowest))             && isnan(nonstd::cx::ceil(f32__f32_lowest)) )             ));
        REQUIRE(( ( std::ceil(f32__f64_smallest_p)         == nonstd::cx::ceil(f32__f64_smallest_p) )         || ( isnan(std::ceil(f32__f64_smallest_p))         && isnan(nonstd::cx::ceil(f32__f64_smallest_p)) )         ));
        REQUIRE(( ( std::ceil(f32__f64_smallest_n)         == nonstd::cx::ceil(f32__f64_smallest_n) )         || ( isnan(std::ceil(f32__f64_smallest_n))         && isnan(nonstd::cx::ceil(f32__f64_smallest_n)) )         ));
    //  REQUIRE(( ( std::ceil(f32__f64_highest)            == nonstd::cx::ceil(f32__f64_highest) )            || ( isnan(std::ceil(f32__f64_highest))            && isnan(nonstd::cx::ceil(f32__f64_highest)) )            ));
    //  REQUIRE(( ( std::ceil(f32__f64_lowest)             == nonstd::cx::ceil(f32__f64_lowest) )             || ( isnan(std::ceil(f32__f64_lowest))             && isnan(nonstd::cx::ceil(f32__f64_lowest)) )             ));
        REQUIRE(( ( std::ceil(f32__f_long_smallest_p)      == nonstd::cx::ceil(f32__f_long_smallest_p) )      || ( isnan(std::ceil(f32__f_long_smallest_p))      && isnan(nonstd::cx::ceil(f32__f_long_smallest_p)) )      ));
        REQUIRE(( ( std::ceil(f32__f_long_smallest_n)      == nonstd::cx::ceil(f32__f_long_smallest_n) )      || ( isnan(std::ceil(f32__f_long_smallest_n))      && isnan(nonstd::cx::ceil(f32__f_long_smallest_n)) )      ));
    //  REQUIRE(( ( std::ceil(f32__f_long_highest)         == nonstd::cx::ceil(f32__f_long_highest) )         || ( isnan(std::ceil(f32__f_long_highest))         && isnan(nonstd::cx::ceil(f32__f_long_highest)) )         ));
    //  REQUIRE(( ( std::ceil(f32__f_long_lowest)          == nonstd::cx::ceil(f32__f_long_lowest) )          || ( isnan(std::ceil(f32__f_long_lowest))          && isnan(nonstd::cx::ceil(f32__f_long_lowest)) )          ));
        REQUIRE(( ( std::ceil(f32__f32_nan)                == nonstd::cx::ceil(f32__f32_nan) )                || ( isnan(std::ceil(f32__f32_nan))                && isnan(nonstd::cx::ceil(f32__f32_nan)) )                ));
        REQUIRE(( ( std::ceil(f32__f64_nan)                == nonstd::cx::ceil(f32__f64_nan) )                || ( isnan(std::ceil(f32__f64_nan))                && isnan(nonstd::cx::ceil(f32__f64_nan)) )                ));
        REQUIRE(( ( std::ceil(f32__f_long_nan)             == nonstd::cx::ceil(f32__f_long_nan) )             || ( isnan(std::ceil(f32__f_long_nan))             && isnan(nonstd::cx::ceil(f32__f_long_nan)) )             ));
        REQUIRE(( ( std::ceil(f32__f32_p_inf)              == nonstd::cx::ceil(f32__f32_p_inf) )              || ( isnan(std::ceil(f32__f32_p_inf))              && isnan(nonstd::cx::ceil(f32__f32_p_inf)) )              ));
        REQUIRE(( ( std::ceil(f32__f32_n_inf)              == nonstd::cx::ceil(f32__f32_n_inf) )              || ( isnan(std::ceil(f32__f32_n_inf))              && isnan(nonstd::cx::ceil(f32__f32_n_inf)) )              ));
        REQUIRE(( ( std::ceil(f32__f64_p_inf)              == nonstd::cx::ceil(f32__f64_p_inf) )              || ( isnan(std::ceil(f32__f64_p_inf))              && isnan(nonstd::cx::ceil(f32__f64_p_inf)) )              ));
        REQUIRE(( ( std::ceil(f32__f64_n_inf)              == nonstd::cx::ceil(f32__f64_n_inf) )              || ( isnan(std::ceil(f32__f64_n_inf))              && isnan(nonstd::cx::ceil(f32__f64_n_inf)) )              ));
        REQUIRE(( ( std::ceil(f32__f_long_p_inf)           == nonstd::cx::ceil(f32__f_long_p_inf) )           || ( isnan(std::ceil(f32__f_long_p_inf))           && isnan(nonstd::cx::ceil(f32__f_long_p_inf)) )           ));
        REQUIRE(( ( std::ceil(f32__f_long_n_inf)           == nonstd::cx::ceil(f32__f_long_n_inf) )           || ( isnan(std::ceil(f32__f_long_n_inf))           && isnan(nonstd::cx::ceil(f32__f_long_n_inf)) )           ));
        REQUIRE(( ( std::ceil(f32__f32_less_smallest_p)    == nonstd::cx::ceil(f32__f32_less_smallest_p) )    || ( isnan(std::ceil(f32__f32_less_smallest_p))    && isnan(nonstd::cx::ceil(f32__f32_less_smallest_p)) )    ));
        REQUIRE(( ( std::ceil(f32__f32_less_smallest_n)    == nonstd::cx::ceil(f32__f32_less_smallest_n) )    || ( isnan(std::ceil(f32__f32_less_smallest_n))    && isnan(nonstd::cx::ceil(f32__f32_less_smallest_n)) )    ));
        REQUIRE(( ( std::ceil(f32__f32_more_highest)       == nonstd::cx::ceil(f32__f32_more_highest) )       || ( isnan(std::ceil(f32__f32_more_highest))       && isnan(nonstd::cx::ceil(f32__f32_more_highest)) )       ));
        REQUIRE(( ( std::ceil(f32__f32_more_lowest)        == nonstd::cx::ceil(f32__f32_more_lowest) )        || ( isnan(std::ceil(f32__f32_more_lowest))        && isnan(nonstd::cx::ceil(f32__f32_more_lowest)) )        ));
        REQUIRE(( ( std::ceil(f32__f64_less_smallest_p)    == nonstd::cx::ceil(f32__f64_less_smallest_p) )    || ( isnan(std::ceil(f32__f64_less_smallest_p))    && isnan(nonstd::cx::ceil(f32__f64_less_smallest_p)) )    ));
        REQUIRE(( ( std::ceil(f32__f64_less_smallest_n)    == nonstd::cx::ceil(f32__f64_less_smallest_n) )    || ( isnan(std::ceil(f32__f64_less_smallest_n))    && isnan(nonstd::cx::ceil(f32__f64_less_smallest_n)) )    ));
        REQUIRE(( ( std::ceil(f32__f64_more_highest)       == nonstd::cx::ceil(f32__f64_more_highest) )       || ( isnan(std::ceil(f32__f64_more_highest))       && isnan(nonstd::cx::ceil(f32__f64_more_highest)) )       ));
        REQUIRE(( ( std::ceil(f32__f64_more_lowest)        == nonstd::cx::ceil(f32__f64_more_lowest) )        || ( isnan(std::ceil(f32__f64_more_lowest))        && isnan(nonstd::cx::ceil(f32__f64_more_lowest)) )        ));
        REQUIRE(( ( std::ceil(f32__f_long_less_smallest_p) == nonstd::cx::ceil(f32__f_long_less_smallest_p) ) || ( isnan(std::ceil(f32__f_long_less_smallest_p)) && isnan(nonstd::cx::ceil(f32__f_long_less_smallest_p)) ) ));
        REQUIRE(( ( std::ceil(f32__f_long_less_smallest_n) == nonstd::cx::ceil(f32__f_long_less_smallest_n) ) || ( isnan(std::ceil(f32__f_long_less_smallest_n)) && isnan(nonstd::cx::ceil(f32__f_long_less_smallest_n)) ) ));
        REQUIRE(( ( std::ceil(f32__f_long_more_highest)    == nonstd::cx::ceil(f32__f_long_more_highest) )    || ( isnan(std::ceil(f32__f_long_more_highest))    && isnan(nonstd::cx::ceil(f32__f_long_more_highest)) )    ));
        REQUIRE(( ( std::ceil(f32__f_long_more_lowest)     == nonstd::cx::ceil(f32__f_long_more_lowest) )     || ( isnan(std::ceil(f32__f_long_more_lowest))     && isnan(nonstd::cx::ceil(f32__f_long_more_lowest)) )     ));

        REQUIRE(( ( std::ceil(f64__positive)               == nonstd::cx::ceil(f64__positive) )               || ( isnan(std::ceil(f64__positive))               && isnan(nonstd::cx::ceil(f64__positive)) )               ));
        REQUIRE(( ( std::ceil(f64__zero)                   == nonstd::cx::ceil(f64__zero) )                   || ( isnan(std::ceil(f64__zero))                   && isnan(nonstd::cx::ceil(f64__zero)) )                   ));
        REQUIRE(( ( std::ceil(f64__negative)               == nonstd::cx::ceil(f64__negative) )               || ( isnan(std::ceil(f64__negative))               && isnan(nonstd::cx::ceil(f64__negative)) )               ));
        REQUIRE(( ( std::ceil(f64__f32_smallest_p)         == nonstd::cx::ceil(f64__f32_smallest_p) )         || ( isnan(std::ceil(f64__f32_smallest_p))         && isnan(nonstd::cx::ceil(f64__f32_smallest_p)) )         ));
        REQUIRE(( ( std::ceil(f64__f32_smallest_n)         == nonstd::cx::ceil(f64__f32_smallest_n) )         || ( isnan(std::ceil(f64__f32_smallest_n))         && isnan(nonstd::cx::ceil(f64__f32_smallest_n)) )         ));
        REQUIRE(( ( std::ceil(f64__f32_highest)            == nonstd::cx::ceil(f64__f32_highest) )            || ( isnan(std::ceil(f64__f32_highest))            && isnan(nonstd::cx::ceil(f64__f32_highest)) )            ));
        REQUIRE(( ( std::ceil(f64__f32_lowest)             == nonstd::cx::ceil(f64__f32_lowest) )             || ( isnan(std::ceil(f64__f32_lowest))             && isnan(nonstd::cx::ceil(f64__f32_lowest)) )             ));
        REQUIRE(( ( std::ceil(f64__f64_smallest_p)         == nonstd::cx::ceil(f64__f64_smallest_p) )         || ( isnan(std::ceil(f64__f64_smallest_p))         && isnan(nonstd::cx::ceil(f64__f64_smallest_p)) )         ));
        REQUIRE(( ( std::ceil(f64__f64_smallest_n)         == nonstd::cx::ceil(f64__f64_smallest_n) )         || ( isnan(std::ceil(f64__f64_smallest_n))         && isnan(nonstd::cx::ceil(f64__f64_smallest_n)) )         ));
        REQUIRE(( ( std::ceil(f64__f64_highest)            == nonstd::cx::ceil(f64__f64_highest) )            || ( isnan(std::ceil(f64__f64_highest))            && isnan(nonstd::cx::ceil(f64__f64_highest)) )            ));
        REQUIRE(( ( std::ceil(f64__f64_lowest)             == nonstd::cx::ceil(f64__f64_lowest) )             || ( isnan(std::ceil(f64__f64_lowest))             && isnan(nonstd::cx::ceil(f64__f64_lowest)) )             ));
        REQUIRE(( ( std::ceil(f64__f_long_smallest_p)      == nonstd::cx::ceil(f64__f_long_smallest_p) )      || ( isnan(std::ceil(f64__f_long_smallest_p))      && isnan(nonstd::cx::ceil(f64__f_long_smallest_p)) )      ));
        REQUIRE(( ( std::ceil(f64__f_long_smallest_n)      == nonstd::cx::ceil(f64__f_long_smallest_n) )      || ( isnan(std::ceil(f64__f_long_smallest_n))      && isnan(nonstd::cx::ceil(f64__f_long_smallest_n)) )      ));
    //  REQUIRE(( ( std::ceil(f64__f_long_highest)         == nonstd::cx::ceil(f64__f_long_highest) )         || ( isnan(std::ceil(f64__f_long_highest))         && isnan(nonstd::cx::ceil(f64__f_long_highest)) )         ));
    //  REQUIRE(( ( std::ceil(f64__f_long_lowest)          == nonstd::cx::ceil(f64__f_long_lowest) )          || ( isnan(std::ceil(f64__f_long_lowest))          && isnan(nonstd::cx::ceil(f64__f_long_lowest)) )          ));
        REQUIRE(( ( std::ceil(f64__f32_nan)                == nonstd::cx::ceil(f64__f32_nan) )                || ( isnan(std::ceil(f64__f32_nan))                && isnan(nonstd::cx::ceil(f64__f32_nan)) )                ));
        REQUIRE(( ( std::ceil(f64__f64_nan)                == nonstd::cx::ceil(f64__f64_nan) )                || ( isnan(std::ceil(f64__f64_nan))                && isnan(nonstd::cx::ceil(f64__f64_nan)) )                ));
        REQUIRE(( ( std::ceil(f64__f_long_nan)             == nonstd::cx::ceil(f64__f_long_nan) )             || ( isnan(std::ceil(f64__f_long_nan))             && isnan(nonstd::cx::ceil(f64__f_long_nan)) )             ));
        REQUIRE(( ( std::ceil(f64__f32_p_inf)              == nonstd::cx::ceil(f64__f32_p_inf) )              || ( isnan(std::ceil(f64__f32_p_inf))              && isnan(nonstd::cx::ceil(f64__f32_p_inf)) )              ));
        REQUIRE(( ( std::ceil(f64__f32_n_inf)              == nonstd::cx::ceil(f64__f32_n_inf) )              || ( isnan(std::ceil(f64__f32_n_inf))              && isnan(nonstd::cx::ceil(f64__f32_n_inf)) )              ));
        REQUIRE(( ( std::ceil(f64__f64_p_inf)              == nonstd::cx::ceil(f64__f64_p_inf) )              || ( isnan(std::ceil(f64__f64_p_inf))              && isnan(nonstd::cx::ceil(f64__f64_p_inf)) )              ));
        REQUIRE(( ( std::ceil(f64__f64_n_inf)              == nonstd::cx::ceil(f64__f64_n_inf) )              || ( isnan(std::ceil(f64__f64_n_inf))              && isnan(nonstd::cx::ceil(f64__f64_n_inf)) )              ));
        REQUIRE(( ( std::ceil(f64__f_long_p_inf)           == nonstd::cx::ceil(f64__f_long_p_inf) )           || ( isnan(std::ceil(f64__f_long_p_inf))           && isnan(nonstd::cx::ceil(f64__f_long_p_inf)) )           ));
        REQUIRE(( ( std::ceil(f64__f_long_n_inf)           == nonstd::cx::ceil(f64__f_long_n_inf) )           || ( isnan(std::ceil(f64__f_long_n_inf))           && isnan(nonstd::cx::ceil(f64__f_long_n_inf)) )           ));
        REQUIRE(( ( std::ceil(f64__f32_less_smallest_p)    == nonstd::cx::ceil(f64__f32_less_smallest_p) )    || ( isnan(std::ceil(f64__f32_less_smallest_p))    && isnan(nonstd::cx::ceil(f64__f32_less_smallest_p)) )    ));
        REQUIRE(( ( std::ceil(f64__f32_less_smallest_n)    == nonstd::cx::ceil(f64__f32_less_smallest_n) )    || ( isnan(std::ceil(f64__f32_less_smallest_n))    && isnan(nonstd::cx::ceil(f64__f32_less_smallest_n)) )    ));
        REQUIRE(( ( std::ceil(f64__f32_more_highest)       == nonstd::cx::ceil(f64__f32_more_highest) )       || ( isnan(std::ceil(f64__f32_more_highest))       && isnan(nonstd::cx::ceil(f64__f32_more_highest)) )       ));
        REQUIRE(( ( std::ceil(f64__f32_more_lowest)        == nonstd::cx::ceil(f64__f32_more_lowest) )        || ( isnan(std::ceil(f64__f32_more_lowest))        && isnan(nonstd::cx::ceil(f64__f32_more_lowest)) )        ));
        REQUIRE(( ( std::ceil(f64__f64_less_smallest_p)    == nonstd::cx::ceil(f64__f64_less_smallest_p) )    || ( isnan(std::ceil(f64__f64_less_smallest_p))    && isnan(nonstd::cx::ceil(f64__f64_less_smallest_p)) )    ));
        REQUIRE(( ( std::ceil(f64__f64_less_smallest_n)    == nonstd::cx::ceil(f64__f64_less_smallest_n) )    || ( isnan(std::ceil(f64__f64_less_smallest_n))    && isnan(nonstd::cx::ceil(f64__f64_less_smallest_n)) )    ));
        REQUIRE(( ( std::ceil(f64__f64_more_highest)       == nonstd::cx::ceil(f64__f64_more_highest) )       || ( isnan(std::ceil(f64__f64_more_highest))       && isnan(nonstd::cx::ceil(f64__f64_more_highest)) )       ));
        REQUIRE(( ( std::ceil(f64__f64_more_lowest)        == nonstd::cx::ceil(f64__f64_more_lowest) )        || ( isnan(std::ceil(f64__f64_more_lowest))        && isnan(nonstd::cx::ceil(f64__f64_more_lowest)) )        ));
        REQUIRE(( ( std::ceil(f64__f_long_less_smallest_p) == nonstd::cx::ceil(f64__f_long_less_smallest_p) ) || ( isnan(std::ceil(f64__f_long_less_smallest_p)) && isnan(nonstd::cx::ceil(f64__f_long_less_smallest_p)) ) ));
        REQUIRE(( ( std::ceil(f64__f_long_less_smallest_n) == nonstd::cx::ceil(f64__f_long_less_smallest_n) ) || ( isnan(std::ceil(f64__f_long_less_smallest_n)) && isnan(nonstd::cx::ceil(f64__f_long_less_smallest_n)) ) ));
        REQUIRE(( ( std::ceil(f64__f_long_more_highest)    == nonstd::cx::ceil(f64__f_long_more_highest) )    || ( isnan(std::ceil(f64__f_long_more_highest))    && isnan(nonstd::cx::ceil(f64__f_long_more_highest)) )    ));
        REQUIRE(( ( std::ceil(f64__f_long_more_lowest)     == nonstd::cx::ceil(f64__f_long_more_lowest) )     || ( isnan(std::ceil(f64__f_long_more_lowest))     && isnan(nonstd::cx::ceil(f64__f_long_more_lowest)) )     ));

        REQUIRE(( ( std::ceil(f_long__positive)               == nonstd::cx::ceil(f_long__positive) )               || ( isnan(std::ceil(f_long__positive))               && isnan(nonstd::cx::ceil(f_long__positive)) )               ));
        REQUIRE(( ( std::ceil(f_long__zero)                   == nonstd::cx::ceil(f_long__zero) )                   || ( isnan(std::ceil(f_long__zero))                   && isnan(nonstd::cx::ceil(f_long__zero)) )                   ));
        REQUIRE(( ( std::ceil(f_long__negative)               == nonstd::cx::ceil(f_long__negative) )               || ( isnan(std::ceil(f_long__negative))               && isnan(nonstd::cx::ceil(f_long__negative)) )               ));
        REQUIRE(( ( std::ceil(f_long__f32_smallest_p)         == nonstd::cx::ceil(f_long__f32_smallest_p) )         || ( isnan(std::ceil(f_long__f32_smallest_p))         && isnan(nonstd::cx::ceil(f_long__f32_smallest_p)) )         ));
        REQUIRE(( ( std::ceil(f_long__f32_smallest_n)         == nonstd::cx::ceil(f_long__f32_smallest_n) )         || ( isnan(std::ceil(f_long__f32_smallest_n))         && isnan(nonstd::cx::ceil(f_long__f32_smallest_n)) )         ));
        REQUIRE(( ( std::ceil(f_long__f32_highest)            == nonstd::cx::ceil(f_long__f32_highest) )            || ( isnan(std::ceil(f_long__f32_highest))            && isnan(nonstd::cx::ceil(f_long__f32_highest)) )            ));
        REQUIRE(( ( std::ceil(f_long__f32_lowest)             == nonstd::cx::ceil(f_long__f32_lowest) )             || ( isnan(std::ceil(f_long__f32_lowest))             && isnan(nonstd::cx::ceil(f_long__f32_lowest)) )             ));
        REQUIRE(( ( std::ceil(f_long__f64_smallest_p)         == nonstd::cx::ceil(f_long__f64_smallest_p) )         || ( isnan(std::ceil(f_long__f64_smallest_p))         && isnan(nonstd::cx::ceil(f_long__f64_smallest_p)) )         ));
        REQUIRE(( ( std::ceil(f_long__f64_smallest_n)         == nonstd::cx::ceil(f_long__f64_smallest_n) )         || ( isnan(std::ceil(f_long__f64_smallest_n))         && isnan(nonstd::cx::ceil(f_long__f64_smallest_n)) )         ));
        REQUIRE(( ( std::ceil(f_long__f64_highest)            == nonstd::cx::ceil(f_long__f64_highest) )            || ( isnan(std::ceil(f_long__f64_highest))            && isnan(nonstd::cx::ceil(f_long__f64_highest)) )            ));
        REQUIRE(( ( std::ceil(f_long__f64_lowest)             == nonstd::cx::ceil(f_long__f64_lowest) )             || ( isnan(std::ceil(f_long__f64_lowest))             && isnan(nonstd::cx::ceil(f_long__f64_lowest)) )             ));
        REQUIRE(( ( std::ceil(f_long__f_long_smallest_p)      == nonstd::cx::ceil(f_long__f_long_smallest_p) )      || ( isnan(std::ceil(f_long__f_long_smallest_p))      && isnan(nonstd::cx::ceil(f_long__f_long_smallest_p)) )      ));
        REQUIRE(( ( std::ceil(f_long__f_long_smallest_n)      == nonstd::cx::ceil(f_long__f_long_smallest_n) )      || ( isnan(std::ceil(f_long__f_long_smallest_n))      && isnan(nonstd::cx::ceil(f_long__f_long_smallest_n)) )      ));
        REQUIRE(( ( std::ceil(f_long__f_long_highest)         == nonstd::cx::ceil(f_long__f_long_highest) )         || ( isnan(std::ceil(f_long__f_long_highest))         && isnan(nonstd::cx::ceil(f_long__f_long_highest)) )         ));
        REQUIRE(( ( std::ceil(f_long__f_long_lowest)          == nonstd::cx::ceil(f_long__f_long_lowest) )          || ( isnan(std::ceil(f_long__f_long_lowest))          && isnan(nonstd::cx::ceil(f_long__f_long_lowest)) )          ));
        REQUIRE(( ( std::ceil(f_long__f32_nan)                == nonstd::cx::ceil(f_long__f32_nan) )                || ( isnan(std::ceil(f_long__f32_nan))                && isnan(nonstd::cx::ceil(f_long__f32_nan)) )                ));
        REQUIRE(( ( std::ceil(f_long__f64_nan)                == nonstd::cx::ceil(f_long__f64_nan) )                || ( isnan(std::ceil(f_long__f64_nan))                && isnan(nonstd::cx::ceil(f_long__f64_nan)) )                ));
        REQUIRE(( ( std::ceil(f_long__f_long_nan)             == nonstd::cx::ceil(f_long__f_long_nan) )             || ( isnan(std::ceil(f_long__f_long_nan))             && isnan(nonstd::cx::ceil(f_long__f_long_nan)) )             ));
        REQUIRE(( ( std::ceil(f_long__f32_p_inf)              == nonstd::cx::ceil(f_long__f32_p_inf) )              || ( isnan(std::ceil(f_long__f32_p_inf))              && isnan(nonstd::cx::ceil(f_long__f32_p_inf)) )              ));
        REQUIRE(( ( std::ceil(f_long__f32_n_inf)              == nonstd::cx::ceil(f_long__f32_n_inf) )              || ( isnan(std::ceil(f_long__f32_n_inf))              && isnan(nonstd::cx::ceil(f_long__f32_n_inf)) )              ));
        REQUIRE(( ( std::ceil(f_long__f64_p_inf)              == nonstd::cx::ceil(f_long__f64_p_inf) )              || ( isnan(std::ceil(f_long__f64_p_inf))              && isnan(nonstd::cx::ceil(f_long__f64_p_inf)) )              ));
        REQUIRE(( ( std::ceil(f_long__f64_n_inf)              == nonstd::cx::ceil(f_long__f64_n_inf) )              || ( isnan(std::ceil(f_long__f64_n_inf))              && isnan(nonstd::cx::ceil(f_long__f64_n_inf)) )              ));
        REQUIRE(( ( std::ceil(f_long__f_long_p_inf)           == nonstd::cx::ceil(f_long__f_long_p_inf) )           || ( isnan(std::ceil(f_long__f_long_p_inf))           && isnan(nonstd::cx::ceil(f_long__f_long_p_inf)) )           ));
        REQUIRE(( ( std::ceil(f_long__f_long_n_inf)           == nonstd::cx::ceil(f_long__f_long_n_inf) )           || ( isnan(std::ceil(f_long__f_long_n_inf))           && isnan(nonstd::cx::ceil(f_long__f_long_n_inf)) )           ));
        REQUIRE(( ( std::ceil(f_long__f32_less_smallest_p)    == nonstd::cx::ceil(f_long__f32_less_smallest_p) )    || ( isnan(std::ceil(f_long__f32_less_smallest_p))    && isnan(nonstd::cx::ceil(f_long__f32_less_smallest_p)) )    ));
        REQUIRE(( ( std::ceil(f_long__f32_less_smallest_n)    == nonstd::cx::ceil(f_long__f32_less_smallest_n) )    || ( isnan(std::ceil(f_long__f32_less_smallest_n))    && isnan(nonstd::cx::ceil(f_long__f32_less_smallest_n)) )    ));
        REQUIRE(( ( std::ceil(f_long__f32_more_highest)       == nonstd::cx::ceil(f_long__f32_more_highest) )       || ( isnan(std::ceil(f_long__f32_more_highest))       && isnan(nonstd::cx::ceil(f_long__f32_more_highest)) )       ));
        REQUIRE(( ( std::ceil(f_long__f32_more_lowest)        == nonstd::cx::ceil(f_long__f32_more_lowest) )        || ( isnan(std::ceil(f_long__f32_more_lowest))        && isnan(nonstd::cx::ceil(f_long__f32_more_lowest)) )        ));
        REQUIRE(( ( std::ceil(f_long__f64_less_smallest_p)    == nonstd::cx::ceil(f_long__f64_less_smallest_p) )    || ( isnan(std::ceil(f_long__f64_less_smallest_p))    && isnan(nonstd::cx::ceil(f_long__f64_less_smallest_p)) )    ));
        REQUIRE(( ( std::ceil(f_long__f64_less_smallest_n)    == nonstd::cx::ceil(f_long__f64_less_smallest_n) )    || ( isnan(std::ceil(f_long__f64_less_smallest_n))    && isnan(nonstd::cx::ceil(f_long__f64_less_smallest_n)) )    ));
        REQUIRE(( ( std::ceil(f_long__f64_more_highest)       == nonstd::cx::ceil(f_long__f64_more_highest) )       || ( isnan(std::ceil(f_long__f64_more_highest))       && isnan(nonstd::cx::ceil(f_long__f64_more_highest)) )       ));
        REQUIRE(( ( std::ceil(f_long__f64_more_lowest)        == nonstd::cx::ceil(f_long__f64_more_lowest) )        || ( isnan(std::ceil(f_long__f64_more_lowest))        && isnan(nonstd::cx::ceil(f_long__f64_more_lowest)) )        ));
        REQUIRE(( ( std::ceil(f_long__f_long_less_smallest_p) == nonstd::cx::ceil(f_long__f_long_less_smallest_p) ) || ( isnan(std::ceil(f_long__f_long_less_smallest_p)) && isnan(nonstd::cx::ceil(f_long__f_long_less_smallest_p)) ) ));
        REQUIRE(( ( std::ceil(f_long__f_long_less_smallest_n) == nonstd::cx::ceil(f_long__f_long_less_smallest_n) ) || ( isnan(std::ceil(f_long__f_long_less_smallest_n)) && isnan(nonstd::cx::ceil(f_long__f_long_less_smallest_n)) ) ));
        REQUIRE(( ( std::ceil(f_long__f_long_more_highest)    == nonstd::cx::ceil(f_long__f_long_more_highest) )    || ( isnan(std::ceil(f_long__f_long_more_highest))    && isnan(nonstd::cx::ceil(f_long__f_long_more_highest)) )    ));
        REQUIRE(( ( std::ceil(f_long__f_long_more_lowest)     == nonstd::cx::ceil(f_long__f_long_more_lowest) )     || ( isnan(std::ceil(f_long__f_long_more_lowest))     && isnan(nonstd::cx::ceil(f_long__f_long_more_lowest)) )     ));

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
            0.0f,
            std::numeric_limits<float>::max());
        std::uniform_real_distribution<double> rnd_double_range(
            0.0,
            std::numeric_limits<double>::max());
        std::uniform_real_distribution<long double> rnd_long_double_range(
            0.0l,
            std::numeric_limits<long double>::max());

        constexpr int iterations = 10000;
        for (int i = 0; i < iterations; i++) {
            float px = rnd_float_range(re);
            float nx = -px;
            REQUIRE(( ( std::ceil(px)  == nonstd::cx::ceil(px) ) || ( isnan(std::ceil(px)) && isnan(nonstd::cx::ceil(px)) ) ));
            REQUIRE(( ( std::ceil(nx)  == nonstd::cx::ceil(nx) ) || ( isnan(std::ceil(nx)) && isnan(nonstd::cx::ceil(nx)) ) ));
        }
        for (int i = 0; i < iterations; i++) {
            double px = rnd_double_range(re);
            double nx = -px;
            REQUIRE(( ( std::ceil(px)  == nonstd::cx::ceil(px) ) || ( isnan(std::ceil(px)) && isnan(nonstd::cx::ceil(px)) ) ));
            REQUIRE(( ( std::ceil(nx)  == nonstd::cx::ceil(nx) ) || ( isnan(std::ceil(nx)) && isnan(nonstd::cx::ceil(nx)) ) ));
        }
        for (int i = 0; i < iterations; i++) {
            long double px = rnd_long_double_range(re);
            long double nx = -px;
            REQUIRE(( ( std::ceil(px)  == nonstd::cx::ceil(px) ) || ( isnan(std::ceil(px)) && isnan(nonstd::cx::ceil(px)) ) ));
            REQUIRE(( ( std::ceil(nx)  == nonstd::cx::ceil(nx) ) || ( isnan(std::ceil(nx)) && isnan(nonstd::cx::ceil(nx)) ) ));
        }
    }

    SECTION("floor") {
        constexpr auto calls_to_floor_are_constexpr = nonstd::cx::floor(f32__positive);

        using std::isnan;
        REQUIRE(( ( std::floor(f32__positive)               == nonstd::cx::floor(f32__positive) )               || ( isnan(std::floor(f32__positive))               && isnan(nonstd::cx::floor(f32__positive)) )               ));
        REQUIRE(( ( std::floor(f32__zero)                   == nonstd::cx::floor(f32__zero) )                   || ( isnan(std::floor(f32__zero))                   && isnan(nonstd::cx::floor(f32__zero)) )                   ));
        REQUIRE(( ( std::floor(f32__negative)               == nonstd::cx::floor(f32__negative) )               || ( isnan(std::floor(f32__negative))               && isnan(nonstd::cx::floor(f32__negative)) )               ));
        REQUIRE(( ( std::floor(f32__f32_smallest_p)         == nonstd::cx::floor(f32__f32_smallest_p) )         || ( isnan(std::floor(f32__f32_smallest_p))         && isnan(nonstd::cx::floor(f32__f32_smallest_p)) )         ));
        REQUIRE(( ( std::floor(f32__f32_smallest_n)         == nonstd::cx::floor(f32__f32_smallest_n) )         || ( isnan(std::floor(f32__f32_smallest_n))         && isnan(nonstd::cx::floor(f32__f32_smallest_n)) )         ));
        REQUIRE(( ( std::floor(f32__f32_highest)            == nonstd::cx::floor(f32__f32_highest) )            || ( isnan(std::floor(f32__f32_highest))            && isnan(nonstd::cx::floor(f32__f32_highest)) )            ));
        REQUIRE(( ( std::floor(f32__f32_lowest)             == nonstd::cx::floor(f32__f32_lowest) )             || ( isnan(std::floor(f32__f32_lowest))             && isnan(nonstd::cx::floor(f32__f32_lowest)) )             ));
        REQUIRE(( ( std::floor(f32__f64_smallest_p)         == nonstd::cx::floor(f32__f64_smallest_p) )         || ( isnan(std::floor(f32__f64_smallest_p))         && isnan(nonstd::cx::floor(f32__f64_smallest_p)) )         ));
        REQUIRE(( ( std::floor(f32__f64_smallest_n)         == nonstd::cx::floor(f32__f64_smallest_n) )         || ( isnan(std::floor(f32__f64_smallest_n))         && isnan(nonstd::cx::floor(f32__f64_smallest_n)) )         ));
    //  REQUIRE(( ( std::floor(f32__f64_highest)            == nonstd::cx::floor(f32__f64_highest) )            || ( isnan(std::floor(f32__f64_highest))            && isnan(nonstd::cx::floor(f32__f64_highest)) )            ));
    //  REQUIRE(( ( std::floor(f32__f64_lowest)             == nonstd::cx::floor(f32__f64_lowest) )             || ( isnan(std::floor(f32__f64_lowest))             && isnan(nonstd::cx::floor(f32__f64_lowest)) )             ));
        REQUIRE(( ( std::floor(f32__f_long_smallest_p)      == nonstd::cx::floor(f32__f_long_smallest_p) )      || ( isnan(std::floor(f32__f_long_smallest_p))      && isnan(nonstd::cx::floor(f32__f_long_smallest_p)) )      ));
        REQUIRE(( ( std::floor(f32__f_long_smallest_n)      == nonstd::cx::floor(f32__f_long_smallest_n) )      || ( isnan(std::floor(f32__f_long_smallest_n))      && isnan(nonstd::cx::floor(f32__f_long_smallest_n)) )      ));
    //  REQUIRE(( ( std::floor(f32__f_long_highest)         == nonstd::cx::floor(f32__f_long_highest) )         || ( isnan(std::floor(f32__f_long_highest))         && isnan(nonstd::cx::floor(f32__f_long_highest)) )         ));
    //  REQUIRE(( ( std::floor(f32__f_long_lowest)          == nonstd::cx::floor(f32__f_long_lowest) )          || ( isnan(std::floor(f32__f_long_lowest))          && isnan(nonstd::cx::floor(f32__f_long_lowest)) )          ));
        REQUIRE(( ( std::floor(f32__f32_nan)                == nonstd::cx::floor(f32__f32_nan) )                || ( isnan(std::floor(f32__f32_nan))                && isnan(nonstd::cx::floor(f32__f32_nan)) )                ));
        REQUIRE(( ( std::floor(f32__f64_nan)                == nonstd::cx::floor(f32__f64_nan) )                || ( isnan(std::floor(f32__f64_nan))                && isnan(nonstd::cx::floor(f32__f64_nan)) )                ));
        REQUIRE(( ( std::floor(f32__f_long_nan)             == nonstd::cx::floor(f32__f_long_nan) )             || ( isnan(std::floor(f32__f_long_nan))             && isnan(nonstd::cx::floor(f32__f_long_nan)) )             ));
        REQUIRE(( ( std::floor(f32__f32_p_inf)              == nonstd::cx::floor(f32__f32_p_inf) )              || ( isnan(std::floor(f32__f32_p_inf))              && isnan(nonstd::cx::floor(f32__f32_p_inf)) )              ));
        REQUIRE(( ( std::floor(f32__f32_n_inf)              == nonstd::cx::floor(f32__f32_n_inf) )              || ( isnan(std::floor(f32__f32_n_inf))              && isnan(nonstd::cx::floor(f32__f32_n_inf)) )              ));
        REQUIRE(( ( std::floor(f32__f64_p_inf)              == nonstd::cx::floor(f32__f64_p_inf) )              || ( isnan(std::floor(f32__f64_p_inf))              && isnan(nonstd::cx::floor(f32__f64_p_inf)) )              ));
        REQUIRE(( ( std::floor(f32__f64_n_inf)              == nonstd::cx::floor(f32__f64_n_inf) )              || ( isnan(std::floor(f32__f64_n_inf))              && isnan(nonstd::cx::floor(f32__f64_n_inf)) )              ));
        REQUIRE(( ( std::floor(f32__f_long_p_inf)           == nonstd::cx::floor(f32__f_long_p_inf) )           || ( isnan(std::floor(f32__f_long_p_inf))           && isnan(nonstd::cx::floor(f32__f_long_p_inf)) )           ));
        REQUIRE(( ( std::floor(f32__f_long_n_inf)           == nonstd::cx::floor(f32__f_long_n_inf) )           || ( isnan(std::floor(f32__f_long_n_inf))           && isnan(nonstd::cx::floor(f32__f_long_n_inf)) )           ));
        REQUIRE(( ( std::floor(f32__f32_less_smallest_p)    == nonstd::cx::floor(f32__f32_less_smallest_p) )    || ( isnan(std::floor(f32__f32_less_smallest_p))    && isnan(nonstd::cx::floor(f32__f32_less_smallest_p)) )    ));
        REQUIRE(( ( std::floor(f32__f32_less_smallest_n)    == nonstd::cx::floor(f32__f32_less_smallest_n) )    || ( isnan(std::floor(f32__f32_less_smallest_n))    && isnan(nonstd::cx::floor(f32__f32_less_smallest_n)) )    ));
        REQUIRE(( ( std::floor(f32__f32_more_highest)       == nonstd::cx::floor(f32__f32_more_highest) )       || ( isnan(std::floor(f32__f32_more_highest))       && isnan(nonstd::cx::floor(f32__f32_more_highest)) )       ));
        REQUIRE(( ( std::floor(f32__f32_more_lowest)        == nonstd::cx::floor(f32__f32_more_lowest) )        || ( isnan(std::floor(f32__f32_more_lowest))        && isnan(nonstd::cx::floor(f32__f32_more_lowest)) )        ));
        REQUIRE(( ( std::floor(f32__f64_less_smallest_p)    == nonstd::cx::floor(f32__f64_less_smallest_p) )    || ( isnan(std::floor(f32__f64_less_smallest_p))    && isnan(nonstd::cx::floor(f32__f64_less_smallest_p)) )    ));
        REQUIRE(( ( std::floor(f32__f64_less_smallest_n)    == nonstd::cx::floor(f32__f64_less_smallest_n) )    || ( isnan(std::floor(f32__f64_less_smallest_n))    && isnan(nonstd::cx::floor(f32__f64_less_smallest_n)) )    ));
        REQUIRE(( ( std::floor(f32__f64_more_highest)       == nonstd::cx::floor(f32__f64_more_highest) )       || ( isnan(std::floor(f32__f64_more_highest))       && isnan(nonstd::cx::floor(f32__f64_more_highest)) )       ));
        REQUIRE(( ( std::floor(f32__f64_more_lowest)        == nonstd::cx::floor(f32__f64_more_lowest) )        || ( isnan(std::floor(f32__f64_more_lowest))        && isnan(nonstd::cx::floor(f32__f64_more_lowest)) )        ));
        REQUIRE(( ( std::floor(f32__f_long_less_smallest_p) == nonstd::cx::floor(f32__f_long_less_smallest_p) ) || ( isnan(std::floor(f32__f_long_less_smallest_p)) && isnan(nonstd::cx::floor(f32__f_long_less_smallest_p)) ) ));
        REQUIRE(( ( std::floor(f32__f_long_less_smallest_n) == nonstd::cx::floor(f32__f_long_less_smallest_n) ) || ( isnan(std::floor(f32__f_long_less_smallest_n)) && isnan(nonstd::cx::floor(f32__f_long_less_smallest_n)) ) ));
        REQUIRE(( ( std::floor(f32__f_long_more_highest)    == nonstd::cx::floor(f32__f_long_more_highest) )    || ( isnan(std::floor(f32__f_long_more_highest))    && isnan(nonstd::cx::floor(f32__f_long_more_highest)) )    ));
        REQUIRE(( ( std::floor(f32__f_long_more_lowest)     == nonstd::cx::floor(f32__f_long_more_lowest) )     || ( isnan(std::floor(f32__f_long_more_lowest))     && isnan(nonstd::cx::floor(f32__f_long_more_lowest)) )     ));

        REQUIRE(( ( std::floor(f64__positive)               == nonstd::cx::floor(f64__positive) )               || ( isnan(std::floor(f64__positive))               && isnan(nonstd::cx::floor(f64__positive)) )               ));
        REQUIRE(( ( std::floor(f64__zero)                   == nonstd::cx::floor(f64__zero) )                   || ( isnan(std::floor(f64__zero))                   && isnan(nonstd::cx::floor(f64__zero)) )                   ));
        REQUIRE(( ( std::floor(f64__negative)               == nonstd::cx::floor(f64__negative) )               || ( isnan(std::floor(f64__negative))               && isnan(nonstd::cx::floor(f64__negative)) )               ));
        REQUIRE(( ( std::floor(f64__f32_smallest_p)         == nonstd::cx::floor(f64__f32_smallest_p) )         || ( isnan(std::floor(f64__f32_smallest_p))         && isnan(nonstd::cx::floor(f64__f32_smallest_p)) )         ));
        REQUIRE(( ( std::floor(f64__f32_smallest_n)         == nonstd::cx::floor(f64__f32_smallest_n) )         || ( isnan(std::floor(f64__f32_smallest_n))         && isnan(nonstd::cx::floor(f64__f32_smallest_n)) )         ));
        REQUIRE(( ( std::floor(f64__f32_highest)            == nonstd::cx::floor(f64__f32_highest) )            || ( isnan(std::floor(f64__f32_highest))            && isnan(nonstd::cx::floor(f64__f32_highest)) )            ));
        REQUIRE(( ( std::floor(f64__f32_lowest)             == nonstd::cx::floor(f64__f32_lowest) )             || ( isnan(std::floor(f64__f32_lowest))             && isnan(nonstd::cx::floor(f64__f32_lowest)) )             ));
        REQUIRE(( ( std::floor(f64__f64_smallest_p)         == nonstd::cx::floor(f64__f64_smallest_p) )         || ( isnan(std::floor(f64__f64_smallest_p))         && isnan(nonstd::cx::floor(f64__f64_smallest_p)) )         ));
        REQUIRE(( ( std::floor(f64__f64_smallest_n)         == nonstd::cx::floor(f64__f64_smallest_n) )         || ( isnan(std::floor(f64__f64_smallest_n))         && isnan(nonstd::cx::floor(f64__f64_smallest_n)) )         ));
        REQUIRE(( ( std::floor(f64__f64_highest)            == nonstd::cx::floor(f64__f64_highest) )            || ( isnan(std::floor(f64__f64_highest))            && isnan(nonstd::cx::floor(f64__f64_highest)) )            ));
        REQUIRE(( ( std::floor(f64__f64_lowest)             == nonstd::cx::floor(f64__f64_lowest) )             || ( isnan(std::floor(f64__f64_lowest))             && isnan(nonstd::cx::floor(f64__f64_lowest)) )             ));
        REQUIRE(( ( std::floor(f64__f_long_smallest_p)      == nonstd::cx::floor(f64__f_long_smallest_p) )      || ( isnan(std::floor(f64__f_long_smallest_p))      && isnan(nonstd::cx::floor(f64__f_long_smallest_p)) )      ));
        REQUIRE(( ( std::floor(f64__f_long_smallest_n)      == nonstd::cx::floor(f64__f_long_smallest_n) )      || ( isnan(std::floor(f64__f_long_smallest_n))      && isnan(nonstd::cx::floor(f64__f_long_smallest_n)) )      ));
    //  REQUIRE(( ( std::floor(f64__f_long_highest)         == nonstd::cx::floor(f64__f_long_highest) )         || ( isnan(std::floor(f64__f_long_highest))         && isnan(nonstd::cx::floor(f64__f_long_highest)) )         ));
    //  REQUIRE(( ( std::floor(f64__f_long_lowest)          == nonstd::cx::floor(f64__f_long_lowest) )          || ( isnan(std::floor(f64__f_long_lowest))          && isnan(nonstd::cx::floor(f64__f_long_lowest)) )          ));
        REQUIRE(( ( std::floor(f64__f32_nan)                == nonstd::cx::floor(f64__f32_nan) )                || ( isnan(std::floor(f64__f32_nan))                && isnan(nonstd::cx::floor(f64__f32_nan)) )                ));
        REQUIRE(( ( std::floor(f64__f64_nan)                == nonstd::cx::floor(f64__f64_nan) )                || ( isnan(std::floor(f64__f64_nan))                && isnan(nonstd::cx::floor(f64__f64_nan)) )                ));
        REQUIRE(( ( std::floor(f64__f_long_nan)             == nonstd::cx::floor(f64__f_long_nan) )             || ( isnan(std::floor(f64__f_long_nan))             && isnan(nonstd::cx::floor(f64__f_long_nan)) )             ));
        REQUIRE(( ( std::floor(f64__f32_p_inf)              == nonstd::cx::floor(f64__f32_p_inf) )              || ( isnan(std::floor(f64__f32_p_inf))              && isnan(nonstd::cx::floor(f64__f32_p_inf)) )              ));
        REQUIRE(( ( std::floor(f64__f32_n_inf)              == nonstd::cx::floor(f64__f32_n_inf) )              || ( isnan(std::floor(f64__f32_n_inf))              && isnan(nonstd::cx::floor(f64__f32_n_inf)) )              ));
        REQUIRE(( ( std::floor(f64__f64_p_inf)              == nonstd::cx::floor(f64__f64_p_inf) )              || ( isnan(std::floor(f64__f64_p_inf))              && isnan(nonstd::cx::floor(f64__f64_p_inf)) )              ));
        REQUIRE(( ( std::floor(f64__f64_n_inf)              == nonstd::cx::floor(f64__f64_n_inf) )              || ( isnan(std::floor(f64__f64_n_inf))              && isnan(nonstd::cx::floor(f64__f64_n_inf)) )              ));
        REQUIRE(( ( std::floor(f64__f_long_p_inf)           == nonstd::cx::floor(f64__f_long_p_inf) )           || ( isnan(std::floor(f64__f_long_p_inf))           && isnan(nonstd::cx::floor(f64__f_long_p_inf)) )           ));
        REQUIRE(( ( std::floor(f64__f_long_n_inf)           == nonstd::cx::floor(f64__f_long_n_inf) )           || ( isnan(std::floor(f64__f_long_n_inf))           && isnan(nonstd::cx::floor(f64__f_long_n_inf)) )           ));
        REQUIRE(( ( std::floor(f64__f32_less_smallest_p)    == nonstd::cx::floor(f64__f32_less_smallest_p) )    || ( isnan(std::floor(f64__f32_less_smallest_p))    && isnan(nonstd::cx::floor(f64__f32_less_smallest_p)) )    ));
        REQUIRE(( ( std::floor(f64__f32_less_smallest_n)    == nonstd::cx::floor(f64__f32_less_smallest_n) )    || ( isnan(std::floor(f64__f32_less_smallest_n))    && isnan(nonstd::cx::floor(f64__f32_less_smallest_n)) )    ));
        REQUIRE(( ( std::floor(f64__f32_more_highest)       == nonstd::cx::floor(f64__f32_more_highest) )       || ( isnan(std::floor(f64__f32_more_highest))       && isnan(nonstd::cx::floor(f64__f32_more_highest)) )       ));
        REQUIRE(( ( std::floor(f64__f32_more_lowest)        == nonstd::cx::floor(f64__f32_more_lowest) )        || ( isnan(std::floor(f64__f32_more_lowest))        && isnan(nonstd::cx::floor(f64__f32_more_lowest)) )        ));
        REQUIRE(( ( std::floor(f64__f64_less_smallest_p)    == nonstd::cx::floor(f64__f64_less_smallest_p) )    || ( isnan(std::floor(f64__f64_less_smallest_p))    && isnan(nonstd::cx::floor(f64__f64_less_smallest_p)) )    ));
        REQUIRE(( ( std::floor(f64__f64_less_smallest_n)    == nonstd::cx::floor(f64__f64_less_smallest_n) )    || ( isnan(std::floor(f64__f64_less_smallest_n))    && isnan(nonstd::cx::floor(f64__f64_less_smallest_n)) )    ));
        REQUIRE(( ( std::floor(f64__f64_more_highest)       == nonstd::cx::floor(f64__f64_more_highest) )       || ( isnan(std::floor(f64__f64_more_highest))       && isnan(nonstd::cx::floor(f64__f64_more_highest)) )       ));
        REQUIRE(( ( std::floor(f64__f64_more_lowest)        == nonstd::cx::floor(f64__f64_more_lowest) )        || ( isnan(std::floor(f64__f64_more_lowest))        && isnan(nonstd::cx::floor(f64__f64_more_lowest)) )        ));
        REQUIRE(( ( std::floor(f64__f_long_less_smallest_p) == nonstd::cx::floor(f64__f_long_less_smallest_p) ) || ( isnan(std::floor(f64__f_long_less_smallest_p)) && isnan(nonstd::cx::floor(f64__f_long_less_smallest_p)) ) ));
        REQUIRE(( ( std::floor(f64__f_long_less_smallest_n) == nonstd::cx::floor(f64__f_long_less_smallest_n) ) || ( isnan(std::floor(f64__f_long_less_smallest_n)) && isnan(nonstd::cx::floor(f64__f_long_less_smallest_n)) ) ));
        REQUIRE(( ( std::floor(f64__f_long_more_highest)    == nonstd::cx::floor(f64__f_long_more_highest) )    || ( isnan(std::floor(f64__f_long_more_highest))    && isnan(nonstd::cx::floor(f64__f_long_more_highest)) )    ));
        REQUIRE(( ( std::floor(f64__f_long_more_lowest)     == nonstd::cx::floor(f64__f_long_more_lowest) )     || ( isnan(std::floor(f64__f_long_more_lowest))     && isnan(nonstd::cx::floor(f64__f_long_more_lowest)) )     ));

        REQUIRE(( ( std::floor(f_long__positive)               == nonstd::cx::floor(f_long__positive) )               || ( isnan(std::floor(f_long__positive))               && isnan(nonstd::cx::floor(f_long__positive)) )               ));
        REQUIRE(( ( std::floor(f_long__zero)                   == nonstd::cx::floor(f_long__zero) )                   || ( isnan(std::floor(f_long__zero))                   && isnan(nonstd::cx::floor(f_long__zero)) )                   ));
        REQUIRE(( ( std::floor(f_long__negative)               == nonstd::cx::floor(f_long__negative) )               || ( isnan(std::floor(f_long__negative))               && isnan(nonstd::cx::floor(f_long__negative)) )               ));
        REQUIRE(( ( std::floor(f_long__f32_smallest_p)         == nonstd::cx::floor(f_long__f32_smallest_p) )         || ( isnan(std::floor(f_long__f32_smallest_p))         && isnan(nonstd::cx::floor(f_long__f32_smallest_p)) )         ));
        REQUIRE(( ( std::floor(f_long__f32_smallest_n)         == nonstd::cx::floor(f_long__f32_smallest_n) )         || ( isnan(std::floor(f_long__f32_smallest_n))         && isnan(nonstd::cx::floor(f_long__f32_smallest_n)) )         ));
        REQUIRE(( ( std::floor(f_long__f32_highest)            == nonstd::cx::floor(f_long__f32_highest) )            || ( isnan(std::floor(f_long__f32_highest))            && isnan(nonstd::cx::floor(f_long__f32_highest)) )            ));
        REQUIRE(( ( std::floor(f_long__f32_lowest)             == nonstd::cx::floor(f_long__f32_lowest) )             || ( isnan(std::floor(f_long__f32_lowest))             && isnan(nonstd::cx::floor(f_long__f32_lowest)) )             ));
        REQUIRE(( ( std::floor(f_long__f64_smallest_p)         == nonstd::cx::floor(f_long__f64_smallest_p) )         || ( isnan(std::floor(f_long__f64_smallest_p))         && isnan(nonstd::cx::floor(f_long__f64_smallest_p)) )         ));
        REQUIRE(( ( std::floor(f_long__f64_smallest_n)         == nonstd::cx::floor(f_long__f64_smallest_n) )         || ( isnan(std::floor(f_long__f64_smallest_n))         && isnan(nonstd::cx::floor(f_long__f64_smallest_n)) )         ));
        REQUIRE(( ( std::floor(f_long__f64_highest)            == nonstd::cx::floor(f_long__f64_highest) )            || ( isnan(std::floor(f_long__f64_highest))            && isnan(nonstd::cx::floor(f_long__f64_highest)) )            ));
        REQUIRE(( ( std::floor(f_long__f64_lowest)             == nonstd::cx::floor(f_long__f64_lowest) )             || ( isnan(std::floor(f_long__f64_lowest))             && isnan(nonstd::cx::floor(f_long__f64_lowest)) )             ));
        REQUIRE(( ( std::floor(f_long__f_long_smallest_p)      == nonstd::cx::floor(f_long__f_long_smallest_p) )      || ( isnan(std::floor(f_long__f_long_smallest_p))      && isnan(nonstd::cx::floor(f_long__f_long_smallest_p)) )      ));
        REQUIRE(( ( std::floor(f_long__f_long_smallest_n)      == nonstd::cx::floor(f_long__f_long_smallest_n) )      || ( isnan(std::floor(f_long__f_long_smallest_n))      && isnan(nonstd::cx::floor(f_long__f_long_smallest_n)) )      ));
        REQUIRE(( ( std::floor(f_long__f_long_highest)         == nonstd::cx::floor(f_long__f_long_highest) )         || ( isnan(std::floor(f_long__f_long_highest))         && isnan(nonstd::cx::floor(f_long__f_long_highest)) )         ));
        REQUIRE(( ( std::floor(f_long__f_long_lowest)          == nonstd::cx::floor(f_long__f_long_lowest) )          || ( isnan(std::floor(f_long__f_long_lowest))          && isnan(nonstd::cx::floor(f_long__f_long_lowest)) )          ));
        REQUIRE(( ( std::floor(f_long__f32_nan)                == nonstd::cx::floor(f_long__f32_nan) )                || ( isnan(std::floor(f_long__f32_nan))                && isnan(nonstd::cx::floor(f_long__f32_nan)) )                ));
        REQUIRE(( ( std::floor(f_long__f64_nan)                == nonstd::cx::floor(f_long__f64_nan) )                || ( isnan(std::floor(f_long__f64_nan))                && isnan(nonstd::cx::floor(f_long__f64_nan)) )                ));
        REQUIRE(( ( std::floor(f_long__f_long_nan)             == nonstd::cx::floor(f_long__f_long_nan) )             || ( isnan(std::floor(f_long__f_long_nan))             && isnan(nonstd::cx::floor(f_long__f_long_nan)) )             ));
        REQUIRE(( ( std::floor(f_long__f32_p_inf)              == nonstd::cx::floor(f_long__f32_p_inf) )              || ( isnan(std::floor(f_long__f32_p_inf))              && isnan(nonstd::cx::floor(f_long__f32_p_inf)) )              ));
        REQUIRE(( ( std::floor(f_long__f32_n_inf)              == nonstd::cx::floor(f_long__f32_n_inf) )              || ( isnan(std::floor(f_long__f32_n_inf))              && isnan(nonstd::cx::floor(f_long__f32_n_inf)) )              ));
        REQUIRE(( ( std::floor(f_long__f64_p_inf)              == nonstd::cx::floor(f_long__f64_p_inf) )              || ( isnan(std::floor(f_long__f64_p_inf))              && isnan(nonstd::cx::floor(f_long__f64_p_inf)) )              ));
        REQUIRE(( ( std::floor(f_long__f64_n_inf)              == nonstd::cx::floor(f_long__f64_n_inf) )              || ( isnan(std::floor(f_long__f64_n_inf))              && isnan(nonstd::cx::floor(f_long__f64_n_inf)) )              ));
        REQUIRE(( ( std::floor(f_long__f_long_p_inf)           == nonstd::cx::floor(f_long__f_long_p_inf) )           || ( isnan(std::floor(f_long__f_long_p_inf))           && isnan(nonstd::cx::floor(f_long__f_long_p_inf)) )           ));
        REQUIRE(( ( std::floor(f_long__f_long_n_inf)           == nonstd::cx::floor(f_long__f_long_n_inf) )           || ( isnan(std::floor(f_long__f_long_n_inf))           && isnan(nonstd::cx::floor(f_long__f_long_n_inf)) )           ));
        REQUIRE(( ( std::floor(f_long__f32_less_smallest_p)    == nonstd::cx::floor(f_long__f32_less_smallest_p) )    || ( isnan(std::floor(f_long__f32_less_smallest_p))    && isnan(nonstd::cx::floor(f_long__f32_less_smallest_p)) )    ));
        REQUIRE(( ( std::floor(f_long__f32_less_smallest_n)    == nonstd::cx::floor(f_long__f32_less_smallest_n) )    || ( isnan(std::floor(f_long__f32_less_smallest_n))    && isnan(nonstd::cx::floor(f_long__f32_less_smallest_n)) )    ));
        REQUIRE(( ( std::floor(f_long__f32_more_highest)       == nonstd::cx::floor(f_long__f32_more_highest) )       || ( isnan(std::floor(f_long__f32_more_highest))       && isnan(nonstd::cx::floor(f_long__f32_more_highest)) )       ));
        REQUIRE(( ( std::floor(f_long__f32_more_lowest)        == nonstd::cx::floor(f_long__f32_more_lowest) )        || ( isnan(std::floor(f_long__f32_more_lowest))        && isnan(nonstd::cx::floor(f_long__f32_more_lowest)) )        ));
        REQUIRE(( ( std::floor(f_long__f64_less_smallest_p)    == nonstd::cx::floor(f_long__f64_less_smallest_p) )    || ( isnan(std::floor(f_long__f64_less_smallest_p))    && isnan(nonstd::cx::floor(f_long__f64_less_smallest_p)) )    ));
        REQUIRE(( ( std::floor(f_long__f64_less_smallest_n)    == nonstd::cx::floor(f_long__f64_less_smallest_n) )    || ( isnan(std::floor(f_long__f64_less_smallest_n))    && isnan(nonstd::cx::floor(f_long__f64_less_smallest_n)) )    ));
        REQUIRE(( ( std::floor(f_long__f64_more_highest)       == nonstd::cx::floor(f_long__f64_more_highest) )       || ( isnan(std::floor(f_long__f64_more_highest))       && isnan(nonstd::cx::floor(f_long__f64_more_highest)) )       ));
        REQUIRE(( ( std::floor(f_long__f64_more_lowest)        == nonstd::cx::floor(f_long__f64_more_lowest) )        || ( isnan(std::floor(f_long__f64_more_lowest))        && isnan(nonstd::cx::floor(f_long__f64_more_lowest)) )        ));
        REQUIRE(( ( std::floor(f_long__f_long_less_smallest_p) == nonstd::cx::floor(f_long__f_long_less_smallest_p) ) || ( isnan(std::floor(f_long__f_long_less_smallest_p)) && isnan(nonstd::cx::floor(f_long__f_long_less_smallest_p)) ) ));
        REQUIRE(( ( std::floor(f_long__f_long_less_smallest_n) == nonstd::cx::floor(f_long__f_long_less_smallest_n) ) || ( isnan(std::floor(f_long__f_long_less_smallest_n)) && isnan(nonstd::cx::floor(f_long__f_long_less_smallest_n)) ) ));
        REQUIRE(( ( std::floor(f_long__f_long_more_highest)    == nonstd::cx::floor(f_long__f_long_more_highest) )    || ( isnan(std::floor(f_long__f_long_more_highest))    && isnan(nonstd::cx::floor(f_long__f_long_more_highest)) )    ));
        REQUIRE(( ( std::floor(f_long__f_long_more_lowest)     == nonstd::cx::floor(f_long__f_long_more_lowest) )     || ( isnan(std::floor(f_long__f_long_more_lowest))     && isnan(nonstd::cx::floor(f_long__f_long_more_lowest)) )     ));

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
            0.0f,
            std::numeric_limits<float>::max());
        std::uniform_real_distribution<double> rnd_double_range(
            0.0,
            std::numeric_limits<double>::max());
        std::uniform_real_distribution<long double> rnd_long_double_range(
            0.0l,
            std::numeric_limits<long double>::max());

        constexpr int iterations = 10000;
        for (int i = 0; i < iterations; i++) {
            float px = rnd_float_range(re);
            float nx = -px;
            REQUIRE(( ( std::floor(px)  == nonstd::cx::floor(px) ) || ( isnan(std::floor(px)) && isnan(nonstd::cx::floor(px)) ) ));
            REQUIRE(( ( std::floor(nx)  == nonstd::cx::floor(nx) ) || ( isnan(std::floor(nx)) && isnan(nonstd::cx::floor(nx)) ) ));
        }
        for (int i = 0; i < iterations; i++) {
            double px = rnd_double_range(re);
            double nx = -px;
            REQUIRE(( ( std::floor(px)  == nonstd::cx::floor(px) ) || ( isnan(std::floor(px)) && isnan(nonstd::cx::floor(px)) ) ));
            REQUIRE(( ( std::floor(nx)  == nonstd::cx::floor(nx) ) || ( isnan(std::floor(nx)) && isnan(nonstd::cx::floor(nx)) ) ));
        }
        for (int i = 0; i < iterations; i++) {
            long double px = rnd_long_double_range(re);
            long double nx = -px;
            REQUIRE(( ( std::floor(px)  == nonstd::cx::floor(px) ) || ( isnan(std::floor(px)) && isnan(nonstd::cx::floor(px)) ) ));
            REQUIRE(( ( std::floor(nx)  == nonstd::cx::floor(nx) ) || ( isnan(std::floor(nx)) && isnan(nonstd::cx::floor(nx)) ) ));
        }
    }

    SECTION("trunc") {
        constexpr auto calls_to_trunc_are_constexpr = nonstd::cx::trunc(f32__positive);

        using std::isnan;
        REQUIRE(( ( std::trunc(f32__positive)               == nonstd::cx::trunc(f32__positive) )               || ( isnan(std::trunc(f32__positive))               && isnan(nonstd::cx::trunc(f32__positive)) )               ));
        REQUIRE(( ( std::trunc(f32__zero)                   == nonstd::cx::trunc(f32__zero) )                   || ( isnan(std::trunc(f32__zero))                   && isnan(nonstd::cx::trunc(f32__zero)) )                   ));
        REQUIRE(( ( std::trunc(f32__negative)               == nonstd::cx::trunc(f32__negative) )               || ( isnan(std::trunc(f32__negative))               && isnan(nonstd::cx::trunc(f32__negative)) )               ));
        REQUIRE(( ( std::trunc(f32__f32_smallest_p)         == nonstd::cx::trunc(f32__f32_smallest_p) )         || ( isnan(std::trunc(f32__f32_smallest_p))         && isnan(nonstd::cx::trunc(f32__f32_smallest_p)) )         ));
        REQUIRE(( ( std::trunc(f32__f32_smallest_n)         == nonstd::cx::trunc(f32__f32_smallest_n) )         || ( isnan(std::trunc(f32__f32_smallest_n))         && isnan(nonstd::cx::trunc(f32__f32_smallest_n)) )         ));
        REQUIRE(( ( std::trunc(f32__f32_highest)            == nonstd::cx::trunc(f32__f32_highest) )            || ( isnan(std::trunc(f32__f32_highest))            && isnan(nonstd::cx::trunc(f32__f32_highest)) )            ));
        REQUIRE(( ( std::trunc(f32__f32_lowest)             == nonstd::cx::trunc(f32__f32_lowest) )             || ( isnan(std::trunc(f32__f32_lowest))             && isnan(nonstd::cx::trunc(f32__f32_lowest)) )             ));
        REQUIRE(( ( std::trunc(f32__f64_smallest_p)         == nonstd::cx::trunc(f32__f64_smallest_p) )         || ( isnan(std::trunc(f32__f64_smallest_p))         && isnan(nonstd::cx::trunc(f32__f64_smallest_p)) )         ));
        REQUIRE(( ( std::trunc(f32__f64_smallest_n)         == nonstd::cx::trunc(f32__f64_smallest_n) )         || ( isnan(std::trunc(f32__f64_smallest_n))         && isnan(nonstd::cx::trunc(f32__f64_smallest_n)) )         ));
    //  REQUIRE(( ( std::trunc(f32__f64_highest)            == nonstd::cx::trunc(f32__f64_highest) )            || ( isnan(std::trunc(f32__f64_highest))            && isnan(nonstd::cx::trunc(f32__f64_highest)) )            ));
    //  REQUIRE(( ( std::trunc(f32__f64_lowest)             == nonstd::cx::trunc(f32__f64_lowest) )             || ( isnan(std::trunc(f32__f64_lowest))             && isnan(nonstd::cx::trunc(f32__f64_lowest)) )             ));
        REQUIRE(( ( std::trunc(f32__f_long_smallest_p)      == nonstd::cx::trunc(f32__f_long_smallest_p) )      || ( isnan(std::trunc(f32__f_long_smallest_p))      && isnan(nonstd::cx::trunc(f32__f_long_smallest_p)) )      ));
        REQUIRE(( ( std::trunc(f32__f_long_smallest_n)      == nonstd::cx::trunc(f32__f_long_smallest_n) )      || ( isnan(std::trunc(f32__f_long_smallest_n))      && isnan(nonstd::cx::trunc(f32__f_long_smallest_n)) )      ));
    //  REQUIRE(( ( std::trunc(f32__f_long_highest)         == nonstd::cx::trunc(f32__f_long_highest) )         || ( isnan(std::trunc(f32__f_long_highest))         && isnan(nonstd::cx::trunc(f32__f_long_highest)) )         ));
    //  REQUIRE(( ( std::trunc(f32__f_long_lowest)          == nonstd::cx::trunc(f32__f_long_lowest) )          || ( isnan(std::trunc(f32__f_long_lowest))          && isnan(nonstd::cx::trunc(f32__f_long_lowest)) )          ));
        REQUIRE(( ( std::trunc(f32__f32_nan)                == nonstd::cx::trunc(f32__f32_nan) )                || ( isnan(std::trunc(f32__f32_nan))                && isnan(nonstd::cx::trunc(f32__f32_nan)) )                ));
        REQUIRE(( ( std::trunc(f32__f64_nan)                == nonstd::cx::trunc(f32__f64_nan) )                || ( isnan(std::trunc(f32__f64_nan))                && isnan(nonstd::cx::trunc(f32__f64_nan)) )                ));
        REQUIRE(( ( std::trunc(f32__f_long_nan)             == nonstd::cx::trunc(f32__f_long_nan) )             || ( isnan(std::trunc(f32__f_long_nan))             && isnan(nonstd::cx::trunc(f32__f_long_nan)) )             ));
        REQUIRE(( ( std::trunc(f32__f32_p_inf)              == nonstd::cx::trunc(f32__f32_p_inf) )              || ( isnan(std::trunc(f32__f32_p_inf))              && isnan(nonstd::cx::trunc(f32__f32_p_inf)) )              ));
        REQUIRE(( ( std::trunc(f32__f32_n_inf)              == nonstd::cx::trunc(f32__f32_n_inf) )              || ( isnan(std::trunc(f32__f32_n_inf))              && isnan(nonstd::cx::trunc(f32__f32_n_inf)) )              ));
        REQUIRE(( ( std::trunc(f32__f64_p_inf)              == nonstd::cx::trunc(f32__f64_p_inf) )              || ( isnan(std::trunc(f32__f64_p_inf))              && isnan(nonstd::cx::trunc(f32__f64_p_inf)) )              ));
        REQUIRE(( ( std::trunc(f32__f64_n_inf)              == nonstd::cx::trunc(f32__f64_n_inf) )              || ( isnan(std::trunc(f32__f64_n_inf))              && isnan(nonstd::cx::trunc(f32__f64_n_inf)) )              ));
        REQUIRE(( ( std::trunc(f32__f_long_p_inf)           == nonstd::cx::trunc(f32__f_long_p_inf) )           || ( isnan(std::trunc(f32__f_long_p_inf))           && isnan(nonstd::cx::trunc(f32__f_long_p_inf)) )           ));
        REQUIRE(( ( std::trunc(f32__f_long_n_inf)           == nonstd::cx::trunc(f32__f_long_n_inf) )           || ( isnan(std::trunc(f32__f_long_n_inf))           && isnan(nonstd::cx::trunc(f32__f_long_n_inf)) )           ));
        REQUIRE(( ( std::trunc(f32__f32_less_smallest_p)    == nonstd::cx::trunc(f32__f32_less_smallest_p) )    || ( isnan(std::trunc(f32__f32_less_smallest_p))    && isnan(nonstd::cx::trunc(f32__f32_less_smallest_p)) )    ));
        REQUIRE(( ( std::trunc(f32__f32_less_smallest_n)    == nonstd::cx::trunc(f32__f32_less_smallest_n) )    || ( isnan(std::trunc(f32__f32_less_smallest_n))    && isnan(nonstd::cx::trunc(f32__f32_less_smallest_n)) )    ));
        REQUIRE(( ( std::trunc(f32__f32_more_highest)       == nonstd::cx::trunc(f32__f32_more_highest) )       || ( isnan(std::trunc(f32__f32_more_highest))       && isnan(nonstd::cx::trunc(f32__f32_more_highest)) )       ));
        REQUIRE(( ( std::trunc(f32__f32_more_lowest)        == nonstd::cx::trunc(f32__f32_more_lowest) )        || ( isnan(std::trunc(f32__f32_more_lowest))        && isnan(nonstd::cx::trunc(f32__f32_more_lowest)) )        ));
        REQUIRE(( ( std::trunc(f32__f64_less_smallest_p)    == nonstd::cx::trunc(f32__f64_less_smallest_p) )    || ( isnan(std::trunc(f32__f64_less_smallest_p))    && isnan(nonstd::cx::trunc(f32__f64_less_smallest_p)) )    ));
        REQUIRE(( ( std::trunc(f32__f64_less_smallest_n)    == nonstd::cx::trunc(f32__f64_less_smallest_n) )    || ( isnan(std::trunc(f32__f64_less_smallest_n))    && isnan(nonstd::cx::trunc(f32__f64_less_smallest_n)) )    ));
        REQUIRE(( ( std::trunc(f32__f64_more_highest)       == nonstd::cx::trunc(f32__f64_more_highest) )       || ( isnan(std::trunc(f32__f64_more_highest))       && isnan(nonstd::cx::trunc(f32__f64_more_highest)) )       ));
        REQUIRE(( ( std::trunc(f32__f64_more_lowest)        == nonstd::cx::trunc(f32__f64_more_lowest) )        || ( isnan(std::trunc(f32__f64_more_lowest))        && isnan(nonstd::cx::trunc(f32__f64_more_lowest)) )        ));
        REQUIRE(( ( std::trunc(f32__f_long_less_smallest_p) == nonstd::cx::trunc(f32__f_long_less_smallest_p) ) || ( isnan(std::trunc(f32__f_long_less_smallest_p)) && isnan(nonstd::cx::trunc(f32__f_long_less_smallest_p)) ) ));
        REQUIRE(( ( std::trunc(f32__f_long_less_smallest_n) == nonstd::cx::trunc(f32__f_long_less_smallest_n) ) || ( isnan(std::trunc(f32__f_long_less_smallest_n)) && isnan(nonstd::cx::trunc(f32__f_long_less_smallest_n)) ) ));
        REQUIRE(( ( std::trunc(f32__f_long_more_highest)    == nonstd::cx::trunc(f32__f_long_more_highest) )    || ( isnan(std::trunc(f32__f_long_more_highest))    && isnan(nonstd::cx::trunc(f32__f_long_more_highest)) )    ));
        REQUIRE(( ( std::trunc(f32__f_long_more_lowest)     == nonstd::cx::trunc(f32__f_long_more_lowest) )     || ( isnan(std::trunc(f32__f_long_more_lowest))     && isnan(nonstd::cx::trunc(f32__f_long_more_lowest)) )     ));

        REQUIRE(( ( std::trunc(f64__positive)               == nonstd::cx::trunc(f64__positive) )               || ( isnan(std::trunc(f64__positive))               && isnan(nonstd::cx::trunc(f64__positive)) )               ));
        REQUIRE(( ( std::trunc(f64__zero)                   == nonstd::cx::trunc(f64__zero) )                   || ( isnan(std::trunc(f64__zero))                   && isnan(nonstd::cx::trunc(f64__zero)) )                   ));
        REQUIRE(( ( std::trunc(f64__negative)               == nonstd::cx::trunc(f64__negative) )               || ( isnan(std::trunc(f64__negative))               && isnan(nonstd::cx::trunc(f64__negative)) )               ));
        REQUIRE(( ( std::trunc(f64__f32_smallest_p)         == nonstd::cx::trunc(f64__f32_smallest_p) )         || ( isnan(std::trunc(f64__f32_smallest_p))         && isnan(nonstd::cx::trunc(f64__f32_smallest_p)) )         ));
        REQUIRE(( ( std::trunc(f64__f32_smallest_n)         == nonstd::cx::trunc(f64__f32_smallest_n) )         || ( isnan(std::trunc(f64__f32_smallest_n))         && isnan(nonstd::cx::trunc(f64__f32_smallest_n)) )         ));
        REQUIRE(( ( std::trunc(f64__f32_highest)            == nonstd::cx::trunc(f64__f32_highest) )            || ( isnan(std::trunc(f64__f32_highest))            && isnan(nonstd::cx::trunc(f64__f32_highest)) )            ));
        REQUIRE(( ( std::trunc(f64__f32_lowest)             == nonstd::cx::trunc(f64__f32_lowest) )             || ( isnan(std::trunc(f64__f32_lowest))             && isnan(nonstd::cx::trunc(f64__f32_lowest)) )             ));
        REQUIRE(( ( std::trunc(f64__f64_smallest_p)         == nonstd::cx::trunc(f64__f64_smallest_p) )         || ( isnan(std::trunc(f64__f64_smallest_p))         && isnan(nonstd::cx::trunc(f64__f64_smallest_p)) )         ));
        REQUIRE(( ( std::trunc(f64__f64_smallest_n)         == nonstd::cx::trunc(f64__f64_smallest_n) )         || ( isnan(std::trunc(f64__f64_smallest_n))         && isnan(nonstd::cx::trunc(f64__f64_smallest_n)) )         ));
        REQUIRE(( ( std::trunc(f64__f64_highest)            == nonstd::cx::trunc(f64__f64_highest) )            || ( isnan(std::trunc(f64__f64_highest))            && isnan(nonstd::cx::trunc(f64__f64_highest)) )            ));
        REQUIRE(( ( std::trunc(f64__f64_lowest)             == nonstd::cx::trunc(f64__f64_lowest) )             || ( isnan(std::trunc(f64__f64_lowest))             && isnan(nonstd::cx::trunc(f64__f64_lowest)) )             ));
        REQUIRE(( ( std::trunc(f64__f_long_smallest_p)      == nonstd::cx::trunc(f64__f_long_smallest_p) )      || ( isnan(std::trunc(f64__f_long_smallest_p))      && isnan(nonstd::cx::trunc(f64__f_long_smallest_p)) )      ));
        REQUIRE(( ( std::trunc(f64__f_long_smallest_n)      == nonstd::cx::trunc(f64__f_long_smallest_n) )      || ( isnan(std::trunc(f64__f_long_smallest_n))      && isnan(nonstd::cx::trunc(f64__f_long_smallest_n)) )      ));
    //  REQUIRE(( ( std::trunc(f64__f_long_highest)         == nonstd::cx::trunc(f64__f_long_highest) )         || ( isnan(std::trunc(f64__f_long_highest))         && isnan(nonstd::cx::trunc(f64__f_long_highest)) )         ));
    //  REQUIRE(( ( std::trunc(f64__f_long_lowest)          == nonstd::cx::trunc(f64__f_long_lowest) )          || ( isnan(std::trunc(f64__f_long_lowest))          && isnan(nonstd::cx::trunc(f64__f_long_lowest)) )          ));
        REQUIRE(( ( std::trunc(f64__f32_nan)                == nonstd::cx::trunc(f64__f32_nan) )                || ( isnan(std::trunc(f64__f32_nan))                && isnan(nonstd::cx::trunc(f64__f32_nan)) )                ));
        REQUIRE(( ( std::trunc(f64__f64_nan)                == nonstd::cx::trunc(f64__f64_nan) )                || ( isnan(std::trunc(f64__f64_nan))                && isnan(nonstd::cx::trunc(f64__f64_nan)) )                ));
        REQUIRE(( ( std::trunc(f64__f_long_nan)             == nonstd::cx::trunc(f64__f_long_nan) )             || ( isnan(std::trunc(f64__f_long_nan))             && isnan(nonstd::cx::trunc(f64__f_long_nan)) )             ));
        REQUIRE(( ( std::trunc(f64__f32_p_inf)              == nonstd::cx::trunc(f64__f32_p_inf) )              || ( isnan(std::trunc(f64__f32_p_inf))              && isnan(nonstd::cx::trunc(f64__f32_p_inf)) )              ));
        REQUIRE(( ( std::trunc(f64__f32_n_inf)              == nonstd::cx::trunc(f64__f32_n_inf) )              || ( isnan(std::trunc(f64__f32_n_inf))              && isnan(nonstd::cx::trunc(f64__f32_n_inf)) )              ));
        REQUIRE(( ( std::trunc(f64__f64_p_inf)              == nonstd::cx::trunc(f64__f64_p_inf) )              || ( isnan(std::trunc(f64__f64_p_inf))              && isnan(nonstd::cx::trunc(f64__f64_p_inf)) )              ));
        REQUIRE(( ( std::trunc(f64__f64_n_inf)              == nonstd::cx::trunc(f64__f64_n_inf) )              || ( isnan(std::trunc(f64__f64_n_inf))              && isnan(nonstd::cx::trunc(f64__f64_n_inf)) )              ));
        REQUIRE(( ( std::trunc(f64__f_long_p_inf)           == nonstd::cx::trunc(f64__f_long_p_inf) )           || ( isnan(std::trunc(f64__f_long_p_inf))           && isnan(nonstd::cx::trunc(f64__f_long_p_inf)) )           ));
        REQUIRE(( ( std::trunc(f64__f_long_n_inf)           == nonstd::cx::trunc(f64__f_long_n_inf) )           || ( isnan(std::trunc(f64__f_long_n_inf))           && isnan(nonstd::cx::trunc(f64__f_long_n_inf)) )           ));
        REQUIRE(( ( std::trunc(f64__f32_less_smallest_p)    == nonstd::cx::trunc(f64__f32_less_smallest_p) )    || ( isnan(std::trunc(f64__f32_less_smallest_p))    && isnan(nonstd::cx::trunc(f64__f32_less_smallest_p)) )    ));
        REQUIRE(( ( std::trunc(f64__f32_less_smallest_n)    == nonstd::cx::trunc(f64__f32_less_smallest_n) )    || ( isnan(std::trunc(f64__f32_less_smallest_n))    && isnan(nonstd::cx::trunc(f64__f32_less_smallest_n)) )    ));
        REQUIRE(( ( std::trunc(f64__f32_more_highest)       == nonstd::cx::trunc(f64__f32_more_highest) )       || ( isnan(std::trunc(f64__f32_more_highest))       && isnan(nonstd::cx::trunc(f64__f32_more_highest)) )       ));
        REQUIRE(( ( std::trunc(f64__f32_more_lowest)        == nonstd::cx::trunc(f64__f32_more_lowest) )        || ( isnan(std::trunc(f64__f32_more_lowest))        && isnan(nonstd::cx::trunc(f64__f32_more_lowest)) )        ));
        REQUIRE(( ( std::trunc(f64__f64_less_smallest_p)    == nonstd::cx::trunc(f64__f64_less_smallest_p) )    || ( isnan(std::trunc(f64__f64_less_smallest_p))    && isnan(nonstd::cx::trunc(f64__f64_less_smallest_p)) )    ));
        REQUIRE(( ( std::trunc(f64__f64_less_smallest_n)    == nonstd::cx::trunc(f64__f64_less_smallest_n) )    || ( isnan(std::trunc(f64__f64_less_smallest_n))    && isnan(nonstd::cx::trunc(f64__f64_less_smallest_n)) )    ));
        REQUIRE(( ( std::trunc(f64__f64_more_highest)       == nonstd::cx::trunc(f64__f64_more_highest) )       || ( isnan(std::trunc(f64__f64_more_highest))       && isnan(nonstd::cx::trunc(f64__f64_more_highest)) )       ));
        REQUIRE(( ( std::trunc(f64__f64_more_lowest)        == nonstd::cx::trunc(f64__f64_more_lowest) )        || ( isnan(std::trunc(f64__f64_more_lowest))        && isnan(nonstd::cx::trunc(f64__f64_more_lowest)) )        ));
        REQUIRE(( ( std::trunc(f64__f_long_less_smallest_p) == nonstd::cx::trunc(f64__f_long_less_smallest_p) ) || ( isnan(std::trunc(f64__f_long_less_smallest_p)) && isnan(nonstd::cx::trunc(f64__f_long_less_smallest_p)) ) ));
        REQUIRE(( ( std::trunc(f64__f_long_less_smallest_n) == nonstd::cx::trunc(f64__f_long_less_smallest_n) ) || ( isnan(std::trunc(f64__f_long_less_smallest_n)) && isnan(nonstd::cx::trunc(f64__f_long_less_smallest_n)) ) ));
        REQUIRE(( ( std::trunc(f64__f_long_more_highest)    == nonstd::cx::trunc(f64__f_long_more_highest) )    || ( isnan(std::trunc(f64__f_long_more_highest))    && isnan(nonstd::cx::trunc(f64__f_long_more_highest)) )    ));
        REQUIRE(( ( std::trunc(f64__f_long_more_lowest)     == nonstd::cx::trunc(f64__f_long_more_lowest) )     || ( isnan(std::trunc(f64__f_long_more_lowest))     && isnan(nonstd::cx::trunc(f64__f_long_more_lowest)) )     ));

        REQUIRE(( ( std::trunc(f_long__positive)               == nonstd::cx::trunc(f_long__positive) )               || ( isnan(std::trunc(f_long__positive))               && isnan(nonstd::cx::trunc(f_long__positive)) )               ));
        REQUIRE(( ( std::trunc(f_long__zero)                   == nonstd::cx::trunc(f_long__zero) )                   || ( isnan(std::trunc(f_long__zero))                   && isnan(nonstd::cx::trunc(f_long__zero)) )                   ));
        REQUIRE(( ( std::trunc(f_long__negative)               == nonstd::cx::trunc(f_long__negative) )               || ( isnan(std::trunc(f_long__negative))               && isnan(nonstd::cx::trunc(f_long__negative)) )               ));
        REQUIRE(( ( std::trunc(f_long__f32_smallest_p)         == nonstd::cx::trunc(f_long__f32_smallest_p) )         || ( isnan(std::trunc(f_long__f32_smallest_p))         && isnan(nonstd::cx::trunc(f_long__f32_smallest_p)) )         ));
        REQUIRE(( ( std::trunc(f_long__f32_smallest_n)         == nonstd::cx::trunc(f_long__f32_smallest_n) )         || ( isnan(std::trunc(f_long__f32_smallest_n))         && isnan(nonstd::cx::trunc(f_long__f32_smallest_n)) )         ));
        REQUIRE(( ( std::trunc(f_long__f32_highest)            == nonstd::cx::trunc(f_long__f32_highest) )            || ( isnan(std::trunc(f_long__f32_highest))            && isnan(nonstd::cx::trunc(f_long__f32_highest)) )            ));
        REQUIRE(( ( std::trunc(f_long__f32_lowest)             == nonstd::cx::trunc(f_long__f32_lowest) )             || ( isnan(std::trunc(f_long__f32_lowest))             && isnan(nonstd::cx::trunc(f_long__f32_lowest)) )             ));
        REQUIRE(( ( std::trunc(f_long__f64_smallest_p)         == nonstd::cx::trunc(f_long__f64_smallest_p) )         || ( isnan(std::trunc(f_long__f64_smallest_p))         && isnan(nonstd::cx::trunc(f_long__f64_smallest_p)) )         ));
        REQUIRE(( ( std::trunc(f_long__f64_smallest_n)         == nonstd::cx::trunc(f_long__f64_smallest_n) )         || ( isnan(std::trunc(f_long__f64_smallest_n))         && isnan(nonstd::cx::trunc(f_long__f64_smallest_n)) )         ));
        REQUIRE(( ( std::trunc(f_long__f64_highest)            == nonstd::cx::trunc(f_long__f64_highest) )            || ( isnan(std::trunc(f_long__f64_highest))            && isnan(nonstd::cx::trunc(f_long__f64_highest)) )            ));
        REQUIRE(( ( std::trunc(f_long__f64_lowest)             == nonstd::cx::trunc(f_long__f64_lowest) )             || ( isnan(std::trunc(f_long__f64_lowest))             && isnan(nonstd::cx::trunc(f_long__f64_lowest)) )             ));
        REQUIRE(( ( std::trunc(f_long__f_long_smallest_p)      == nonstd::cx::trunc(f_long__f_long_smallest_p) )      || ( isnan(std::trunc(f_long__f_long_smallest_p))      && isnan(nonstd::cx::trunc(f_long__f_long_smallest_p)) )      ));
        REQUIRE(( ( std::trunc(f_long__f_long_smallest_n)      == nonstd::cx::trunc(f_long__f_long_smallest_n) )      || ( isnan(std::trunc(f_long__f_long_smallest_n))      && isnan(nonstd::cx::trunc(f_long__f_long_smallest_n)) )      ));
        REQUIRE(( ( std::trunc(f_long__f_long_highest)         == nonstd::cx::trunc(f_long__f_long_highest) )         || ( isnan(std::trunc(f_long__f_long_highest))         && isnan(nonstd::cx::trunc(f_long__f_long_highest)) )         ));
        REQUIRE(( ( std::trunc(f_long__f_long_lowest)          == nonstd::cx::trunc(f_long__f_long_lowest) )          || ( isnan(std::trunc(f_long__f_long_lowest))          && isnan(nonstd::cx::trunc(f_long__f_long_lowest)) )          ));
        REQUIRE(( ( std::trunc(f_long__f32_nan)                == nonstd::cx::trunc(f_long__f32_nan) )                || ( isnan(std::trunc(f_long__f32_nan))                && isnan(nonstd::cx::trunc(f_long__f32_nan)) )                ));
        REQUIRE(( ( std::trunc(f_long__f64_nan)                == nonstd::cx::trunc(f_long__f64_nan) )                || ( isnan(std::trunc(f_long__f64_nan))                && isnan(nonstd::cx::trunc(f_long__f64_nan)) )                ));
        REQUIRE(( ( std::trunc(f_long__f_long_nan)             == nonstd::cx::trunc(f_long__f_long_nan) )             || ( isnan(std::trunc(f_long__f_long_nan))             && isnan(nonstd::cx::trunc(f_long__f_long_nan)) )             ));
        REQUIRE(( ( std::trunc(f_long__f32_p_inf)              == nonstd::cx::trunc(f_long__f32_p_inf) )              || ( isnan(std::trunc(f_long__f32_p_inf))              && isnan(nonstd::cx::trunc(f_long__f32_p_inf)) )              ));
        REQUIRE(( ( std::trunc(f_long__f32_n_inf)              == nonstd::cx::trunc(f_long__f32_n_inf) )              || ( isnan(std::trunc(f_long__f32_n_inf))              && isnan(nonstd::cx::trunc(f_long__f32_n_inf)) )              ));
        REQUIRE(( ( std::trunc(f_long__f64_p_inf)              == nonstd::cx::trunc(f_long__f64_p_inf) )              || ( isnan(std::trunc(f_long__f64_p_inf))              && isnan(nonstd::cx::trunc(f_long__f64_p_inf)) )              ));
        REQUIRE(( ( std::trunc(f_long__f64_n_inf)              == nonstd::cx::trunc(f_long__f64_n_inf) )              || ( isnan(std::trunc(f_long__f64_n_inf))              && isnan(nonstd::cx::trunc(f_long__f64_n_inf)) )              ));
        REQUIRE(( ( std::trunc(f_long__f_long_p_inf)           == nonstd::cx::trunc(f_long__f_long_p_inf) )           || ( isnan(std::trunc(f_long__f_long_p_inf))           && isnan(nonstd::cx::trunc(f_long__f_long_p_inf)) )           ));
        REQUIRE(( ( std::trunc(f_long__f_long_n_inf)           == nonstd::cx::trunc(f_long__f_long_n_inf) )           || ( isnan(std::trunc(f_long__f_long_n_inf))           && isnan(nonstd::cx::trunc(f_long__f_long_n_inf)) )           ));
        REQUIRE(( ( std::trunc(f_long__f32_less_smallest_p)    == nonstd::cx::trunc(f_long__f32_less_smallest_p) )    || ( isnan(std::trunc(f_long__f32_less_smallest_p))    && isnan(nonstd::cx::trunc(f_long__f32_less_smallest_p)) )    ));
        REQUIRE(( ( std::trunc(f_long__f32_less_smallest_n)    == nonstd::cx::trunc(f_long__f32_less_smallest_n) )    || ( isnan(std::trunc(f_long__f32_less_smallest_n))    && isnan(nonstd::cx::trunc(f_long__f32_less_smallest_n)) )    ));
        REQUIRE(( ( std::trunc(f_long__f32_more_highest)       == nonstd::cx::trunc(f_long__f32_more_highest) )       || ( isnan(std::trunc(f_long__f32_more_highest))       && isnan(nonstd::cx::trunc(f_long__f32_more_highest)) )       ));
        REQUIRE(( ( std::trunc(f_long__f32_more_lowest)        == nonstd::cx::trunc(f_long__f32_more_lowest) )        || ( isnan(std::trunc(f_long__f32_more_lowest))        && isnan(nonstd::cx::trunc(f_long__f32_more_lowest)) )        ));
        REQUIRE(( ( std::trunc(f_long__f64_less_smallest_p)    == nonstd::cx::trunc(f_long__f64_less_smallest_p) )    || ( isnan(std::trunc(f_long__f64_less_smallest_p))    && isnan(nonstd::cx::trunc(f_long__f64_less_smallest_p)) )    ));
        REQUIRE(( ( std::trunc(f_long__f64_less_smallest_n)    == nonstd::cx::trunc(f_long__f64_less_smallest_n) )    || ( isnan(std::trunc(f_long__f64_less_smallest_n))    && isnan(nonstd::cx::trunc(f_long__f64_less_smallest_n)) )    ));
        REQUIRE(( ( std::trunc(f_long__f64_more_highest)       == nonstd::cx::trunc(f_long__f64_more_highest) )       || ( isnan(std::trunc(f_long__f64_more_highest))       && isnan(nonstd::cx::trunc(f_long__f64_more_highest)) )       ));
        REQUIRE(( ( std::trunc(f_long__f64_more_lowest)        == nonstd::cx::trunc(f_long__f64_more_lowest) )        || ( isnan(std::trunc(f_long__f64_more_lowest))        && isnan(nonstd::cx::trunc(f_long__f64_more_lowest)) )        ));
        REQUIRE(( ( std::trunc(f_long__f_long_less_smallest_p) == nonstd::cx::trunc(f_long__f_long_less_smallest_p) ) || ( isnan(std::trunc(f_long__f_long_less_smallest_p)) && isnan(nonstd::cx::trunc(f_long__f_long_less_smallest_p)) ) ));
        REQUIRE(( ( std::trunc(f_long__f_long_less_smallest_n) == nonstd::cx::trunc(f_long__f_long_less_smallest_n) ) || ( isnan(std::trunc(f_long__f_long_less_smallest_n)) && isnan(nonstd::cx::trunc(f_long__f_long_less_smallest_n)) ) ));
        REQUIRE(( ( std::trunc(f_long__f_long_more_highest)    == nonstd::cx::trunc(f_long__f_long_more_highest) )    || ( isnan(std::trunc(f_long__f_long_more_highest))    && isnan(nonstd::cx::trunc(f_long__f_long_more_highest)) )    ));
        REQUIRE(( ( std::trunc(f_long__f_long_more_lowest)     == nonstd::cx::trunc(f_long__f_long_more_lowest) )     || ( isnan(std::trunc(f_long__f_long_more_lowest))     && isnan(nonstd::cx::trunc(f_long__f_long_more_lowest)) )     ));

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
            0.0f,
            std::numeric_limits<float>::max());
        std::uniform_real_distribution<double> rnd_double_range(
            0.0,
            std::numeric_limits<double>::max());
        std::uniform_real_distribution<long double> rnd_long_double_range(
            0.0l,
            std::numeric_limits<long double>::max());

        constexpr int iterations = 10000;
        for (int i = 0; i < iterations; i++) {
            float px = rnd_float_range(re);
            float nx = -px;
            REQUIRE(( ( std::trunc(px)  == nonstd::cx::trunc(px) ) || ( isnan(std::trunc(px)) && isnan(nonstd::cx::trunc(px)) ) ));
            REQUIRE(( ( std::trunc(nx)  == nonstd::cx::trunc(nx) ) || ( isnan(std::trunc(nx)) && isnan(nonstd::cx::trunc(nx)) ) ));
        }
        for (int i = 0; i < iterations; i++) {
            double px = rnd_double_range(re);
            double nx = -px;
            REQUIRE(( ( std::trunc(px)  == nonstd::cx::trunc(px) ) || ( isnan(std::trunc(px)) && isnan(nonstd::cx::trunc(px)) ) ));
            REQUIRE(( ( std::trunc(nx)  == nonstd::cx::trunc(nx) ) || ( isnan(std::trunc(nx)) && isnan(nonstd::cx::trunc(nx)) ) ));
        }
        for (int i = 0; i < iterations; i++) {
            long double px = rnd_long_double_range(re);
            long double nx = -px;
            REQUIRE(( ( std::trunc(px)  == nonstd::cx::trunc(px) ) || ( isnan(std::trunc(px)) && isnan(nonstd::cx::trunc(px)) ) ));
            REQUIRE(( ( std::trunc(nx)  == nonstd::cx::trunc(nx) ) || ( isnan(std::trunc(nx)) && isnan(nonstd::cx::trunc(nx)) ) ));
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
        REQUIRE(( ( std::fmod(f32__f32_smallest_p,  1.0f) == nonstd::cx::fmod(f32__f32_smallest_p,  1.0f) ) || ( isnan(std::fmod(f32__f32_smallest_p,  1.0f)) && isnan(nonstd::cx::fmod(f32__f32_smallest_p,  1.0f)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_smallest_p, -1.0f) == nonstd::cx::fmod(f32__f32_smallest_p, -1.0f) ) || ( isnan(std::fmod(f32__f32_smallest_p, -1.0f)) && isnan(nonstd::cx::fmod(f32__f32_smallest_p, -1.0f)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_smallest_n,  1.0f) == nonstd::cx::fmod(f32__f32_smallest_n,  1.0f) ) || ( isnan(std::fmod(f32__f32_smallest_n,  1.0f)) && isnan(nonstd::cx::fmod(f32__f32_smallest_n,  1.0f)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_smallest_n, -1.0f) == nonstd::cx::fmod(f32__f32_smallest_n, -1.0f) ) || ( isnan(std::fmod(f32__f32_smallest_n, -1.0f)) && isnan(nonstd::cx::fmod(f32__f32_smallest_n, -1.0f)) ) ));
        REQUIRE(( ( std::fmod( 1.0f, f32__f32_smallest_p) == nonstd::cx::fmod( 1.0f, f32__f32_smallest_p) ) || ( isnan(std::fmod( 1.0f, f32__f32_smallest_p)) && isnan(nonstd::cx::fmod( 1.0f, f32__f32_smallest_p)) ) ));
        REQUIRE(( ( std::fmod(-1.0f, f32__f32_smallest_p) == nonstd::cx::fmod(-1.0f, f32__f32_smallest_p) ) || ( isnan(std::fmod(-1.0f, f32__f32_smallest_p)) && isnan(nonstd::cx::fmod(-1.0f, f32__f32_smallest_p)) ) ));
        REQUIRE(( ( std::fmod( 1.0f, f32__f32_smallest_n) == nonstd::cx::fmod( 1.0f, f32__f32_smallest_n) ) || ( isnan(std::fmod( 1.0f, f32__f32_smallest_n)) && isnan(nonstd::cx::fmod( 1.0f, f32__f32_smallest_n)) ) ));
        REQUIRE(( ( std::fmod(-1.0f, f32__f32_smallest_n) == nonstd::cx::fmod(-1.0f, f32__f32_smallest_n) ) || ( isnan(std::fmod(-1.0f, f32__f32_smallest_n)) && isnan(nonstd::cx::fmod(-1.0f, f32__f32_smallest_n)) ) ));

        REQUIRE(( ( std::fmod(f32__f32_highest,  1.0f) == nonstd::cx::fmod(f32__f32_highest,  1.0f) ) || ( isnan(std::fmod(f32__f32_highest,  1.0f)) && isnan(nonstd::cx::fmod(f32__f32_highest,  1.0f)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_highest, -1.0f) == nonstd::cx::fmod(f32__f32_highest, -1.0f) ) || ( isnan(std::fmod(f32__f32_highest, -1.0f)) && isnan(nonstd::cx::fmod(f32__f32_highest, -1.0f)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_lowest,  1.0f)  == nonstd::cx::fmod(f32__f32_lowest,  1.0f) )  || ( isnan(std::fmod(f32__f32_lowest,  1.0f))  && isnan(nonstd::cx::fmod(f32__f32_lowest,  1.0f)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_lowest, -1.0f)  == nonstd::cx::fmod(f32__f32_lowest, -1.0f) )  || ( isnan(std::fmod(f32__f32_lowest, -1.0f))  && isnan(nonstd::cx::fmod(f32__f32_lowest, -1.0f)) ) ));
        REQUIRE(( ( std::fmod( 1.0f, f32__f32_highest) == nonstd::cx::fmod( 1.0f, f32__f32_highest) ) || ( isnan(std::fmod( 1.0f, f32__f32_highest)) && isnan(nonstd::cx::fmod( 1.0f, f32__f32_highest)) ) ));
        REQUIRE(( ( std::fmod(-1.0f, f32__f32_highest) == nonstd::cx::fmod(-1.0f, f32__f32_highest) ) || ( isnan(std::fmod(-1.0f, f32__f32_highest)) && isnan(nonstd::cx::fmod(-1.0f, f32__f32_highest)) ) ));
        REQUIRE(( ( std::fmod( 1.0f, f32__f32_lowest)  == nonstd::cx::fmod( 1.0f, f32__f32_lowest) )  || ( isnan(std::fmod( 1.0f, f32__f32_lowest))  && isnan(nonstd::cx::fmod( 1.0f, f32__f32_lowest)) ) ));
        REQUIRE(( ( std::fmod(-1.0f, f32__f32_lowest)  == nonstd::cx::fmod(-1.0f, f32__f32_lowest) )  || ( isnan(std::fmod(-1.0f, f32__f32_lowest))  && isnan(nonstd::cx::fmod(-1.0f, f32__f32_lowest)) ) ));


        REQUIRE(( ( std::fmod(f64__f64_smallest_p,  1.0) == nonstd::cx::fmod(f64__f64_smallest_p,  1.0) ) || ( isnan(std::fmod(f64__f64_smallest_p,  1.0)) && isnan(nonstd::cx::fmod(f64__f64_smallest_p,  1.0)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_smallest_p, -1.0) == nonstd::cx::fmod(f64__f64_smallest_p, -1.0) ) || ( isnan(std::fmod(f64__f64_smallest_p, -1.0)) && isnan(nonstd::cx::fmod(f64__f64_smallest_p, -1.0)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_smallest_n,  1.0) == nonstd::cx::fmod(f64__f64_smallest_n,  1.0) ) || ( isnan(std::fmod(f64__f64_smallest_n,  1.0)) && isnan(nonstd::cx::fmod(f64__f64_smallest_n,  1.0)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_smallest_n, -1.0) == nonstd::cx::fmod(f64__f64_smallest_n, -1.0) ) || ( isnan(std::fmod(f64__f64_smallest_n, -1.0)) && isnan(nonstd::cx::fmod(f64__f64_smallest_n, -1.0)) ) ));
        REQUIRE(( ( std::fmod( 1.0, f64__f64_smallest_p) == nonstd::cx::fmod( 1.0, f64__f64_smallest_p) ) || ( isnan(std::fmod( 1.0, f64__f64_smallest_p)) && isnan(nonstd::cx::fmod( 1.0, f64__f64_smallest_p)) ) ));
        REQUIRE(( ( std::fmod(-1.0, f64__f64_smallest_p) == nonstd::cx::fmod(-1.0, f64__f64_smallest_p) ) || ( isnan(std::fmod(-1.0, f64__f64_smallest_p)) && isnan(nonstd::cx::fmod(-1.0, f64__f64_smallest_p)) ) ));
        REQUIRE(( ( std::fmod( 1.0, f64__f64_smallest_n) == nonstd::cx::fmod( 1.0, f64__f64_smallest_n) ) || ( isnan(std::fmod( 1.0, f64__f64_smallest_n)) && isnan(nonstd::cx::fmod( 1.0, f64__f64_smallest_n)) ) ));
        REQUIRE(( ( std::fmod(-1.0, f64__f64_smallest_n) == nonstd::cx::fmod(-1.0, f64__f64_smallest_n) ) || ( isnan(std::fmod(-1.0, f64__f64_smallest_n)) && isnan(nonstd::cx::fmod(-1.0, f64__f64_smallest_n)) ) ));

        REQUIRE(( ( std::fmod(f64__f64_highest,  1.0) == nonstd::cx::fmod(f64__f64_highest,  1.0) ) || ( isnan(std::fmod(f64__f64_highest,  1.0)) && isnan(nonstd::cx::fmod(f64__f64_highest,  1.0)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_highest, -1.0) == nonstd::cx::fmod(f64__f64_highest, -1.0) ) || ( isnan(std::fmod(f64__f64_highest, -1.0)) && isnan(nonstd::cx::fmod(f64__f64_highest, -1.0)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_lowest,  1.0)  == nonstd::cx::fmod(f64__f64_lowest,  1.0) )  || ( isnan(std::fmod(f64__f64_lowest,  1.0))  && isnan(nonstd::cx::fmod(f64__f64_lowest,  1.0)) )  ));
        REQUIRE(( ( std::fmod(f64__f64_lowest, -1.0)  == nonstd::cx::fmod(f64__f64_lowest, -1.0) )  || ( isnan(std::fmod(f64__f64_lowest, -1.0))  && isnan(nonstd::cx::fmod(f64__f64_lowest, -1.0)) )  ));
        REQUIRE(( ( std::fmod( 1.0, f64__f64_highest) == nonstd::cx::fmod( 1.0, f64__f64_highest) ) || ( isnan(std::fmod( 1.0, f64__f64_highest)) && isnan(nonstd::cx::fmod( 1.0, f64__f64_highest)) ) ));
        REQUIRE(( ( std::fmod(-1.0, f64__f64_highest) == nonstd::cx::fmod(-1.0, f64__f64_highest) ) || ( isnan(std::fmod(-1.0, f64__f64_highest)) && isnan(nonstd::cx::fmod(-1.0, f64__f64_highest)) ) ));
        REQUIRE(( ( std::fmod( 1.0, f64__f64_lowest)  == nonstd::cx::fmod( 1.0, f64__f64_lowest) )  || ( isnan(std::fmod( 1.0, f64__f64_lowest))  && isnan(nonstd::cx::fmod( 1.0, f64__f64_lowest)) )  ));
        REQUIRE(( ( std::fmod(-1.0, f64__f64_lowest)  == nonstd::cx::fmod(-1.0, f64__f64_lowest) )  || ( isnan(std::fmod(-1.0, f64__f64_lowest))  && isnan(nonstd::cx::fmod(-1.0, f64__f64_lowest)) )  ));


        REQUIRE(( ( std::fmod(f_long__f_long_smallest_p,  1.0l) == nonstd::cx::fmod(f_long__f_long_smallest_p,  1.0l) ) || ( isnan(std::fmod(f_long__f_long_smallest_p,  1.0l)) && isnan(nonstd::cx::fmod(f_long__f_long_smallest_p,  1.0l)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_smallest_p, -1.0l) == nonstd::cx::fmod(f_long__f_long_smallest_p, -1.0l) ) || ( isnan(std::fmod(f_long__f_long_smallest_p, -1.0l)) && isnan(nonstd::cx::fmod(f_long__f_long_smallest_p, -1.0l)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_smallest_n,  1.0l) == nonstd::cx::fmod(f_long__f_long_smallest_n,  1.0l) ) || ( isnan(std::fmod(f_long__f_long_smallest_n,  1.0l)) && isnan(nonstd::cx::fmod(f_long__f_long_smallest_n,  1.0l)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_smallest_n, -1.0l) == nonstd::cx::fmod(f_long__f_long_smallest_n, -1.0l) ) || ( isnan(std::fmod(f_long__f_long_smallest_n, -1.0l)) && isnan(nonstd::cx::fmod(f_long__f_long_smallest_n, -1.0l)) ) ));
        REQUIRE(( ( std::fmod( 1.0l, f_long__f_long_smallest_p) == nonstd::cx::fmod( 1.0l, f_long__f_long_smallest_p) ) || ( isnan(std::fmod( 1.0l, f_long__f_long_smallest_p)) && isnan(nonstd::cx::fmod( 1.0l, f_long__f_long_smallest_p)) ) ));
        REQUIRE(( ( std::fmod(-1.0l, f_long__f_long_smallest_p) == nonstd::cx::fmod(-1.0l, f_long__f_long_smallest_p) ) || ( isnan(std::fmod(-1.0l, f_long__f_long_smallest_p)) && isnan(nonstd::cx::fmod(-1.0l, f_long__f_long_smallest_p)) ) ));
        REQUIRE(( ( std::fmod( 1.0l, f_long__f_long_smallest_n) == nonstd::cx::fmod( 1.0l, f_long__f_long_smallest_n) ) || ( isnan(std::fmod( 1.0l, f_long__f_long_smallest_n)) && isnan(nonstd::cx::fmod( 1.0l, f_long__f_long_smallest_n)) ) ));
        REQUIRE(( ( std::fmod(-1.0l, f_long__f_long_smallest_n) == nonstd::cx::fmod(-1.0l, f_long__f_long_smallest_n) ) || ( isnan(std::fmod(-1.0l, f_long__f_long_smallest_n)) && isnan(nonstd::cx::fmod(-1.0l, f_long__f_long_smallest_n)) ) ));

        REQUIRE(( ( std::fmod(f_long__f_long_highest,  1.0l) == nonstd::cx::fmod(f_long__f_long_highest,  1.0l) ) || ( isnan(std::fmod(f_long__f_long_highest,  1.0l)) && isnan(nonstd::cx::fmod(f_long__f_long_highest,  1.0l)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_highest, -1.0l) == nonstd::cx::fmod(f_long__f_long_highest, -1.0l) ) || ( isnan(std::fmod(f_long__f_long_highest, -1.0l)) && isnan(nonstd::cx::fmod(f_long__f_long_highest, -1.0l)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_lowest,  1.0l)  == nonstd::cx::fmod(f_long__f_long_lowest,  1.0l) )  || ( isnan(std::fmod(f_long__f_long_lowest,  1.0l))  && isnan(nonstd::cx::fmod(f_long__f_long_lowest,  1.0l)) )  ));
        REQUIRE(( ( std::fmod(f_long__f_long_lowest, -1.0l)  == nonstd::cx::fmod(f_long__f_long_lowest, -1.0l) )  || ( isnan(std::fmod(f_long__f_long_lowest, -1.0l))  && isnan(nonstd::cx::fmod(f_long__f_long_lowest, -1.0l)) )  ));
        REQUIRE(( ( std::fmod( 1.0l, f_long__f_long_highest) == nonstd::cx::fmod( 1.0l, f_long__f_long_highest) ) || ( isnan(std::fmod( 1.0l, f_long__f_long_highest)) && isnan(nonstd::cx::fmod( 1.0l, f_long__f_long_highest)) ) ));
        REQUIRE(( ( std::fmod(-1.0l, f_long__f_long_highest) == nonstd::cx::fmod(-1.0l, f_long__f_long_highest) ) || ( isnan(std::fmod(-1.0l, f_long__f_long_highest)) && isnan(nonstd::cx::fmod(-1.0l, f_long__f_long_highest)) ) ));
        REQUIRE(( ( std::fmod( 1.0l, f_long__f_long_lowest)  == nonstd::cx::fmod( 1.0l, f_long__f_long_lowest) )  || ( isnan(std::fmod( 1.0l, f_long__f_long_lowest))  && isnan(nonstd::cx::fmod( 1.0l, f_long__f_long_lowest)) )  ));
        REQUIRE(( ( std::fmod(-1.0l, f_long__f_long_lowest)  == nonstd::cx::fmod(-1.0l, f_long__f_long_lowest) )  || ( isnan(std::fmod(-1.0l, f_long__f_long_lowest))  && isnan(nonstd::cx::fmod(-1.0l, f_long__f_long_lowest)) )  ));

        // Test INF
        REQUIRE(( ( std::fmod(f32__f32_smallest_p, f32__f32_p_inf) == nonstd::cx::fmod(f32__f32_smallest_p, f32__f32_p_inf) ) || ( isnan(std::fmod(f32__f32_smallest_p, f32__f32_p_inf)) && isnan(nonstd::cx::fmod(f32__f32_smallest_p, f32__f32_p_inf)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_smallest_p, f32__f32_n_inf) == nonstd::cx::fmod(f32__f32_smallest_p, f32__f32_n_inf) ) || ( isnan(std::fmod(f32__f32_smallest_p, f32__f32_n_inf)) && isnan(nonstd::cx::fmod(f32__f32_smallest_p, f32__f32_n_inf)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_smallest_n, f32__f32_p_inf) == nonstd::cx::fmod(f32__f32_smallest_n, f32__f32_p_inf) ) || ( isnan(std::fmod(f32__f32_smallest_n, f32__f32_p_inf)) && isnan(nonstd::cx::fmod(f32__f32_smallest_n, f32__f32_p_inf)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_smallest_n, f32__f32_n_inf) == nonstd::cx::fmod(f32__f32_smallest_n, f32__f32_n_inf) ) || ( isnan(std::fmod(f32__f32_smallest_n, f32__f32_n_inf)) && isnan(nonstd::cx::fmod(f32__f32_smallest_n, f32__f32_n_inf)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_p_inf, f32__f32_smallest_p) == nonstd::cx::fmod(f32__f32_p_inf, f32__f32_smallest_p) ) || ( isnan(std::fmod(f32__f32_p_inf, f32__f32_smallest_p)) && isnan(nonstd::cx::fmod(f32__f32_p_inf, f32__f32_smallest_p)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_n_inf, f32__f32_smallest_p) == nonstd::cx::fmod(f32__f32_n_inf, f32__f32_smallest_p) ) || ( isnan(std::fmod(f32__f32_n_inf, f32__f32_smallest_p)) && isnan(nonstd::cx::fmod(f32__f32_n_inf, f32__f32_smallest_p)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_p_inf, f32__f32_smallest_n) == nonstd::cx::fmod(f32__f32_p_inf, f32__f32_smallest_n) ) || ( isnan(std::fmod(f32__f32_p_inf, f32__f32_smallest_n)) && isnan(nonstd::cx::fmod(f32__f32_p_inf, f32__f32_smallest_n)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_n_inf, f32__f32_smallest_n) == nonstd::cx::fmod(f32__f32_n_inf, f32__f32_smallest_n) ) || ( isnan(std::fmod(f32__f32_n_inf, f32__f32_smallest_n)) && isnan(nonstd::cx::fmod(f32__f32_n_inf, f32__f32_smallest_n)) ) ));

        REQUIRE(( ( std::fmod(f32__f32_highest, f32__f32_p_inf) == nonstd::cx::fmod(f32__f32_highest, f32__f32_p_inf) ) || ( isnan(std::fmod(f32__f32_highest, f32__f32_p_inf)) && isnan(nonstd::cx::fmod(f32__f32_highest, f32__f32_p_inf)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_highest, f32__f32_n_inf) == nonstd::cx::fmod(f32__f32_highest, f32__f32_n_inf) ) || ( isnan(std::fmod(f32__f32_highest, f32__f32_n_inf)) && isnan(nonstd::cx::fmod(f32__f32_highest, f32__f32_n_inf)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_lowest, f32__f32_p_inf)  == nonstd::cx::fmod(f32__f32_lowest, f32__f32_p_inf) )  || ( isnan(std::fmod(f32__f32_lowest, f32__f32_p_inf))  && isnan(nonstd::cx::fmod(f32__f32_lowest, f32__f32_p_inf)) )  ));
        REQUIRE(( ( std::fmod(f32__f32_lowest, f32__f32_n_inf)  == nonstd::cx::fmod(f32__f32_lowest, f32__f32_n_inf) )  || ( isnan(std::fmod(f32__f32_lowest, f32__f32_n_inf))  && isnan(nonstd::cx::fmod(f32__f32_lowest, f32__f32_n_inf)) )  ));
        REQUIRE(( ( std::fmod(f32__f32_p_inf, f32__f32_highest) == nonstd::cx::fmod(f32__f32_p_inf, f32__f32_highest) ) || ( isnan(std::fmod(f32__f32_p_inf, f32__f32_highest)) && isnan(nonstd::cx::fmod(f32__f32_p_inf, f32__f32_highest)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_n_inf, f32__f32_highest) == nonstd::cx::fmod(f32__f32_n_inf, f32__f32_highest) ) || ( isnan(std::fmod(f32__f32_n_inf, f32__f32_highest)) && isnan(nonstd::cx::fmod(f32__f32_n_inf, f32__f32_highest)) ) ));
        REQUIRE(( ( std::fmod(f32__f32_p_inf, f32__f32_lowest)  == nonstd::cx::fmod(f32__f32_p_inf, f32__f32_lowest) )  || ( isnan(std::fmod(f32__f32_p_inf, f32__f32_lowest))  && isnan(nonstd::cx::fmod(f32__f32_p_inf, f32__f32_lowest)) )  ));
        REQUIRE(( ( std::fmod(f32__f32_n_inf, f32__f32_lowest)  == nonstd::cx::fmod(f32__f32_n_inf, f32__f32_lowest) )  || ( isnan(std::fmod(f32__f32_n_inf, f32__f32_lowest))  && isnan(nonstd::cx::fmod(f32__f32_n_inf, f32__f32_lowest)) )  ));


        REQUIRE(( ( std::fmod(f64__f64_smallest_p, f64__f64_p_inf) == nonstd::cx::fmod(f64__f64_smallest_p, f64__f64_p_inf) ) || ( isnan(std::fmod(f64__f64_smallest_p, f64__f64_p_inf)) && isnan(nonstd::cx::fmod(f64__f64_smallest_p, f64__f64_p_inf)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_smallest_p, f64__f64_n_inf) == nonstd::cx::fmod(f64__f64_smallest_p, f64__f64_n_inf) ) || ( isnan(std::fmod(f64__f64_smallest_p, f64__f64_n_inf)) && isnan(nonstd::cx::fmod(f64__f64_smallest_p, f64__f64_n_inf)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_smallest_n, f64__f64_p_inf) == nonstd::cx::fmod(f64__f64_smallest_n, f64__f64_p_inf) ) || ( isnan(std::fmod(f64__f64_smallest_n, f64__f64_p_inf)) && isnan(nonstd::cx::fmod(f64__f64_smallest_n, f64__f64_p_inf)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_smallest_n, f64__f64_n_inf) == nonstd::cx::fmod(f64__f64_smallest_n, f64__f64_n_inf) ) || ( isnan(std::fmod(f64__f64_smallest_n, f64__f64_n_inf)) && isnan(nonstd::cx::fmod(f64__f64_smallest_n, f64__f64_n_inf)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_p_inf, f64__f64_smallest_p) == nonstd::cx::fmod(f64__f64_p_inf, f64__f64_smallest_p) ) || ( isnan(std::fmod(f64__f64_p_inf, f64__f64_smallest_p)) && isnan(nonstd::cx::fmod(f64__f64_p_inf, f64__f64_smallest_p)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_n_inf, f64__f64_smallest_p) == nonstd::cx::fmod(f64__f64_n_inf, f64__f64_smallest_p) ) || ( isnan(std::fmod(f64__f64_n_inf, f64__f64_smallest_p)) && isnan(nonstd::cx::fmod(f64__f64_n_inf, f64__f64_smallest_p)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_p_inf, f64__f64_smallest_n) == nonstd::cx::fmod(f64__f64_p_inf, f64__f64_smallest_n) ) || ( isnan(std::fmod(f64__f64_p_inf, f64__f64_smallest_n)) && isnan(nonstd::cx::fmod(f64__f64_p_inf, f64__f64_smallest_n)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_n_inf, f64__f64_smallest_n) == nonstd::cx::fmod(f64__f64_n_inf, f64__f64_smallest_n) ) || ( isnan(std::fmod(f64__f64_n_inf, f64__f64_smallest_n)) && isnan(nonstd::cx::fmod(f64__f64_n_inf, f64__f64_smallest_n)) ) ));

        REQUIRE(( ( std::fmod(f64__f64_highest, f64__f64_p_inf) == nonstd::cx::fmod(f64__f64_highest, f64__f64_p_inf) ) || ( isnan(std::fmod(f64__f64_highest, f64__f64_p_inf)) && isnan(nonstd::cx::fmod(f64__f64_highest, f64__f64_p_inf)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_highest, f64__f64_n_inf) == nonstd::cx::fmod(f64__f64_highest, f64__f64_n_inf) ) || ( isnan(std::fmod(f64__f64_highest, f64__f64_n_inf)) && isnan(nonstd::cx::fmod(f64__f64_highest, f64__f64_n_inf)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_lowest, f64__f64_p_inf)  == nonstd::cx::fmod(f64__f64_lowest, f64__f64_p_inf) )  || ( isnan(std::fmod(f64__f64_lowest, f64__f64_p_inf))  && isnan(nonstd::cx::fmod(f64__f64_lowest, f64__f64_p_inf)) )  ));
        REQUIRE(( ( std::fmod(f64__f64_lowest, f64__f64_n_inf)  == nonstd::cx::fmod(f64__f64_lowest, f64__f64_n_inf) )  || ( isnan(std::fmod(f64__f64_lowest, f64__f64_n_inf))  && isnan(nonstd::cx::fmod(f64__f64_lowest, f64__f64_n_inf)) )  ));
        REQUIRE(( ( std::fmod(f64__f64_p_inf, f64__f64_highest) == nonstd::cx::fmod(f64__f64_p_inf, f64__f64_highest) ) || ( isnan(std::fmod(f64__f64_p_inf, f64__f64_highest)) && isnan(nonstd::cx::fmod(f64__f64_p_inf, f64__f64_highest)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_n_inf, f64__f64_highest) == nonstd::cx::fmod(f64__f64_n_inf, f64__f64_highest) ) || ( isnan(std::fmod(f64__f64_n_inf, f64__f64_highest)) && isnan(nonstd::cx::fmod(f64__f64_n_inf, f64__f64_highest)) ) ));
        REQUIRE(( ( std::fmod(f64__f64_p_inf, f64__f64_lowest)  == nonstd::cx::fmod(f64__f64_p_inf, f64__f64_lowest) )  || ( isnan(std::fmod(f64__f64_p_inf, f64__f64_lowest))  && isnan(nonstd::cx::fmod(f64__f64_p_inf, f64__f64_lowest)) )  ));
        REQUIRE(( ( std::fmod(f64__f64_n_inf, f64__f64_lowest)  == nonstd::cx::fmod(f64__f64_n_inf, f64__f64_lowest) )  || ( isnan(std::fmod(f64__f64_n_inf, f64__f64_lowest))  && isnan(nonstd::cx::fmod(f64__f64_n_inf, f64__f64_lowest)) )  ));


        REQUIRE(( ( std::fmod(f_long__f_long_smallest_p, f_long__f_long_p_inf) == nonstd::cx::fmod(f_long__f_long_smallest_p, f_long__f_long_p_inf) ) || ( isnan(std::fmod(f_long__f_long_smallest_p, f_long__f_long_p_inf)) && isnan(nonstd::cx::fmod(f_long__f_long_smallest_p, f_long__f_long_p_inf)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_smallest_p, f_long__f_long_n_inf) == nonstd::cx::fmod(f_long__f_long_smallest_p, f_long__f_long_n_inf) ) || ( isnan(std::fmod(f_long__f_long_smallest_p, f_long__f_long_n_inf)) && isnan(nonstd::cx::fmod(f_long__f_long_smallest_p, f_long__f_long_n_inf)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_smallest_n, f_long__f_long_p_inf) == nonstd::cx::fmod(f_long__f_long_smallest_n, f_long__f_long_p_inf) ) || ( isnan(std::fmod(f_long__f_long_smallest_n, f_long__f_long_p_inf)) && isnan(nonstd::cx::fmod(f_long__f_long_smallest_n, f_long__f_long_p_inf)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_smallest_n, f_long__f_long_n_inf) == nonstd::cx::fmod(f_long__f_long_smallest_n, f_long__f_long_n_inf) ) || ( isnan(std::fmod(f_long__f_long_smallest_n, f_long__f_long_n_inf)) && isnan(nonstd::cx::fmod(f_long__f_long_smallest_n, f_long__f_long_n_inf)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_p_inf, f_long__f_long_smallest_p) == nonstd::cx::fmod(f_long__f_long_p_inf, f_long__f_long_smallest_p) ) || ( isnan(std::fmod(f_long__f_long_p_inf, f_long__f_long_smallest_p)) && isnan(nonstd::cx::fmod(f_long__f_long_p_inf, f_long__f_long_smallest_p)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_n_inf, f_long__f_long_smallest_p) == nonstd::cx::fmod(f_long__f_long_n_inf, f_long__f_long_smallest_p) ) || ( isnan(std::fmod(f_long__f_long_n_inf, f_long__f_long_smallest_p)) && isnan(nonstd::cx::fmod(f_long__f_long_n_inf, f_long__f_long_smallest_p)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_p_inf, f_long__f_long_smallest_n) == nonstd::cx::fmod(f_long__f_long_p_inf, f_long__f_long_smallest_n) ) || ( isnan(std::fmod(f_long__f_long_p_inf, f_long__f_long_smallest_n)) && isnan(nonstd::cx::fmod(f_long__f_long_p_inf, f_long__f_long_smallest_n)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_n_inf, f_long__f_long_smallest_n) == nonstd::cx::fmod(f_long__f_long_n_inf, f_long__f_long_smallest_n) ) || ( isnan(std::fmod(f_long__f_long_n_inf, f_long__f_long_smallest_n)) && isnan(nonstd::cx::fmod(f_long__f_long_n_inf, f_long__f_long_smallest_n)) ) ));

        REQUIRE(( ( std::fmod(f_long__f_long_highest, f_long__f_long_p_inf) == nonstd::cx::fmod(f_long__f_long_highest, f_long__f_long_p_inf) ) || ( isnan(std::fmod(f_long__f_long_highest, f_long__f_long_p_inf)) && isnan(nonstd::cx::fmod(f_long__f_long_highest, f_long__f_long_p_inf)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_highest, f_long__f_long_n_inf) == nonstd::cx::fmod(f_long__f_long_highest, f_long__f_long_n_inf) ) || ( isnan(std::fmod(f_long__f_long_highest, f_long__f_long_n_inf)) && isnan(nonstd::cx::fmod(f_long__f_long_highest, f_long__f_long_n_inf)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_lowest, f_long__f_long_p_inf)  == nonstd::cx::fmod(f_long__f_long_lowest, f_long__f_long_p_inf) )  || ( isnan(std::fmod(f_long__f_long_lowest, f_long__f_long_p_inf))  && isnan(nonstd::cx::fmod(f_long__f_long_lowest, f_long__f_long_p_inf)) )  ));
        REQUIRE(( ( std::fmod(f_long__f_long_lowest, f_long__f_long_n_inf)  == nonstd::cx::fmod(f_long__f_long_lowest, f_long__f_long_n_inf) )  || ( isnan(std::fmod(f_long__f_long_lowest, f_long__f_long_n_inf))  && isnan(nonstd::cx::fmod(f_long__f_long_lowest, f_long__f_long_n_inf)) )  ));
        REQUIRE(( ( std::fmod(f_long__f_long_p_inf, f_long__f_long_highest) == nonstd::cx::fmod(f_long__f_long_p_inf, f_long__f_long_highest) ) || ( isnan(std::fmod(f_long__f_long_p_inf, f_long__f_long_highest)) && isnan(nonstd::cx::fmod(f_long__f_long_p_inf, f_long__f_long_highest)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_n_inf, f_long__f_long_highest) == nonstd::cx::fmod(f_long__f_long_n_inf, f_long__f_long_highest) ) || ( isnan(std::fmod(f_long__f_long_n_inf, f_long__f_long_highest)) && isnan(nonstd::cx::fmod(f_long__f_long_n_inf, f_long__f_long_highest)) ) ));
        REQUIRE(( ( std::fmod(f_long__f_long_p_inf, f_long__f_long_lowest)  == nonstd::cx::fmod(f_long__f_long_p_inf, f_long__f_long_lowest) )  || ( isnan(std::fmod(f_long__f_long_p_inf, f_long__f_long_lowest))  && isnan(nonstd::cx::fmod(f_long__f_long_p_inf, f_long__f_long_lowest)) )  ));
        REQUIRE(( ( std::fmod(f_long__f_long_n_inf, f_long__f_long_lowest)  == nonstd::cx::fmod(f_long__f_long_n_inf, f_long__f_long_lowest) )  || ( isnan(std::fmod(f_long__f_long_n_inf, f_long__f_long_lowest))  && isnan(nonstd::cx::fmod(f_long__f_long_n_inf, f_long__f_long_lowest)) )  ));


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
            0.0f,
            std::numeric_limits<float>::max());
        std::uniform_real_distribution<double> rnd_double_range(
            0.0,
            std::numeric_limits<double>::max());
        std::uniform_real_distribution<long double> rnd_long_double_range(
            0.0l,
            std::numeric_limits<long double>::max());

        constexpr int iterations = 10000;
        for (int i = 0; i < iterations; i++) {
            float px = rnd_float_range(re);
            float nx = -px;
            float py = rnd_float_range(re);
            float ny = -py;
            REQUIRE(( ( std::fmod(px, py)  == nonstd::cx::fmod(px, py) ) || ( isnan(std::fmod(px, py)) && isnan(nonstd::cx::fmod(px, py)) ) ));
            REQUIRE(( ( std::fmod(px, ny)  == nonstd::cx::fmod(px, ny) ) || ( isnan(std::fmod(px, ny)) && isnan(nonstd::cx::fmod(px, ny)) ) ));
            REQUIRE(( ( std::fmod(nx, py)  == nonstd::cx::fmod(nx, py) ) || ( isnan(std::fmod(nx, py)) && isnan(nonstd::cx::fmod(nx, py)) ) ));
            REQUIRE(( ( std::fmod(nx, ny)  == nonstd::cx::fmod(nx, ny) ) || ( isnan(std::fmod(nx, ny)) && isnan(nonstd::cx::fmod(nx, ny)) ) ));
        }
        for (int i = 0; i < iterations; i++) {
            double px = rnd_double_range(re);
            double nx = -px;
            double py = rnd_double_range(re);
            double ny = -py;
            REQUIRE(( ( std::fmod(px, py)  == nonstd::cx::fmod(px, py) ) || ( isnan(std::fmod(px, py)) && isnan(nonstd::cx::fmod(px, py)) ) ));
            REQUIRE(( ( std::fmod(px, ny)  == nonstd::cx::fmod(px, ny) ) || ( isnan(std::fmod(px, ny)) && isnan(nonstd::cx::fmod(px, ny)) ) ));
            REQUIRE(( ( std::fmod(nx, py)  == nonstd::cx::fmod(nx, py) ) || ( isnan(std::fmod(nx, py)) && isnan(nonstd::cx::fmod(nx, py)) ) ));
            REQUIRE(( ( std::fmod(nx, ny)  == nonstd::cx::fmod(nx, ny) ) || ( isnan(std::fmod(nx, ny)) && isnan(nonstd::cx::fmod(nx, ny)) ) ));
        }
        for (int i = 0; i < iterations; i++) {
            long double px = rnd_long_double_range(re);
            long double nx = -px;
            long double py = rnd_long_double_range(re);
            long double ny = -py;
            REQUIRE(( ( std::fmod(px, py)  == nonstd::cx::fmod(px, py) ) || ( isnan(std::fmod(px, py)) && isnan(nonstd::cx::fmod(px, py)) ) ));
            REQUIRE(( ( std::fmod(px, ny)  == nonstd::cx::fmod(px, ny) ) || ( isnan(std::fmod(px, ny)) && isnan(nonstd::cx::fmod(px, ny)) ) ));
            REQUIRE(( ( std::fmod(nx, py)  == nonstd::cx::fmod(nx, py) ) || ( isnan(std::fmod(nx, py)) && isnan(nonstd::cx::fmod(nx, py)) ) ));
            REQUIRE(( ( std::fmod(nx, ny)  == nonstd::cx::fmod(nx, ny) ) || ( isnan(std::fmod(nx, ny)) && isnan(nonstd::cx::fmod(nx, ny)) ) ));
        }
    }

}

} /* namespace math */
} /* namespace nonstd_test */
