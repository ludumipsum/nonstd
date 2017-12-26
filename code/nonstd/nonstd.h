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
#include "preprocessor/disallow_copy_and_assign.h"
#include "preprocessor/homogenize.h"
#include "preprocessor/require_linkage.h"
#include "preprocessor/symbol_stringifier.h"
#include "preprocessor/unused.h"

#include "cpp1z/any_t.h"
#include "cpp1z/in_place_t.h"
#include "cpp1z/special_member_filters.h"
#include "cpp1z/type_trait_assertions.h"
#include "cpp1z/type_traits.h"
#include "cpp1z/void_t.h"

#include "core/break.h"
#include "core/error_types.h"
#include "core/primitive_types.h"
#include "core/variadic_expand.h"

#include "memory/buffer.h"
#include "memory/core_functions.h"
#include "memory/array.h"
#include "memory/hashtable.h"
#include "memory/nr_ptr.h"
#include "memory/ring.h"
#include "memory/stream.h"

#include "c_ish/math.h"
#include "c_ish/mem.h"
#include "c_ish/n2strerr.h"
#include "c_ish/stacktrace.h"
#include "c_ish/type_name.h"
#include "c_ish/value_category.h"

#include "std_ish/compare.h"
#include "std_ish/cx_math.h"
#include "std_ish/hash.h"
#include "std_ish/math.h"
#include "std_ish/to_cstr.h"

#include "utility/color.h"
#include "utility/four_char_code.h"
#include "utility/iterator.h"
#include "utility/nop.h"
#include "utility/optional.h"
#include "utility/scope_guard.h"

/* Platform specific includes */
#if defined(NONSTD_OS_WINDOWS) || defined(__MINGW32__)
#include <io.h>     // mktmp
#include <malloc.h> // alloca
#else
#include <alloca.h> // alloca
#endif

/* Nonstandard library initialization */
namespace nonstd {
    inline void init(void) {
        REGISTER_STACK_HANDLERS();
    }
}
