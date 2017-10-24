/** N2 Custom Error Types
 *  =====================
 *  What it says on the tin.
 *
 *  TODO: Move towards uisng exceptions, rather than hand-rolled error codes.
 */
#pragma once

#include "primitive_types.h"


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
                "and the nearest keybaord.",
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
    /* DoubleInitialization */
                "Double (non-idempotent) initialized detected.",
};
