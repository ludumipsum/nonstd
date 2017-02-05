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

/* CRASH Macro
 * -----------
 * Used as a macro-ized passthrough to the above crash::boom function. This lets
 * us skip adding FILE and LINE macros to every invocation of crash::boom.
 */
#define CRASH(ERROR_NUMBER, MESSAGE, ...)                           \
    ::crash::crash(ERROR_NUMBER,                                    \
                   _variadicExpand(MESSAGE, ##__VA_ARGS__).c_str(), \
                   __FILE__,                                        \
                   __LINE__,                                        \
                   __PRETTY_FUNCTION__)

/* N2ASSERT Macro
 * --------------
 * Calls the above CRASH
 */
#if defined(DEBUG)
#define N2ASSERT(COND, ERRNO, MESSAGE, ...)                                  \
  ((COND) ? 0 :                                                              \
   CRASH(ERRNO, "Assertion Failed ( " #COND " )\n" MESSAGE , ##__VA_ARGS__))
#else
#define N2ASSERT(COND, ERRNO, MESSAGE, ...)
#endif

/* CAPTURE_CRASH Test Rigging
 * --------------------------
 */
#if defined(N2_TEST)
int CAPTURE_CRASH(std::function<void(void)> test);
#endif

} /* namespace crash */
#endif /* IFACE_CRASH */
