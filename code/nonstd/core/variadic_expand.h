/** Variadic Message Format Expander
 *  ================================
 *  nonstd::variadicExpand allows the `MESSAGE` parameter that's passed into
 *  `LOG` take the form of a format string.
 *  Modified from Erik Aronesty's answer of this SO question:
 *  http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
 *  but also: http://stackoverflow.com/questions/7825648/vscprintf-equivalent-on-android/
 */

#pragma once

#include <cstdarg>
#include <string>

#include "primitive_types.h"


namespace nonstd {

inline std::string variadicExpand(c_cstr message, ...)
{
    va_list format_args;

    va_start(format_args, message);
    u32 size = 1 + vsnprintf(NULL, 0, message, format_args);
    va_end(format_args);

    std::string expanded;
    expanded.resize(size);
    cstr data = const_cast<cstr>(expanded.data());

    va_start(format_args, message);
    vsnprintf(data, size, message, format_args);
    va_end(format_args);

    return expanded;
}

} /* namespace nonstd */
