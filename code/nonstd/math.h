/** Math Utilities
 *  ==============
 *  A small library of commonly used math functions. Stuff like interpolators,
 *  rescalers, and power-of-two manipulations.
 */

#pragma once

#include <nonstd/nonstd.h>
#include <nonstd/error.h>
#include <nonstd/type_traits_ext.h>


namespace nonstd {

/** Range Rescaling
 *  ---------------
 *  Take a value in a given domain and rescale it to the provided range.
 */
template<typename T>
constexpr inline T rescale(T value,
                           T domain_min, T domain_max,
                           T range_min,  T range_max) {
    T range_size = range_max - range_min;
    T domain_size = domain_max - domain_min;
    return range_min + range_size * (value - domain_min) / domain_size;
}

template<typename T>
constexpr inline T rescale(T value,
                           T domain_max,
                           T range_max) {
    return rescale(value, (T)0, domain_max, (T)0, range_max);
}


/** Bit Mask
 *  --------
 *  Returns a bit mask of the given type (please only use integer types) with the
 *  lowest `nbits` bits set to 1 and the rest set to 0.
 *  This implementation was lifted from a PR against the RapidCheck codebase that
 *  I (Drew) and RapidCheck's author (emil-e) have been working on.
 */
template<typename T> inline
constexpr T mask_lowest_bits(i32 nbits) noexcept {
    using UT = std::make_unsigned_t<T>;
    // There are two pieces of undefined behavior we're avoiding here,
    //   1. Shifting past the width of a type (ex `<< 32` against an `i32`)
    //   2. Shifting a negative operand (which `~0` is for all signed types)
    // First we branch to avoid shifting the past the width of the type, then
    // (assuming we are shifting, and aren't just returning `~0`) we cast `~0`
    // to an explicitly unsigned type before performing the shift.
    return nbits < (sizeof(UT) * 8)
         ? ~T(UT(~0) << nbits)
         : ~0;
}


/** Power of 2 Calculations/Comparisons
 *  ===================================
 *  These were lifted from Sean Eron Anderson's _Bit Twiddling Hacks_.
 *  http://graphics.stanford.edu/~seander/bithacks.html
 */

/** Is Power Of Two?
 *  ----------------
 */
template<typename T>
constexpr auto is_power_of_two(T num) noexcept -> /* bool */
std::enable_if_t<std::is_integral_v<T>, bool> {
    ASSERT(num >= 0);
    return (num && !(num & (num - 1)));
}
template<typename T>
constexpr auto is_power_of_two(T num) noexcept -> /* bool */
std::enable_if_t<std::is_enum_v<T>, bool> {
    auto n = static_cast<std::underlying_type_t<T>>(num);
    return is_power_of_two(n);
}

/** Rounding to a Power of Two
 *  --------------------------
 */
template<typename T>
constexpr auto ceil_power_of_two(T num) noexcept -> /* T */
std::enable_if_t<std::is_integral_v<T> && sizeof(T) == 1, T> {
    ASSERT(num >= 0);
    if (num == 0) return 1;
    num -= 1;
    num |= num >> 1;
    num |= num >> 2;
    num |= num >> 4;
    num += 1;
    return num;
}
template<typename T>
constexpr auto ceil_power_of_two(T num) noexcept -> /* T */
std::enable_if_t<std::is_integral_v<T> && sizeof(T) == 2, T> {
    ASSERT(num >= 0);
    if (num == 0) return 1;
    num -= 1;
    num |= num >> 1;
    num |= num >> 2;
    num |= num >> 4;
    num |= num >> 8;
    num += 1;
    return num;
}
template<typename T>
constexpr auto ceil_power_of_two(T num) noexcept -> /* T */
std::enable_if_t<std::is_integral_v<T> && sizeof(T) == 4, T> {
    ASSERT(num >= 0);
    if (num == 0) return 1;
    num -= 1;
    num |= num >> 1;
    num |= num >> 2;
    num |= num >> 4;
    num |= num >> 8;
    num |= num >> 16;
    num += 1;
    return num;
}
template<typename T>
constexpr auto ceil_power_of_two(T num) noexcept -> /* T */
std::enable_if_t<std::is_integral_v<T> && sizeof(T) == 8, T> {
    ASSERT(num >= 0);
    if (num == 0) return 1;
    num -= 1;
    num |= num >> 1;
    num |= num >> 2;
    num |= num >> 4;
    num |= num >> 8;
    num |= num >> 16;
    num |= num >> 32;
    num += 1;
    return num;
}


template<typename T>
constexpr auto floor_power_of_two(T num) noexcept -> /* T */
std::enable_if_t<std::is_integral_v<T> && sizeof(T) == 1, T> {
    ASSERT(num >= 0);
    num |= (num >> 1);
    num |= (num >> 2);
    num |= (num >> 4);
    return num - (num >> 1);
}
template<typename T>
constexpr auto floor_power_of_two(T num) noexcept -> /* T */
std::enable_if_t<std::is_integral_v<T> && sizeof(T) == 2, T> {
    ASSERT(num >= 0);
    num |= (num >> 1);
    num |= (num >> 2);
    num |= (num >> 4);
    num |= (num >> 8);
    return num - (num >> 1);
}
template<typename T>
constexpr auto floor_power_of_two(T num) noexcept -> /* T */
std::enable_if_t<std::is_integral_v<T> && sizeof(T) == 4, T> {
    ASSERT(num >= 0);
    num |= (num >> 1);
    num |= (num >> 2);
    num |= (num >> 4);
    num |= (num >> 8);
    num |= (num >> 16);
    return num - (num >> 1);
}
template<typename T>
constexpr auto floor_power_of_two(T num) noexcept -> /* T */
std::enable_if_t<std::is_integral_v<T> && sizeof(T) == 8, T> {
    ASSERT(num >= 0);
    num |= (num >> 1);
    num |= (num >> 2);
    num |= (num >> 4);
    num |= (num >> 8);
    num |= (num >> 16);
    num |= (num >> 32);
    return num - (num >> 1);
}

} /* namespace nonstd */
