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
    template <typename T> constexpr bool feq(T x, T y);

} /* namespace detail */


/** Constexpr Math Utilities
 *  ------------------------
 */

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

// Take the absolute value.
template <typename FloatingPoint,
          enable_int_if_floating_point_t<FloatingPoint> = 0>
constexpr FloatingPoint abs(FloatingPoint x) {
    return x >= 0 ?  x
         : x <  0 ? -x
         : x;
}

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
