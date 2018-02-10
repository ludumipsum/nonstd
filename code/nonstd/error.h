/** nonstd:: Error Handling
 *  =======================
 *  This file defines a few significant types, functions, and macros;
 *
 *  * `nonstd::error`
 *     An `enum class` of nonstd-specific error code
 *  * `nonstd::error_category()`
 *     A std::error_category overload to help manage our error code set
 *  * `BREAKPOINT` and `DEBUG_BREAKPOINT`
 *     Macros that will raise a programmatic breakpoint when called
 *  * `BREAK`, `BREAK_IF`, and `BREAK_UNLESS`
 *     Macros that (when triggered) will log function, file, and line
 *     information, and trigger a system crash.
 *  * `ASSERT`, and `ASSERT_M`
 *     <cassert>-style Macros that can be used to perform quick tests. Only
 *     executed when `DEBUG` is defined. Should be used as compiler-verified
 *     documentation for pre/postconditions, value verifications, etc..
 */
#pragma once

#include <system_error>
#include <nonstd/nonstd.h>


/** nonstd:: Error Codes
 *  ----------------------------------------------------------------------------
 *  For when something breaks, and it's totally our fault.
 *
 *  Implemented as an extension of `<system_error>` to allow us to use the
 *  `nonstd::error` enum class as a source of formal error codes, whether or not
 *  those codes are included in thrown `std::system_error` exceptions.
 *
 *  For an in-depth discussion of the design methodology of <system_error>,
 *  I would recommend Andrzej Krzemieński's series on the subjet;
 *  https://akrzemi1.wordpress.com/2017/07/12/your-own-error-code/
 *  https://akrzemi1.wordpress.com/2017/08/12/your-own-error-condition/
 *  https://akrzemi1.wordpress.com/2017/09/04/using-error-codes-effectively/
 */
namespace nonstd {

enum class error : int {
    error         = 0x100, // generic error
    pebcak        = 0x101, // generic author error
    unimplemented = 0x102, // the code isn't there
    os            = 0x1e0, // unspecified error from the operating system
    external      = 0x1e1, // unspecified error from an external system

    memory               = 0x200, // generic memory error
    insufficient_memory  = 0x201, // not enough memory
    uninitialized_memory = 0x202, // memory not initialized
    reinitialized_memory = 0x203, // non-idempotent memory re-initialization
    invalid_memory       = 0x204, // memory not valid

    // The below are highly specific errors that don't really fit into a larger
    // category. The `uncategorized` type is more to set the value range of
    // these errors than to be used as an actual error code.

    uncategorized = 0x1000, // uncategorized error. Don't use this.
    module_not_started, // module was used before it was started
    hash_collision, // distinct values returned an identical hash
};

} /* namespace nonstd */

/** Error Code Registration
 *  -----------------------
 *  `nonstd::error` represents specific error codes (not a queryable set of
 *  error conditions), so we need to register it as such.
 */
namespace std {
    template <> struct is_error_code_enum<nonstd::error> : public true_type {};
} /* namespace std */


/** nonstd:: Error Category Type
 *  ----------------------------------------------------------------------------
 */
namespace nonstd {

class error_category_t : public std::error_category {
public:
    virtual c_cstr name() const noexcept;
    virtual std::string message(int condition) const;
    virtual bool equivalent(int code, std::error_condition const & condition) const noexcept;
};

/** nonstd:: Error Category Singleton
 *  ---------------------------------
 *  The identification of a `std::error_category`s is based on the address of
 *  the singleton object that defines the category. We use a local-static to
 *  guarantee that all callers of this function share the common instance.
 */
inline std::error_category const & error_category() noexcept {
    //FIXME: Who wants to bet that the platform and loaded libraries won't share
    //       a common address on Windows? This should definitely be tested.
    static const error_category_t instance {};
    return instance;
}

/** N2 Error Code / Condition Construction Helpers
 *  ----------------------------------------------
 *  To help implicit conversion constructors (which are defined in the std::
 *  namespace) get the right kind of error code using Argument Dependent Lookup.
 */
inline std::error_code make_error_code(nonstd::error e) noexcept {
    return std::error_code(static_cast<int>(e), error_category());
}
inline std::error_condition make_error_condition(nonstd::error e) noexcept {
    return std::error_condition(static_cast<int>(e), error_category());
}


/** N2 Error Category Member Functions
 *  ----------------------------------
 */

/**
 * Return a null-terminated c_cstr that gives the name of the
 * nonstd::error category.
 */
inline c_cstr error_category_t::name() const noexcept {
    return "N2 Error";
}

/**
 * Return a string describing the given error code for the
 * nonstd::error category.
 */
inline std::string error_category_t::message(int code) const {
    switch (static_cast<nonstd::error>(code)) {
    case nonstd::error::error:
        return "Unspecified generic N2 error (Please update nonstd_error.h "
               "with an appropriate and descriptive error for this case)";
    case nonstd::error::pebcak:
        return "A problem exists between someone's chair and keyboard";
    case nonstd::error::unimplemented:
        return "Attempted to invoke unimplemented logic";
    case nonstd::error::os:
        return "Unspecified OS-level error";
    case nonstd::error::external:
        return "Unspecified external system error";
    case nonstd::error::memory:
        return "Unspecified N2 memory error (Please update nonstd_error.h "
               "with an appropriate and descriptive error for this case)";
    case nonstd::error::insufficient_memory:
        return "Insufficient system or buffered memory";
    case nonstd::error::uninitialized_memory:
        return "Uninitialized memory or buffer";
    case nonstd::error::reinitialized_memory:
        return "Non-idempotent (destructive?) memory re-initialization";
    case nonstd::error::invalid_memory:
        return "Invalid memory or buffer";
    case nonstd::error::uncategorized:
        return "Uncategorized error ((Please update nonstd_error.h "
               "with an appropriate and descriptive error for this case)";
    case nonstd::error::module_not_started:
        return "Attempted interaction with uninitialized module";
    case nonstd::error::hash_collision:
        return "Non-reconcilable hash collision detected";
    } /* switch (static_cast<nonstd::error>(code)) */
}

/** Check if the nonstd::error `code` is equivalent to the given (generic) error
 *  `condition`. Note that `condition` may not be in the nonstd::error category.
 */
inline
bool error_category_t::equivalent(int code, std::error_condition const & cond)
const noexcept {
    // As the preamble states, this function allows us to declare nonstd::error
    // *codes* equivalent to more generic error *conditions*. For example, if we
    // want to allow users to respond to `nonstd::error::insufficient_memory` in
    // the same way they respond to the `std::errc::not_enough_memory` (the
    // POSIX `ENOMEM`) condition, we could make that equivalency explicit here.
    switch (static_cast<nonstd::error>(code)) {
    default: {
        return default_error_condition(code) == cond;
    }
    } /* switch (static_cast<nonstd::error>(code)) */
}

} /* namespace nonstd */


/** Programmatic Breakpoints
 *  ----------------------------------------------------------------------------
 *  Force a breakpoint, maybe log, maybe just explode.
 *  These procedures should only be used in very low level code. Raising
 *  exceptions will likely become the better option, but sometimes you just want
 *  to blow up.
 */

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
