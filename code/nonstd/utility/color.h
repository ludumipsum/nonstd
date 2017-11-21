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

    Color() noexcept                 = default;
    Color(Color const &)             = default;
    Color(Color &&)                  = default;
    Color& operator= (Color const &) = default;
    Color& operator= (Color &&)      = default;

    /* Construct from 4 numeric parameters */
    template <typename T, typename U, typename V, typename W,
              typename Enabled = ENABLE_IF_TYPE(IS_INTEGRAL(T) &&
                                                IS_INTEGRAL(U) &&
                                                IS_INTEGRAL(V) &&
                                                IS_INTEGRAL(W)   )>
    constexpr Color(T r, U g, V b, W a) noexcept
        : r(r), g(g), b(b), a(a) { }
    /* Construct from 3 numeric parameters (full alpha) */
    template <typename T, typename U, typename V,
              typename Enabled = ENABLE_IF_TYPE(IS_INTEGRAL(T) &&
                                                IS_INTEGRAL(U) &&
                                                IS_INTEGRAL(V)   )>
    constexpr Color(T r, U g, V b) noexcept
        : r(r), g(g), b(b), a(0xFF) { }
    /* Construct from an array of four values */
    template <typename T,
              typename Enabled = ENABLE_IF_TYPE(IS_INTEGRAL(T))>
    constexpr Color(T i[4]) noexcept
        : r(i[0]), g(i[1]), b(i[2]), a(i[3]) { }


    /* Get the red component as a decimal between 0 and 1 */
    constexpr float r_to_float() noexcept { return r / 255.f; }
    /* Get the blue component as a decimal between 0 and 1 */
    constexpr float g_to_float() noexcept { return r / 255.f; }
    /* Get the green component as a decimal between 0 and 1 */
    constexpr float b_to_float() noexcept { return r / 255.f; }
    /* Get the alpha value as a decimal between 0 and 1 */
    constexpr float a_to_float() noexcept { return r / 255.f; }

}; ENFORCE_POD(Color); ENFORCE_SIZE(Color, 4);

} /* namespace nonstd */
