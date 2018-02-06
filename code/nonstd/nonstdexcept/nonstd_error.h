/** N2 Error Codes
 *  ==============
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
#pragma once

#include <cassert>
#include <system_error>
#include <nonstd/nonstd.h>


namespace nonstd {

/** nonstd:: Error Codes
 *  --------------------
 */
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
    // these error than to be used as an actual error code.

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


namespace nonstd {

/** nonstd:: Error Category Type
 *  ----------------------------
 */
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
//FIXME: Who wants to bet that the platform and loaded libraries won't share a
//       common address on Windows? This should definitely be tested.
inline std::error_category const & error_category() noexcept {
  static const error_category_t instance {};
  return instance;
}

/** N2 Erro Code / Condition Construction Helpers
 *  ---------------------------------------------
 *  To help implicit conversion constructors (defined in the std:: namespace, of
 *  course) get the right kind of error code using Argument Dependent Lookup.
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
