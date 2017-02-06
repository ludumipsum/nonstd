/* Crash Handling
 * ==============
 * Defines the macro and datatypes used for to crashing.
 * The Platform and the Game TUs will define their own implementations.
 */

#if !defined(IFACE_CRASH)
#define IFACE_CRASH

#include "batteries_included.h"
#include "primitive_types.h"


namespace crash {

/**
 * N2 Error Codes
 */
enum class Error {
    Success,
    Undefined,
    Unknown,
    System,
    Unimplemented,
    Unsupported,
    NullPtr,
    OutOfBounds,
    InUse,
    InsufficientMemory,
    InvalidMemory,
    ModuleNotStarted,
    Uninitialized,
};


/* Kerplow */
i32 crash(Error  error,
          c_cstr reason,
          c_cstr file,
          u64    line,
          c_cstr funcsig);

/* N2CRASH Macro
 * -----------
 * Used as a macro-ized passthrough to the above crash::boom function. This lets
 * us skip adding FILE and LINE macros to every invocation of crash::boom.
 */
//TODO: Add a 1-arity crash macro s.t. the MESSAGE is optional?
#define N2CRASH(ERROR, MESSAGE, ...)                                \
    ::crash::crash(crash::Error::ERROR,                             \
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


#if defined(DEBUG)
#define N2ASSERT(COND, ERR, MESSAGE, ...)                \
    ( (COND) ?                                           \
      0 :                                                \
      N2CRASH(ERR,                                       \
            "Assertion Failed ( " #COND " )\n" MESSAGE , \
            ##__VA_ARGS__) )
#define N2ASSERT_FALSE(COND, ERR, MESSAGE, ...)           \
    ( (COND) ?                                            \
      N2CRASH(ERR,                                        \
            "Assertion Failed !( " #COND " )\n" MESSAGE , \
            ##__VA_ARGS__) :                              \
      0 )
#else
#define N2ASSERT(COND, ERR, MESSAGE, ...)
#define N2ASSERT_FALSE(COND, ERR, MESSAGE, ...)
#endif

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
c_cstr n2strerr(Error err);

static c_cstr error_strings[] = {
    /* Success */            "No error. (Please update this to a clean exit).",
    /* Undefined */          "Undefined error. (Please update crash.h and "
                             "incorporate a descriptive Error.)",
    /* Unknown */            "Unknown error.",
    /* System */             "System error.",
    /* Unimplemented */      "Entering unimplemented block.",
    /* Unsupported */        "Unsupported state or operation.",
    /* NullPtr */            "Dereferencing a `nullptr`.",
    /* OutOfBounds */        "Out Of Bounds address or index.",
    /* InUse */              "Address or Index already in use.",
    /* InsufficientMemory */ "Insufficient memory for operation.",
    /* InvalidMemory */      "Memory location or state is invalid.",
    /* ModuleNotStarted */   "Using a Module that has not been started.",
    /* Uninitialized */      "Accessing uninitialized data.",
};

c_cstr n2strerr(Error err) {
    return error_strings[(i32)(err)];
}


} /* namespace crash */
#endif /* IFACE_CRASH */
