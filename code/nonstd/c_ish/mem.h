/** Memory Wrappers
 *  ===============
 *  Like it says on the tin, these helpers wrap C memory manipulation functions
 *  s.t. they take and return our `ptr` type, rather than `void *`s
 */

#pragma once

#include "nonstd/core/primitive_types.h"


inline ptr  n2malloc(size_t size)           { return (ptr)(malloc(size)); }
inline ptr  n2alloca(size_t size)           { return (ptr)(alloca(size)); }
inline ptr  n2realloc(ptr p, size_t size)   { return (ptr)(realloc(p, size)); }
inline ptr  n2calloc(size_t n, size_t size) { return (ptr)(calloc(n, size)); }
inline void n2free(ptr p)                   { free((void *)(p)); }

inline ptr  n2memset(ptr dst, int v, size_t len) {
  return (ptr)(memset((void *)dst, v, len));
}
inline ptr  n2memcpy(ptr dst, ptr src, size_t size) {
  return (ptr)(memcpy((void *)dst, (void *)src, size));
}
inline ptr  n2memmove(ptr dst, ptr src, size_t size) {
  return (ptr)(memmove((void *)dst, (void *)src, size));
}
