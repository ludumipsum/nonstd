/** Platform Homogenization
 *  =======================
 */

#pragma once

/* Platform and compiler characteristics */
namespace nonstd {
enum struct operating_system_t {
    pc_unknown,

    mobile_ios,
    pc_linux,
    pc_macos,
    pc_windows,
};
enum struct compiler_t {
    unknown,

    clang,
    gcc,
    msvc,
};

/* OS Detection */
#if defined(__linux__)
    constexpr auto os_string = "Linux";
    constexpr auto os = operating_system_t::pc_linux;
    #define NONSTD_OS_LINUX true
#elif defined(__APPLE__) && defined(__MACH__)
    // Apple's compiler detection macros only let you know that you're on
    // a mac host, not what you're building for. To get that you have to
    // include TargetConditionals and check the TARGET_OS macros
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR == 1 || TARGET_OS_IPHONE == 1
        constexpr auto os_string = "iOS";
        constexpr auto os = operating_system_t::mobile_ios;
        #define NONSTD_OS_IOS true
    #elif TARGET_OS_MAC == 1
        constexpr auto os_string = "MacOS";
        constexpr auto os = operating_system_t::pc_macos;
        #define NONSTD_OS_MACOS true
    #endif
#elif defined(_WIN64)
    constexpr auto os_string = "Windows";
    constexpr auto os = operating_system_t::pc_windows;
    #define NONSTD_OS_WINDOWS true
#else
    #pragma message("Nonstd's homogenize macros don't know your OS!");
    constexpr auto os_string = "Unknown";
    constexpr auto os = operating_system_t::pc_unknown;
    #define NONSTD_OS_UNKNOWN true
#endif

/* Compiler Detection */
#if defined(__clang__)
    constexpr auto compiler_string = "Clang";
    constexpr auto compiler = compiler_t::clang;
    #define NONSTD_COMPILER_CLANG true
#elif defined(__GNUC__) || defined(__GNUG__)
    constexpr auto compiler_string = "GCC";
    constexpr auto compiler = compiler_t::gcc;
    #define NONSTD_COMPILER_GCC true
#elif defined(_MSC_VER)
    constexpr auto compiler_string = "Microsoft Visual C++";
    constexpr auto compiler = compiler_t::msvc;
    #define NONSTD_COMPILER_MSVC true
#else
    #pragma message("Nonstd's homogenize macros don't know your compiler!");
    constexpr auto compiler_string = "Unknown Compiler";
    constexpr auto compiler = compiler_t::msvc;
    #define NONSTD_COMPILER_MSVC true
#endif

} /* namespace nonstd */

/* Compiler-specific warning suppressions */
//TODO: Should these be moved to cmake?
#if defined(NONSTD_COMPILER_MSVC)
#pragma warning(disable: 4200)  // Nonstandard extension: zero length array
#pragma warning(disable: 4201)  // Nonstandard extension: nameless struct/union
#endif

/** __PRETTY_FUNCTION__
 *  ------------------- */
#if defined(NONSTD_COMPILER_MSVC)
#  define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

/** Alignment Macro
 *  ---------------
 *  Force alignment of a given instance or datatype.
 */
#if defined(NONSTD_COMPILER_MSVC)
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
#  if defined(NONSTD_COMPILER_MSVC)
#    define FORCEINLINE __forceinline inline
#  elif defined(NONSTD_COMPILER_CLANG)
#    define FORCEINLINE __attribute__((always_inline)) inline
#  elif defined(NONSTD_COMPILER_GCC)
#    define FORCEINLINE __attribute__((always_inline)) inline
#  else
#    define FORCEINLINE inline
#  endif
#endif

/** Preferred Path Separator
 *  ------------------------
 */
namespace nonstd {
#if defined(NONSTD_OS_WINDOWS)
    constexpr char preferred_separator = '\\';
#else
    constexpr char preferred_separator = '/';
#endif
}

/** String Manipulation Functions
 *  -----------------------------
 */
#if defined(NONSTD_OS_WINDOWS)
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
