/** Primitive Type Size Requirements
 *  ==============================
 *  Out of an abundance of caution, we sanity check our targets by requiring the
 *  compiler to confirm for us that arithmetic types are the sizes we list and
 *  assume them to be in our code.
 */

#include <nonstd/core/primitive_types.h>
#include <testrunner/testrunner.h>

#include <nonstd/type_traits_ext.h>


/** Basic Types
 *  -----------
 */
ENFORCE_SIZE( u8,  1);
ENFORCE_SIZE(u16,  2);
ENFORCE_SIZE(u32,  4);
ENFORCE_SIZE(u64,  8);
ENFORCE_SIZE( i8,  1);
ENFORCE_SIZE(i16,  2);
ENFORCE_SIZE(i32,  4);
ENFORCE_SIZE(i64,  8);
ENFORCE_SIZE(f32,  4);
ENFORCE_SIZE(f64,  8);


/** Windows Specific Types
 *  ----------------------
 */
#if defined(NONSTD_OS_WINDOWS)

#define WIN32_LEAN_AND_MEAN 1
#define NOMINMAX 1
#include <windows.h>

ENFORCE_SIZE(WORD,      2);
ENFORCE_SIZE(DWORD,     4);
ENFORCE_SIZE(DWORDLONG, 8);
ENFORCE_SIZE(DWORD32,   4);
ENFORCE_SIZE(DWORD64,   8);

#endif

// noop main, just to make sure we can compile/
int main(int argc, char** argv) {
    return 0;
}
