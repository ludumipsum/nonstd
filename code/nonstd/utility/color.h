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
struct color_f;
struct color_hsva;


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

    constexpr explicit operator color_f const () noexcept;
    constexpr explicit operator color_hsva const () noexcept;

}; ENFORCE_POD(color); ENFORCE_SIZE(color, 4);


struct color_f {
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

    constexpr explicit operator color const () noexcept;
    constexpr explicit operator color_hsva const () noexcept;

}; ENFORCE_POD(color_f); ENFORCE_SIZE(color_f, 16);

struct color_hsva {
    union {
        struct {
            f32 h;
            f32 s;
            f32 v;
            f32 a;
        };
        f32 hsva[4];
        f32 hsv[3];
    };

    constexpr explicit operator color const () noexcept;
    constexpr explicit operator color_f const () noexcept;
};

constexpr color::operator color_f const () noexcept {
    return { r/255.f,
             g/255.f,
             b/255.f,
             a/255.f };
}
constexpr color::operator color_hsva const () noexcept {
    f32 r_ = r / 255.f;
    f32 g_ = g / 255.f;
    f32 b_ = b / 255.f;
    f32 a_ = a / 255.f;

    f32 min_rg  = r_      < g_ ? r_     : g_;
    f32 min_rgb = min_rg  < b_ ? min_rg : b_;
    f32 max_rg  = r_      > g_ ? r_     : g_;
    f32 max_rgb = max_rg  > b_ ? max_rg : b_;
    f32 delta   = max_rgb - min_rgb;

    if (delta < 0.00001 || max_rgb < 0.00001) {
        return { 0.f, 0.f, max_rgb, 1.f };
    }

    f32 v = max_rgb;
    f32 s = delta / max_rgb;

    f32 h2 = max_rgb == r_
           ? (g_ - b_) / delta
           : max_rgb == g_
           ? (b_ - r_) / delta + 2.f
           /* max_rgb == b_ */
           : (r_ - g_) / delta + 4.f;
    f32 h1 = h2 < 0.f
           ? h2 + 6.f
           : h2;
    f32 h  = h1 / 6.f;

    return { h, s, v, a_ };
}


constexpr color_f::operator color const () noexcept {
    return { (u8)(r*255),
             (u8)(g*255),
             (u8)(b*255),
             (u8)(a*255) };
}
constexpr color_f::operator color_hsva const () noexcept {
    f32 min_rg  = r      < g ? r      : g;
    f32 min_rgb = min_rg < b ? min_rg : b;
    f32 max_rg  = r      > g ? r      : g;
    f32 max_rgb = max_rg > b ? max_rg : b;
    f32 delta   = max_rgb - min_rgb;

    if (delta < 0.00001 || max_rgb <= 0.0) {
        return { 0.f, 0.f, max_rgb, 1.f };
    }

    f32 v = max_rgb;
    f32 s = delta / max_rgb;

    f32 h2 = max_rgb == r
           ? (g - b) / delta
           : max_rgb == g
           ? (b - r) / delta + 2.f
           /* max_rgb == b */
           : (r - g) / delta + 4.f;
    f32 h1 = h2 < 0.f
           ? h2 + 6.f
           : h2;
    f32 h  = h1 / 6.f;

    return { h, s, v, a };
}

constexpr color_hsva::operator color const () noexcept {
    // Thanks to stackoverflow.com/questions/3018313#answer-36209005
    // and wikipedia for this implementation
    if (s == 0) { return { (u8)(v*255), (u8)(v*255), (u8)(v*255), (u8)(a*255) }; }

    f32 h_ = h == 1.f
           ? 0.f
           : h * 6.f;
    f32 fract = h_ - std::floor(h_);

    f32 p = v*(1. - s);
    f32 q = v*(1. - s * fract);
    f32 t = v*(1. - s * (1. - fract));

    if      (0. <= h_ && h_ < 1.) return { (u8)(v*255), (u8)(t*255), (u8)(p*255), (u8)(a*255) };
    else if (1. <= h_ && h_ < 2.) return { (u8)(q*255), (u8)(v*255), (u8)(p*255), (u8)(a*255) };
    else if (2. <= h_ && h_ < 3.) return { (u8)(p*255), (u8)(v*255), (u8)(t*255), (u8)(a*255) };
    else if (3. <= h_ && h_ < 4.) return { (u8)(p*255), (u8)(q*255), (u8)(v*255), (u8)(a*255) };
    else if (4. <= h_ && h_ < 5.) return { (u8)(t*255), (u8)(p*255), (u8)(v*255), (u8)(a*255) };
    else if (5. <= h_ && h_ < 6.) return { (u8)(v*255), (u8)(p*255), (u8)(q*255), (u8)(a*255) };
    else                          return { 0          , 0          , 0          , (u8)(a*255) };
}
constexpr color_hsva::operator color_f const () noexcept {
    // Thanks to stackoverflow.com/questions/3018313#answer-36209005
    // and wikipedia for this implementation
    if (s == 0) { return { v, v, v, a }; }

    f32 h_ = h == 1.f
           ? 0.f
           : h * 6.f;
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


/* Build a color w/ red, green, blue, and alpha components. */
constexpr inline color rgba_color(u8 r, u8 g, u8 b, u8 a) noexcept {
    return { r, g, b, a };
}
/* Build a gray color at full alpha. */
constexpr inline color rgba_color(u8 gray) noexcept {
    return rgba_color(gray, gray, gray, 255);
}
/* Build a basic red, green, blue color at full alpha. */
constexpr inline color rgba_color(u8 r, u8 g, u8 b) noexcept {
    return rgba_color(r, g, b, 255);
}


/* Build a color w/ red, green, blue, and alpha components. */
constexpr inline color_f rgba_colorf(f32 r, f32 g, f32 b, f32 a) noexcept {
    return { r, g, b, a };
}
/* Build a gray color at full alpha. */
constexpr inline color_f rgba_colorf(f32 color) noexcept {
    return rgba_colorf(color, color, color, 1.f);
}
/* Build a basic red, green, blue color at full alpha. */
constexpr inline color_f rgba_colorf(f32 r, f32 g, f32 b) noexcept {
    return rgba_colorf(r, g, b, 1.f);
}


/* Build a color w/ hue [0,360], saturation [0.f,1.f], value [0.f,1.f], and
 * alpha components. */
constexpr inline color_hsva hsva_color(u16 h, f32 s, f32 v, f32 a) noexcept {
    return { h/360.f, s, v, a };
}
/* Build a color with a hue [0,360] at full saturation, value, and alpha. */
constexpr inline color_hsva hsva_color(u16 h) noexcept {
    return hsva_color(h, 1.f, 1.f, 1.f);
}
/* Build a color with a hue [0,360] and saturation [0.f, 1.f] at full value
 * and alpha. */
constexpr inline color_hsva hsva_color(u16 h, f32 s) noexcept {
    return hsva_color(h, s, 1.f, 1.f);
}
/* Build a color with a hue [0,360], saturation [0.f,1.f], and value [0.f,1.f]
 * component at full alpha. */
constexpr inline color_hsva hsva_color(u16 h, f32 s, f32 v) noexcept {
    return hsva_color(h, s, v, 1.f);
}

/* Build a color w/ hue [0.f,1.f], saturation [0.f,1.f], value [0.f,1.f], and
 * alpha components. */
constexpr inline color_hsva hsva_color(f32 h, f32 s, f32 v, f32 a) noexcept {
    return { h, s, v, a };
}
/* Build a color with a hue [0.f,1.f] at full saturation, value, and alpha. */
constexpr inline color_hsva hsva_color(f32 h) noexcept {
    return hsva_color(h, 1.f, 1.f, 1.f);
}
/* Build a color with a hue [0.f,1.f] and saturation [0.f, 1.f] at full value
 * and alpha. */
constexpr inline color_hsva hsva_color(f32 h, f32 s) noexcept {
    return hsva_color(h, s, 1.f, 1.f);
}
/* Build a color with a hue [0.f,1.f], saturation [0.f,1.f], and value [0.f,1.f]
 * component at full alpha. */
constexpr inline color_hsva hsva_color(f32 h, f32 s, f32 v) noexcept {
    return hsva_color(h, s, v, 1.f);
}

} /* namespace nonstd */
