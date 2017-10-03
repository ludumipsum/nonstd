/** Loggin' Utilities
 *  =================
 *  Utility functions and macros for logging, variadic printf-expansion, and
 *  similar chores.
 *
 *  https://www.youtube.com/watch?v=siwpn14IE7E
 */

#pragma once

#include <iostream>

#include <fmt/format.h>

#include "primitive_types.h"
#include "variadic_expand.h"


/** Logging Macro Shorthand
 *  -----------------------
 *  If you want to log a thing, you almost 100% definitely certainly want to
 *  probably use this one.
 *  If you've compiled with DEBUG active, we're not just going to munge and print
 *  your message. We're also going to run it through an `snprintf(NULL, 0, ...)`
 *  to give your compiler a change to check that your format string is correctly
 *  structured. Actually using `NULL` doesn't work here, unfortunately, so this
 *  trick will add the cost of writing `\0` into `format_verification` to every
 *  message actually printed.
 */
#if defined(N2_DISABLE_LOGGING)
#   define LOG(FMT_STRING, ...)
#elif defined(DEBUG)
#   define LOG(FMT_STRING, ...)                                        \
        ::nonstd::detail::logMessage(FMT_STRING,                       \
                                     __FILE__, __LINE__, __FUNCTION__, \
                                     ##__VA_ARGS__)
#endif


namespace nonstd {
namespace detail {

/** Logging Implementation
 *  ----------------------
 *  Actually emit text from `LOG(...)` invocations.
 */
template <typename ... Args>
inline void logMessage(c_cstr    format_str,
                       c_cstr    __file__,
                       i32 const __line__,
                       c_cstr    __pretty_function__,
                       Args ...  args) {
    #if defined(_MSC_VER)
        static const char path_delimiter = '\\';
    #else
        static const char path_delimiter = '/';
    #endif
    cstr filename = (cstr)strrchr(__file__, path_delimiter)+1;

    fmt::MemoryWriter o;
    if (filename) {
        o.write("{}:{} {} -- ", filename, __line__, __pretty_function__);
    } else {
        o.write("{}:{} {} -- ", __file__, __line__, __pretty_function__);
    }
    o.write(format_str, args...);
    o.write("\n");
    std::cout << o.str();
    return;
}

} /* namespace detail */
} /* namespace nonstd */


