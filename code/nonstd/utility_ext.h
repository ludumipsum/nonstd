/** Nonstd Utility Extensions
 *  -------------------------
 *  If we were developing the C++ standard library, this functionality might
 *  land in the <utility> header. But we're not. So we're gonna write extension.
 */
#pragma once

// Utility Types
#include "utility_ext/any_t.h"
#include "utility_ext/in_place_t.h"
#include "utility_ext/void_t.h"

// Helper Functions
#include "utility_ext/compare.h"
#include "utility_ext/nop.h"
#include "utility_ext/to_cstr.h"
