/** Programmatic Breakpoints
 *  ========================
 *  Force a breakpoint on X86 when called, and maybe log, too.
 *  These procedures should only be used in very low level code. Raising
 *  exceptions will likely become the better option, but sometimes you just want
 *  to blow up.
 */

#pragma once

#include <string>
#include <system_error>
#include <nonstd/nonstd.h>



/** BREAKPOINT and DEBUG_BREAKPOINT
 *  -------------------------------
 *  Raises a sigint (or similar) to allow for programmatic breakpoints
 */
#if defined(NONSTD_OS_WINDOWS)
#  ifdef _X86_
#    define _DEBUG_BREAK_IMPL { __asm { int 3 } }
#  else
#    define _DEBUG_BREAK_IMPL  { __debugbreak(); }
#  endif
#else
#  include <signal.h>
#  define _DEBUG_BREAK_IMPL { raise(SIGINT); } // GCC x86/64
#endif


#define BREAKPOINT() { _DEBUG_BREAK_IMPL; }

#if defined(DEBUG)
#  define DEBUG_BREAKPOINT() { _DEBUG_BREAK_IMPL; }
#else
#  define DEBUG_BREAKPOINT()
#endif


/** N2BREAK
 *  -------
 *  Convenience Macro to ensure the function name, file, and line number are
 *  correctly captured on breaks.
 */
#define N2BREAK(ERROR, REASON, ...)           \
    ::nonstd::detail::logAndBreak(            \
        ERROR,                                \
        ::fmt::format(REASON, ##__VA_ARGS__), \
        __PRETTY_FUNCTION__, __FILE__, __LINE__)


/** N2BREAK_IF & N2BREAK_UNLESS
 *  ---------------------------
 *  Conditional N2BREAK helpers.
 *
 *  Note that these macros prepend a new line -- `"Condition [un]met (. . .)"`
 *  -- to the user-provided `REASON` string.
 */
#define N2BREAK_IF(COND, ERROR, REASON, ...)            \
    ( (COND) ?                                          \
      ::nonstd::detail::logAndBreak(                    \
          ERROR,                                        \
          ::fmt::format("Condition met ( " #COND " )\n" \
                        "- - - - -\n"                   \
                        REASON,                         \
                        ##__VA_ARGS__),                 \
          __PRETTY_FUNCTION__, __FILE__, __LINE__) :    \
      0                                                 \
    )
#define N2BREAK_UNLESS(COND, ERROR, REASON, ...)          \
    ( (COND) ?                                            \
      0      :                                            \
      ::nonstd::detail::logAndBreak(                      \
          ERROR,                                          \
          ::fmt::format("Condition unmet ( " #COND " )\n" \
                        "- - - - -\n"                     \
                        REASON,                           \
                        ##__VA_ARGS__),                   \
          __PRETTY_FUNCTION__, __FILE__, __LINE__)        \
    )


/** logAndBreak Implementation
 *  --------------------------
 *  Users should never directly call this function, but we do need more than
 *  just macros to correctly log.
 */
namespace nonstd {
namespace detail {

inline i32 logAndBreak(std::error_code error, std::string && reason,
                       c_cstr __function__, c_cstr __file__, u64 __line__) {
    fmt::print("~~~~~~~~~~~~~~~\n"
               "Fatal Error in:\n"
               "    {}\n"
               "    {}:{}\n"
               "Error:  {} -- {}\n"
               "Reason: {}\n"
               "~~~~~~~~~~~~~~~\n",
               __function__, __file__, __line__,
               error, error.message(),
               reason);
    BREAKPOINT();
    exit(error.value());
}

} /* namespace detail */
} /* namespace nonstd */
