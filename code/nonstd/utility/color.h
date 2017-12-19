/** COLOR
 *  =====
 *  Simple 4-vector of u8s or f32s used to represent RGB colors.
 *  More complext construction functions to convert from HSV to RBG colors.
 */
#pragma once

#include "nonstd/cpp1z/type_trait_assertions.h"
#include "nonstd/core/primitive_types.h"


namespace nonstd {

struct color;
struct colorf;


struct color {
    union {
        struct {
            u8 r;
            u8 g;
            u8 b;
            u8 a;
        };
        u8 rgba[4];
        u8 rgb[3];
    };

    /* Implicit conversion from color to colorf, to make argument passing
     * much simpler. */
    constexpr operator colorf const () noexcept;

}; ENFORCE_POD(color); ENFORCE_SIZE(color, 4);


struct colorf {
    union {
        struct {
            f32 r;
            f32 g;
            f32 b;
            f32 a;
        };
        f32 rgba[4];
        f32 rgb[3];
    };

    /* Explicit conversion from colorf to color, to make you consider why you're
     * moving away from the more standard floating-point representation. */
    constexpr explicit operator color const () noexcept;

}; ENFORCE_POD(colorf); ENFORCE_SIZE(colorf, 16);

/* Implicit conversion from color to colorf, to make argument passing
 * much simpler. */
constexpr color::operator colorf const () noexcept {
    return { this->r/255.f,
             this->g/255.f,
             this->b/255.f,
             this->a/255.f };
}

/* Explicit conversion from colorf to color, to make you consider why you're
 * moving away from the more standard floating-point representation. */
constexpr colorf::operator color const () noexcept {
    return { static_cast<u8>(this->r*255),
             static_cast<u8>(this->g*255),
             static_cast<u8>(this->b*255),
             static_cast<u8>(this->a*255) };
}


/* Build a color w/ red, green, blue, and alpha components. */
constexpr inline color rgb_color(u8 r, u8 g, u8 b, u8 a) noexcept {
    return { r, g, b, a };
}
/* Build a gray color at full alpha. */
constexpr inline color rgb_color(u8 color) noexcept {
    return rgb_color(color, color, color, 255);
}
/* Build a basic red, green, blue color at full alpha. */
constexpr inline color rgb_color(u8 r, u8 g, u8 b) noexcept {
    return rgb_color(r, g, b, 255);
}


/* Build a color w/ red, green, blue, and alpha components. */
constexpr inline colorf rgb_colorf(f32 r, f32 g, f32 b, f32 a) noexcept {
    return { r, g, b, a };
}
/* Build a gray color at full alpha. */
constexpr inline colorf rgb_colorf(f32 color) noexcept {
    return rgb_colorf(color, color, color, 1.f);
}
/* Build a basic red, green, blue color at full alpha. */
constexpr inline colorf rgb_colorf(f32 r, f32 g, f32 b) noexcept {
    return rgb_colorf(r, g, b, 1.f);
}

/* Build a color w/ hue [0,360], saturation [0.f,1.f], value [0.f,1.f], and
 * alpha components. */
constexpr inline colorf hsv_color(u16 h, f32 s, f32 v, f32 a) noexcept {
    // Thanks to stackoverflow.com/questions/3018313#answer-36209005
    // and wikipedia for this implementation
    if (s == 0) { return { v, v, v, a }; }

    f32 h_ = h == 360.f
           ? 0.f
           : h / 60.f;
    f32 fract = h_ - std::floor(h_);

    f32 p = v*(1. - s);
    f32 q = v*(1. - s * fract);
    f32 t = v*(1. - s * (1. - fract));

    if      (0. <= h_ && h_ < 1.) return { v, t, p, a };
    else if (1. <= h_ && h_ < 2.) return { q, v, p, a };
    else if (2. <= h_ && h_ < 3.) return { p, v, t, a };
    else if (3. <= h_ && h_ < 4.) return { p, q, v, a };
    else if (4. <= h_ && h_ < 5.) return { t, p, v, a };
    else if (5. <= h_ && h_ < 6.) return { v, p, q, a };
    else                          return { 0.f, 0.f, 0.f, a };
}
/* Build a color with a hue [0,360] at full saturation, value, and alpha. */
constexpr inline colorf hsv_color(u16 h) noexcept {
    return hsv_color(h, 1.f, 1.f, 1.f);
}
/* Build a color with a hue [0,360] and saturation [0.f, 1.f] at full value
 * and alpha. */
constexpr inline colorf hsv_color(u16 h, f32 s) noexcept {
    return hsv_color(h, s, 1.f, 1.f);
}
/* Build a color with a hue [0,360], saturation [0.f,1.f], and value [0.f,1.f]
 * component at full alpha. */
constexpr inline colorf hsv_color(u16 h, f32 s, f32 v) noexcept {
    return hsv_color(h, s, v, 1.f);
}

} /* namespace nonstd */
