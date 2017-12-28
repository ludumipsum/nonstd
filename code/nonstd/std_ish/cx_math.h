/** Constexpr Math Utilities
 *  ========================
 *  This set of functions take their signatures from the <cmath> header, but are
 *  notably constexpr versions of the functions in the std:: namespace.
 *  It should be noted that, while these functions may act like a drop-in
 *  replacement for their std:: counterparts, they should not be used as such.
 *  You can expect that these constexpr functions take at least 10x as long to
 *  run as a runtime-optimized version.
 *
 *  This file contains novel implementations, but takes significant inspiration
 *  from a set of functions written by Ben Deane (https://github.com/elbeno),
 *  and provided as part of his constexpr git repository;
 *  https://github.com/elbeno/constexpr/blob/a98b1db39c/src/include/cx_math.h
 */
#pragma once

#include <cmath>
#include <cfloat>
#include <limits>
#include <type_traits>

#include <nonstd/core/primitive_types.h>


namespace nonstd {

/** Constexpr Math Utilities
 *  ------------------------
 *  This struct is acting as a well-contained namespace; it only contains static
 *  members, and all its functions will be inline and/or templates. We use it,
 *  rather than a "real" namespace, to avoid issues with the order of function
 *  declaration.
 *
 *  The provide functions are as follows;
 *
 *    bool isinf(float arg);
 *    bool isinf(double arg);
 *    bool isinf(long double arg);
 *    bool isinf(Integral arg);
 *
 *    bool isnan(float arg);
 *    bool isnan(double arg);
 *    bool isnan(long double arg);
 *    bool isnan(Integral arg);
 *
 *    float       abs(float arg);
 *    double      abs(double arg);
 *    long double abs(long double arg);
 *
 *    float       abs(float arg);
 *    double      abs(double arg);
 *    long double abs(long double arg);
 *    double      abs(Integral arg);
 *
 *    float       ceil(float arg);
 *    double      ceil(double arg);
 *    long double ceil(long double arg);
 *    double      ceil(Integral arg);
 *
 *    float       floor(float arg);
 *    double      floor(double arg);
 *    long double floor(long double arg);
 *    double      floor(Integral arg);
 *
 *    float       trunc(float arg);
 *    double      trunc(double arg);
 *    long double trunc(long double arg);
 *    double      trunc(Integral arg);
 *
 *  There will also be a set of non-standard functions that aid in the
 *  implementation of core functions, but may be useful to other packages.
 *
 *    // Check if two numbers are with (an approximation) of the machine's
 *    // adjusted epsilon.
 *    constexpr bool nearly_equal(float x, float y);
 *    constexpr bool nearly_equal(double x, double y);
 *    constexpr bool nearly_equal(long double x, long double y);
 *    constexpr bool nearly_equal(Integral x, Integral; y);
 *
 *    // Raise an arbitrary floating point to an arbitrary integral power.
 *    constexpr float       fpow(float x, int pow);
 *    constexpr double      fpow(double x, int pow);
 *    constexpr long double fpow(long double x, int pow);
 */
struct cx {

template <typename T>
using enable_int_if_floating_point_t = typename std::enable_if_t<std::is_floating_point_v<T>, int>;
template <typename T>
using enable_int_if_integral_t = typename std::enable_if_t<std::is_integral_v<T>, int>;

struct detail {
    // nearly_equal -- Check if two numbers are with (an approximation) of the
    // machine's adjusted epsilon.
    template <typename T>
    static constexpr bool nearly_equal(T x, T y) {
        return nonstd::cx::abs(x - y) <= std::numeric_limits<T>::epsilon();
    }

    // fpow -- Raise an arbitrary floating point to an arbitrary integral power.
    template <typename FloatingPoint,
              enable_int_if_floating_point_t<FloatingPoint> = 0>
    static constexpr FloatingPoint fpow(FloatingPoint x, int n) {
        return (n == 0) ? FloatingPoint {1}
             : (n == 1) ? x
             : (n >  1) ? ( (n & 1)
                          ? x * fpow(x, n-1)
                          : fpow(x, n/2) * fpow(x, n/2) )
             : FloatingPoint {1} / fpow(x, -n);
    }

    // Implementation Helpers

    template <typename T>
    static constexpr T floor(T x, T guess, T inc) {
        return inc < T{8}                   ? floor2(x, guess, inc)
             : guess + inc <= x             ? floor(x, guess + inc, inc)
             : guess + (inc/T{8})*T{7} <= x ? floor(x, guess + (inc/T{8})*T{7}, inc/T{8})
             : guess + (inc/T{8})*T{6} <= x ? floor(x, guess + (inc/T{8})*T{6}, inc/T{8})
             : guess + (inc/T{8})*T{5} <= x ? floor(x, guess + (inc/T{8})*T{5}, inc/T{8})
             : guess + (inc/T{8})*T{4} <= x ? floor(x, guess + (inc/T{8})*T{4}, inc/T{8})
             : guess + (inc/T{8})*T{3} <= x ? floor(x, guess + (inc/T{8})*T{3}, inc/T{8})
             : guess + (inc/T{8})*T{2} <= x ? floor(x, guess + (inc/T{8})*T{2}, inc/T{8})
             : guess + inc/T{8} <= x        ? floor(x, guess + inc/T{8}, inc/T{8})
             : floor(x, guess, inc/T{8});
    }
    template <typename T>
    static constexpr T floor2(T x, T guess, T inc) {
        return guess + inc <= x ? floor2(x, guess + inc, inc)
             : inc <= T{1}      ? guess
             : floor2(x, guess, inc/T{2});
    }

    template <typename T>
    static constexpr T ceil(T x, T guess, T dec) {
        return dec < T{8}                   ? ceil2(x, guess, dec)
             : guess - dec >= x             ? ceil(x, guess - dec, dec)
             : guess - (dec/T{8})*T{7} >= x ? ceil(x, guess - (dec/T{8})*T{7}, dec/T{8})
             : guess - (dec/T{8})*T{6} >= x ? ceil(x, guess - (dec/T{8})*T{6}, dec/T{8})
             : guess - (dec/T{8})*T{5} >= x ? ceil(x, guess - (dec/T{8})*T{5}, dec/T{8})
             : guess - (dec/T{8})*T{4} >= x ? ceil(x, guess - (dec/T{8})*T{4}, dec/T{8})
             : guess - (dec/T{8})*T{3} >= x ? ceil(x, guess - (dec/T{8})*T{3}, dec/T{8})
             : guess - (dec/T{8})*T{2} >= x ? ceil(x, guess - (dec/T{8})*T{2}, dec/T{8})
             : guess - dec/T{8} >= x        ? ceil(x, guess - dec/T{8}, dec/T{8})
             : guess < dec                  ? ceil(x, guess, guess)
             : ceil(x, guess, dec/T{8});
    }
    template <typename T>
    static constexpr T ceil2(T x, T guess, T dec) {
        return guess - dec >= x ? ceil2(x, guess - dec, dec)
             : dec <= T{1}      ? guess
             : ceil2(x, guess, dec/T{2});
    }

}; /* struct detail */

/** Constexpr Math Utilities
 *  ----------------------------------------------------------------------------
 */

// isinf -- Check if the given value is INFINITE.
template <typename FloatingPoint,
          enable_int_if_floating_point_t<FloatingPoint> = 0>
static constexpr bool isinf(FloatingPoint x) {
    return std::numeric_limits<FloatingPoint>::max() < x;
}
template <typename Integral,
          enable_int_if_integral_t<Integral> = 0>
static constexpr bool isinf(Integral x) {
    return isinf(static_cast<double>(x));
}

// isnan -- Check if the given value is NaN.
template <typename FloatingPoint,
          enable_int_if_floating_point_t<FloatingPoint> = 0>
static constexpr bool isnan(FloatingPoint x) {
    return (0.0 >= x) == false
        && (0.0 <= x) == false;
}
template <typename Integral,
          enable_int_if_integral_t<Integral> = 0>
static constexpr bool isnan(Integral x) {
    return isnan(static_cast<double>(x));
}

// abs -- take the absolute value of the given floating point number.
template <typename FloatingPoint,
          enable_int_if_floating_point_t<FloatingPoint> = 0>
static constexpr FloatingPoint abs(FloatingPoint x) {
    return x >= 0 ?  x
         : x <  0 ? -x
         : x;
}

// fabs -- take the absolute value of the given number.
template <typename FloatingPoint,
          enable_int_if_floating_point_t<FloatingPoint> = 0>
static constexpr FloatingPoint fabs(FloatingPoint x) {
    return x >= 0 ?  x
         : x <  0 ? -x
         : x;
}
template <typename Integral,
          enable_int_if_integral_t<Integral> = 0>
static constexpr double fabs(Integral x) {
    // Today in Two's Compliment Arithmetic is Weird, multiplying INT_MIN
    // by `-1` results in INT_MIN. Remember that INT_MIN is `-2^32/2` and
    // INT_MAX is `2^32/2 - 1`. Trying to assign `+2^32/2` into an int can't
    // fit, and the resulting overflow will set the sign bit s.t. the resulting
    // number is negative.
    // To get around that weirdness, we cast to double first, then negate.
    return x >= 0 ?  x
         : x <  0 ? -static_cast<double>(x)
         : x;
}

// ceil -- Round up to the nearest integer.
static constexpr float ceil(float x) {
    constexpr float max = std::numeric_limits<float>::max();
    constexpr float epsilon = std::numeric_limits<float>::epsilon();
    constexpr int   max_exponent = std::numeric_limits<float>::max_exponent - 1;
    constexpr float largest_power_of_two = detail::fpow(2.0f, max_exponent);
    return isinf(x)              ? x
         : x == 0.0f             ? x
         : (x * epsilon) >= 1.0f ? x
         : x <  0 ? -floor(fabs(x))
         : x >= 0 ? detail::ceil(x, max, largest_power_of_two)
         : NAN;
}
static constexpr double ceil(double x) {
    constexpr double max = std::numeric_limits<double>::max();
    constexpr double epsilon = std::numeric_limits<double>::epsilon();
    constexpr int    max_exponent = std::numeric_limits<double>::max_exponent - 1;
    constexpr double largest_power_of_two = detail::fpow(2.0, max_exponent);
    return isinf(x)              ? x
         : x == 0.0              ? x
         : (x * epsilon) >= 1.0  ? x
         : x <  0 ? -floor(fabs(x))
         : x >= 0 ? detail::ceil(x, max, largest_power_of_two)
         : NAN;
}
static constexpr long double ceil(long double x) {
    constexpr long double max = std::numeric_limits<long double>::max();
    constexpr long double epsilon = std::numeric_limits<long double>::epsilon();
    constexpr int         max_exponent = std::numeric_limits<long double>::max_exponent - 1;
    constexpr long double largest_power_of_two = detail::fpow(2.0l, max_exponent);
    if (isinf(x))              { return x; }
    if (x == 0.0l)             { return x; }
    if (isnan(x))              { return x; }
    if ((x * epsilon) >= 1.0l) { return x; }
    if (x < 0.0l)              { return -floor(fabs(x)); }
    long double guess = max;
    long double dec   = largest_power_of_two;
    for (;;) {
        while (guess - dec < x) {
            dec /= 2.0l;
            if (dec < 1.0l)    { return guess; }
        }
        guess -= dec;
    }
    return NAN;
}
template <typename Integral,
          enable_int_if_integral_t<Integral> = 0>
static constexpr double ceil(Integral x) {
    return x;
}

// floor -- Round down to the nearest integer.
static constexpr float floor(float x) {
    constexpr float epsilon = std::numeric_limits<float>::epsilon();
    constexpr int   max_exponent = std::numeric_limits<float>::max_exponent - 1;
    constexpr float largest_power_of_two = detail::fpow(2.0f, max_exponent);
    return isinf(x)              ? x
         : x == 0.0f             ? x
         : (x * epsilon) >= 1.0f ? x
         : x <  0 ? -ceil(abs(x))
         : x >= 0 ? detail::floor(x, 0.0f, largest_power_of_two)
         : NAN;
}
static constexpr double floor(double x) {
    constexpr double epsilon = std::numeric_limits<double>::epsilon();
    constexpr int    max_exponent = std::numeric_limits<double>::max_exponent - 1;
    constexpr double largest_power_of_two = detail::fpow(2.0, max_exponent);
    return isinf(x)              ? x
         : x == 0.0              ? x
         : (x * epsilon) >= 1.0  ? x
         : x <  0 ? -ceil(abs(x))
         : x >= 0 ? detail::floor(x, 0.0, largest_power_of_two)
         : NAN;
}
static constexpr long double floor(long double x) {
    constexpr long double epsilon = std::numeric_limits<long double>::epsilon();
    constexpr int         max_exponent = std::numeric_limits<long double>::max_exponent - 1;
    constexpr long double largest_power_of_two = detail::fpow(2.0l, max_exponent);
    if (isinf(x))              { return x; }
    if (x == 0.0l)             { return x; }
    if (isnan(x))              { return x; }
    if ((x * epsilon) >= 1.0l) { return x; }
    if (x < 0.0l)              { return -ceil(fabs(x)); }
    long double inc   = largest_power_of_two;
    long double guess = 0.0l;
    for (;;) {
        while (guess + inc > x) {
            inc /= 2.0l;
            if (inc < 1.0l)    { return guess; }
        }
        guess += inc;
    }
    return NAN;
}
template <typename Integral,
          enable_int_if_integral_t<Integral> = 0>
static constexpr double floor(Integral x) {
    return x;
}

// trunc -- Truncate to the nearest integer.
template <typename FloatingPoint,
          enable_int_if_floating_point_t<FloatingPoint> = 0>
static constexpr FloatingPoint trunc(FloatingPoint x) {
    return (x >= 0)
         ?  floor(x)
         : -floor(-x);
}
template <typename Integral,
          enable_int_if_integral_t<Integral> = 0>
static constexpr double trunc(Integral x) {
    return x;
}

}; /* struct cx */
} /* namespace nonstd */
