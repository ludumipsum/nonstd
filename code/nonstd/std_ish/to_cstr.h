/** nonstd::to_cstr
 *  ===============
 *  This template function is designed to be specialized over types that are
 *  annoying to turn into string (like `bool`s) to simplify `printf`s.
 */
#pragma once

#include "../core/primitive_types.h"


namespace nonstd {

template<typename T>
constexpr inline c_cstr to_cstr(T a);

template<>
constexpr inline c_cstr to_cstr(bool a) {
    return a ? "true" : "false";
}

} /* namespace nonstd */
