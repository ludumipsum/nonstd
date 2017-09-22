/** Loggin' Utilities
 *  =================
 *  Utility functions and macros for logging, variadic printf-expansion, and
 *  similar chores.
 *
 *  https://www.youtube.com/watch?v=siwpn14IE7E
 */

#pragma once

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
    #define LOG(MESSAGE, ...)
#elif defined(DEBUG)
    static char format_verification[1] = {0};
    #define LOG(MESSAGE, ...)                                         \
        snprintf(format_verification, 0, MESSAGE, ##__VA_ARGS__);     \
        ::nonstd::detail::logMessage(                                 \
            ::nonstd::variadicExpand(MESSAGE, ##__VA_ARGS__).c_str(), \
            __FILE__, __LINE__, __FUNCTION__)
#else
    #define LOG(MESSAGE, ...)                                         \
        ::nonstd::detail::logMessage(                                 \
            ::nonstd::variadicExpand(MESSAGE, ##__VA_ARGS__).c_str(), \
            __FILE__, __LINE__, __FUNCTION__)
#endif


namespace nonstd {
namespace detail {

/** Logging Implementation
 *  ----------------------
 *  Actually emit text from calls made with LOG(...)
 */
inline i32 logMessage(c_cstr    message,
                      c_cstr    file,
                      i32 const line,
                      c_cstr    function)
{
    #if defined(_MSC_VER)
        static const char path_delimiter = '\\';
    #else
        static const char path_delimiter = '/';
    #endif
    cstr filename = (cstr)strrchr(file, path_delimiter);
    if (filename==NULL) {
        return printf("%s:%d %s -- %s\n",
                      file, line,
                      function, message);
    }
    return printf("%s:%d %s -- %s\n",
                  filename+1, line,
                  function, message);
}

} /* namespace detail */
} /* namespace nonstd */


