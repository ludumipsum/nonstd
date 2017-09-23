/** Constexpr Type-Name Printing
 *  ============================
 *  Heavily inspired by http://stackoverflow.com/questions/35941045.
 *  Designed to work with the `Ftype` format string macro.
 *
 *  Usage;
 *      printf("My type name is: \"" Ftype "\"\n", TYPE_NAME(MyType));
 */

#pragma once

#include "nonstd/preprocessor/homogenize.h"
#include "nonstd/core/primitive_types.h"

/** Ftype
 *  -----
 *  String format helper. In case you (like me) have never seen the `.*s`
 *  construct, it consumes two variadic arguments; first a maximum number of
 *  characters to print, then a potentially non-null terminated `char const *`
 *  to pull a segment from.
 */
#define Ftype "%.*s"


/** TYPE_NAME and TYPE_NAME_OF (and implementation)
 *  -----------------------------------------------
 *  These work by capturing a slice of the `__PRETTY_FUNCTION__` string literal,
 *  figuring out the number of characters worth printing, and exposing those two
 *  values as `static constexpr` members of a type. These two members are used
 *  in conjunction with the `Ftype` format string, defined above.
 *
 *  NB. This only works in GCC and Clang because MSVC hasn't implemented C++14's
 *  constexpr extensions, and we can't ex; `char const * p = __FUNCSIG__;`.
 *  TODO: Double check if this is still true.
 *   */
#if defined(_MSC_VER)

#define TYPE_NAME(TYPE)    36, "[[Sorry, no type name from MSVC :C]]"
#define TYPE_NAME_OF(TYPE) 36, "[[Sorry, no type name from MSVC :C]]"

#else
#define TYPE_NAME(TYPE) nonstd::detail::TypeName<TYPE>::length, \
                        nonstd::detail::TypeName<TYPE>::name
#define TYPE_NAME_OF(TYPE) TYPE_NAME(decltype(TYPE))

namespace nonstd {
namespace detail {

template<typename Type>
struct TypeName {
    static constexpr c_cstr _getName() {
        c_cstr p = __PRETTY_FUNCTION__;
        while (*(++p) != '='); // Skip p to the first `=`.
        while (*(++p) == ' '); // Skip p past any spaces.
        return p;
    }

    static constexpr i32 const _getLength() {
        c_cstr p     = _getName();
        c_cstr p2    = p;
        i32    count = 1;
        // In _getName, we will have skipped past the first `[` (which is why
        // count == 1). We now increment p2 until the matching `]` is found.
        while (count > 0) {
            ++p2;
            switch (*p2) {
            case '[': { ++count; } break;
            case ']': { --count; } break;
            }
        }
        return (i32)(p2 - p);
    }

    // Making these `static constexpr` members ensures compile-time resolution.
    static constexpr c_cstr    name   = _getName();
    static constexpr i32 const length = _getLength();
};

} /* namespace detail */
} /* namespace nonstd */

#endif
