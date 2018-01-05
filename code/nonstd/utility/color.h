/** COLOR
 *  =====
 *  Simple 4-vector of u8s or f32s used to represent RGB colors.
 *  More complext construction functions to convert from HSV to RBG colors.
 */
#pragma once

#include <nonstd/cpp1z/type_trait_assertions.h>
#include <nonstd/core/primitive_types.h>
#include <nonstd/quantity/angle.h>


namespace nonstd {

struct color;
struct color_f;
struct color_hsva;


/*  RGBA Color -- u8[4]
 *  -------------------
 */
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

    constexpr inline explicit operator color_f const () noexcept;
    constexpr inline explicit operator color_hsva const () noexcept;

}; ENFORCE_POD(color); ENFORCE_SIZE(color, 4);

/** RGBA Color -- f32[4]
 *  --------------------
 */
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

    constexpr inline explicit operator color const () noexcept;
    constexpr inline explicit operator color_hsva const () noexcept;

}; ENFORCE_POD(color_f); ENFORCE_SIZE(color_f, 16);

/** HSVA Color -- f32[4]
 *  --------------------
 */
struct color_hsva {
    quantity::angle h;
    f32             s;
    f32             v;
    f32             a;

    constexpr inline explicit operator color const () noexcept;
    constexpr inline explicit operator color_f const () noexcept;
}; ENFORCE_POD(color_hsva); ENFORCE_SIZE(color_hsva, 16);


/** Conversion Operations -- From `color`
 *  -------------------------------------
 */
constexpr inline color::operator color_f const () noexcept {
    return { r/255.f,
             g/255.f,
             b/255.f,
             a/255.f };
}
constexpr inline color::operator color_hsva const () noexcept {
    using nonstd::quantity::angle;
    // Thanks to stackoverflow.com/questions/3018313#answer-6930407 and
    // wikipedia for this implementation
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
        return { angle::zero, 0.f, max_rgb, 1.f };
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
    f32 h  = h1 / 6.f; // [0.f,1.f]

    return { h * angle::cx::tau, s, v, a_ };
}

/** Conversion Operations -- From `color_f`
 *  ---------------------------------------
 */
constexpr inline color_f::operator color const () noexcept {
    return { (u8)(r*255),
             (u8)(g*255),
             (u8)(b*255),
             (u8)(a*255) };
}
constexpr inline color_f::operator color_hsva const () noexcept {
    using nonstd::quantity::angle;
    // Thanks to stackoverflow.com/questions/3018313#answer-6930407 and
    // wikipedia for this implementation
    f32 min_rg  = r      < g ? r      : g;
    f32 min_rgb = min_rg < b ? min_rg : b;
    f32 max_rg  = r      > g ? r      : g;
    f32 max_rgb = max_rg > b ? max_rg : b;
    f32 delta   = max_rgb - min_rgb;

    if (delta < 0.00001 || max_rgb <= 0.0) {
        return { angle::zero, 0.f, max_rgb, 1.f };
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
    f32 h  = h1 / 6.f; // [0.f,1.f)

    return { h * angle::cx::tau, s, v, a };
}

/** Conversion Operations -- From `color_hsva`
 *  ------------------------------------------
 */
constexpr inline color_hsva::operator color const () noexcept {
    using nonstd::quantity::angle;
    // Thanks to stackoverflow.com/questions/3018313#answer-36209005 and
    // wikipedia for this implementation
    if (s == 0) { return { (u8)(v*255), (u8)(v*255), (u8)(v*255), (u8)(a*255) }; }

    f32 h_ = h == angle::cx::tau ? 0.f
           : h.rads() / angle::cx::tau.rads() * 6.f; // [0.f, 6.f)
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
constexpr inline color_hsva::operator color_f const () noexcept {
    using nonstd::quantity::angle;
    // Thanks to stackoverflow.com/questions/3018313#answer-36209005 and
    // wikipedia for this implementation
    if (s == 0) { return { v, v, v, a }; }

    f32 h_ = h == angle::cx::tau ? 0.f
           : h.rads() / angle::cx::tau.rads() * 6.f; // [0.f, 6.f)
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


/** Free Builder Functions -- for `color`
 *  -------------------------------------
 */

/* Build a color w/ red, green, blue, and alpha components. */
constexpr inline color rgba_color(u8 r, u8 g, u8 b, u8 a=255) noexcept {
    return { r, g, b, a };
}
/* Build a gray color at full alpha. */
constexpr inline color rgba_color(u8 value, u8 a=255) noexcept {
    return { value, value, value, a };
}

/** Free Builder Functions -- for `color_f`
 *  ---------------------------------------
 */

/* Build a color w/ red, green, blue, and alpha components. */
constexpr inline color_f rgba_colorf(f32 r, f32 g, f32 b, f32 a=1.f) noexcept {
    return { r, g, b, a };
}
/* Build a gray color at full alpha. */
constexpr inline color_f rgba_colorf(f32 value, f32 a=1.f) noexcept {
    return { value, value, value, a };
}

/** Free Builder Functions -- for `color_hsva`
 *  ------------------------------------------
 */

/* Build a color w/ hue [angular], saturation [0.0,1.0], value [0.0,1.0], and
 * alpha components. */
template <typename S=f32, typename V=f32, typename A=f32,
    typename std::enable_if_t<   std::is_floating_point_v<S>
                              && std::is_floating_point_v<V>
                              && std::is_floating_point_v<A>, int> = 0 >
constexpr inline color_hsva hsva_color(quantity::angle h,
                                       S               s = 1.f,
                                       V               v = 1.f,
                                       A               a = 1.f) {
    return { h, static_cast<f32>(s), static_cast<f32>(v), static_cast<f32>(a) };
}

/** Print Overloads -- for `color`
 *  ------------------------------
 */
inline std::ostream& operator << (std::ostream & s, color const & c) {
    return s <<
        "color{{ #{:02x}{:02x}{:02x} a:{:03} }}"_format(c.r, c.g, c.b, c.a);
}

inline void format_arg(fmt::BasicFormatter<char> & f,
                       char const * & format_str,
                       color const & c) {
    f.writer().write(
        "color{{ #{:02x}{:02x}{:02x} a:{:03} }}"_format(c.r, c.g, c.b, c.a)
    );
}

/** Print Overloads -- for `color_f`
 *  --------------------------------
 */
inline std::ostream& operator << (std::ostream & s, color_f const & c) {
    return s <<
        "color{{ r:{:0.2f} g:{:0.2f} b:{:0.2f} a:{:0.2f} }}"_format(c.r, c.g, c.b, c.a);
}

inline void format_arg(fmt::BasicFormatter<char> & f,
                       char const * & format_str,
                       color_f const & c) {
    f.writer().write(
        "color{{ r:{:0.2f} g:{:0.2f} b:{:0.2f} a:{:0.2f} }}"_format(c.r, c.g, c.b, c.a)
    );
}

/** Print Overloads -- for `color_hsva`
 *  -----------------------------------
 */
inline std::ostream& operator << (std::ostream & s, color_hsva const & c) {
    return s <<
        "color{{ h:{:03} s:{:0.2f} v:{:0.2f} a:{:0.2f} }}"_format(c.h.degs(), c.s, c.v, c.a);
}

inline void format_arg(fmt::BasicFormatter<char> & f,
                       char const * & format_str,
                       color_hsva const & c) {
    f.writer().write(
        "color{{ h:{:03} s:{:0.2f} v:{:0.2f} a:{:0.2f} }}"_format(c.h.degs(), c.s, c.v, c.a)
    );
}

} /* namespace nonstd */
