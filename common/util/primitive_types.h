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
struct N2Color {
    union {
        u8 rgba[4];
        u8 hsva[4];
        struct {
            union {
                struct { u8 r, g, b; }; /* `a` cannot be defined twice, so... */
                struct { u8 h, s, v; }; /* ... it will be included below.     */
            };
            u8 a;
            //TODO: Is this worth it? Will this struct even pack into 4x`u8`s?
        };
    };
}; ENFORCE_POD(N2Color);


/* Construct empty */
N2Color make_color() { return N2Color {0}; }

/* Construct from 4 numeric parameters */
template <class T, class U, class V, class W>
N2Color make_color(T r, U g, V b, W a) { return N2Color {r, g, b, a}; }
template <class T>
N2Color make_color(T i[4]) { return N2Color {i[0], i[1], i[2], i[3]}; }

/* Construct from 3 numeric parameters */
template <class T, class U, class V>
N2Color make_color(T r, U g, V b) { return make_color(r, g, b, 0xFF); }
//NOTE: We want the below, but because we have `make_color(T i[*])` above, this
//      looks like it's shadowing the above function, rather than overloading it
// template <class T>
// N2Color make_color(T i[3]) { return make_color(i[0], i[1], i[2], 0xFF); }

/* Construct from 1 numeric parameter */
template <class T>
N2Color make_color(T c) { return make_color(c, c, c); }
