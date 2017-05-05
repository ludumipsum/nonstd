/* C Standard Library Includes
 * ===========================
 * This file contains all C standard library includes we use. It is the first
 * file included in common.h, and provides these symbols to everything else in
 * the project.
 *
 * It is also the home for a collection of macros and utility functions which
 * make C++ more comfortable to work in and closer to homogenous across our
 * target platforms. If you would have put something in `util.h` in the old
 * world, there's a good chance it should live here.
 *
 * All files under common/ should also include this header.
 */

#pragma once

/* If we're not explicitly in a debug build, make sure NDEBUG is defined */
#if !defined(DEBUG) && !defined(_DEBUG)
# define NDEBUG
#endif

/* # C Standard Library Includes */
#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* # C++ Standard Library Includes */
#include <functional>
#include <type_traits>
#include <string>
#include <unordered_map>


/* Platform specific includes */
#if defined(_MSC_VER) || defined(__MINGW32__)
#include <malloc.h>     // alloca
#else
#include <alloca.h>     // alloca
#endif

/* C++ 14/17 Futures
 * =================
 * Helper types / functions that weren't included in C++14 or aren't yet
 * implemented in MSVC. `namespace`d as Native 2 Futures, because as soon as we
 * can, we should switch these out for std:: implementations.
 */
namespace n2f {

/* `void_t<Ts...>`
 * ---------------
 * C++17 feature; its definition will be simplified with that spec.
 * Utility metafunction that maps a sequence of any types to the type `void`.
 * This is primarily used in SFINAE idioms, ex; to remove class template
 * overloads based on type traits (`class` used for brevity);
 *
 *     template <class, class = std::void_t<>>
 *     struct has_member_foo : std::false_type { };
 *
 *     template <class T>
 *     struct has_member_foo<T, std::void_t<class T::foo>> : std::true_type { };
 */
template <typename... Ts> struct make_void { typedef void type; };
template <typename... Ts> using void_t = typename make_void<Ts...>::type;

/* Helper Types
 * ------------
 * Mapping exactly to `[type_trait]<Ts..>::type`.
 */

template <bool B, typename T = void>
using enable_if_t = typename ::std::enable_if<B,T>::type;
#define ENABLE_IF(B, T) ::n2f::enable_if_t<B,T>

template <typename T>
using remove_reference_t = typename ::std::remove_reference<T>::type;
#define REMOVE_REFERENCE_TYPE(T) ::n2f::remove_reference_t<T>

template <typename T>
using add_lvalue_reference_t = typename ::std::add_lvalue_reference<T>::type;
#define ADD_REFERENCE_TYPE(T) ::n2f::add_lvalue_reference_t<T>

template <typename T>
using add_rvalue_reference_t = typename ::std::add_rvalue_reference<T>::type;
#define ADD_RVAL_REFERENCE_TYPE(T) ::n2f::add_rvalue_reference_t<T>

template< class T >
using decay_t = typename ::std::decay<T>::type;
#define DECAY_TYPE(T) ::n2f::decay_t<T>


/* Helper Values
 * -------------
 * Mapping exactly to `[type_trait]<Ts...>::value`.
 * NB. We're using macros rather than inline constexpr `n2f::[type_trait]_v`
 * variables that C++17 will define because inline variables (which are required
 * for making the `_v` helpers useful) are a C++1z extension at this point. No
 * idea when MSVC will get them, or if they're consistently available in GCC.
 */

/* IS_REFERENCE
 * ------------
 * Macro wrapping std::is_reference<T>::value to extract the referentiality
 * of an object. Answers the "Is this type a reference?" question.
 */
#define IS_REFERENCE_TYPE(T)      (::std::is_reference<T>::value)
#define IS_NOT_REFERENCE_TYPE(T) !(::std::is_reference<T>::value)

/* IS_SAME_TYPE
 * ------------
 * Macro wrapping std::is_same<T,U>::value, s.t. types can be compared.
 */
#define IS_SAME_TYPE(LEFT,RIGHT)       (::std::is_same<LEFT,RIGHT>::value)
#define IS_DIFFERENT_TYPE(LEFT,RIGHT) !(::std::is_same<LEFT,RIGHT>::value)

/* HAS_SAME_TYPE
 * -------------
 * Macro wrapping std::is_same<T,U>::value and decltype, s.t. types of
 * instances can be compared.
 */
#define HAS_SAME_TYPE(LEFT,RIGHT)      \
     (::std::is_same<decltype(LEFT),decltype(RIGHT)>::value)
#define HAS_DIFFERENT_TYPE(LEFT,RIGHT) \
    !(::std::is_same<decltype(LEFT),decltype(RIGHT)>::value)

/* IS_CONVERTIBLE
 * --------------
 * Macro wrapping std::is_convertible<TO, FROM>::value. Useful in copy/move
 * assignment operators and constructors.
 * Note 'convertible' in this case means implicitly convertible. Specifically,
 * this will be true iff the below imaginary function is well formed;
 *
 *     TO test() { return std::declval<FROM>(); }
 */
#define IS_CONVERTIBLE(FROM,TO)      (::std::is_convertible<FROM,TO>::value)
#define IS_NOT_CONVERTIBLE(FROM,TO) !(::std::is_convertible<FROM,TO>::value)

} /* namespace n2f */


/* Primitive Type Definitions
 * ==========================
 * Macros, typedefs, and structs that will be used as this project's most basic
 * types. Builds off of e.g. stdint -- modified for our personal style -- and
 * builds types useful for a vidja garm.
 */

/* Symbol Stringifyer
 * ------------------
 * Uses the preprocessor to create a static string version of the passed symbol
 * or macro. Usage:
 *     char const* msg = "This is a string literal defined at "
 *                       STRING(__FILE__) ":" STRING(__LINE__);
 */
#define __STRING_SECONDPASS(X) #X
#define STRING(X) __STRING_SECONDPASS(X)

/* Symbol Concatenator
 * ------------------
 * Mushes two symbols together into one at the preprocessor level.
 */
#define CONCAT_SYMBOL(a, b) CONCAT_SYMBOL_I(a, b)
#define CONCAT_SYMBOL_I(a, b) CONCAT_SYMBOL_II(~, a ## b)
#define CONCAT_SYMBOL_II(p, res) res

/* struct/class type_traits Assertions
 * -----------------------------------
 * A macro to cause compile-time failures when we incorrectly build non-POD
 * datatypes.
 */
#define ENFORCE_POD(T) \
    static_assert(::std::is_trivially_copyable<T>::value, "Type '" STRING(T) "' was marked as Plain Old Data, but is not trivially copyable. Defined near [" STRING(__FILE__) ":" STRING(__LINE__) "]"); \
    static_assert(::std::is_trivially_default_constructible<T>::value, "Type '" STRING(T) "' was marked as Plain Old Data, but is not trivially default constructible. Defined near [" STRING(__FILE__) ":" STRING(__LINE__) "]"); \
    static_assert(::std::is_standard_layout<T>::value, "Type '" STRING(T) "' was marked as Plain Old Data, but is not standard layout. Defined near [" STRING(__FILE__) ":" STRING(__LINE__) "]")

#define ENFORCE_SIZE(T, bytes) \
    static_assert(sizeof(T) == bytes, "Type '" STRING(T) "' is the wrong size (it is required to be " STRING(bytes) " bytes).")
#define ENFORCE_MAX_SIZE(T, max_bytes) \
    static_assert(sizeof(T) <= bytes, "Type '" STRING(T) "' is the wrong size (it is required to be at most " STRING(bytes) " bytes).")


/* Platform Homogenization Macros
 * ==============================
 * The macros below (probably) exist in one form or another on at least one
 * major platform. They probably don't agree on the details though, so we
 * (re)define them below to ensure a consistent feel.
 */

/* __PRETTY_FUNCTION__
 * -------------------
 * Use the same prettier function signature macro everywhere.
 */
#if defined(_MSC_VER)
# define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

/* Stringifcation Functions
 * ------------------------
 * for easy printf-ing.
 */
inline char const* const bool2string(bool b) {
    return b ? "true" : "false";
};

/* Alignment Macro
 * ---------------
 * Force alignment of a given instance or datatype.
 */
#if defined(_MSC_VER)
#   define ALIGNAS(X) __declspec(align(X))
#else
#   define ALIGNAS(X) alignas(X)
#endif

/* BREAKPOINT
 * ----------
 * Programmatically force a breakpoint on X86
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

/* ALIGNMENT CORRECT FREE
 * ----------------------
 * Alias the appropriate free function for destroying underlying buffers. This
 * utility function is necessary because the visual C runtime differentiates
 * aligned from unaligned buffers in terms of how they're freed, unlike POSIX.
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

/* FORCEINLINE
 * -----------
 * Forces the compiler to inline this function, if possible, otherwise simply
 * suggests it.
 */
#if defined(_MSC_VER)
#  define FORCEINLINE __forceinline inline
#elif defined(__clang__)
#  define FORCEINLINE __attribute__((always_inline)) inline
#elif defined(__GNUC__) || defined(__GNUG__)
#  define FORCEINLINE __attribute__((always_inline)) inline
#else
#  define FORCEINLINE inline
#endif

/* Shim for mktemp
 * ---------------
 * On the windows runtimes, the posix function mktemp requires some
 * goofy shimming.
 */
#if defined(__MINGW32__) || defined(_MSC_VER)
#include <io.h>
#endif



/* General Utility Macros
 * ======================
 * Macros that are designed for general quality-of-life improvements.
 */

/* UNUSED()
 * --------
 * Utility macro for marking variables deliberately-unused. Nixes warnings.
 */
#define UNUSED(_var) do { (void)(true ? (void)0 : ( (void)(_var) ) ); } while(0)

/* FOURCC Construction Shorthand
 * -----------------------------
 * Many data file formats, especially microsoft ones, use four-character-codes
 * to identify segments. These are represented as a 32-bit integer with the
 * value of four adjacent ASCII characters.
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

/* C++ Default Constructor Removal
 * -------------------------------
 * A macro to disallow the copy constructor and operator- functions
 * This should be used in the private: declarations for a class.
 * Note that if there is a user-defined Copy Ctor there will be no implicitly
 * defined Move Ctor (or Move Assignment Operator), so deleting those members
 * is, at best, redundant.
 */
#define DISALLOW_COPY_AND_ASSIGN(TypeName)   \
    TypeName(const TypeName&) = delete;      \
    void operator=(const TypeName&) = delete

/* TEMPLATE_ENABLE
 * ---------------
 * std::enable_if convenience wrapper.
 * Utility tools for easily enabling/disabling functions in templated
 * types based on a trait or boolean expression. Usage requires passing
 * in a parameter from the class template in the first variable of the
 * macro. For example, if you had:
 *
 *     template<bool ConfigBoolean, class T> MyClass { ... };
 *
 * You'd use use the macro like so:
 *
 *     TEMPLATE_ENABLE(ConfigBoolean, T) void EitherOr() { ... };
 *     TEMPLATE_ENABLE(!ConfigBoolean, T) void EitherOr() { ... };
 *
 * Which will yield only the correct version of that function being chosen
 * during overload resolution based on the template parameter. Poor man's
 * type-level overloading, poorer man's pattern match.
 * Notes on the implementation;
 * - For this form to correctly operate using SFINAE, the `enable_if_t` needs to
 *   return a dependent type. `_DEP_T` is explicitly defined based on `T` to
 *   guarantee this. (TODO: Experiment some more and guarantee this is the most
 *   robust form we can safely use... I have a feeling this invariant... isn't).
 * - `_DEP_T` needs to be a type decayed from `T` for the cases in which T is a
 *   reference type. When SFINAE succeeds, we're defaulting the second template
 *   parameter to `_DEP_T * = nullptr`, and `T& * = nullptr` ain't so good.
 * - Because TEMPLATE_ENABLE is defining and operating on a dependent type that
 *   is not cleanly accessible outside of the macro, users will need to be very
 *   careful with the signatures of functions they disable with this macro. Take
 *   for example the below, assuming T is... say, a reference to a u32;
 *
 *       TEMPLTE_ENABLE(IS_NOT_REFERENCE_TYPE(T), T)
 *       T * getAPointer() { ... }
 *
 *   This will fail to compile. `T` will be defined before template argument
 *   deduction occurs (the same point at which _DEP_T is defined by its default,
 *   `T`), so before the enable_if clause can remove this function from the
 *   overload set, `T *` will be resolved to `u32 & *`, which is a hard error
 *   outside of the immediate context of the function.
 */
#define TEMPLATE_ENABLE(COND, T)            \
    template<typename _DEP_T=DECAY_TYPE(T), \
             typename ::n2f::enable_if_t<COND,_DEP_T> * = nullptr>


/* Constexpr Type-Name Printing
 * ----------------------------
 * Heavily inspired by http://stackoverflow.com/questions/35941045.
 * Designed to work with the `Ftype` format string macro.
 * Example;
 *     printf("My type name is: \"" Ftype "\"\n", PRINT_TYPE_NAME(MyType));
 *
 * This function works by capturing a slice of the `__PRETTY_FUNCTION__` string
 * literal, and figuring out the number of characters worth printing. The
 * `Ftype` format string ('%.*s') consumes two arguments; first a maximum number
 * of characters to print, then the `char const *` to pull from. We use ::length
 * followed my ::name to fill in those arguments.
 *
 * NB. This only works in GCC and Clang because MSVC hasn't implemented C++14's
 * constexpr extensions, and we can't ex; `char const * p  = __FUNCSIG__;`. Boo.
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


/* Value Category Checker
 * ======================
 * Like it says on the tin.
 * Taken nearly directly from http://stackoverflow.com/questions/16637945
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
