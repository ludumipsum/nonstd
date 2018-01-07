/** Windows Type Size Requirements
 *  ==============================
 *  Out of an abundance of caution, we sanity check our windows targets by
 *  requiring the compiler to confirm for us that WORD-derived types are the
 *  sizes we assume them to be in our code.
 */

#include <nonstd/cpp1z/type_traits_ext.h>
#include <nonstd/preprocessor/homogenize.h>

 #if defined(NONSTD_OS_WINDOWS)
// Guarantee Windows API types are what we think they are.
#define WIN32_LEAN_AND_MEAN 1
#define NOMINMAX 1
#include <windows.h>
ENFORCE_SIZE(WORD,      2);
ENFORCE_SIZE(DWORD,     4);
ENFORCE_SIZE(DWORDLONG, 8);
ENFORCE_SIZE(DWORD32,   4);
ENFORCE_SIZE(DWORD64,   8);
#endif
