/** n2strerr
 *  ========
 *  Analogous to the C `strerr` function; turns an N2Error into a string.
 */
#pragma once

#include "../core/error_types.h"
#include "../core/primitive_types.h"


inline c_cstr n2strerr(N2Error err) {
    return N2_ERROR_STRINGS[(i32)(err)];
}
