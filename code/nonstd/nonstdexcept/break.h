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


/** BREAK(ERROR, REASON[, FORMAT_ARGS...])
 *  -----------------------------------
 *  Convenience Macro to ensure the function name, file, and line number are
 *  correctly captured on breaks.
 *  If `DEBUG` is defined, `BREAK` will trigger an immediate, programmatic
 *  breakpoint; otherwise a `std::system_error` will be thrown.
 */
#define BREAK(ERROR, REASON, ...)             \
    ::nonstd::detail::log_and_break(          \
        ERROR,                                \
        ::fmt::format(REASON, ##__VA_ARGS__), \
        __PRETTY_FUNCTION__, __FILE__, __LINE__)


/** BREAK_[IF|_UNLESS](COND, ERROR, REASON[, FORMAT_ARGS...])
 *  ---------------------------------------------------------
 *  Conditional BREAK helpers.
 *
 *  Note that these macros prepend a new line -- `"Condition [un]met (. . .)"`
 *  -- to the user-provided `REASON` string.
 */
#define BREAK_IF(COND, ERROR, REASON, ...)              \
    ( (COND) ?                                          \
      ::nonstd::detail::log_and_break(                  \
          ERROR,                                        \
          ::fmt::format("Condition met ( " #COND " )\n" \
                        "- - - - -\n"                   \
                        REASON,                         \
                        ##__VA_ARGS__),                 \
          __PRETTY_FUNCTION__, __FILE__, __LINE__) :    \
      0                                                 \
    )
#define BREAK_UNLESS(COND, ERROR, REASON, ...)            \
    ( (COND) ?                                            \
      0      :                                            \
      ::nonstd::detail::log_and_break(                    \
          ERROR,                                          \
          ::fmt::format("Condition unmet ( " #COND " )\n" \
                        "- - - - -\n"                     \
                        REASON,                           \
                        ##__VA_ARGS__),                   \
          __PRETTY_FUNCTION__, __FILE__, __LINE__)        \
    )


/** `ASSERT(COND)` & `ASSERT_M(COND, MESSAGE[, FORMAT_ARGS...])
 *  -----------------------------------------------------------
 *  <cassert> style convenience macro to perform quick checks (ex; verifying
 *  preconditions) in code. The `COND` of this macro will only executed
 *  (potentially causing a crash) if the `DEBUG` symbol is defined.
 *
 *  Note that `COND` will not be executed unless `DEBUG` is defined.
 *  **SIDE-EFFECTS IN `COND` WILL NOT OCCUR UNLESS `DEBUG` IS DEFINED!**
 *  Seriously. Do not put side-effects in the `COND` of an `ASSERT`.
 */
#if defined(DEBUG)
#   define ASSERT(COND)                                   \
        do { if (!(COND)) {                               \
            ::nonstd::detail::log_and_assert(             \
                #COND,                                    \
                __PRETTY_FUNCTION__, __FILE__, __LINE__); \
        } } while (0)
#   define ASSERT_M(COND, MESSAGE, ...)                           \
        do {                                                      \
            if (!(COND)) {                                        \
                ::nonstd::detail::log_and_assert(                 \
                    #COND, ::fmt::format(MESSAGE, ##__VA_ARGS__), \
                    __PRETTY_FUNCTION__, __FILE__, __LINE__);     \
            }                                                     \
        } while (0)
#else /* ndef(DEBUG) */
#   define ASSERT(COND) \
        do { (void)sizeof((COND)); } while(0)
#   define ASSERT_M(COND, MESSAGE, ...) \
        do { (void)sizeof((COND)); } while(0)
#endif


namespace nonstd::detail {

/** log_and_break Implementation
 *  ----------------------------
 *  Users should never directly call this function, but we do need more than
 *  just macros to correctly log.
 */
inline i32 log_and_break(std::error_code error_code, std::string && reason,
                         c_cstr __function__, c_cstr __file__, u64 __line__) {
    fmt::print("~~~~~~~~~~~~~~~\n"
               "Fatal Error in:\n"
               "    {}\n"
               "    {}:{}\n"
               "Error:  {} -- {}\n"
               "Reason: {}\n"
               "~~~~~~~~~~~~~~~\n",
               __function__, __file__, __line__,
               error_code, error_code.message(),
               reason);
    DEBUG_BREAKPOINT();
    throw std::system_error { error_code, reason };
    exit(error_code.value());
}

/** log_and_assert Implementation
 *  --------------=--------------
 *  Users should never directly call this function, but we do need more than
 *  just macros to correctly log.
 */
inline i32 log_and_assert(c_cstr assert_str,
                          c_cstr __function__, c_cstr __file__, u64 __line__) {
    fmt::print("~~~~~~~~~~~~~~~~~\n"
               "Assertion Failed: {}\n"
               "    {}\n"
               "    {}:{}\n"
               "~~~~~~~~~~~~~~~~~\n",
               assert_str,
               __function__, __file__, __line__);
    BREAKPOINT();
    exit(0);
}
inline i32 log_and_assert(c_cstr assert_str, std::string && message_str,
                          c_cstr __function__, c_cstr __file__, u64 __line__) {
    fmt::print("~~~~~~~~~~~~~~~~~\n"
               "Assertion Failed: {}\n"
               "    {}\n"
               "    {}:{}\n"
               "Message: {}\n"
               "~~~~~~~~~~~~~~~~~\n",
               assert_str,
               __function__, __file__, __line__,
               message_str);
    BREAKPOINT();
    exit(0);
}

} /* namespace nonstd::detail */
