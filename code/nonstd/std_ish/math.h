/** Math Utilities
 *  ==============
 *  A small library of commonly used math functions. Stuff like interpolators
 *  and rescalers.
 */

#pragma once

#include "nonstd/cpp1z/type_traits.h"
#include "nonstd/core/primitive_types.h"


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
constexpr T maskLowestBits(u16 nbits) noexcept {
      using UT = MAKE_UNSIGNED(T);
      // There are two pieces of undefined behavior we're avoiding here,
      //   1. Shifting past the width of a type (ex `<< 32` against an `i32`)
      //   2. Shifting a negative operand (which `~0` is for all signed types)
      // First we branch to avoid shifting the past the width of the type, then
      // (assuming we are shifting, and aren't just returning `~0`) we cast `~0`
      // to an explicitly unsigned type before performing the shift.
      return (nbits != (sizeof(UT) * 8)) ?
             ~T(UT(~UT(0)) << nbits)     :
             ~T(0);
}


/** Power of 2 Calculations/Comparisons
 *  ===================================
 *  These were lifted from Sean Eron Anderson's _Bit Twiddling Hacks_.
 *  http://graphics.stanford.edu/~seander/bithacks.html
 */

/** Is Power Of Two?
 *  ----------------
 */
template<typename T> inline
constexpr bool isPowerOfTwo(T num) noexcept {
    return (num && !(num & (num - 1)));
}

/** Powers of Two
 *  -------------
 *  TODO: Consider making these template specializations based on the size of T.
 */
inline u32 next_power_of_two(u32 num) noexcept {
    num -= 1;
    num |= num >> 1;
    num |= num >> 2;
    num |= num >> 4;
    num |= num >> 8;
    num |= num >> 16;
    num += 1;
    return num;
}

inline u64 next_power_of_two(u64 num) noexcept {
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


inline u32 previous_power_of_two(u32 num) noexcept {
    num |= (num >> 1);
    num |= (num >> 2);
    num |= (num >> 4);
    num |= (num >> 8);
    num |= (num >> 16);
    return num - (num >> 1);
}

inline u64 previous_power_of_two(u64 num) noexcept {
    num |= (num >> 1);
    num |= (num >> 2);
    num |= (num >> 4);
    num |= (num >> 8);
    num |= (num >> 16);
    num |= (num >> 32);
    return num - (num >> 1);
}

} /* namespace nonstd */
