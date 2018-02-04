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

#include <system_error>
#include <nonstd/nonstd.h>


namespace nonstd {

/** nonstd:: Error Codes
 *  --------------------
 */
enum class error : int {
    success = 0,
    undefined,
    uncategorized,
    pebcak,
    unimplemented_code,
    null_ptr,
    out_of_bounds,
    in_use,
    insufficient_memory,
    invalid_memory,
    uninitialized_memory,
    double_initialization,
    module_not_started,
    missing_data,
    invalid_arguments,
    target_not_found,
    hash_collision,
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
    case nonstd::error::success:
        return "No error. (Please update this codepath to cleanly exit.)";
    case nonstd::error::undefined:
        return "Undefined N2 Error. (Please update error_types.h with an "
                "appropriate and descriptive error for this case.)";
    case nonstd::error::uncategorized:
        return "Uncategorized system Failure.";
    case nonstd::error::pebcak:
        return "Unidentified problem exists somewhere between a chair and the "
               "nearest keyboard.";
    case nonstd::error::unimplemented_code:
        return "Attempted to invoke unimplemented logic.";
    case nonstd::error::null_ptr:
        return "Attempted to dereference a null pointer.";
    case nonstd::error::out_of_bounds:
        return "Accessing out-of-bounds address or index.";
    case nonstd::error::in_use:
        return "Address or index already in use.";
    case nonstd::error::insufficient_memory:
        return "Insufficient memory.";
    case nonstd::error::invalid_memory:
        return "Invalid memory location or system state.";
    case nonstd::error::uninitialized_memory:
        return "Uninitialized memory location or system state.";
    case nonstd::error::double_initialization:
        return "Double (non-idempotent) initialization detected.";
    case nonstd::error::module_not_started:
        return "Attempted interaction with uninitialized module.";
    case nonstd::error::missing_data:
        return "Data is missing or unreachable.";
    case nonstd::error::invalid_arguments:
        return "Invalid arguments for operation.";
    case nonstd::error::hash_collision:
        return "Non-reconcilable hash collision detected.";
    case nonstd::error::target_not_found:
        return "Execution target not found.";
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
