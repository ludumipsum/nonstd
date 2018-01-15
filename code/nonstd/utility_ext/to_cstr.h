/** nonstd::to_cstr
 *  ===============
 *  This template function is designed to be specialized over types that are
 *  annoying to turn into string (like `bool`s) to simplify `printf`s.
 *
 *  NB. Additional overloads (noexcept, or otherwise) may be added by
 *  overloading -- rather than specializing on -- the to_cstr function.
 */
#pragma once

#include <nonstd/nonstd.h>


namespace nonstd {

template <typename T>
inline constexpr c_cstr to_cstr(T a);

inline constexpr c_cstr to_cstr(bool a) noexcept {
    return a ? "true" : "false";
}

} /* namespace nonstd */
