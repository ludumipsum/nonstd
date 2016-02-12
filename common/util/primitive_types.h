/* PRIMITIVE TYPES
   ===============

   Defines some basic types used across different pieces of game and platform
   code. Things like shorthand basic types, colors, and vectors.
*/

#pragma once

/***************
 * Basic Types *
 ***************/
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef float f32;
typedef double f64;

typedef u32 ID;

/* Power-of-two Byte Prefixes
 * NB. these implicitly cast the provided value to 64-bit int
 */
#define KBYTES(N) ((i64)N*1024)
#define MBYTES(N) ((i64)N*1024*1024)
#define GBYTES(N) ((i64)N*1024*1024*1024)
#define TBYTES(N) ((i64)N*1024*1024*1024*1024)

/* Human-readable temporal unit conversions */
#define NS_PER_US  1000
#define NS_PER_MS  1000000
#define NS_PER_SEC 1000000000

#define US_PER_MS  1000
#define US_PER_SEC 1000000

#define MS_PER_SEC 1000


/**********
 * Colors *
 **********/
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
Color make_color() { return Color {0}; }

/* Construct from 4 numeric parameters */
template <class T, class U, class V, class W>
Color make_color(T r, U g, V b, W a) {
  return Color {(u8)r, (u8)g, (u8)b, (u8)a};
}
template <class T>
Color make_color(T i[4]) {
  return Color {(u8)i[0], (u8)i[1], (u8)i[2], (u8)i[3]};
}

/* Construct from 3 numeric parameters */
template <class T, class U, class V>
Color make_color(T r, U g, V b) { return make_color(r, g, b, 0xFF); }

/* Construct from 1 numeric parameter */
template <class T>
Color make_color(T c) { return make_color(c, c, c); }