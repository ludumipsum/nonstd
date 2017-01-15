/* Primitive Types
   ===============

   Defines some basic types used across different pieces of game and platform
   code. Things like shorthand basic types, colors, and vectors.
*/

#pragma once

#include "batteries_included.h"

/* Basic Types
   -----------
*/
typedef uint8_t          u8;
typedef uint16_t        u16;
typedef uint32_t        u32;
typedef uint64_t        u64;
typedef int8_t           i8;
typedef int16_t         i16;
typedef int32_t         i32;
typedef int64_t         i64;
typedef float           f32;
typedef double          f64;
typedef char *          cstr;
typedef char const *    c_cstr;
typedef ptrdiff_t       ptrdiff;
typedef uint8_t *       ptr;
typedef uint8_t const * c_ptr;

inline ptr  n2malloc(size_t size)           { return (ptr)(malloc(size)); }
inline ptr  n2alloca(size_t size)           { return (ptr)(alloca(size)); }
inline ptr  n2realloc(ptr p, size_t size)   { return (ptr)(realloc(p, size)); }
inline ptr  n2calloc(size_t n, size_t size) { return (ptr)(calloc(n, size)); }
inline void n2free(ptr p)                   { free( (void *)( p ) ); }

inline void n2alignment_correct_free(ptr p, bool aligned) {
    alignment_correct_free( (void *)( p ), aligned);
}

/* IDs uniquely identify game entities. We reserve the bottom few for special
   meanings in our container types (unset, deleted, etc).
*/
typedef u64 ID;
enum ReservedIDs {
    ID_NOTFOUND = 0,
    ID_DELETED  = 9,
    ID_FIRST,
};

/* Power-of-two Byte Prefixes
   NB. these implicitly cast the provided value to 64-bit int
*/
#define KBYTES(N) ((i64)N*1024)
#define MBYTES(N) ((i64)N*1024*1024)
#define GBYTES(N) ((i64)N*1024*1024*1024)
#define TBYTES(N) ((i64)N*1024*1024*1024*1024)

/* Human-readable temporal unit conversions */
#define NS_PER_NS  1
#define NS_PER_US  1000
#define NS_PER_MS  1000000
#define NS_PER_SEC 1000000000

#define US_PER_MS  1000
#define US_PER_SEC 1000000

#define MS_PER_SEC 1000


/* COLORS
   ------
   Simple 4-vector of u8s used to represent RGB colors.
*/
struct Color {
    union {
        u8 rgba[4];
        u8 hsva[4];
        struct {
            /* The compiler isn't smart enough to figure out that both rgba and
               hsva accessors put the 'a' in the same spot in memory, so if you
               try to put them all together in one struct in the top level union
               you'll get a redefinition error. We've done this instead -- a bit
               harder to read here, but the accessors are the same for users. */
            union {u8 r; u8 h;};
            union {u8 g; u8 s;};
            union {u8 b; u8 v;};
            u8 a;
        };
    };
}; ENFORCE_POD(Color);


/* Construct empty */
inline Color color() { return Color {0}; }

/* Construct from 4 numeric parameters */
template <class T, class U, class V, class W> inline
Color color(T r, U g, V b, W a) {
    return Color { (u8)r, (u8)g, (u8)b, (u8)a };
}
template <class T> inline
Color color(T i[4]) {
    return Color { (u8)i[0], (u8)i[1], (u8)i[2], (u8)i[3] };
}

/* Construct from 3 numeric parameters */
template <class T, class U, class V> inline
Color color(T r, U g, V b) { return color(r, g, b, 0xFF); }

/* Construct from 1 numeric parameter */
template <class T> inline
Color color(T c) { return color(c, c, c); }

