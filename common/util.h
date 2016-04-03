/* UTILITY MACROS & FUNCTIONS
   ==========================

   Universally included header that provides a foundational set of functionality
   to both the Platform- and Game- portions of N2. The provided functionality
   includes standard library includes, homogenization of *nix and MSDN compiler
   macros/functions, logging logic, etc., etc..

   This file is designed to be the single basic header that needs to be included
   in every other file in this project. As such all headers located inside the
   util/ directory will be included in the last section of this file.
*/

#pragma once

/* C Standard Library Components
   =============================
*/
//TODO: At some point, we're going to need to move through our code, get a firm
//      understanding of our include graph, and clean it up.
//      We probably want to keep most-to-all of the system includes here.
#include <stdint.h>
#include <limits.h>
#include <string>
#include <functional>
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unordered_map>



/* Primitive Type Definitions
   ==========================

   Macros, typedefs, and structs that will be used as this project's most basic
   types. Builds off of e.g. stdint -- modified for our personal style -- and
   builds types useful for a vidja garm.
*/


/* Symbol Stringifyer
   ==================
   Uses the preprocessor to create a static string version of the passed symbol
   or macro. Usage:
       char const* msg = "This is a string literal defined at "
                         STRING(__FILE__) ":" STRING(__LINE);
*/
#define __STRING_SECONDPASS(X) #X
#define STRING(X) __STRING_SECONDPASS(X)


/* struct/class type_traits Assertions
   ===================================
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


#include "util/primitive_types.h"



/* Platform Homogenization Macros
   ==============================

   The macros below (probably) exist in one form or another on at least one
   major platform. They probably don't agree on the details though, so we
   (re)define them below to ensure a consistent feel.
*/


/* __PRETTY_FUNCTION__
   ===============================
   Use the same prettier function signature macro everywhere.
*/
#if defined(_MSC_VER)
# define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

/* Alignment Macro
   ===============
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
   ==========
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
   ======================
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
   ==============
   Both type- and compiler-safe.
*/
#ifndef n2max
#define n2max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef n2min
#define n2min(a,b) (((a) < (b)) ? (a) : (b))
#endif




/* General Utility Macros
   ======================

   Macros that are designed for general quality-of-life improvements.
*/


/* UNUSED()
   ========
   Utility macro for marking variables deliberately-unused. Nixes warnings.
*/
#define UNUSED(_var) do { (void)(true ? (void)0 : ( (void)(_var) ) ); } while(0)

/* FOURCC Construction Shorthand
   =============================
   Many data file formats, especially microsoft ones, use four-character-codes
   to identify segments. These are represented as a 32-bit integer with the
   value of four adjacent ASCII characters.
   NOTE: FOURCC is also defined in the winapi used in our testing framework, so
         we need to wrap it in the `!defined(N2_TEST)`.
   TODO: Consider if there's a better way to do this?
*/
#if !defined(N2_TEST)
inline u32 FOURCC(u8 a, u8 b, u8 c, u8 d) {
  return ( (u32) (((d)<<24) | (u32(c)<<16) | (u32(b)<<8) | u32(a)) );
}
inline u32 FOURCC(char const* code) {
  return ( (u32) (((code[3])<<24)
             | (u32(code[2])<<16)
             | (u32(code[1])<<8)
             |  u32(code[0])) );
}
#endif

/* C++ Default Constructor Removal
   ===============================
   A macro to disallow the copy constructor and operator= functions
   This should be used in the private: declarations for a class
*/
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);             \
    void operator=(const TypeName&)



/* TEMPLATE_ENABLE
   ===============

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



/* Hashing Logic
   =============

   Sha1, CRC, and DJB2.
*/

/* Sha1 gets its own file. */
#include "util/sha1.h"

/* DJB2 Hash -- Simple bytestring to 64bit integer hash */
inline u64 djb2(char const* str) {
  u64 hash = 5381;
  i32 c;
  while ((c=*str++))
      hash = ((hash << 5) + hash) + c;
  return hash;
};

/* Define HASH as something useful. */
//TODO: provide some method of choosing which hashing method to use.
#define HASH djb2



/*Text Processing Utilities
  =========================

  Utility functions and macros for logging, variadic printf-expansion, and
  similar chores.
*/

//TODO: Move this... somewhere more correct? Maybe?
#if defined(_MSC_VER)
    static const char path_delimiter = '\\';
#else
    static const char path_delimiter = '/';
#endif


/* Variadic Message Format Expander
   ================================
  _variadicExpand allows the `MESSAGE` parameter that's passed into `LOG` take
  the form of a format string.
  Modified from Erik Aronesty's answer of this SO question:
  http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
  but also: http://stackoverflow.com/questions/7825648/vscprintf-equivalent-on-android/
*/
std::string _variadicExpand(char const* message, ...)
{
    va_list format_args;

    va_start(format_args, message);
    u32 size = vsnprintf(NULL, 0, message, format_args);
    va_end(format_args);

    std::string expanded;
    expanded.resize(size + 1);

    va_start(format_args, message);
    vsnprintf(const_cast<char*>(expanded.c_str()), size + 1, message, format_args);
    va_end(format_args);

    return expanded;
}

/* Logging Implementation
   ======================
*/
i32 _logMessage(char const* message,
                char const* file,
                i32  const  line,
                char const* function)
{
    char* filename = (char*)strrchr(file, path_delimiter);
    if (filename==NULL) {
        return printf("%s:%d %s -- %s\n",
                      file, line,
                      function, message);
    }
    return printf("%s:%d %s -- %s\n",
                  filename+1, line,
                  function, message);
}

/* Logging Macro Shorthand
   =======================
*/
#define LOG(MESSAGE, ...) \
    _logMessage(::_variadicExpand(MESSAGE, ##__VA_ARGS__).c_str(), \
                __FILE__, \
                __LINE__, \
                __FUNCTION__)


/* Stringifcation Functions
   ========================
   for easy printf-ing.
*/
inline char const* const bool2string(bool b) {
  return b ? "true" : "false";
};



/* SCOPE TIMER
   ===========

   Tiny utility for timing segments of code. Given a now function which reports
   nanoseconds, this defaults to microseconds.
*/
class ScopeTimer {
public:
    u64 start, unit_divisor;
    u64 &target;
    u64 (*now)(void);

    ScopeTimer(u64& target, u64 (*now)(void), u64 unit_divisor = NS_PER_US)
              : start        ( now()        )
              , unit_divisor ( unit_divisor )
              , target       ( target       )
              , now          ( now          ) { }

  ~ScopeTimer() { target = (now() - start) / unit_divisor; }
  ScopeTimer(ScopeTimer const&) = delete;
  ScopeTimer& operator=(ScopeTimer const&) = delete;
  ScopeTimer(ScopeTimer&&) = delete;
  ScopeTimer& operator=(ScopeTimer&&) = delete;
};

// Easymode macro for the above. Designed to be used when opening a scope. Ex:
//      /* Handle incoming inputs */
//      { TIME_SCOPE_US(step_stat.input_poll_usec, state.functions.now);
//          handleInput(state);
//      }
#define TIME_SCOPE(target, now_function) ScopeTimer st(target, now_function)
#define TIME_SCOPE_NS(target, now_function) ScopeTimer st(target, now_function, 1)
#define TIME_SCOPE_US(target, now_function) ScopeTimer st(target, now_function, NS_PER_US)
#define TIME_SCOPE_MS(target, now_function) ScopeTimer st(target, now_function, NS_PER_MS)
#define TIME_SCOPE_SEC(target, now_function) ScopeTimer st(target, now_function, NS_PER_SEC)



/* UTLITY SUB-HEADER INCLUDES
   ==========================

   All files located under the util/ directory should be include here to allow
   for this file's universal, single-header-include pattern.
*/

//FIXME: We're including shit from platform here. Because refactoring. And gross
#include "../platform/crash.h"

#include "util/cvar.h"
#include "util/itertools.h"
#include "util/region.h"
#include "util/pool.h"
#include "util/ring.h"
#include "util/sdl_keymap.h"

#include "util/gui_builder.h"
#include "util/vg_builder.h"
