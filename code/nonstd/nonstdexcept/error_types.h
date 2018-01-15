/** N2 Custom Error Types
 *  =====================
 *  What it says on the tin.
 *
 *  TODO: Move towards using exceptions, rather than hand-rolled error codes.
 */
#pragma once

#include <nonstd/nonstd.h>


/** N2 Error Codes
 *  --------------
 */
enum class N2Error {
    Success,
    Undefined,
    PEBCAK,
    System,
    UnimplementedCode,
    NullPtr,
    OutOfBounds,
    InUse,
    InsufficientMemory,
    InvalidMemory,
    ModuleNotStarted,
    UninitializedMemory,
    MissingData,
    InvalidArguments,
    HashCollision,
    DoubleInitialization,
};

/** N2 Error Strings
 *  ----------------
 *  These _must_ be ordered identically to the above.
 *
 *  TODO: Come up with a constexpr function to associate a specific N2Error
 *        with a string, so we don't have to manually maintain an order.
 */
constexpr c_cstr N2_ERROR_STRINGS[] = {
    /* Success */
                "No error. (Please update this to a clean exit).",
    /* Undefined */
                "Undefined error. (Please update error_types.h with an "
                "appropriate and descriptive Error.)",
    /* PEBCAK */
                "There is an unidentified problem somewhere between a chair "
                "and the nearest keyboard.",
    /* System */
                "System error.",
    /* UnimplementedCode */
                "Encountered unimplemented logic.",
    /* NullPtr */
                "Dereferencing a `nullptr`.",
    /* OutOfBounds */
                "Accessing out of bounds address or index.",
    /* InUse */
                "Address or index already in use.",
    /* InsufficientMemory */
                "Insufficient memory for operation.",
    /* InvalidMemory */
                "Memory location or system state is invalid.",
    /* ModuleNotStarted */
                "Calling into a Module that has not been started.",
    /* UninitializedMemory */
                "Accessing uninitialized data.",
    /* MissingData */
                "Expected data is unreachable or nonexistent.",
    /* InvalidArguments */
                "Operation executed with invalid arguments.",
    /* HashCollision */
                "A hash collision has been detected",
    /* DoubleInitialization */
                "Double (non-idempotent) initialized detected.",
};


/** `strerror` for N2Errors
 *  =======================
 *  Analogous to the C `strerr` function; turns an N2Error into a string.
 *
 *  TODO: This is a bad place for this functionality. Either it should be a
 *  to_cstr override, moved into explicit exception types, or... something?
 *  Else? Something better.
 *        Give this more thought when we're well into the exceptions shift.
 */
constexpr inline c_cstr n2strerr(N2Error err) noexcept {
    return N2_ERROR_STRINGS[(i32)(err)];
}
