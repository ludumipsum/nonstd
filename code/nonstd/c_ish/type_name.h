/** Constexpr Type-Name Printing
 *  ============================
 *  Lifted nearly directly from https://stackoverflow.com/questions/81870
 *  Designed to work with the `Ftype` format string macro, the {fmt} library,
 *  and "simple" `ostream & <<`s.
 *
 *  Usage;
 *      LOG("\n"     "LOG macro  :: " Ftype "", TYPE_NAME(u32));
 *      fmt::print(  "fmt format :: {}\n", nonstd::type_name<u32>());
 *      std::cout << "std::cout  :: " << nonstd::type_name<u32>() << '\n';
 */

#pragma once

#include <cstddef>
#include <cstring>
#include <stdexcept>
#include <ostream>

#include <fmt/format.h>
#include <fmt/printf.h>

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

#define TYPE_NAME(T) (int)nonstd::type_name<T>().size(), \
                          nonstd::type_name<T>().data()


namespace nonstd {

/** StaticString
 *  ------------
 *  Very similar to std::string, without many (most) of the bells and whistles,
 *  and with the very nice feature of being constexpr.
 */
class StaticString {
    const c_cstr p;
    const size_t len;

public:
    using const_iterator = c_cstr;

    template <size_t N>
    constexpr StaticString(const char(&a)[N]) noexcept
        : p   ( a   )
        , len ( N-1 ) { }

    constexpr StaticString(c_cstr p, size_t N) noexcept
        : p   ( p )
        , len ( N ) { }

    constexpr c_cstr data() const noexcept { return p; }
    constexpr size_t size() const noexcept { return len; }

    constexpr const_iterator begin() const noexcept { return p; }
    constexpr const_iterator end()   const noexcept { return p + len; }

    constexpr char operator[](size_t n) const {
        return n < len ? p[n] : throw std::out_of_range("StaticString");
    }
};


/** type_name Free Function
 *  -----------------------
 *  Returns a compile-time (if possible, runtime if not) StaticString that
 *  points to the name of the template argument passed in to its invocation.
 */
template <class T>
constexpr StaticString type_name() {
    c_cstr p = __PRETTY_FUNCTION__;
    while (*(++p) != '='); // Skip p to the first `=`.
    while (*(++p) == ' '); // Skip p past any spaces.

    c_cstr q = p;
    u16 count = 1; // We will have skipped over the first '['. Find its pair.
    while (count > 0) {
        ++q;
        switch (*q) {
        case '[': { ++count; } break;
        case ']': { --count; } break;
        }
    }
    return StaticString(p, (i32)(q-p));
}


/** StaticString Print Specializations
 *  ----------------------------------
 *  Both {fmt} and `std::ostream&`.
 */

inline std::ostream & operator << (std::ostream & ostream,
                                   StaticString const & str) {
    return ostream.write(str.data(), str.size());
}

inline void format_arg(fmt::BasicFormatter<char> & f,
                       char const * & format_str,
                       nonstd::StaticString const & str) {
    f.writer().write(fmt::sprintf(Ftype, str.size(), str.data()));
}

} /* namespace nonstd */
