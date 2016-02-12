#pragma once
/*# Utility Macros & Functions */

/*## C Standard Library Components */
#include <stdint.h>
#include <limits.h>
#include <string>
#include <functional>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <stdlib.h>
#include <inttypes.h>


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

/* C++ Default Constructor Removal
   ===============================
   A macro to disallow the copy constructor and operator= functions
   This should be used in the private: declarations for a class
*/
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

//Note: These were defined first s.t. we can use them to operate on the structs
//      defined in "util/primitive_types.h"


/* Defines some basic types we use everywhere */
#include "util/primitive_types.h"


#ifndef n2max
#define n2max(a,b)            (((a) > (b)) ? (a) : (b))
#endif
#ifndef n2min
#define n2min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

/*## Force alignment of a given instance or datatype */
#ifndef alignof
#   define alignof(x) __alignof(x)
#endif
#if defined(_MSC_VER)
#   define ALIGNAS(X) __declspec(align(X))
#else
#   define ALIGNAS(X) alignas(X)
#endif

/*## DJB2 Hash
  Simple bytestring to 64bit integer hash
*/
inline uint64_t djb2(char const* str) {
  uint64_t hash = 5381;
  int32_t c;
  while ((c=*str++))
      hash = ((hash << 5) + hash) + c;
  return hash;
};
#define HASH djb2

/*## Stringify boolean values for easy printf-ing */
inline char const* const bool2string(bool b) {
  return b ? "true" : "false";
};

/*## Prettier function signature macro */
#if defined(_MSC_VER)
# define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

/*## UNUSED()
  Utility macro for marking variables deliberately-unused. Nixes warnings.
*/
#define UNUSED(_var) do { (void)(true ? (void)0 : ( (void)(_var) ) ); } while(0)

/*## std::enable_if convenience wrapper
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

/*## Force a breakpoint on X86*/
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

/*Text Processing Utilities
  =========================

  Utility functions and macros for logging, variadic printf-expansion, and
  similar chores.
*/

/*## Variadic Message Format Expander
  _variadicExpand allows the `MESSAGE` parameter that's passed into `LOG` take the form of a format string.
  Modified from Erik Aronesty's answer of this SO question:
  http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
  but also: http://stackoverflow.com/questions/7825648/vscprintf-equivalent-on-android/
*/
std::string _variadicExpand(const char* message, ...);

/*## Logging Macro Shorthand */
#define LOG(MESSAGE, ...) \
    _logMessage(::_variadicExpand(MESSAGE, ##__VA_ARGS__).c_str(), \
                __FILE__, \
                __LINE__, \
                __FUNCTION__)

/*## Logging Implementation */
i32 _logMessage(char const* message,
                char const* file,
                i32 const line,
                char const* function);

#if !defined(N2_TEST)
/*## FOURCC construction shorthand
  Many data file formats, especially microsoft ones, use four-character-codes
  to identify segments. These are represented as a 32-bit integer with the
  value of four adjacent ASCII characters.
*/
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

/*Configuration Variables
  =======================

  DSS/DATA allocated configuration variable list. Can be freely added
  anywhere in one line, and queried in other compilation units either via
  extern or by-name lookup. Can also be watched by a single std::function.
*/

#include <functional>

#define CV(some_cvar) ((some_cvar).value())
#define SCV(some_cvar_p) ((some_cvar_p != NULL) ? some_cvar_p->value() : NULL)

// TODO: config file generator from defined CVars

template<typename VALUE_TYPE>
class CVar {
public:
  CVar(char const* name, char const* synopsis, VALUE_TYPE value)
      : m_next(nullptr),
        m_name(name),
        m_synopsis(synopsis),
        m_value(value),
        m_min(value),
        m_max(value),
        m_watch_callback(nullptr) {
      addToList();
  }
  CVar(char const* name, char const* synopsis, VALUE_TYPE value,
                                               VALUE_TYPE min,
                                               VALUE_TYPE max)
      : m_next(nullptr),
        m_name(name),
        m_synopsis(synopsis),
        m_value(value),
        m_min(min),
        m_max(max),
        m_watch_callback(nullptr) {
      addToList();
  }

  // Transparent assignment as VALUE_TYPE
  inline CVar<VALUE_TYPE>& operator=(VALUE_TYPE value) {
    // If min=max, no clamping, just assign
    auto old_value = m_value;
    m_value = (m_min == m_max) ? value : clampValue(value);
    if (m_watch_callback && old_value != m_value) {
        m_watch_callback(this->value());
    }
    return *this;
  }

  // Transparent cast to value type
  inline operator VALUE_TYPE(void) const {
    return m_value;
  }

  // Explicit value cast
  inline VALUE_TYPE value(void) const {
    return m_value;
  }

  // Register a callback to take some action when this setting is changed.
  // NB. Only one such callback may be registered per cvar.
  inline void watch(std::function<void(VALUE_TYPE)> const& callback) {
    m_watch_callback = callback;
  }

  static CVar<VALUE_TYPE>* find(char const* name);

private:
  inline VALUE_TYPE clampValue(VALUE_TYPE value) {
    if (value < m_min) {
      //DEBUGTODO: Emit debug event here
      value = m_min;
    } else if (value > m_max) {
      //DEBUGTODO: Emit debug event here
      value = m_max;
    }
    return value;
  }

  void addToList(void);
  CVar<VALUE_TYPE>* m_next;

  char const* m_name;
  char const* m_synopsis;
  VALUE_TYPE m_value;
  decltype(m_value) m_min;
  decltype(m_value) m_max;
  std::function<void(VALUE_TYPE)> m_watch_callback;
};

class CVar_s {
public:
  CVar_s(char const* name, char const* synopsis, char const* value)
        : m_next(nullptr),
          m_name(name),
          m_synopsis(synopsis),
          m_value(value) {
        addToList();
  }

  // Transparent assignment as bool
  inline CVar_s& operator=(char const* value) {
    auto old_value = m_value;
    m_value = value;
    if (m_watch_callback && m_value != old_value) {
        m_watch_callback(this->value());
    }
    return *this;
  }

  // Transparent cast to bool
  inline operator char const*(void) const {
    return m_value.c_str();
  }

  // Explicit value cast
  inline char const* value(void) const {
    return m_value.c_str();
  }

  // Register a callback to take some action when this setting is changed.
  // NB. Only one such callback may be registered per cvar.
  inline void watch(std::function<void(char const*)> const& callback) {
    m_watch_callback = callback;
  }

  static CVar_s* find(char const* name);

private:
  DISALLOW_COPY_AND_ASSIGN(CVar_s);

  void addToList(void);
  CVar_s* m_next;

  char const* m_name;
  char const* m_synopsis;
  std::string m_value;
  std::function<void(char const*)> m_watch_callback;
};

typedef CVar<double> CVar_f;
typedef CVar<int64_t> CVar_i;
typedef CVar<bool> CVar_b;

/* Convenience function for toggling boolean cvars from pointers */
inline bool SCV_TOGGLE(CVar_b *cv_ptr) {
  if (cv_ptr != NULL) {
    *cv_ptr = !CV(*cv_ptr);
    return CV(*cv_ptr);
  }
  return false;
}

/* Crash Handling
   ==============
*/
#include <string.h>

#define CRASH(ERROR_NUMBER, MESSAGE, ...) \
    _CRASH(ERROR_NUMBER, \
           _variadicExpand(MESSAGE, ##__VA_ARGS__).c_str(), \
           __FILE__, \
           __LINE__, \
           __PRETTY_FUNCTION__)

void _CRASH(i32 error_number,
            char const* reason,
            char const* file,
            u64 line,
            char const* funcsig);

// When we're building the testing binary, we expect there to be a
// jump environment for us to hop to when a crash is triggered. This
// is written this way to support tests that are *supposed* to crash.
#if defined(N2_TEST)
#include <mutex>
#include <setjmp.h>
static bool g_crash_expected;
static jmp_buf g_crash_jmp_target;
//TODO: Reimplement mutex correctly
// static std::mutex g_crash_mutex;
inline int CAPTURE_CRASH(std::function<void(void)> test) {
  // g_crash_mutex.lock();
  g_crash_expected = true;
  errno = setjmp(g_crash_jmp_target);
  if (0 == errno) {
    test();
  } else {
    g_crash_expected = false;
    memset(&g_crash_jmp_target, 0, sizeof(g_crash_jmp_target));
  }
  // g_crash_mutex.unlock();
  return errno;
}
#endif

#define N2ASSERT(COND, ERRNO, MESSAGE, ...)                                    \
  ((COND) ? true :                                                             \
    CRASH(ERRNO, "Assertion Failed (" #COND "): " #MESSAGE , ##__VA_ARGS__))

// Alias the appropriate free function for destroying underlying buffers. This
// utility function is necessary because the visual C runtime differentiates
// aligned from unaligned buffers in terms of how they're freed, unlike POSIX.
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

/* Tuple-expanding call template, from Stack Overflow:
   http://stackoverflow.com/questions/10766112/c11-i-can-go-from-multiple-args-to-tuple-but-can-i-go-from-tuple-to-multiple
*/
namespace detail
{
    template <typename F, typename Tuple, bool Done, int Total, int... N>
    struct call_impl
    {
        static void call(F f, Tuple && t)
        {
            call_impl<F, Tuple, Total == 1 + sizeof...(N), Total, N..., sizeof...(N)>::call(f, std::forward<Tuple>(t));
        }
    };

    template <typename F, typename Tuple, int Total, int... N>
    struct call_impl<F, Tuple, true, Total, N...>
    {
        static void call(F f, Tuple && t)
        {
            f(std::get<N>(std::forward<Tuple>(t))...);
        }
    };
}

// expand a std::tuple into function arguments at the callsite
// (parameters inferred)
// e.g. tcall(myFunc, my_tuple);
template <typename F, typename Tuple>
void tcall(F f, Tuple && t)
{
    typedef typename std::decay<Tuple>::type ttype;
    detail::call_impl<F, Tuple, 0 == std::tuple_size<ttype>::value, std::tuple_size<ttype>::value>::call(f, std::forward<Tuple>(t));
}

// Tiny utility for timing segments of code. Given a now function which reports
// nanoseconds, this defaults to microseconds.
class ScopeTimer {
public:
  u64 start, unit_divisor;
  u64 &target;
  u64 (*now)(void);

  ScopeTimer(u64& target, u64 (*now)(void), u64 unit_divisor = NS_PER_US)
    : start(now())
    , unit_divisor(unit_divisor)
    , target(target)
    , now(now) { }

  ~ScopeTimer() { target = (now() - start) / unit_divisor; }
  ScopeTimer(ScopeTimer const&) = delete;
  ScopeTimer& operator=(ScopeTimer const&) = delete;
  ScopeTimer(ScopeTimer&&) = delete;
  ScopeTimer& operator=(ScopeTimer&&) = delete;
};

// Easymode macro for the above, designed to be used when opening a scope. E.g.
//      /* Handle incoming inputs */
//      { TIME_SCOPE_US(step_stat.input_poll_usec, state.functions.now);
//        handleInput(state);
//      }
#define TIME_SCOPE(target, now_function) ScopeTimer st(target, now_function)
#define TIME_SCOPE_NS(target, now_function) ScopeTimer st(target, now_function, 1)
#define TIME_SCOPE_US(target, now_function) ScopeTimer st(target, now_function, NS_PER_US)
#define TIME_SCOPE_MS(target, now_function) ScopeTimer st(target, now_function, NS_PER_MS)
#define TIME_SCOPE_SEC(target, now_function) ScopeTimer st(target, now_function, NS_PER_SEC)

#include "util/itertools.h"
#include "util/region.h"
#include "util/pool.h"
#include "util/ring.h"
#include "util/sha1.h"
#include "util/sdl_keymap.h"

#include "util/gui_builder.h"
#include "util/vg_builder.h"
