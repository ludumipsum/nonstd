/* Math Utilities
 * ==============
 * A small library of commonly-used math functions. Stuff like interpolators
 * and rescalers.
 */

#pragma once

#include "batteries_included.h"
#include "primitive_types.h"

/* Min/Max Macros
 * --------------
 * Both type- and compiler-safe.
 */
#ifndef n2max
#define n2max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef n2min
#define n2min(a,b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef n2clamp
#define n2clamp(v, a, b) (n2min((b), (((v) > (a)) ? v : a)))
#endif


/* Range Rescaling
 * ---------------
 * Take a value in a given domain and rescale it to the provided range.
 */
template<typename T> inline
T rescale(T value, decltype(value) domain_min, decltype(value) domain_max,
                   decltype(value) range_min,  decltype(value) range_max) {
    T range_size = range_max - range_min;
    T domain_size = domain_max - domain_min;
    return range_min + range_size * (value - domain_min) / domain_size;
}

template<typename T> inline
T rescale(T value, decltype(value) domain_max,
                   decltype(value) range_max) {
    return rescale(value, (T)0, domain_max, (T)0, range_max);
}

/* Bit Mask
 * --------
 * Returns a bit mask of the given type (please only use integer types) with the
 * lowest `nbits` bits set to 1 and the rest set to 0.
 * This implementation was lifted from a PR against the RapidCheck codebase that
 * I (Drew) and RapidCheck's author (emil-e) have been working on.
 */
template<typename T> inline
constexpr T bitMask(int nbits) {
      using UT = typename std::make_unsigned<T>::type;
      // There are two pieces of undefined behavior we're avoiding here,
      //   1. Shifting past the width of a type (ex `<< 32` against an `i32`)
      //   2. Shifting a negative operand (which `~0` is for all signed types)
      // First we branch to avoid shifting the past the width of the type, then
      // (assuming we are shifting, and aren't just returning `~0`) we cast `~0`
      // to an explicitly unsigned type before performing the shift.
      return nbits != std::numeric_limits<UT>::digits ?
                      ~T(~UT(0) << nbits)             :
                      ~T(0);
}


/* Power of 2 Calculations/Comparisons
 * ===================================
 * These were lifted from Sean Eron Anderson's _Bit Twiddling Hacks_.
 * http://graphics.stanford.edu/~seander/bithacks.html
 */

/* Is Power Of Two?
 * ----------------
 */
template<typename T> inline
constexpr bool is_power_of_two(T num) {
    return (num && !(num & (num - 1)));
}

/* Next Power of Two
 * -----------------
 *TODO: Consider making these template specializations based on the size of T.
 */
inline u32 next_power_of_two(u32 num) {
    u32 v = num - 1;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

inline u64 next_power_of_two(u64 num) {
    u64 v = num - 1;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v |= v >> 32;
    v++;
    return v;
}
