/** Type Trait Extensions
 *  =====================
 *  Functionality based on the std <type_traits> include, extended to allow for
 *  additional checks or simplified usage.
 */
#pragma once

#include <type_traits>
#include <utility>


/** Extensions
 *  ----------
 */

namespace nonstd {

/** have_same_type
 *  Check if, essentially, ::std::is_same_v<decltype(L), decltype(R)>
 */
template <typename L, typename R>
constexpr bool have_same_type(L&& l, R&& r) noexcept {
    return ::std::is_same_v<L, R>;
}


/** remove_cvref
 *  If the type `T` is a reference type, provides the member type `type` which
 *  is the type referred to by `T` with its topmost cv-qualifiers removed.
 *  Otherwise `type` is `T` with its topmost cv-qualifiers removed.
 *
 *  This is a C++20-ism, so is being included as a future-polyfil.
 */
template <typename T>
struct remove_cvref {
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};
template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;


/** is_swappable && is_nothrow_swappable
 *  If the expressions `swap(std::declval<T>(), std::declval<T>())` is
 *  well-formed in an unevaluated context after using std::swap; provides the
 *  member constant `value` equal to `true`. Otherwise, `value` is `false`.
 *
 *  Not all standards in use (Apple clang, at least) include a std::is_swappable
 *  implementation, so this is being included to complete the standard library.
 *
 *  This implementation was largely taken from the GCC 7.2 libstdc++ v3.
 */
namespace __is_swappable_details  {

    struct do_impl {
        template < typename T
                 , typename _ = decltype(std::swap(std::declval<T&>(),
                                                   std::declval<T&>()))>
        static std::true_type test(int);

        template <typename>
        static std::false_type test(...);
    };
    template <typename T>
    struct impl : public do_impl {
        using type = decltype(test<T>(0));
    };
    template <typename T>
    using impl_t = typename impl<T>::type;

    struct do_nothrow {
        template <typename T>
        static std::bool_constant<noexcept(std::swap(std::declval<T&>(),
                                                     std::declval<T&>()))>
        test(int);

        template <typename>
        static std::false_type test(...);
    };
    template <typename T>
    struct nothrow : public do_nothrow {
        using type = decltype(test<T>(0));
    };
    template <typename T>
    using nothrow_t = typename nothrow<T>::type;

} /* namespace __is_swappable_details */

template <typename T>
struct is_swappable
    : public __is_swappable_details::impl_t<T>
{ };
template <typename T>
inline constexpr bool is_swappable_v = is_swappable<T>::value;


template <typename T>
struct is_nothrow_swappable
    : public __is_swappable_details::nothrow_t<T>
{ };
template <typename T>
inline constexpr bool is_nothrow_swappable_v = is_nothrow_swappable<T>::value;

} /* namespace nonstd */


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
