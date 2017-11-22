/** COLOR
 *  =====
 *  Simple 4-vector of u8s used to represent RGB colors.
 */
#pragma once

#include "nonstd/cpp1z/type_trait_assertions.h"
#include "nonstd/core/primitive_types.h"


namespace nonstd {

struct Color;
using  Color_u = Color;
struct Color_f;


struct Color {
    union {
        u8 rgba[4];
        u8 hsva[4];
        struct {
            // The compiler isn't smart enough to figure out that both rgba and
            // hsva accessors put the 'a' in the same spot in memory, so if you
            // try to put them all together in one struct in the top level union
            // you'll get a redefinition error. We've done this instead -- a bit
            // harder to read here, but the accessors are the same for users.
            union {u8 r; u8 h;};
            union {u8 g; u8 s;};
            union {u8 b; u8 v;};
            u8 a;
        };
    };

    // NB. This class _must_ be an Aggregate type, not just a POD type. This
    // means no user-defiend constructors, and no protected/private members.
}; ENFORCE_POD(Color); ENFORCE_SIZE(Color, 4);


struct Color_f {
    union {
        f32 rgba[4];
        f32 hsva[4];
        struct {
            // The compiler isn't smart enough to figure out that both rgba and
            // hsva accessors put the 'a' in the same spot in memory, so if you
            // try to put them all together in one struct in the top level union
            // you'll get a redefinition error. We've done this instead -- a bit
            // harder to read here, but the accessors are the same for users.
            union {f32 r; f32 h;};
            union {f32 g; f32 s;};
            union {f32 b; f32 v;};
            f32 a;
        };
    };

    // NB. This class _must_ be an Aggregate type, not just a POD type. This
    // means no user-defiend constructors, and no protected/private members.
}; ENFORCE_POD(Color_f); ENFORCE_SIZE(Color_f, 16);


/* Color (Color_u) creation functions */

/* Construct a Color from 4 numeric parameters */
template <typename T, typename U, typename V, typename W>
constexpr Color make_color(T r, U g, V b, W a) noexcept {
    static_assert(IS_CONVERTIBLE(T, u8), "'Color_u's must be constructed using integral types.");
    static_assert(IS_CONVERTIBLE(U, u8), "'Color_u's must be constructed using integral types.");
    static_assert(IS_CONVERTIBLE(V, u8), "'Color_u's must be constructed using integral types.");
    static_assert(IS_CONVERTIBLE(W, u8), "'Color_u's must be constructed using integral types.");
    return { (u8)(r), (u8)(g), (u8)(b), (u8)(a)};
}
/* Construct a Color from 3 numeric parameters (full alpha) */
template <typename T, typename U, typename V>
constexpr Color make_color(T r, U g, V b) noexcept {
    static_assert(IS_CONVERTIBLE(T, u8), "'Color_u's must be constructed using integral types.");
    static_assert(IS_CONVERTIBLE(U, u8), "'Color_u's must be constructed using integral types.");
    static_assert(IS_CONVERTIBLE(V, u8), "'Color_u's must be constructed using integral types.");
    return { (u8)(r), (u8)(g), (u8)(b), 0xFF };
}
/* Construct a Color from an array of four values */
template <typename T>
constexpr Color make_color(T i[4]) noexcept {
    static_assert(IS_CONVERTIBLE(T, u8), "'Color_u's must be constructed using integral types.");
    return { (u8)(i[0]), (u8)(i[1]), (u8)(i[2]), (u8)(i[3]) };
}
/* Construct a grayscale Color from a single value */
template <typename T>
constexpr Color make_color(T i) noexcept {
    static_assert(IS_CONVERTIBLE(T, u8), "'Color_u's must be constructed using integral types.");
    return { (u8)(i), (u8)(i), (u8)(i), 0xFF };
}
/* Construct a Color_u from a Color_f */
constexpr Color make_color(Color_f c) noexcept {
    return { (u8)(c.r * 255), (u8)(c.g * 255), (u8)(c.b * 255),
             (u8)(c.a * 255) };
}


/* Color_f creation functions */

/* Construct a Color_f from 4 numeric parameters */
template <typename T, typename U, typename V, typename W>
constexpr Color_f make_color_f(T r, U g, V b, W a) noexcept {
    static_assert(IS_CONVERTIBLE(T, f32), "'Color_f's must be constructed using floating point types.");
    static_assert(IS_CONVERTIBLE(U, f32), "'Color_f's must be constructed using floating point types.");
    static_assert(IS_CONVERTIBLE(V, f32), "'Color_f's must be constructed using floating point types.");
    static_assert(IS_CONVERTIBLE(W, f32), "'Color_f's must be constructed using floating point types.");
    return { (f32)(r), (f32)(g), (f32)(b), (f32)(a)};
}
/* Construct a Color_f from 3 numeric parameters (full alpha) */
template <typename T, typename U, typename V>
constexpr Color_f make_color_f(T r, U g, V b) noexcept {
    static_assert(IS_CONVERTIBLE(T, f32), "'Color_f's must be constructed using floating point types.");
    static_assert(IS_CONVERTIBLE(U, f32), "'Color_f's must be constructed using floating point types.");
    static_assert(IS_CONVERTIBLE(V, f32), "'Color_f's must be constructed using floating point types.");
    return { (f32)(r), (f32)(g), (f32)(b), 1.f };
}
/* Construct a Color_f from an array of four values */
template <typename T>
constexpr Color_f make_color_f(T i[4]) noexcept {
    static_assert(IS_CONVERTIBLE(T, f32), "'Color_f's must be constructed using floating point types.");
    return { (f32)(i[0]), (f32)(i[1]), (f32)(i[2]), (f32)(i[3]) };
}
/* Construct a grayscale Color_f from a single value */
template <typename T>
constexpr Color_f make_color_f(T i) noexcept {
    static_assert(IS_CONVERTIBLE(T, f32), "'Color_f's must be constructed using floating point types.");
    return { (f32)(i), (f32)(i), (f32)(i), (f32)(0xFF) };
}
/* Construct a Color_f from a Color_u */
constexpr Color_f make_color(Color_u c) noexcept {
    return { (f32)(c.r / 255.f), (f32)(c.g / 255.f), (f32)(c.b / 255.f),
             (f32)(c.a / 255.f) };
}

} /* namespace nonstd */
