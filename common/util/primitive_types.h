/* PRIMITIVE TYPES
   ===============

   Defines some basic types used across different pieces of game and platform
   code. Things like shorthand basic types, colors, and vectors.
*/

#pragma once

#if defined(N2_PLATFORM)
/* Include all platform headers necessary for generating implicit conversions
   from our basic types to those used by platform dependencies. */
#include <bgfx-extras/nanovg.h>
#endif

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

/**********
 * Colors *
 **********/
struct Color {
    union {
        u8 rgba[4];
        struct { u8 r, g, b, a; };
        u8 hsva[4];
        struct { u8 h, s, v, a; };
    }

    /* Generate implicit conversion operators to platform color types */
    #if defined(N2_PLATFORM)
    operator NVGColor() const { return nvgRGBA(r, g, b, a); }
    #endif
}; ENFORCE_POD(Color);

/* Construct empty */
Color make_color() { return Color {0}; }

/* Construct from 4 numeric parameters */
template <class T, class U, class V, class W>
Color make_color(T r, U g, V b, W a) { return Color {r, g, b, a}; }
template <class T>
Color make_color(T i[4]) { return Color {i[0], i[1], i[2], i[3]}; }

/* Construct from 3 numeric parameters */
template <class T, class U, class V>
Color make_color(T r, U g, V b) { return make_color(r, g, b, 0xFF); }
template <class T>
Color make_color(T i[3]) { return make_color(i[0], i[1], i[2], 0xFF); }

/* Construct from 1 numeric parameter */
template <class T>
Color make_color(T c) { return make_color(c, c, c); }
