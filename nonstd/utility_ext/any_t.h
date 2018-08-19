/** any_t
 *  =====
 *  Not a real type.
 *
 *  Used as a catch-all argument when low-priority coercion is required for
 *  sizeof and/or declval tricks.
 */

#pragma once


namespace nonstd {

struct any_t {
    // NB. This is a Ctor that will take one of any type (automatically deduced)
    template<typename T>
    any_t(T const &) { };
};

} /* namespace nonstd */
