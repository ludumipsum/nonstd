/** Platform Homogenization
 *  =======================
 */

#pragma once


/* Compiler-specific warning suppressions */
//TODO: Should these be moved to cmake?
#if defined(_MSC_VER)
#pragma warning(disable: 4200)  // Nonstandard extension: zero length array
#pragma warning(disable: 4201)  // Nonstandard extension: nameless struct/union
#endif

/** __PRETTY_FUNCTION__
 *  ------------------- */
#if defined(_MSC_VER)
#  define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

/** Alignment Macro
 *  ---------------
 *  Force alignment of a given instance or datatype.
 */
#if defined(_MSC_VER)
#  define ALIGNAS(X) __declspec(align(X))
#else
#  define ALIGNAS(X) alignas(X)
#endif

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

/** Preferred Path Separator
 *  ------------------------
 */
namespace nonstd {
#if defined(_MSC_VER)
    constexpr char preferred_separator = '\\';
#else
    constexpr char preferred_separator = '/';
#endif
}

/** String Manipulation Functions
 *  -----------------------------
 */
#if defined(_MSC_VER)
#define strtok_r strtok_s
#define strtoq strtoll

inline char* stpcpy(char* dest, const char* src) {
    char *d = dest;
    const char* s = src;

    do {
      *d++ = *s;
    } while (*s++ != '\0');

    return d - 1;
}
#endif
