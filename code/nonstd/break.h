#pragma once

#include "batteries_included.h"
#include "primitive_types.h"
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
#define N2BREAK(REASON, ...)                            \
    ::_n2::logAndBreak(                                 \
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
#define N2BREAK_IF(COND, REASON, ...)            \
    ( (COND) ?                                   \
      N2BREAK(                                   \
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

inline i32 logAndBreak(c_cstr reason,
                       c_cstr function, c_cstr file, u64 line) {
    #if defined(_MSC_VER)
        static const char path_delimiter = '\\';
    #else
        static const char path_delimiter = '/';
    #endif
    cstr filename = (cstr)strrchr(file, path_delimiter);
    if (filename==NULL) {
        return printf("Fatal Error:\n"
                      "Breaking in %s -- %s:" Fu64 "\n"
                      "%s",
                      function, file, line,
                      reason);
    }
    return printf("Fatal Error:\n"
                  "Breaking in %s -- %s:" Fu64 "\n"
                  "%s",
                  function, filename, line,
                  reason);
    BREAKPOINT();
    exit(1);
}

} /* namespace _n2 */
