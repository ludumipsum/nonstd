/* Crash Handling Interface
 * ========================
 * Defines the macros and datatypes used for to crashing.
 * The Platform and Game binaries will each require a separate implementation of
 * the interface defined in this file. For more code, see,
 *   platform/modules/crash.cc
 *   game/modules/crash.cc
 */

#if !defined(IFACE_CRASH)
#define IFACE_CRASH

#include "batteries_included.h"
#include "primitive_types.h"
#include "logging.h"


/**
 * N2 Error Codes
 */
enum class N2Error {
    Success,
    PEBCAK,
    Undefined,
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
};


namespace crash {

/* Kerplow */
i32 crash(N2Error error,
          c_cstr  reason,
          c_cstr  file,
          u64     line,
          c_cstr  funcsig);


/* N2CRASH Macro
 * -----------
 * Used as a macro-ized passthrough to the above crash::crash function. This
 * lets us skip adding FILE and LINE macros to every invocation of crash::crash.
 */
#define N2CRASH(ERROR, MESSAGE, ...)                                \
    ::crash::crash(ERROR,                                           \
                   _variadicExpand(MESSAGE, ##__VA_ARGS__).c_str(), \
                   __FILE__,                                        \
                   __LINE__,                                        \
                   __PRETTY_FUNCTION__)

/* _IF / _UNLESS Macros & DEBUG-only N2ASSERT Macros
 * -------------------------------------------------
 * Wraping the above N2CRASH in condition checks.
 */
#define N2CRASH_IF(COND, ERR, MESSAGE, ...)           \
    ( (COND) ?                                        \
      N2CRASH(ERR,                                    \
            "Condition met ( " #COND " )\n" MESSAGE , \
            ##__VA_ARGS__) :                          \
      0 )
#define N2CRASH_UNLESS(COND, ERR, MESSAGE, ...)           \
    ( (COND) ?                                            \
      0 :                                                 \
      N2CRASH(ERR,                                        \
            "Condition not met ( " #COND " )\n" MESSAGE , \
            ##__VA_ARGS__) )


/* CAPTURE_CRASH Test Rigging
 * --------------------------
 */
#if defined(N2_TEST)
int CAPTURE_CRASH(std::function<void(void)> test);
#endif

/**
 * N2 String descriptors / to-string helper
 * -------------------------------------------------------
 */
c_cstr n2strerr(N2Error err);

static c_cstr error_strings[] = {
    /* Success */       "No error. (Please update this to a clean exit).",
    /* PEBCAK */        "There is an unidentified problem somewhere between "
                        "a chair and the nearest keybaord.",
    /* Undefined */     "Undefined error. (Please update crash.h and "
                        "incorporate a descriptive Error.)",
    /* System */        "System error.",
    /* UnimplementedCode */
                        "Encountered unimplemented logic.",
    /* NullPtr */       "Dereferencing a `nullptr`.",
    /* OutOfBounds */   "Accessing out of bounds address or index.",
    /* InUse */         "Address or index already in use.",
    /* InsufficientMemory */
                        "Insufficient memory for operation.",
    /* InvalidMemory */ "Memory location or system state is invalid.",
    /* ModuleNotStarted */
                        "Calling into a Module that has not been started.",
    /* UninitializedMemory */
                        "Accessing uninitialized data.",
    /* MissingData */   "Expected data is unreachable or nonexistant.",
    /* InvalidArguments */
                        "Operation executed with invalid arguments.",
};

c_cstr n2strerr(N2Error err) {
    return error_strings[(i32)(err)];
}


} /* namespace crash */
#endif /* IFACE_CRASH */
