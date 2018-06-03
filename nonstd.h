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


/** Globally-Available Thirdparty Libraries
 *  =======================================
 */

/** Boot Preprocessor
 *  -----------------
 *  For files inside the nonstd/ library, it will still be best to include
 *  exactly the file needed (ex; <boost/preprocessor/seq.hpp>) to help manage
 *  which boost_pp headers are used, where. The thirdparty/boost/preprocessor.h
 *  header will aggregate those.
 */
#include <thirdparty/boost/preprocessor.h>

/** {fmt} Printing Library
 *  ----------------------
 *  Printing is hard, but it's slightly less hard with {fmt}. Expect that it's
 *  usable everywhere.
 */
#include <thirdparty/fmt.h>


/** Nonstandard Core
 *  ================
 */
#include "core/disallow_copy_and_assign.h"
#include "core/enumerate.h"
#include "core/error.h"
#include "core/homogenize.h"
#include "core/math.h"
#include "core/mem.h"
#include "core/primitive_types.h"
#include "core/range.h"
#include "core/require_linkage.h"
#include "core/stacktrace.h"
#include "core/symbol_stringifier.h"
#include "core/unused.h"


/** Nonstandard Library Initialization
 *  ==================================
 */
namespace nonstd {
    inline void init(void) {
        REGISTER_STACK_HANDLERS();
    }
}
