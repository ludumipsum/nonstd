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

#define TYPE_NAME_OF(Var) (int)nonstd::type_name_of(Var).size(), \
                               nonstd::type_name_of(Var).data()


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
template <typename T>
constexpr StaticString type_name() {
    #if !defined(_MSC_VER)
        // Clang (Apple LLVM version 9.0.0 (clang-900.0.37) ) emits
        // __PRETTY_FUNCTION__ strings that include templated in this form:
        //
        //     nonstd::StaticString nonstd::type_name() [T = SomeBullshit]
        //
        // GCC (5.4.0) emits templated strings in this form:
        //
        //      nonstd::StaticString nonstd::type_name() [with T = SomeBullshit]
        //
        // To parse these, we need to find the stuff betwee the first `= ` and
        // last `]`, since that will be the type included in the template.
        c_cstr p = __PRETTY_FUNCTION__;
        while (*(++p) != '='); //< Find the first `=`.
        while (*(++p) == ' '); //< Move past any spaces.

        c_cstr q = p;
        u16 count = 1; //< Track how many `[`s we've seen in order to find the
                       //< final `]`.
        while (count > 0 && *q != '\0') {
            ++q;
            switch (*q) {
            case '[': { ++count; } break;
            case ']': { --count; } break;
            }
        }
        return StaticString(p, (i32)(q-p));
    #else
        // Visual Studio emits templated strings in this form:
        //
        //     class nonstd::StaticString __cdecl nonstd::type_name<struct SomeBullshit>(void)
        //
        // To parse that, we need to find the stuff between the first < and the
        // last >, since that'll be everything within our class' template.

        // Find the start of our typename
        c_cstr p = __PRETTY_FUNCTION__;
        while (*(++p) != '<');
        p += 1;

        // Find the end of our typename
        c_cstr q = p;
        u16 count = 1; //< Track how many `<`s we've seen, in order to find the
                       //< final `>`.
        while (count > 0 && *q != '\0') {
            ++q;
            switch (*q) {
            case '<': { ++count; } break;
            case '>': { --count; } break;
            }
        }
        return StaticString(p, (i32)(q-p));
    #endif
}

template <typename T>
constexpr StaticString type_name_of(T const & /*unused*/) {
    return type_name<T>();
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
