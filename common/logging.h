/* Loggin' Utilities
 * =================
 * Utility functions and macros for logging, variadic printf-expansion, and
 * similar chores.
 *
 * https://www.youtube.com/watch?v=siwpn14IE7E
 */

#pragma once

#include "batteries_included.h"
#include "primitive_types.h"


/* Variadic Message Format Expander
 * --------------------------------
 * _variadicExpand allows the `MESSAGE` parameter that's passed into `LOG` take
 * the form of a format string.
 * Modified from Erik Aronesty's answer of this SO question:
 * http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
 * but also: http://stackoverflow.com/questions/7825648/vscprintf-equivalent-on-android/
 */
inline std::string _variadicExpand(c_cstr message, ...)
{
    va_list format_args;

    va_start(format_args, message);
    u32 size = vsnprintf(NULL, 0, message, format_args);
    va_end(format_args);

    std::string expanded;
    expanded.resize(size + 1);

    va_start(format_args, message);
    vsnprintf(const_cast<cstr>(expanded.c_str()), size + 1,
                                message, format_args);
    va_end(format_args);

    return expanded;
}

/* Logging Implementation
 * ----------------------
 * Actually emit text from calls made with LOG(...)
 */
inline i32 _logMessage(c_cstr    message,
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

/* Logging Macro Shorthand
 * -----------------------
 * If you want to log a thing, you almost 100% definitely certainly want to
 * probably use this one.
 */
#define LOG(MESSAGE, ...)                                          \
    _logMessage(::_variadicExpand(MESSAGE, ##__VA_ARGS__).c_str(), \
                __FILE__,                                          \
                __LINE__,                                          \
                __FUNCTION__)
