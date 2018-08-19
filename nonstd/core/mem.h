/** Primitive Memory Functions
 *  ==========================
 *  Defines some foundational memory functions used across different pieces of
 *  the system.
 *  Consider moving all of these into `nonstd::`, and out of core.
 */

#pragma once

#include "homogenize.h"
#include "primitive_types.h"

/* Platform specific includes */
#if defined(NONSTD_OS_WINDOWS) || defined(__MINGW32__)
#include <io.h>     // mktmp
#include <malloc.h> // alloca
#else
#include <alloca.h> // alloca
#endif

/** C-ish Memory Overloads
 *  ----------------------
 *  We overload the standard C memory allocators to skip the cast to a common,
 *  arithmetic, byte-sized type.
 */
inline ptr  n2malloc(size_t size) {
    return static_cast<ptr>(std::malloc(size));
}
inline ptr  n2alloca(size_t size) {
    return static_cast<ptr>(alloca(size));
}
inline ptr  n2realloc(ptr pointer, size_t size) {
    return static_cast<ptr>(std::realloc(pointer, size));
}
inline ptr  n2calloc(size_t num, size_t size) {
    return static_cast<ptr>(std::calloc(num, size));
}
inline void n2free(ptr pointer) {
    std::free(static_cast<void*>(pointer));
}

inline ptr  n2memset(ptr dst, int val, size_t len) {
    void* _dst = static_cast<void*>(dst);
    return static_cast<ptr>(std::memset(_dst, val, len));
}
inline ptr  n2memcpy(ptr dst, ptr src, size_t size) {
    void* _dst = static_cast<void*>(dst);
    void* _src = static_cast<void*>(src);
    return static_cast<ptr>(std::memcpy(_dst, _src, size));
}
inline ptr  n2memmove(ptr dst, ptr src, size_t size) {
    void* _dst = static_cast<void*>(dst);
    void* _src = static_cast<void*>(src);
    return static_cast<ptr>(std::memmove(_dst, _src, size));
}
