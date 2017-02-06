/* Crash Handling
 * ==============
 * Defines the macro and datatypes used for to crashing.
 * The Platform and the Game TUs will define their own implementations.
 */

#if !defined(IFACE_CRASH)
#define IFACE_CRASH

#include "common.h"


namespace crash {

/* Kerplow */
int crash(i32    error_number,
          c_cstr reason,
          c_cstr file,
          u64    line,
          c_cstr funcsig);

/* N2CRASH Macro
 * -----------
 * Used as a macro-ized passthrough to the above crash::boom function. This lets
 * us skip adding FILE and LINE macros to every invocation of crash::boom.
 */
#define N2CRASH(ERROR_NUMBER, MESSAGE, ...)                         \
    ::crash::crash(ERROR_NUMBER,                                    \
                   _variadicExpand(MESSAGE, ##__VA_ARGS__).c_str(), \
                   __FILE__,                                        \
                   __LINE__,                                        \
                   __PRETTY_FUNCTION__)

/* _IF / _UNLESS Macros & DEBUG-only N2ASSERT Macros
 * -------------------------------------------------
 * Wraping the above N2CRASH in condition checks.
 */
#define N2CRASH_IF(COND, ERRNO, MESSAGE, ...)         \
    ( (COND) ?                                        \
      N2CRASH(ERRNO,                                  \
            "Condition met ( " #COND " )\n" MESSAGE , \
            ##__VA_ARGS__) :                          \
      0 )
#define N2CRASH_UNLESS(COND, ERRNO, MESSAGE, ...)         \
    ( (COND) ?                                            \
      0 :                                                 \
      N2CRASH(ERRNO,                                      \
            "Condition not met ( " #COND " )\n" MESSAGE , \
            ##__VA_ARGS__) )


#if defined(DEBUG)
#define N2ASSERT(COND, ERRNO, MESSAGE, ...)              \
    ( (COND) ?                                           \
      0 :                                                \
      N2CRASH(ERRNO,                                     \
            "Assertion Failed ( " #COND " )\n" MESSAGE , \
            ##__VA_ARGS__) )
#define N2ASSERT_FALSE(COND, ERRNO, MESSAGE, ...)         \
    ( (COND) ?                                            \
      N2CRASH(ERRNO,                                      \
            "Assertion Failed !( " #COND " )\n" MESSAGE , \
            ##__VA_ARGS__) :                              \
      0 )
#else
#define N2ASSERT(COND, ERRNO, MESSAGE, ...)
#define N2ASSERT_FALSE(COND, ERRNO, MESSAGE, ...)
#endif

/* CAPTURE_CRASH Test Rigging
 * --------------------------
 */
#if defined(N2_TEST)
int CAPTURE_CRASH(std::function<void(void)> test);
#endif

} /* namespace crash */
#endif /* IFACE_CRASH */
