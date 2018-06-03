/** Fixes for MSVC Backwards Compatibility Defines
 *  ==============================================
 *  The windows toolchain includes some preprocessor macros that are required to
 *  make some *very* old windows code build, but that breaks new stuff in weird
 *  unexpected ways. This header, if included after `<windows.h>`, removes them.
 */

#pragma once

#include <nonstd/nonstd.h>

#if defined(NONSTD_OS_WINDOWS)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/** `near`/`far` pointer annotation macros
 *  --------------------------------------
 *  These are defined to nothing, to provide compatibility with 16bit programs
 *  which made a distinction between near and far pointers. Context:
 *
 *   * https://blogs.msdn.microsoft.com/oldnewthing/20041101-00/?p=37433
 *
 *  Since we don't even build with 32bit support on any of our platforms, the
 *  chance of breaking anything we use by removing these macros is basically 0.
*/
#if defined(near)
#undef near
#endif
#if defined(far)
#undef far
#endif

/** `small` from MIDL
 *  -----------------
 *  The COM IDL -> C compiler relies on preprocessor macros to make structure
 *  definitions written in MIDL (the COM interface definition language) compile
 *  in plain C source files. Specifically, the `small` datatype is an `i8`.
 *
 *   * https://msdn.microsoft.com/en-us/library/windows/desktop/aa367165(v=vs.85).aspx
 *   * https://msdn.microsoft.com/en-us/library/windows/desktop/aa367314(v=vs.85).aspx
 */
#if defined(small)
#undef small
#endif

#endif /* NONSTD_OS_WINDOWS */
