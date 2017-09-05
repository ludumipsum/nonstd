/* Type-Aware Comparison Functions
 * ===============================
 * General-purpose hash comparison functions.
 */

#pragma once

#include "batteries_included.h"
#include "primitive_types.h"
#include "crash.h"

template<typename L, typename R> constexpr
FORCEINLINE bool n2equals(L left, R right) {
    return left == right;
}

template<typename L, typename R>
FORCEINLINE int n2compare(L left, R right) {
    if (left > right) { return  1; } else
    if (left < right) { return -1; }
    return 0;
}

template<>
inline int n2compare(c_cstr left, c_cstr right) {
    return strcmp(left, right);
}
