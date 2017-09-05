/** C Standard Library Includes
 *  ===========================
 *  This file contains all C standard library includes we use. It is expected to
 *  be included in every other file in the project.
 *
 *  It is also the home for a collection of macros and utility functions which
 *  make C++ more comfortable to work in and closer to homogenous across our
 *  target platforms. If you would have put something in `util.h` in the old
 *  world, or `common.h` in the module system, there's a good chance it should
 *  live here.
 *
 *  All files under nonstd should also include this header.
 */

#pragma once

/* If we're not explicitly in a debug build, make sure NDEBUG is defined */
#if !defined(DEBUG) && !defined(_DEBUG)
# define NDEBUG
#endif

/* ### C Standard Library Includes */
#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ### C++ Standard Library Includes */
#include <functional>
#include <limits>
#include <string>
#include <type_traits>
#include <unordered_map>


/* Platform specific includes */
#if defined(_MSC_VER) || defined(__MINGW32__)
#include <malloc.h>     // alloca
#else
#include <alloca.h>     // alloca
#endif


/* Compiler-specific warning suppressions */
#if defined(_MSC_VER)
#pragma warning(disable: 4200)  // Nonstandard extension: zero length array
#pragma warning(disable: 4201)  // Nonstandard extension: nameless struct/union
#endif


/** Symbol Stringifyer
 *  ==================
 *  Uses the preprocessor to create a static string version of the passed symbol
 *  or macro. Usage:
 *      char const* msg = "This is a string literal defined at "
 *                        STRING(__FILE__) ":" STRING(__LINE__);
 */
#define __STRING_SECONDPASS(X) #X
#define STRING(X) __STRING_SECONDPASS(X)

/** Symbol Concatenator
 *  ------------------
 *  Mushes two symbols together into one at the preprocessor level.
 */
#define CONCAT_SYMBOL(a, b) CONCAT_SYMBOL_I(a, b)
#define CONCAT_SYMBOL_I(a, b) CONCAT_SYMBOL_II(~, a ## b)
#define CONCAT_SYMBOL_II(p, res) res


/** C++ 14/17 Type Trait Polyfills
 *  ==============================
 *  Helper types / struct values / functions that weren't included in C++14 or
 *  aren't yet implemented in MSVC, or aren't practical to implement in
 *  C++14/11. `namespace`d into `n2_` -- the Native 2 Junk Drawer -- because we
 *  should switch these out for the std:: implementations as soon as we can.
 */
namespace n2_ {

/** `void_t<Ts...>`
 *  ---------------
 *  C++17 feature; its definition will be simplified with that spec.
 *  Utility metafunction that maps a sequence of any types to the type `void`.
 *  This is primarily used in SFINAE idioms, ex; to remove class template
 *  overloads based on type traits (`class` used for brevity);
 *
 *     template <class, class = std::void_t<>>
 *     struct has_member_foo : std::false_type { };
 *
 *     template <class T>
 *     struct has_member_foo<T, std::void_t<class T::foo>> : std::true_type { };
 */
template <typename... Ts> struct make_void { typedef void type; };
template <typename... Ts> using void_t = typename make_void<Ts...>::type;

/** In Place Constructor Tags
 *  -------------------------
 *  Useful for disambiguating constructors that accept initializer lists and/or
 *  variadic argument packs. ex;
 *
 *      struct Foo {
 *          template <typename T> Foo(T) { . . . }
 *          template <typename... Args> Foo(Args&&...) { . . . }
 *      };
 *      Foo { 0 }; // Which gets called?

 *      struct Bar {
 *          template <typename T> Bar(T) { . . . }
 *          template <typename... Args> Bar(n2_::in_place_t, Args&&...) { . . }
 *      };
 *      Bar { 0 };           // Unambiguous.
 *      Bar { in_place, 0 }; // Unambiguous.
 *
 *  Note that in the C++17 standard, the default (0-arg) constructors are
 *  explicitly defined with `= default` and are marked `explicit`. I have _no_
 *  idea why this was done, and have chosen to not replicate that definition.
 *
 *  NB. We'd like these values to be inline variables so we can cleanly apply
 *  external linkage to them, but inline variables are a C++17 thing. Also, it
 *  doesn't really matter, because we're in the context of a mono-build.
 *  Fuck yeah, mono-builds.
 */
struct in_place_t { };
constexpr /*inline*/ in_place_t in_place{};

template <typename T> struct in_place_type_t { };
template <typename T> constexpr /*inline*/ in_place_type_t<T> in_place_type{};

template <size_t I> struct in_place_index_t { };
template <size_t I> constexpr /*inline*/ in_place_index_t<I> in_place_index{};


/** Helper Types
 *  ------------
 *  Mapping exactly to `[type_trait]<Ts..>::type`.
 */

/* NB. std::enable_if and n2_::enable_if_t default the returned type to `void`.
 *     Macros don't let me define default arguments, so we have two
 *     `ENABLE_IF_*` macros; one (`_DTYPE`) returns the dependent type, the
 *     other (`_TYPE`) allows the default to come into play. */
template <bool B, typename T = void>
using enable_if_t = typename ::std::enable_if<B,T>::type;
#define ENABLE_IF_TYPE(B) ::n2_::enable_if_t<B>
#define ENABLE_IF_DTYPE(B,T) ::n2_::enable_if_t<B,T>

template <typename T>
using remove_reference_t = typename ::std::remove_reference<T>::type;
#define REMOVE_REFERENCE_TYPE(T) ::n2_::remove_reference_t<T>

template <typename T>
using add_lvalue_reference_t = typename ::std::add_lvalue_reference<T>::type;
#define ADD_LVAL_REFERENCE_TYPE(T) ::n2_::add_lvalue_reference_t<T>

template <typename T>
using add_rvalue_reference_t = typename ::std::add_rvalue_reference<T>::type;
#define ADD_RVAL_REFERENCE_TYPE(T) ::n2_::add_rvalue_reference_t<T>

template <typename T>
using remove_const_t = typename ::std::remove_const<T>::type;
#define REMOVE_CONST_TYPE(T) ::n2_::remove_const_t<T>

template <typename T>
using add_const_t = typename ::std::add_const<T>::type;
#define ADD_CONST_TYPE(T) ::n2_::add_const_t<T>

template< class T >
using decay_t = typename ::std::decay<T>::type;
#define DECAY_TYPE(T) ::n2_::decay_t<T>


/** Helper Values
 *  -------------
 *  Mapping exactly to `[type_trait]<Ts...>::value`.
 *  NB. We're using macros rather than inline constexpr `n2_::[type_trait]_v`
 *  variables that C++17 will define because inline variables (which are
 *  required for making the `_v` helpers useful) are a C++1z extension at this
 *  point. No idea when MSVC will get them, or if they're consistently available
 *  in GCC.
 *
 *  Note; I've given up the 80-column limit for this section, because the macro
 *  names should be clear enough to demarcate intention, and one-line structure
 *  of the definitions provides more clarity than short line-lengths.
 */

/** IS_REFERENCE (and friends)
 *  --------------------------
 *  Macros wrapping std::is_reference<T>::value (and l/r value specifiers) to
 *  extract the referentiality of an object.
 *  Answers the "Is this type a reference? If so, what kind?" question.
 */
#define IS_REFERENCE(T)      (::std::is_reference<T>::value)
#define IS_NOT_REFERENCE(T) !(::std::is_reference<T>::value)

#define IS_LVAL_REFERENCE(T)      (::std::is_lvalue_reference<T>::value)
#define IS_NOT_LVAL_REFERENCE(T) !(::std::is_lvalue_reference<T>::value)

#define IS_RVAL_REFERENCE(T)      (::std::is_rvalue_reference<T>::value)
#define IS_NOT_RVAL_REFERENCE(T) !(::std::is_rvalue_reference<T>::value)

/** IS_SCALAR (and friends)
 *  -----------------------
 *  Macros wrapping std::is_scalar<T>::value, and specializations thereon.
 */
#define IS_SCALAR(T)      (std::is_scalar<T>::value)
#define IS_NOT_SCALAR(T) !(std::is_scalar<T>::value)

#define IS_ARITHMETIC(T)      (std::is_arithmetic<T>::value)
#define IS_NOT_ARITHMETIC(T) !(std::is_arithmetic<T>::value)

#define IS_ENUM(T)      (std::is_enum<T>::value)
#define IS_NOT_ENUM(T) !(std::is_enum<T>::value)

#define IS_POINTER(T)      (std::is_pointer<T>::value)
#define IS_NOT_POINTER(T) !(std::is_pointer<T>::value)

#define IS_MEMBER_POINTER(T)      (std::is_member_pointer<T>::value)
#define IS_NOT_MEMBER_POINTER(T) !(std::is_member_pointer<T>::value)

#define IS_NULL_POINTER(T)      (std::is_null_pointer<T>::value)
#define IS_NOT_NULL_POINTER(T) !(std::is_null_pointer<T>::value)

/** IS_SAME_TYPE
 *  ------------
 *  Macros wrapping std::is_same<T,U>::value, s.t. types can be compared.
 */
#define IS_SAME_TYPE(LEFT,RIGHT)       (::std::is_same<LEFT,RIGHT>::value)
#define IS_DIFFERENT_TYPE(LEFT,RIGHT) !(::std::is_same<LEFT,RIGHT>::value)

/** HAS_SAME_TYPE
 *  -------------
 *  Macros wrapping std::is_same<T,U>::value and decltype, s.t. types of
 *  instances can be compared.
 */
#define HAS_SAME_TYPE(LEFT,RIGHT)       (::std::is_same<decltype(LEFT),decltype(RIGHT)>::value)
#define HAS_DIFFERENT_TYPE(LEFT,RIGHT) !(::std::is_same<decltype(LEFT),decltype(RIGHT)>::value)

/** IS_CONVERTIBLE
 *  --------------
 *  Macros wrapping std::is_convertible<TO, FROM>::value. Useful in copy/move
 *  assignment operators and constructors.
 *  Note 'convertible' in this case means implicitly convertible. Specifically,
 *  this will be true iff the below imaginary function is well formed;
 *
 *      TO test() { return std::declval<FROM>(); }
 */
#define IS_CONVERTIBLE(FROM,TO)      (::std::is_convertible<FROM,TO>::value)
#define IS_NOT_CONVERTIBLE(FROM,TO) !(::std::is_convertible<FROM,TO>::value)

/** Initialization/Deinitialization Checks
 *  --------------------------------------
 *  Macros wrapping `std::is_[trivially_][type_trait]<T>::value`. Tells us what
 *  the given class can or cannot do, and the trivially_ checks will tell us
 *  what the compiler will automatically generate for us.
 */
#define IS_CONSTRUCTIBLE(T, ...)      (::std::is_constructible<T,__VA_ARGS__>::value)
#define IS_NOT_CONSTRUCTIBLE(T, ...) !(::std::is_constructible<T,__VA_ARGS__>::value)
#define IS_TRIVIALLY_CONSTRUCTIBLE(T, ...)      (::std::is_trivially_constructible<T,__VA_ARGS__>::value)
#define IS_NOT_TRIVIALLY_CONSTRUCTIBLE(T, ...) !(::std::is_trivially_constructible<T,__VA_ARGS__>::value)
#define IS_NOTHROW_CONSTRUCTIBLE(T, ...)      (::std::is_nothrow_constructible<T,__VA_ARGS__>::value)
#define IS_NOT_NOTHROW_CONSTRUCTIBLE(T, ...) !(::std::is_nothrow_constructible<T,__VA_ARGS__>::value)

#define IS_DEFAULT_CONSTRUCTIBLE(T)      (::std::is_default_constructible<T>::value)
#define IS_NOT_DEFAULT_CONSTRUCTIBLE(T) !(::std::is_default_constructible<T>::value)
#define IS_TRIVIALLY_DEFAULT_CONSTRUCTIBLE(T)      (::std::is_trivially_default_constructible<T>::value)
#define IS_NOT_TRIVIALLY_DEFAULT_CONSTRUCTIBLE(T) !(::std::is_trivially_default_constructible<T>::value)
#define IS_NOTHROW_DEFAULT_CONSTRUCTIBLE(T)      (::std::is_nothrow_default_constructible<T>::value)
#define IS_NOT_NOTHROW_DEFAULT_CONSTRUCTIBLE(T) !(::std::is_nothrow_default_constructible<T>::value)

#define IS_COPY_CONSTRUCTIBLE(T)      (::std::is_copy_constructible<T>::value)
#define IS_NOT_COPY_CONSTRUCTIBLE(T) !(::std::is_copy_constructible<T>::value)
#define IS_TRIVIALLY_COPY_CONSTRUCTIBLE(T)      (::std::is_trivially_copy_constructible<T>::value)
#define IS_NOT_TRIVIALLY_COPY_CONSTRUCTIBLE(T) !(::std::is_trivially_copy_constructible<T>::value)
#define IS_NOTHROW_COPY_CONSTRUCTIBLE(T)      (::std::is_nothrow_copy_constructible<T>::value)
#define IS_NOT_NOTHROW_COPY_CONSTRUCTIBLE(T) !(::std::is_nothrow_copy_constructible<T>::value)

#define IS_MOVE_CONSTRUCTIBLE(T)      (::std::is_move_constructible<T>::value)
#define IS_NOT_MOVE_CONSTRUCTIBLE(T) !(::std::is_move_constructible<T>::value)
#define IS_TRIVIALLY_MOVE_CONSTRUCTIBLE(T)      (::std::is_trivially_move_constructible<T>::value)
#define IS_NOT_TRIVIALLY_MOVE_CONSTRUCTIBLE(T) !(::std::is_trivially_move_constructible<T>::value)
#define IS_NOTHROW_MOVE_CONSTRUCTIBLE(T)      (::std::is_nothrow_move_constructible<T>::value)
#define IS_NOT_NOTHROW_MOVE_CONSTRUCTIBLE(T) !(::std::is_nothrow_move_constructible<T>::value)


#define IS_ASSIGNABLE(T,U)      (::std::is_assignable<T,U>::value)
#define IS_NOT_ASSIGNABLE(T,U) !(::std::is_assignable<T,U>::value)
#define IS_TRIVIALLY_ASSIGNABLE(T,U)      (::std::is_trivially_assignable<T,U>::value)
#define IS_NOT_TRIVIALLY_ASSIGNABLE(T,U) !(::std::is_trivially_assignable<T,U>::value)
#define IS_NOTHROW_ASSIGNABLE(T,U)      (::std::is_nothrow_assignable<T,U>::value)
#define IS_NOT_NOTHROW_ASSIGNABLE(T,U) !(::std::is_nothrow_assignable<T,U>::value)

#define IS_COPY_ASSIGNABLE(T)      (::std::is_copy_assignable<T>::value)
#define IS_NOT_COPY_ASSIGNABLE(T) !(::std::is_copy_assignable<T>::value)
#define IS_TRIVIALLY_COPY_ASSIGNABLE(T)      (::std::is_trivially_copy_assignable<T>::value)
#define IS_NOT_TRIVIALLY_COPY_ASSIGNABLE(T) !(::std::is_trivially_copy_assignable<T>::value)
#define IS_NOTHROW_COPY_ASSIGNABLE(T)      (::std::is_nothrow_copy_assignable<T>::value)
#define IS_NOT_NOTHROW_COPY_ASSIGNABLE(T) !(::std::is_nothrow_copy_assignable<T>::value)

#define IS_MOVE_ASSIGNABLE(T)      (::std::is_move_assignable<T>::value)
#define IS_NOT_MOVE_ASSIGNABLE(T) !(::std::is_move_assignable<T>::value)
#define IS_TRIVIALLY_MOVE_ASSIGNABLE(T)      (::std::is_trivially_move_assignable<T>::value)
#define IS_NOT_TRIVIALLY_MOVE_ASSIGNABLE(T) !(::std::is_trivially_move_assignable<T>::value)
#define IS_NOTHROW_MOVE_ASSIGNABLE(T)      (::std::is_nothrow_move_assignable<T>::value)
#define IS_NOT_NOTHROW_MOVE_ASSIGNABLE(T) !(::std::is_nothrow_move_assignable<T>::value)


#define IS_DESTRUCTIBLE(T)      (::std::is_destructible<T>::value)
#define IS_NOT_DESTRUCTIBLE(T) !(::std::is_destructible<T>::value)
#define IS_TRIVIALLY_DESTRUCTIBLE(T)      (::std::is_trivially_destructible<T>::value)
#define IS_NOT_TRIVIALLY_DESTRUCTIBLE(T) !(::std::is_trivially_destructible<T>::value)
#define IS_NOTHROW_DESTRUCTIBLE(T)      (::std::is_nothrow_destructible<T>::value)
#define IS_NOT_NOTHROW_DESTRUCTIBLE(T) !(::std::is_nothrow_destructible<T>::value)


/** Type Properties
 *  ---------------
 *  More macros wrapping `std::is_[type_trait}<T>::value`. This set focus on the
 *  properties of the given type, rather than supported operations. */
#define IS_TRIVIAL(T)      (::std::is_trivial<T>::value)
#define IS_NOT_TRIVIAL(T) !(::std::is_trivial<T>::value)

#define IS_TRIVIALLY_COPYABLE(T)      (::std::is_trivially_copyable<T>::value)
#define IS_NOT_TRIVIALLY_COPYABLE(T) !(::std::is_trivially_copyable<T>::value)

#define IS_STANDARD_LAYOUT(T)      (::std::is_standard_layout<T>::value)
#define IS_NOT_STANDARD_LAYOUT(T) !(::std::is_standard_layout<T>::value)

#define IS_POD(T)      (::std::is_pod<T>::value)
#define IS_NOT_POD(T) !(::std::is_pod<T>::value)

} /* namespace n2_ */


/** struct/class Type Trait Assertions
 *  ==================================
 *  Macros that cause compile-time errors when we build non-compliant datatypes.
 */

/** ENFOCE_POD
 *  ----------
 *  Verifies the given type is a POD datatype. The last assertion should be
 *  redundant, but better safe than sorry.
 */
#define ENFORCE_POD(T) \
    static_assert(IS_TRIVIALLY_COPYABLE(T), "Type '" STRING(T) "' was marked as Plain Old Data, but is not trivially copyable. Defined near [" STRING(__FILE__) ":" STRING(__LINE__) "]");                           \
    static_assert(IS_TRIVIALLY_DEFAULT_CONSTRUCTIBLE(T), "Type '" STRING(T) "' was marked as Plain Old Data, but is not trivially default constructible. Defined near [" STRING(__FILE__) ":" STRING(__LINE__) "]"); \
    static_assert(IS_STANDARD_LAYOUT(T), "Type '" STRING(T) "' was marked as Plain Old Data, but is not standard layout. Defined near [" STRING(__FILE__) ":" STRING(__LINE__) "]");                                 \
    static_assert(IS_POD(T), "Type '" STRING(T) "' was marked as Plain Old Data, but is... not. We're not sure why. Please expand the ENFORCE_POD macro. Defined near [" STRING(__FILE__) ":" STRING(__LINE__) "]")

/** ENFORCE_SIZE (and friends)
 *  --------------------------
 *  Verifies the given type is exactly / under / above / within the range of
 *  the give size.
 */
#define ENFORCE_SIZE(T, bytes) \
    static_assert(sizeof(T) == bytes, "Type '" STRING(T) "' is the wrong size (it is required to be " STRING(bytes) " bytes).")
#define ENFORCE_MAX_SIZE(T, max_bytes) \
    static_assert(sizeof(T) <= bytes, "Type '" STRING(T) "' is the wrong size (it is required to be at most " STRING(bytes) " bytes).")



/** Platform Homogenization Macros
 *  ==============================
 *  The macros below (probably) exist in one form or another on at least one
 *  major platform. They probably don't agree on the details though, so we
 *  (re)define them below to ensure a consistent feel.
 */

/** __PRETTY_FUNCTION__
 *  -------------------
 *  Use the same prettier function signature macro everywhere.
 */
#if defined(_MSC_VER)
# define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

/** Stringifcation Functions
 *  ------------------------
 *  for easy printf-ing.
 */
inline char const* const bool2string(bool b) {
    return b ? "true" : "false";
}

/** Alignment Macro
 *  ---------------
 *  Force alignment of a given instance or datatype.
 */
#if defined(_MSC_VER)
#   define ALIGNAS(X) __declspec(align(X))
#else
#   define ALIGNAS(X) alignas(X)
#endif

/** BREAKPOINT
 *  ----------
 *  Programmatically force a breakpoint on X86
 */
#ifdef _MSC_VER
# ifdef _X86_
#  define _debug_break_impl { __asm { int 3 } }
# else
#  define _debug_break_impl  { __debugbreak(); } // need <intrin.h>
# endif
#else
# include "signal.h"
# define _debug_break_impl { raise(SIGINT); } // GCC x86/64
#endif

#define BREAKPOINT() { _debug_break_impl; }

#if defined(DEBUG) || !defined(NDEBUG)
# define DEBUG_BREAKPOINT() { _debug_break_impl; }
#else
# define DEBUG_BREAKPOINT()
#endif

/** ALIGNMENT CORRECT FREE
 *  ----------------------
 *  Alias the appropriate free function for destroying underlying buffers. This
 *  utility function is necessary because the visual C runtime differentiates
 *  aligned from unaligned buffers in terms of how they're freed, unlike POSIX.
 */
inline void alignment_correct_free(void* buffer, bool aligned) {
    #if defined(_MSC_VER)
        if (aligned) {
            _aligned_free(buffer);
        } else {
            free(buffer);
        }
    #else
        free(buffer);
    #endif
}

/** FORCEINLINE
 *  -----------
 *  Forces the compiler to inline this function, if possible, otherwise simply
 *  suggests it.
 */
#if !defined(FORCEINLINE)
#  if defined(_MSC_VER)
#    define FORCEINLINE __forceinline inline
#  elif defined(__clang__)
#    define FORCEINLINE __attribute__((always_inline)) inline
#  elif defined(__GNUC__) || defined(__GNUG__)
#    define FORCEINLINE __attribute__((always_inline)) inline
#  else
#    define FORCEINLINE inline
#  endif
#endif

/** Shim for mktemp
 *  ---------------
 *  On the windows runtimes, the posix function mktemp requires some
 *  goofy shimming.
 */
#if defined(__MINGW32__) || defined(_MSC_VER)
#include <io.h>
#endif



/** General Utility Macros
 *  ======================
 *  Macros that are designed for general quality-of-life improvements.
 */

/** UNUSED()
 * --------
 * Utility macro for marking variables deliberately-unused. Nixes warnings.
 */
#define UNUSED(_var) do { (void)(true ? (void)0 : ( (void)(_var) ) ); } while(0)

/** FOURCC Construction Shorthand
 *  -----------------------------
 *  Many data file formats, especially microsoft ones, use four-character-codes
 *  to identify segments. These are represented as a 32-bit integer with the
 *  value of four adjacent ASCII characters.
 */
inline uint32_t N2FOURCC(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
    return (uint32_t)( (uint32_t(d) << 24 )
                     | (uint32_t(c) << 16 )
                     | (uint32_t(b) <<  8 )
                     | (uint32_t(a)       ));
}
inline uint32_t N2FOURCC(char const* code) {
    return (uint32_t)( (uint32_t(code[3]) << 24)
                     | (uint32_t(code[2]) << 16)
                     | (uint32_t(code[1]) <<  8)
                     | (uint32_t(code[0])      ));
}

/** C++ Default Constructor Removal
 *  -------------------------------
 *  A macro to disallow the copy constructor and operator- functions
 *  This should be used in the private: declarations for a class.
 *  Note that if there is a user-defined Copy Ctor there will be no implicitly
 *  defined Move Ctor (or Move Assignment Operator), so deleting those members
 *  is, at best, redundant.
 */
#define DISALLOW_COPY_AND_ASSIGN(TypeName)   \
    TypeName(const TypeName&) = delete;      \
    void operator=(const TypeName&) = delete

/** TEMPLATE_ENABLE
 *  ---------------
 *  std::enable_if convenience wrapper.
 *  Utility tools for easily enabling/disabling functions in templated
 *  types based on a trait or boolean expression. Usage requires passing
 *  in a parameter from the class template in the first variable of the
 *  macro. For example, if you had:
 *
 *      template<bool ConfigBoolean, class T> MyClass { ... };
 *
 *  You'd use use the macro like so:
 *
 *      TEMPLATE_ENABLE(ConfigBoolean, T) void EitherOr() { ... };
 *      TEMPLATE_ENABLE(!ConfigBoolean, T) void EitherOr() { ... };
 *
 *  Which will yield only the correct version of that function being chosen
 *  during overload resolution based on the template parameter. Poor man's
 *  type-level overloading, poorer man's pattern match.
 *  Notes on the implementation;
 *  - For this form to correctly operate using SFINAE, the `enable_if_t` needs
 *    to return a dependent type. `_DEP_T` is explicitly defined based on `T` to
 *    guarantee this.
 *    TODO: Experiment some more and guarantee this is the most robust form we
 *          can safely use... I have a feeling this invariant... isn't).
 *  - `_DEP_T` needs to be a type decayed from `T` for the cases in which T is a
 *    reference type. When SFINAE succeeds, we're defaulting the second template
 *    parameter to `_DEP_T * = nullptr`, and `T& * = nullptr` ain't so good.
 *  - Because TEMPLATE_ENABLE is defining and operating on a dependent type that
 *    is not cleanly accessible outside of the macro, users will need to be very
 *    careful with the signatures of functions they disable with this macro.
 *    Take for example the below, assuming T is... say, a reference to a u32;
 *
 *        TEMPLTE_ENABLE(IS_NOT_REFERENCE_TYPE(T), T)
 *        T * getAPointer() { ... }
 *
 *    This will fail to compile. `T` will be defined before template argument
 *    deduction occurs (the same point at which _DEP_T is defined by its
 *    default, `T`), so before the enable_if clause can remove this function
 *    from the overload set, `T *` will be resolved to `u32 & *`, which is a
 *    hard error outside of the immediate context of the function.
 */
#define TEMPLATE_ENABLE(COND, T)            \
    template<typename _DEP_T=DECAY_TYPE(T), \
             typename ENABLE_IF_DTYPE(COND,_DEP_T) * = nullptr>


/** Constexpr Type-Name Printing
 *  ----------------------------
 *  Heavily inspired by http://stackoverflow.com/questions/35941045.
 *  Designed to work with the `Ftype` format string macro.
 *  Example;
 *      printf("My type name is: \"" Ftype "\"\n", PRINT_TYPE_NAME(MyType));
 *
 *  This function works by capturing a slice of the `__PRETTY_FUNCTION__` string
 *  literal, and figuring out the number of characters worth printing. The
 *  `Ftype` format string ('%.*s') consumes two arguments; first a maximum
 *  number of characters to print, then the `char const *` to pull from. We use
 *  ::length followed my ::name to fill in those arguments.
 *
 *  NB. This only works in GCC and Clang because MSVC hasn't implemented C++14's
 *  constexpr extensions, and we can't ex; `char const * p  = __FUNCSIG__;`.
 *  TODO: Double check if this is true as of the bump to C++17
 */

#if defined(_MSC_VER)

#  define PRINT_TYPE_NAME(TYPE)    36, "[[Sorry, no type name from MSVC :C]]"
#  define PRINT_TYPE_NAME_OF(TYPE) 36, "[[Sorry, no type name from MSVC :C]]"

#else

#  define PRINT_TYPE_NAME(TYPE) N2TypeName<TYPE>::length, N2TypeName<TYPE>::name
#  define PRINT_TYPE_NAME_OF(TYPE) PRINT_TYPE_NAME(decltype(TYPE))

template<typename Type>
struct N2TypeName {
    static constexpr char const * _getName() {
        char const * p  = __PRETTY_FUNCTION__;
        while (*(++p) != '='); // Skip p to the first `=`.
        while (*(++p) == ' '); // Skip p past any spaces.
        return p;
    }

    static constexpr int32_t const _getLength() {
        char const * p  = _getName();
        char const * p2 = p;
        int count       = 1;
        // In _getName, we will have skipped past the first `[` (which is why
        // count == 1). We now increment p2 until the matching `]` is found.
        while (count > 0) {
            ++p2;
            switch (*p2) {
            case '[': { ++count; } break;
            case ']': { --count; } break;
            }
        }
        return (int32_t)(p2 - p);
    }

    // Making these `static constexpr` members ensures compile-time resolution.
    static constexpr char const *  name   = _getName();
    static constexpr int32_t const length = _getLength();
};

#endif


/** Value Category Checker
 *  ======================
 *  Like it says on the tin.
 *  Taken nearly directly from http://stackoverflow.com/questions/16637945
 */
template<typename T> struct value_category {
    static constexpr auto value = "prvalue";
};

template<typename T> struct value_category<T&> {
    static constexpr auto value = "lvalue";
};

template<typename T> struct value_category<T&&> {
    static constexpr auto value = "xvalue";
};

// Double parens for ensuring we inspect an expression, not an entity
#define VALUE_CATEGORY(expr) value_category<decltype((expr))>::value


/** Special Member Enablers/Disablers
 *  =================================
 *  Beware. There be Template Magic Here.
 *  These helpers allow us selectively disable a parent classes' special member
 *  functions (Copy Constructor, Copy Assignment, Move Constructor, and Move
 *  Assignment) based on template type traits or constexpr boolean checks. The
 *  idiom to use is as follows;
 *
 *  - Let's say we have a wrapper class `<template T> class Foo`, and we want
 *    the assignment operators to not be defined unless `T` is
 *    trivially assignable.
 *  - Rename `Foo` to `_Foo_Base` [TODO: Come up wit a better name].
 *  - Create a new `Foo` using the below definition pattern;
 *        template <typename T>
 *        class Foo
 *              : _Foo_Base<T>
 *              , n2_::_EnableCopyAssign<IS_TRIVIALLY_ASSIGNABLE(T), T>
 *              , n2_::_EnableMoveAssign<IS_TRIVIALLY_ASSIGNABLE(T), T> {
 *            using _Foo_Base::_Foo_Base;
 *        };
 *    NB:
 *    - We allow `Foo`s special members to all be implicitly defined.
 *      This causes any invocation of `Foo& operator=` to call into both
 *      `_Foo_Base& operator=` and `_EnableCopyAssign& operator=`. The later
 *      does nothing, but if `IS_TRIVIALLY_ASSIGNABLE(T)` is false,
 *      `_EnableCopyAssign& operator=` will be explicitly deleted, causing a
 *      compilation error if `Foo& operator=` is ever misused.
 *    - We use `using _Foo_Base::_Foo_Base` to inherit all of `_Foo_Base`'s
 *      valid constructors, s.t. all explicit constructors defined by
 *      `_Foo_Base` will be available to `Foo`.
 *      TODO: Find out if `using` can be used to (cleanly) pull all of
 *            `_Foo_Base`'s member functions in
 */
namespace n2_ {

template <bool EnableCopyCtor, typename UniqueTag>
struct _EnableCopyCtor {
    constexpr _EnableCopyCtor() noexcept                              = default;
    constexpr _EnableCopyCtor(const _EnableCopyCtor&) noexcept        = default;
    _EnableCopyCtor& operator=(const _EnableCopyCtor&) noexcept       = default;
    constexpr _EnableCopyCtor(_EnableCopyCtor &&) noexcept            = default;
    _EnableCopyCtor& operator=(_EnableCopyCtor &&) noexcept           = default;
};
template <typename UniqueTag>
struct _EnableCopyCtor<false, UniqueTag> {
    constexpr _EnableCopyCtor() noexcept                              = default;
    constexpr _EnableCopyCtor(const _EnableCopyCtor&) noexcept        = delete;
    _EnableCopyCtor& operator=(const _EnableCopyCtor&) noexcept       = default;
    constexpr _EnableCopyCtor(_EnableCopyCtor &&) noexcept            = default;
    _EnableCopyCtor& operator=(_EnableCopyCtor &&) noexcept           = default;
};

template <bool EnableCopyAssign, typename UniqueTag>
struct _EnableCopyAssign {
    constexpr _EnableCopyAssign() noexcept                            = default;
    constexpr _EnableCopyAssign(const _EnableCopyAssign&) noexcept    = default;
    _EnableCopyAssign& operator=(const _EnableCopyAssign&) noexcept   = default;
    constexpr _EnableCopyAssign(_EnableCopyAssign &&) noexcept        = default;
    _EnableCopyAssign& operator=(_EnableCopyAssign &&) noexcept       = default;
};
template <typename UniqueTag>
struct _EnableCopyAssign<false, UniqueTag> {
    constexpr _EnableCopyAssign() noexcept                            = default;
    constexpr _EnableCopyAssign(const _EnableCopyAssign&) noexcept    = default;
    _EnableCopyAssign& operator=(const _EnableCopyAssign&) noexcept   = delete;
    constexpr _EnableCopyAssign(_EnableCopyAssign &&) noexcept        = default;
    _EnableCopyAssign& operator=(_EnableCopyAssign &&) noexcept       = default;
};

template <bool EnableMoveCtor, typename UniqueTag>
struct _EnableMoveCtor {
    constexpr _EnableMoveCtor() noexcept                              = default;
    constexpr _EnableMoveCtor(const _EnableMoveCtor&) noexcept        = default;
    _EnableMoveCtor& operator=(const _EnableMoveCtor&) noexcept       = default;
    constexpr _EnableMoveCtor(_EnableMoveCtor &&) noexcept            = default;
    _EnableMoveCtor& operator=(_EnableMoveCtor &&) noexcept           = default;
};
template <typename UniqueTag>
struct _EnableMoveCtor<false, UniqueTag> {
    constexpr _EnableMoveCtor() noexcept                              = default;
    constexpr _EnableMoveCtor(const _EnableMoveCtor&) noexcept        = default;
    _EnableMoveCtor& operator=(const _EnableMoveCtor&) noexcept       = default;
    constexpr _EnableMoveCtor(_EnableMoveCtor &&) noexcept            = delete;
    _EnableMoveCtor& operator=(_EnableMoveCtor &&) noexcept           = default;
};

template <bool EnableMoveAssign, typename UniqueTag>
struct _EnableMoveAssign {
    constexpr _EnableMoveAssign() noexcept                            = default;
    constexpr _EnableMoveAssign(const _EnableMoveAssign&) noexcept    = default;
    _EnableMoveAssign& operator=(const _EnableMoveAssign&) noexcept   = default;
    constexpr _EnableMoveAssign(_EnableMoveAssign &&) noexcept        = default;
    _EnableMoveAssign& operator=(_EnableMoveAssign &&) noexcept       = default;
};
template <typename UniqueTag>
struct _EnableMoveAssign<false, UniqueTag> {
    constexpr _EnableMoveAssign() noexcept                            = default;
    constexpr _EnableMoveAssign(const _EnableMoveAssign&) noexcept    = default;
    _EnableMoveAssign& operator=(const _EnableMoveAssign&) noexcept   = default;
    constexpr _EnableMoveAssign(_EnableMoveAssign &&) noexcept        = default;
    _EnableMoveAssign& operator=(_EnableMoveAssign &&) noexcept       = delete;
};

} /* namespace n2_ */
