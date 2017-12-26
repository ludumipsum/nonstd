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
namespace cx {

template <typename T>
using enable_int_if_floating_point_t = typename std::enable_if_t<std::is_floating_point_v<T>, int>;
template <typename T>
using enable_int_if_integral_t = typename std::enable_if_t<std::is_integral_v<T>, int>;


/** Forward Declarations
 *  --------------------
 *  Many of these functions both rely on and are used by nonstd::cx functions,
 *  so we declare them before the nonstd::cx:: set, but define them after.
 */
namespace detail {
    // Test whether values are within machine epsilon.
    // Used for algorithm termination.
    template <typename T>
    constexpr bool feq(T x, T y);
} /* namespace detail */


// -----------------------------------------------------------------------------
// constexpr math functions

// -----------------------------------------------------------------------------
// is INFINITE / is NaN functions

// float isinf(float x)
// double isinf(double x)
// long double isinf(long double x)
// double isinf(Integral x)

// float isnan(float x)
// double isnan(double x)
// long double isnan(long double x)
// double isnan(Integral x)

// -----------------------------------------------------------------------------
// absolute value functions

// float abs(float x);
// double abs(double x);
// long double abs(long double x);

// float fabs(float x);
// double fabs(double x);
// long double fabs(long double x);
// double fabs(Integral x);

// -----------------------------------------------------------------------------
// square root functions

// float sqrt(float x);
// double sqrt(double x);
// long double sqrt(long double x);
// double sqrt(Integral x);

// -----------------------------------------------------------------------------
// cube root functions

// float cbrt(float x);
// double cbrt(double x);
// long double cbrt(long double x);
// double cbrt(Integral x);

// -----------------------------------------------------------------------------
// hypotenuse function (returns the square root of the sum of the squares)

// float hypot(float x, float y);
// double hypot(double x, double y);
// long double hypot(long double x, long double y);
// Promoted hypot(Arithmetic1 x, Arithmetic2 y);

// Promotion rules:
// When either of Arithmetic1 or Arithmetic2 is long double, Promoted is long
// double. Otherwise Promoted is double.

// -----------------------------------------------------------------------------
// exponent function (e^x)

// float exp(float x);
// double exp(double x);
// long double exp(long double x);
// double exp(Integral x);

// -----------------------------------------------------------------------------
// trigonometric functions

// float sin(float x);
// double sin(double x);
// long double sin(long double x);
// double sin(Integral x);

// float cos(float x);
// double cos(double x);
// long double cos(long double x);
// double cos(Integral x);

// float tan(float x);
// double tan(double x);
// long double tan(long double x);
// double tan(Integral x);

// -----------------------------------------------------------------------------
// inverse trigonometric functions

// float asin(float x);
// double asin(double x);
// long double asin(long double x);
// double asin(Integral x);

// float acos(float x);
// double acos(double x);
// long double acos(long double x);
// double acos(Integral x);

// float atan(float x);
// double atan(double x);
// long double atan(long double x);
// double atan(Integral x);

// float atan2(float x, float y);
// double atan2(double x, double y);
// long double atan2(long double x, long double y);
// Promoted atan2(Arithmetic1 x, Arithmetic2 y);

// -----------------------------------------------------------------------------
// rounding functions (long double versions exist only for C++14)

// float floor(float x);
// double floor(double x);
// long double floor(long double x);
// double floor(Integral x);

// float ceil(float x);
// double ceil(double x);
// long double ceil(long double x);
// double ceil(Integral x);

// float trunc(float x);
// double trunc(double x);
// long double trunc(long double x);
// double trunc(Integral x);

// float round(float x);
// double round(double x);
// long double round(long double x);
// double round(Integral x);

// -----------------------------------------------------------------------------
// remainder functions (long double versions exist only for C++14)

// float fmod(float x, float y);
// double fmod(double x, double y);
// long double fmod(long double x, long double y);
// Promoted fmod(Arithmetic1 x, Arithmetic2 y);

// float remainder(float x, float y);
// double remainder(double x, double y);
// long double remainder(long double x, long double y);
// Promoted remainder(Arithmetic1 x, Arithmetic2 y);

// -----------------------------------------------------------------------------
// max/min functions

// float fmax(float x, float y);
// double fmax(double x, double y);
// long double fmax(long double x, long double y);
// Promoted fmax(Arithmetic1 x, Arithmetic2 y);

// float fmin(float x, float y);
// double fmin(double x, double y);
// long double fmin(long double x, long double y);
// Promoted fmin(Arithmetic1 x, Arithmetic2 y);

// float fdim(float x, float y);
// double fdim(double x, double y);
// long double fdim(long double x, long double y);
// Promoted fdim(Arithmetic1 x, Arithmetic2 y);

// -----------------------------------------------------------------------------
// logarithm functions

// float log(float x);
// double log(double x);
// long double log(long double x);
// double log(Integral x);

// float log10(float x);
// double log10(double x);
// long double log10(long double x);
// double log10(Integral x);

// float log2(float x);
// double log2(double x);
// long double log2(long double x);
// double log2(Integral x);

// -----------------------------------------------------------------------------
// hyperbolic functions

// float sinh(float x);
// double sinh(double x);
// long double sinh(long double x);
// double sinh(Integral x);

// float cosh(float x);
// double cosh(double x);
// long double cosh(long double x);
// double cosh(Integral x);

// float tanh(float x);
// double tanh(double x);
// long double tanh(long double x);
// double tanh(Integral x);

// -----------------------------------------------------------------------------
// inverse hyperbolic functions

// float asinh(float x);
// double asinh(double x);
// long double asinh(long double x);
// double asinh(Integral x);

// float acosh(float x);
// double acosh(double x);
// long double acosh(long double x);
// double acosh(Integral x);

// float atanh(float x);
// double atanh(double x);
// long double atanh(long double x);
// double atanh(Integral x);

// -----------------------------------------------------------------------------
// power function

// float pow(float x, float y);
// double pow(double x, double y);
// long double pow(long double x, long double y);
// Promoted pow(Arithmetic1 x, Arithmetic2 y);

// -----------------------------------------------------------------------------
// Gauss error function

// float erf(float x);
// double erf(double x);
// long double erf(long double x);
// double erf(Integral x);


//----------------------------------------------------------------------------
// Test if a value is INFINITE.
template <typename FloatingPoint,
          enable_int_if_floating_point_t<FloatingPoint> = 0>
constexpr bool isinf(FloatingPoint x) {
    return std::numeric_limits<FloatingPoint>::max() < x;
}
template <typename Integral,
          enable_int_if_integral_t<Integral> = 0>
constexpr bool isinf(Integral x) {
    return isinf(static_cast<double>(x));
}

//----------------------------------------------------------------------------
// Test if a value is NaN.
template <typename FloatingPoint,
          enable_int_if_floating_point_t<FloatingPoint> = 0>
constexpr bool isnan(FloatingPoint x) {
    return (0.0 >= x) == false
        && (0.0 <= x) == false;
}
template <typename Integral,
          enable_int_if_integral_t<Integral> = 0>
constexpr bool isnan(Integral x) {
    return isnan(static_cast<double>(x));
}

//----------------------------------------------------------------------------
// Take the absolute value.
template <typename FloatingPoint,
          enable_int_if_floating_point_t<FloatingPoint> = 0>
constexpr FloatingPoint abs(FloatingPoint x) {
    return x >= 0 ?  x
         : x <  0 ? -x
         : x;
}

//----------------------------------------------------------------------------
// Take the absolute value.
template <typename FloatingPoint,
          enable_int_if_floating_point_t<FloatingPoint> = 0>
constexpr FloatingPoint fabs(FloatingPoint x) {
    return x >= 0 ?  x
         : x <  0 ? -x
         : x;
}
template <typename Integral,
          enable_int_if_integral_t<Integral> = 0>
constexpr double fabs(Integral x) {
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



/** Definitions
 *  -----------
 */
namespace detail {
    // Test whether values are within machine epsilon.
    // Used for algorithm termination.
    template <typename T>
    constexpr bool feq(T x, T y) {
        return nonstd::cx::abs(x - y) <= std::numeric_limits<T>::epsilon();
    }
} /* namespace detail */

} /* namespace cx */
} /* namespace nonstd */
