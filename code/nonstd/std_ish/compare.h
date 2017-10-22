/** Type-Aware Comparison Functions
 *  ===============================
 *  General-purpose hash comparison functions.
 */

#pragma once

#include <cstring>
#include <functional>

#include "nonstd/preprocessor/homogenize.h"
#include "nonstd/core/primitive_types.h"
#include "nonstd/cpp1z/type_traits.h"


namespace nonstd {

/** `equal_to` Free Function
 *  ------------------------
 *  Relies on the given types having correctly overloaded `operator==` members.
 *  Correctly deduces noexcept using the `noexcept(noexcept(...))` construct.
 *  Using std::forward to manipulate the types because... that's what libc++ do?
 */
#if 1
template <typename L, typename R>
constexpr bool equal_to(L const & lhs, R const & rhs)
noexcept(noexcept(lhs == rhs)) {
    return lhs == rhs;
}
#endif
#if 0
template <typename L, typename R>
constexpr bool equal_to(L&& lhs, R&& rhs)
noexcept(noexcept(std::forward<L>(lhs) == std::forward<R>(rhs))) {
    return std::forward<L>(lhs) == std::forward<R>(rhs);
}
#endif
// NB. Not constexpr, because `strcmp`. It's noexcept because segfaults are
//     possible, and we _might_ get segv exceptions rather than terminations.
inline bool equal_to(c_cstr lhs, c_cstr rhs) {
    return (strcmp(lhs, rhs) == 0);
}

// Additional noexcept overloads may be added with ex;
//     template<typename L, typename R>
//     constexpr inline bool equal_to(Optional<L> lhs, Optional<R> rhs)
//     noexcept {
//         return lhs == rhs;
//     }

/** `compare` Free Function
 *  -----------------------
 *  Relies on the given types having correctly overloaded `operator<` and
 *  `operator>` members. Correctly deduces noexcept using the
 * `noexcept(noexcept(...))` construct.
 *  Using std::forward to manipulate the types because... that's what libc++ do?
 */
#if 1
template <typename L, typename R>
constexpr int compare(L const & lhs, R const & rhs)
noexcept(noexcept(lhs < rhs) &&
         noexcept(lhs > rhs)   ) {
    if (lhs > rhs) { return  1; } else
    if (lhs < rhs) { return -1; }
    return 0;
}
#endif
#if 0
template <typename L, typename R>
constexpr int compare(L&& lhs, R&& rhs)
noexcept(noexcept(std::forward<L>(lhs) < std::forward<R>(rhs)) &&
         noexcept(std::forward<L>(lhs) > std::forward<R>(rhs))   ) {
    if (std::forward<L>(lhs) > std::forward<R>(rhs)) { return  1; } else
    if (std::forward<L>(lhs) < std::forward<R>(rhs)) { return -1; }
    return 0;
}
#endif

// NB. Not constexpr, because `strcmp`. It's noexcept because segfaults are
//     possible, and we _might_ get segv exceptions rather than terminations.
inline int compare(c_cstr lhs, c_cstr rhs) {
    return strcmp(lhs, rhs);
}

// Additional noexcept overloads may be added with ex;
//    template<typename L, typename R>
//    constexpr inline bool compare(Optional<L> lhs, Optional<R> rhs)
//    noexcept {
//        if (lhs > rhs) { return  1; } else
//        if (lhs < rhs) { return -1; }
//        return 0;
//    }

} /* namespace nonstd */
