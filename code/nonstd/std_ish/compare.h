/** Type-Aware Comparison Functions
 *  ===============================
 *  General-purpose hash comparison functions.
 */

#pragma once

#include <cstring>

#include "nonstd/preprocessor/homogenize.h"
#include "nonstd/core/primitive_types.h"


namespace nonstd {

template<typename L, typename R>
constexpr FORCEINLINE auto equal_to(L left, R right)
-> ENABLE_IF_DTYPE(!(IS_ARITHMETIC(L) && IS_ARITHMETIC(R)), bool) {
    return left == right;
}

template<typename L, typename R>
constexpr FORCEINLINE auto equal_to(L left, R right) noexcept
-> ENABLE_IF_DTYPE( (IS_ARITHMETIC(L) && IS_ARITHMETIC(R)), bool) {
    return left == right;
}

FORCEINLINE bool equal_to(c_cstr left, c_cstr right) {
    return (strcmp(left, right) == 0);
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
