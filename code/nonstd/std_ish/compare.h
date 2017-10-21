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
 *  This function steals a lot of its functionality from the <functional>
 *  std::equal_to struct. Noteably, though, we define a free-function, rather
 *  than a function object, s.t. specializations can be created out of scope by
 *  defining *  non-template overloads.
 */
template <typename L, typename R>
constexpr bool equal_to(L&& lhs, R&& rhs)
noexcept(noexcept(std::forward<L>(lhs) == std::forward<R>(rhs))) {
    return std::equal_to<>()(std::forward<L>(lhs), std::forward<R>(rhs));
}

// NB. Not constexpr, because `strcmp`. It's noexcept because segfaults are
//     possible, and we _might_ get segv exceptions rather than terminations.
inline bool equal_to(c_cstr lhs, c_cstr rhs) {
    return (strcmp(lhs, rhs) == 0);
}

// Additional noexcept overloads may be added with ex;
//     template<typename L, typename R>
//     constexpr inline bool equal_to(Optional<L> left, Optional<R> right)
//     noexcept {
//         return left == right;
//     }


template<typename L, typename R>
constexpr FORCEINLINE auto compare(L left, R right)
-> ENABLE_IF_DTYPE(!(IS_ARITHMETIC(L) && IS_ARITHMETIC(R)), int) {
    if (left > right) { return  1; } else
    if (left < right) { return -1; }
    return 0;
}

template<typename L, typename R>
constexpr FORCEINLINE auto compare(L left, R right) noexcept
-> ENABLE_IF_DTYPE( (IS_ARITHMETIC(L) && IS_ARITHMETIC(R)), int) {
    if (left > right) { return  1; } else
    if (left < right) { return -1; }
    return 0;
}

FORCEINLINE int compare(c_cstr left, c_cstr right) {
    return strcmp(left, right);
}

// Additional noexcept overloads may be added with ex;
//    template<typename L, typename R>
//    constexpr inline bool compare(Optional<L> left, Optional<R> right)
//    noexcept {
//        if (left > right) { return  1; } else
//        if (left < right) { return -1; }
//        return 0;
//    }

} /* namespace nonstd */
