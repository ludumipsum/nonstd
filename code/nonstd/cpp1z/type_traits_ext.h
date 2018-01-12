/** Type Trait Extensions
 *  =====================
 *  Functionality based on the std <type_traits> include, extended to allow for
 *  additional checks or simplified usage.
 */
#pragma once

#include <type_traits>
#include <nonstd/preprocessor/symbol_stringifier.h>


/** Extensions
 *  ----------
 */

namespace nonstd {

/** has_same_type
 *  Check if, essentially, ::std::is_same_v<decltype(L), decltype(R)>
 */
template <typename L, typename R>
constexpr bool have_same_type(L const & l, R const & r) noexcept {
    return ::std::is_same_v<L, R>;
}

/** remove_cvref
 *  If the type `T` is a reference type, provides the member typedef `type`
 *  which is the type referred to by `T` with its topmost cv-qualifiers removed.
 *  Otherwise `type` is `T` with its topmost cv-qualifiers removed.
 */
template <typename T>
struct remove_cvref {
    typedef std::remove_cv_t<std::remove_reference_t<T>> type;
};
template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

}


/** Assertions
 *  ----------
 *  Macros that cause compile-time errors when we build non-compliant datatypes.
 */

/** ENFORCE
 *  General purpose enforcement macro to simplify one-off static_asserts.
 */
#define ENFORCE(condition) \
    static_assert(condition, "Failed to enforce '" #condition "'.")


/** ENFORCE_POD
 *  Enforce the given type is a POD datatype.
 */
#define ENFORCE_POD(T) \
    static_assert(std::is_trivially_copyable_v<T>, "Type '" STRING(T) "' was marked as Plain Old Data, but is not trivially copyable. Defined near [" STRING(__FILE__) ":" STRING(__LINE__) "]");                           \
    static_assert(std::is_trivially_default_constructible_v<T>, "Type '" STRING(T) "' was marked as Plain Old Data, but is not trivially default constructible. Defined near [" STRING(__FILE__) ":" STRING(__LINE__) "]"); \
    static_assert(std::is_standard_layout_v<T>, "Type '" STRING(T) "' was marked as Plain Old Data, but is not standard layout. Defined near [" STRING(__FILE__) ":" STRING(__LINE__) "]")

/** ENFORCE_SIZE (and friends)
 *  Enforce the given type is exactly / under / above / within the range of the
 *  given size(s).
 */
#define ENFORCE_SIZE(T, bytes) \
    static_assert(sizeof(T) == bytes, "Type '" STRING(T) "' is the wrong size (it is required to be " STRING(bytes) " bytes).")
#define ENFORCE_MAX_SIZE(T, max_bytes) \
    static_assert(sizeof(T) <= bytes, "Type '" STRING(T) "' is the wrong size (it is required to be at most " STRING(bytes) " bytes).")

/** ENFORCE_IS_SAME
 *  Enforce the given types are the same, as shown by `std::is_same_v`.
 */
#define ENFORCE_IS_SAME(LEFT, RIGHT) \
    static_assert(::std::is_same_v<LEFT, RIGHT>, "'" STRING(LEFT) "' doesn't have the same type as '" STRING(RIGHT) "'.")

/** ENFORCE_HAVE_SAME_TYPE
 *  Enforce the given objects have the same type, as shown by `decltype`.
 */
#define ENFORCE_HAVE_SAME_TYPE(LEFT, RIGHT) \
    static_assert(::std::is_same_v<decltype(LEFT), decltype(RIGHT)>, "Type '" STRING(LEFT) "' is not the same as type '" STRING(RIGHT) "'.")
