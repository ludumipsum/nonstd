/** Terraform Nonstandard Library
 *  =============================
 *
 *  This file bundles together everything we would normally consider a standard
 *  part of th elanguage, but which is missing or poorly suited to our code.
 *
 *  This top-level `nonstd` header pulls in all the components.
 */

#pragma once

// Base language features
#include "batteries_included.h"
#include "primitive_types.h"

// Terraform extensions
#include "break.h"
#include "compare.h"
#include "error_types.h"
#include "hash.h"
#include "iterator.h"
#include "log.h"
#include "math.h"
#include "optional.h"
#include "stacktrace.h"

// Nonstandard library initialization
namespace nonstd {
    inline void init(void) {
        REGISTER_STACK_HANDLERS();
    }
}
