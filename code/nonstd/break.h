#pragma once

#include "batteries_included.h"
#include "primitive_types.h"
#include "error_types.h"
#include "log.h"

/** Programmatic Breakpoint
 *  =======================
 *  Force a breakpoint on X86 when called.
 */
#ifdef _MSC_VER
# ifdef _X86_
#  define _DEBUG_BREAK_IMPL { __asm { int 3 } }
# else
#  define _DEBUG_BREAK_IMPL  { __debugbreak(); } // need <intrin.h>
# endif
#else
# include <signal.h>
# define _DEBUG_BREAK_IMPL { raise(SIGINT); } // GCC x86/64
#endif

#define BREAKPOINT() { _DEBUG_BREAK_IMPL; }


#if defined(DEBUG)
# define DEBUG_BREAKPOINT() { _DEBUG_BREAK_IMPL; }
#else
# define DEBUG_BREAKPOINT()
#endif


/** Log an Error, and Break
 *  =======================
 *  N2BREAK -- and its conditional variants -- should only be used only in very
 *  low-level code. If you would otherwise call `exit(1)`, call N2BREAK.
 */

/** N2BREAK
 *  -------
 *  Convenience Macro to ensure the function name, file, and line number are
 *  correctly captured on breaks.
 */
#define N2BREAK(ERROR, REASON, ...)                     \
    ::_n2::logAndBreak(                                 \
        ERROR,                                          \
        _variadicExpand(REASON, ##__VA_ARGS__).c_str(), \
        __PRETTY_FUNCTION__, __FILE__, __LINE__)

/** N2BREAK_IF & N2BREAK_UNLESS
 *  ---------------------------
 *  Conditional N2BREAK helpers.
 *  Note that these macros prepend a new line -- `"Condition [not ]met (. . .)"`
 *  -- to the user-provided `REASON` string. This is accomplished using
 *  `_variadicExpand` to ensure c_cstr variables are correctly passed through
 *  the macros.
 */
#define N2BREAK_IF(COND, ERROR, REASON, ...)     \
    ( (COND) ?                                   \
      N2BREAK(                                   \
          (ERROR),                               \
          _variadicExpand("%s\n"                 \
                          "%s",                  \
                  "Condition met ( " #COND " )", \
                  (REASON)                       \
              ).c_str(),                         \
          ##__VA_ARGS__) :                       \
      0                                          \
    )

#define N2BREAK_UNLESS(COND, REASON, ...)            \
    ( (COND) ?                                       \
      0 :                                            \
      N2BREAK(                                       \
          (ERROR),                                   \
          _variadicExpand("%s\n"                     \
                          "%s",                      \
                  "Condition not met ( " #COND " )", \
                  (REASON)                           \
              ).c_str(),                             \
          ##__VA_ARGS__)                             \
    )


/** logAndBreak Implementation
 *  --------------------------
 *  Users should never directly call this function, but we do need more than
 *  just macros to correctly log.
 */
namespace _n2 {

inline i32 logAndBreak(N2Error error, c_cstr reason,
                       c_cstr function, c_cstr file, u64 line) {
    printf("~~~~~~~~~~~~~~~\n"
           "Fatal Error in:\n"
           "    %s\n"
           "    %s:" Fu64 "\n"
           "Errno:  %d (%s)\n"
           "Reason: %s\n"
           "~~~~~~~~~~~~~~~\n",
           function, file, line,
           (i32)error, n2strerr(error),
           reason);
    BREAKPOINT();
    exit((i32)(error));
}

} /* namespace _n2 */
