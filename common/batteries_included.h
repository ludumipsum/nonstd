/* C Standard Library Includes
   ===========================

   This file contains all C standard library includes we use. It is the first
   file included in common.h, and provides these symbols to everything else in
   the project.

   It is also the home for a collection of macros and utility functions which
   make C++ more comfortable to work in and closer to homogenous across our
   target platforms. If you would have put something in `util.h` in the old
   world, there's a good chance it should live here.

   All files under common/ should also include this header.
   */

#pragma once

/* # C Standard Library Includes */
#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* # C++ Standard Library Includes */
#include <functional>
#include <string>
#include <unordered_map>


/* Primitive Type Definitions
   ==========================

   Macros, typedefs, and structs that will be used as this project's most basic
   types. Builds off of e.g. stdint -- modified for our personal style -- and
   builds types useful for a vidja garm.
*/


/* Symbol Stringifyer
   ------------------
   Uses the preprocessor to create a static string version of the passed symbol
   or macro. Usage:
       char const* msg = "This is a string literal defined at "
                         STRING(__FILE__) ":" STRING(__LINE);
*/
#define __STRING_SECONDPASS(X) #X
#define STRING(X) __STRING_SECONDPASS(X)


/* struct/class type_traits Assertions
   -----------------------------------
   A macro to cause compile-time failures when we incorrectly build non-POD
   datatypes.
 */
#if defined(_MSC_VER)
# define ENFORCE_POD(T) \
    static_assert(::std::is_trivially_copyable<T>::value, "Type \"" STRING(T) "\" was marked as Plain Old Data, but is not trivially copyable. Defined near [" STRING(__FILE__) ":" STRING(__LINE__) "]"); \
    static_assert(::std::is_trivially_default_constructible<T>::value, "Type \"" STRING(T) "\" was marked as Plain Old Data, but is not trivially default constructible. Defined near [" STRING(__FILE__) ":" STRING(__LINE__) "]"); \
    static_assert(::std::is_standard_layout<T>::value, "Type \"" STRING(T) "\" was marked as Plain Old Data, but is not standard layout. Defined near [" STRING(__FILE__) ":" STRING(__LINE__) "]")
#else
# define ENFORCE_POD(T) \
    static_assert(std::is_trivial<T>::value, "Type \"" STRING(T) "\" was marked as Plain Old Data, but is not trivial. Defined near [" STRING(__FILE__) ":" STRING(__LINE__) "]"); \
    static_assert(std::is_standard_layout<T>::value, "Type \"" STRING(T) "\" was marked as Plain Old Data, but is not standard layout. Defined near [" STRING(__FILE__) ":" STRING(__LINE__) "]")
#endif



/* Platform Homogenization Macros
   ==============================

   The macros below (probably) exist in one form or another on at least one
   major platform. They probably don't agree on the details though, so we
   (re)define them below to ensure a consistent feel.
*/


/* __PRETTY_FUNCTION__
   -------------------
   Use the same prettier function signature macro everywhere.
*/
#if defined(_MSC_VER)
# define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

/* Stringifcation Functions
   ------------------------
   for easy printf-ing.
*/
inline char const* const bool2string(bool b) {
    return b ? "true" : "false";
};

/* Alignment Macro
   ---------------
   Force alignment of a given instance or datatype.
*/
#ifndef alignof
#   define alignof(x) __alignof(x)
#endif
#if defined(_MSC_VER)
#   define ALIGNAS(X) __declspec(align(X))
#else
#   define ALIGNAS(X) alignas(X)
#endif

/* BREAKPOINT
   ----------
   Programmatically force a breakpoint on X86
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

#define BREAKPOINT { _debug_break_impl; }

/* ALIGNMENT CORRECT FREE
   ----------------------
   Alias the appropriate free function for destroying underlying buffers. This
   utility function is necessary because the visual C runtime differentiates
   aligned from unaligned buffers in terms of how they're freed, unlike POSIX.
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

/* Min/Max Macros
   --------------
   Both type- and compiler-safe.
*/
#ifndef n2max
#define n2max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef n2min
#define n2min(a,b) (((a) < (b)) ? (a) : (b))
#endif




/* General Utility Macros
   ----------------------

   Macros that are designed for general quality-of-life improvements.
*/


/* UNUSED()
   --------
   Utility macro for marking variables deliberately-unused. Nixes warnings.
*/
#define UNUSED(_var) do { (void)(true ? (void)0 : ( (void)(_var) ) ); } while(0)

/* FOURCC Construction Shorthand
   -----------------------------
   Many data file formats, especially microsoft ones, use four-character-codes
   to identify segments. These are represented as a 32-bit integer with the
   value of four adjacent ASCII characters.
   NOTE: FOURCC is also defined in the winapi used in our testing framework, so
         we need to wrap it in the `!defined(N2_TEST)`.
   TODO: Consider if there's a better way to do this? Or simply removing the
         functions, as it's not used anywhere in the code.
*/
#if !defined(N2_TEST)
inline uint32_t FOURCC(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
    return (uint32_t)( (uint32_t(d) << 24 )
                     | (uint32_t(c) << 16 )
                     | (uint32_t(b) <<  8 )
                     | (uint32_t(a)       ));
}
inline uint32_t FOURCC(char const* code) {
    return (uint32_t)( (uint32_t(code[3]) << 24)
                     | (uint32_t(code[2]) << 16)
                     | (uint32_t(code[1]) <<  8)
                     | (uint32_t(code[0])      ));
}
#endif

/* C++ Default Constructor Removal
   -------------------------------
   A macro to disallow the copy constructor and operator- functions
   This should be used in the private: declarations for a class
*/
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);             \
    void operator=(const TypeName&)



/* TEMPLATE_ENABLE
   ---------------

   std::enable_if convenience wrapper.
   Utility tools for easily enabling/disabling functions in templated
   types based on a trait or boolean expression. Usage requires passing
   in a parameter from the class template in the first variable of the
   macro. For example, if you had:

       template<bool ConfigBoolean, class T> MyClass { ... };

   You'd use use the macro like so:

       TEMPLATE_ENABLE(ConfigBoolean, T) void EitherOr() { ... };
       TEMPLATE_ENABLE(!ConfigBoolean, T) void EitherOr() { ... };

   Which will yield only the correct version of that function based
   on the template parameter. Poor man's type-level overloading, poorer
   man's pattern match.
*/
template< bool B, class T = void >
using enable_if_t = typename std::enable_if<B,T>::type;
#define TEMPLATE_ENABLE(Cond, T)                        \
    template<typename _TEMPLATE_ENABLE_DEPENDENCY_=T,   \
    enable_if_t<Cond, _TEMPLATE_ENABLE_DEPENDENCY_>* =nullptr>