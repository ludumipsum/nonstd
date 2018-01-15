/** Terraform Nonstandard Library
 *  =============================
 *  This file bundles together a number of language "extensions" (helper macros,
 *  really), platform homoginizations, std:: extensions / corrections, low-level
 *  types, helper utilities, and a few C++17 polyfills.
 *  Pretty much, it's everything we wish was part of the language, but isn't.
 *
 *  This top-level `nonstd.h` header pulls in a number of C and C++ libraries,
 *  and all Nitrogen extensions.
 */

#pragma once

/* If we're not explicitly in a debug build, make sure NDEBUG is defined */
#if !defined(DEBUG) && !defined(_DEBUG) && !defined(NDEBUG)
#  define NDEBUG
#endif

/* C Standard Library Includes */
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cinttypes>
#include <cmath>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

/* C++ Standard Library Includes */
#include <functional>
#include <iterator>
#include <limits>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>

/* Globally-available thirdparty libraries */

/* Boot Preprocessor
 * For file inside the nonstd/ dir it will still be best to ex; include exactly
 * <boost/preprocessor/seq.hpp>, to help manage which boost_pp headers are used
 * where. The thirdparty/boost/preprocessor.h header will aggregate those.
 */
#include <thirdparty/boost/preprocessor.h>

/* {fmt} Printing Library */
#include <thirdparty/fmt.h>


/* Nonstandard */
#include "core/disallow_copy_and_assign.h"
#include "core/homogenize.h"
#include "core/math.h"
#include "core/primitive_types.h"
#include "core/require_linkage.h"
#include "core/stacktrace.h"
#include "core/symbol_stringifier.h"
#include "core/unused.h"

/* Nonstandard library initialization */
namespace nonstd {
    inline void init(void) {
        REGISTER_STACK_HANDLERS();
    }
}
