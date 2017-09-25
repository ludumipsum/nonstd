/** nonstd::to_cstr
 *  ===============
 *  This template function is designed to be specialized over types that are
 *  annoying to turn into string (like `bool`s) to simplify `printf`s.
 *
 *  NB. Additional overloads (noexcept, or otherwise) may be added by
 *  overloading -- rather than specializing on -- the to_cstr function.
 */
#pragma once

#include "nonstd/core/primitive_types.h"


namespace nonstd {

template<typename T>
constexpr inline c_cstr to_cstr(T a);

constexpr inline c_cstr to_cstr(bool a) noexcept {
    return a ? "true" : "false";
}

} /* namespace nonstd */
