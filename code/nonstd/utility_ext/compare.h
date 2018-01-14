/** Type-Aware Comparison Functions
 *  ===============================
 *  General-purpose hash comparison functions.
 */

#pragma once

#include <cstring>

#include <nonstd/preprocessor/homogenize.h>
#include <nonstd/core/primitive_types.h>


namespace nonstd {

/** `equal_to` Free Function
 *  ------------------------
 *  Relies on the given types having correctly overloaded `operator==` members.
 *  Correctly deduces noexcept using the `noexcept(noexcept(...))` construct.
 *
 *  Additional noexcept overloads may be added with ex;
 *      template<typename L, typename R>
 *      constexpr inline bool equal_to(optional<L> lhs, optional<R> rhs)
 *      noexcept {
 *          return lhs == rhs;
 *      }
 */
template <typename L, typename R>
constexpr bool equal_to(L const & lhs, R const & rhs)
noexcept(noexcept(lhs == rhs)) {
    return lhs == rhs;
}
// NB. Not constexpr, because `strcmp`. It's noexcept because segfaults are
//     possible, and we _might_ get segv exceptions rather than terminations.
inline bool equal_to(c_cstr lhs, c_cstr rhs) {
    return (strcmp(lhs, rhs) == 0);
}

/** `compare` Free Function
 *  -----------------------
 *  Relies on the given types having correctly overloaded `operator<` and
 *  `operator>` members. Correctly deduces noexcept using the
 * `noexcept(noexcept(...))` construct.
 *
 *  Additional noexcept overloads may be added with ex;
 *     template<typename L, typename R>
 *     constexpr inline bool compare(optional<L> lhs, optional<R> rhs)
 *     noexcept {
 *         if (lhs > rhs) { return  1; } else
 *         if (lhs < rhs) { return -1; }
 *         return 0;
 *     }
 */
template <typename L, typename R>
constexpr int compare(L const & lhs, R const & rhs)
noexcept(noexcept(lhs < rhs) &&
         noexcept(lhs > rhs)   ) {
    if (lhs > rhs) { return  1; } else
    if (lhs < rhs) { return -1; }
    return 0;
}

// NB. Not constexpr, because `strcmp`. It's noexcept because segfaults are
//     possible, and we _might_ get segv exceptions rather than terminations.
inline int compare(c_cstr lhs, c_cstr rhs) {
    return strcmp(lhs, rhs);
}

} /* namespace nonstd */
