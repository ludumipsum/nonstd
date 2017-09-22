/** COLOR
 *  =====
 *  Simple 4-vector of u8s used to represent RGB colors.
 */
#pragma once

#include "nonstd/cpp1z/type_trait_assertions.h"
#include "nonstd/core/primitive_types.h"


namespace nonstd {

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
}; ENFORCE_POD(Color); ENFORCE_SIZE(Color, 4);


/* Construct empty */
inline Color color() { return Color {0}; }

/* Construct from 4 numeric parameters */
template <typename T, typename U, typename V, typename W>
constexpr inline Color color(T r, U g, V b, W a) {
    return Color { (u8)r, (u8)g, (u8)b, (u8)a };
}
template <typename T>
constexpr inline Color color(T i[4]) {
    return Color { (u8)i[0], (u8)i[1], (u8)i[2], (u8)i[3] };
}

/* Construct from 3 numeric parameters */
template <typename T, typename U, typename V>
constexpr inline Color color(T r, U g, V b) {
    return color(r, g, b, 0xFF);
}

/* Construct from 1 numeric parameter */
template <typename T>
constexpr inline Color color(T c) {
    return color(c, c, c);
}

} /* namespace nonstd */
