/** n2strerr
 *  ========
 *  Analogous to the C `strerr` function; turns an N2Error into a string.
 *
 *  TODO: This is a bad place for this functionality. Either it should be a
 *  to_cstr override, moved into explicit exception types, or... something?
 *  Else? Something better.
 *        Give this more thought when we're well into the exceptions shift.
 */
#pragma once

#include "nonstd/core/error_types.h"
#include "nonstd/core/primitive_types.h"


constexpr inline c_cstr n2strerr(N2Error err) noexcept {
    return N2_ERROR_STRINGS[(i32)(err)];
}
