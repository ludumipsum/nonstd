/** Type Trait Extensions
 *  =====================
 *  Functionality based on the std <type_traits> include, extended to allow for
 *  additional checks or simplified usage.
 */
#pragma once

#include <type_traits>
#include <chrono>
#include <utility>


/** Extensions
 *  ==========
 */

namespace nonstd {

/** have_same_type
 *  --------------
 *  Check if, essentially, ::std::is_same_v<decltype(L), decltype(R)>
 */
template <typename L, typename R>
constexpr bool have_same_type(L&& l, R&& r) noexcept {
    return ::std::is_same_v<L, R>;
}

/** have_same_decayed_type
 *  ----------------------
 *  Check if the decayed types of the left and right arguments are identical
 */
template <typename L, typename R>
constexpr bool have_same_decayed_type(L&& l, R&& r) noexcept {
    return ::std::is_same_v<std::decay_t<L>, std::decay_t<R>>;
}

/** is_reference_wrapper
 *  --------------------
 *  Shorthand check for whether or not a type argument is an STL
 *  reference_wrapper (a utility class used to hold references kinda like
 *  pointers when manipulating containers / contained objects).
 */
template <typename T>
struct is_reference_wrapper : std::false_type {};
template <typename T>
struct is_reference_wrapper<std::reference_wrapper<T>> : std::true_type {};
template <typename T>
inline constexpr bool is_reference_wrapper_v = is_reference_wrapper<T>::value;

/** remove_cvref
 *  -------------
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
 *  -----------------------------------
 *  If the expressions `swap(std::declval<T>(), std::declval<T>())` is
 *  well-formed in an unevaluated context after using std::swap; provides the
 *  member constant `value` equal to `true`. Otherwise, `value` is `false`.
 *
 *  Not all standards in use (Apple clang, at least) include a std::is_swappable
 *  implementation, so this is being included to complete the standard library.
 *
 *  This implementation was largely taken from the GCC 7.2 libstdc++ v3.
 */
namespace detail::is_swappable_  {

    struct do_impl {
        template < typename T
                 , typename a_ = decltype(std::swap(std::declval<T&>(),
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

} /* namespace detail::is_swappable_ */

template <typename T>
struct is_swappable
    : public detail::is_swappable_::impl_t<T>
{ };
template <typename T>
inline constexpr bool is_swappable_v = is_swappable<T>::value;

template <typename T>
struct is_nothrow_swappable
    : public detail::is_swappable_::nothrow_t<T>
{ };
template <typename T>
inline constexpr bool is_nothrow_swappable_v = is_nothrow_swappable<T>::value;

/** first_argument_t
 *  ----------------
 *  Fetch the type of the first argument of a given callable type. Note that
 *  this will likely not work for function objects that have multiple
 *  `operator()` overloads.
 *
 *  Hat tip to Martin Boisvert on the CppLang slack, and
 *  stackoverflow.com/questions/37430898 for helping with this initial solution.
 *
 *  TODO: This feels like the start of a whole nonstd::function_traits feature
 *        set. Think about how to expand to `nth_argument_t`?
 */
namespace detail::first_argument_t_ {

    template <typename Fn, typename Return, typename First, typename... Rest>
    First helper(Return (Fn::*)(First, Rest...));

    template <typename Fn, typename Return, typename First, typename... Rest>
    First helper(Return (Fn::*)(First, Rest...) const);

} /* namespace detail::first_argument_t_ */

template <typename Fn>
using first_argument_t = decltype(
    detail::first_argument_t_::helper(&Fn::operator()) );


namespace chrono {

/** is_duration
 *  -----------
 *  Helper trait to check if the given `T` is a std::chrono::duration.
 *  Lifted from XcodeDefault.xctoolchain/usr/include/c++/v1/chrono
 */
template <typename T>
struct is_duration
    : std::false_type { };

template <typename Rep, typename Period>
struct is_duration<std::chrono::duration<Rep, Period>>
    : std::true_type { };

template <typename Rep, typename Period>
struct is_duration<std::chrono::duration<Rep, Period> const>
    : std::true_type { };

template <typename Rep, typename Period>
struct is_duration<std::chrono::duration<Rep, Period> volatile>
    : std::true_type { };

template <typename Rep, typename Period>
struct is_duration<std::chrono::duration<Rep, Period> const volatile>
    : std::true_type { };

template <typename T>
inline constexpr bool is_duration_v = is_duration<T>::value;

} /* namespace chrono */

} /* namespace nonstd */


/** Assertions
 *  ==========
 *  Macros that cause compile-time errors when we build non-compliant datatypes.
 */

/** ENFORCE
 *  -------
 *  General purpose enforcement macro to simplify one-off static_asserts.
 */
#define ENFORCE(condition) \
    static_assert(condition, "Failed to enforce '" #condition "'.")


/** ENFORCE_POD
 *  -----------
 *  Enforce the given type is a POD datatype.
 */
#define ENFORCE_POD(T) \
    static_assert(std::is_trivially_copyable_v<T>, "Type '" STRING(T) "' was marked as Plain Old Data, but is not trivially copyable. Defined near [" STRING(__FILE__) ":" STRING(__LINE__) "]");                           \
    static_assert(std::is_trivially_default_constructible_v<T>, "Type '" STRING(T) "' was marked as Plain Old Data, but is not trivially default constructible. Defined near [" STRING(__FILE__) ":" STRING(__LINE__) "]"); \
    static_assert(std::is_standard_layout_v<T>, "Type '" STRING(T) "' was marked as Plain Old Data, but is not standard layout. Defined near [" STRING(__FILE__) ":" STRING(__LINE__) "]")

/** ENFORCE_SIZE (and friends)
 *  ------------ - - - - - - -
 *  Enforce the given type is exactly / under / above / within the range of the
 *  given size(s).
 */
#define ENFORCE_SIZE(T, bytes) \
    static_assert(sizeof(T) == bytes, "Type '" STRING(T) "' is the wrong size (it is required to be " STRING(bytes) " bytes).")
#define ENFORCE_MAX_SIZE(T, max_bytes) \
    static_assert(sizeof(T) <= bytes, "Type '" STRING(T) "' is the wrong size (it is required to be at most " STRING(bytes) " bytes).")

/** ENFORCE_IS_SAME
 *  ---------------
 *  Enforce the given types are the same, as shown by `std::is_same_v`.
 */
#define ENFORCE_IS_SAME(LEFT, RIGHT) \
    static_assert(::std::is_same_v<LEFT, RIGHT>, "'" STRING(LEFT) "' doesn't have the same type as '" STRING(RIGHT) "'.")

/** ENFORCE_HAVE_SAME_TYPE
 *  ----------------------
 *  Enforce the given objects have the same type, as shown by `decltype`.
 */
#define ENFORCE_HAVE_SAME_TYPE(LEFT, RIGHT) \
    static_assert(::std::is_same_v<decltype(LEFT), decltype(RIGHT)>, "Type '" STRING(LEFT) "' is not the same as type '" STRING(RIGHT) "'.")
