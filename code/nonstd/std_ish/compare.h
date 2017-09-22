/** Type-Aware Comparison Functions
 *  ===============================
 *  General-purpose hash comparison functions.
 */

#pragma once

#include <cstring>

#include "../preprocessor/homogenize.h"
#include "../core/primitive_types.h"


namespace nonstd {

template<typename L, typename R>
constexpr FORCEINLINE bool equal_to(L left, R right) {
    return left == right;
}

template<typename L, typename R>
constexpr FORCEINLINE int compare(L left, R right) {
    if (left > right) { return  1; } else
    if (left < right) { return -1; }
    return 0;
}

template<>
FORCEINLINE int compare(c_cstr left, c_cstr right) {
    return strcmp(left, right);
}

} /* namespace nonstd */