/** COLOR
 *  =====
 *  Simple 4-vector of u8s or f32s used to represent RGB colors.
 *  More complext construction functions to convert from HSV to RBG colors.
 */
#pragma once

#include <nonstd/nonstd.h>
#include <nonstd/angle.h>
#include <nonstd/type_traits_ext.h>


namespace nonstd {


/** Forward Declarations
 *  ====================
 */
struct RGBAu;
struct RGBAf;
struct HSVA;

namespace detail {

    inline constexpr RGBAf rgba_u_to_f(RGBAu const & c) noexcept;
    inline constexpr RGBAu rgba_f_to_u(RGBAf const & c) noexcept;
    inline constexpr HSVA  rgba_to_hsva(RGBAf const & c) noexcept;
    inline constexpr RGBAf hsva_to_rgba(HSVA  const & c) noexcept;

} /* namespace detail */


/** Class Declarations
 *  ==================
 */

struct RGBAu {
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

    RGBAu() noexcept = default;
    RGBAu(RGBAu const &) noexcept = default;
    RGBAu(RGBAu &&) noexcept = default;
    RGBAu& operator= (RGBAu const &) noexcept = default;
    RGBAu& operator= (RGBAu &&) noexcept = default;

    inline constexpr RGBAu(i32 gray, i32 alpha = 255) noexcept
        : r ( gray  )
        , g ( gray  )
        , b ( gray  )
        , a ( alpha )
    {
        ASSERT(0 <= gray && gray <= 255);
        ASSERT(0 <= alpha && alpha <= 255);
    }
    inline constexpr RGBAu(i32 gray, f64 alpha) noexcept
        : r ( gray  )
        , g ( gray  )
        , b ( gray  )
        , a ( static_cast<u8>(alpha * 255.0) )
    {
        ASSERT(0 <= gray && gray <= 255);
        ASSERT(0.0 <= alpha && alpha <= 1.0);
    }

    inline constexpr RGBAu(i32 red, i32 green, i32 blue, i32 alpha = 255)
    noexcept
        : r ( red   )
        , g ( green )
        , b ( blue  )
        , a ( alpha )
    {
        ASSERT(0 <= red && red <= 255);
        ASSERT(0 <= green && green <= 255);
        ASSERT(0 <= blue && blue <= 255);
        ASSERT(0 <= alpha && alpha <= 255);
    }
    inline constexpr RGBAu(i32 red, i32 green, i32 blue, f64 alpha) noexcept
        : r ( red   )
        , g ( green )
        , b ( blue  )
        , a ( static_cast<u8>(alpha * 255.0) )
    {
        ASSERT(0 <= red && red <= 255);
        ASSERT(0 <= green && green <= 255);
        ASSERT(0 <= blue && blue <= 255);
        ASSERT(0.0 <= alpha && alpha <= 1.0);
    }

    inline constexpr RGBAu(f64 red, f64 green, f64 blue, f64 alpha = 1.0)
    noexcept
        : r ( static_cast<u8>(red   * 255.0) )
        , g ( static_cast<u8>(green * 255.0) )
        , b ( static_cast<u8>(blue  * 255.0) )
        , a ( static_cast<u8>(alpha * 255.0) )
    {
        ASSERT(0.0 <= red && red <= 1.0);
        ASSERT(0.0 <= green && green <= 1.0);
        ASSERT(0.0 <= blue && blue <= 1.0);
        ASSERT(0.0 <= alpha && alpha <= 1.0);
    }

    inline constexpr RGBAu(RGBAf const & rhs) noexcept;
    inline constexpr operator RGBAf const () noexcept;
    inline constexpr explicit RGBAu(HSVA const & rhs) noexcept;
    inline constexpr explicit operator HSVA  const () noexcept;

}; ENFORCE_POD(RGBAu); ENFORCE_SIZE(RGBAu, 4);


struct RGBAf {
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

    RGBAf() noexcept = default;
    RGBAf(RGBAf const &) noexcept = default;
    RGBAf(RGBAf &&) noexcept = default;
    RGBAf& operator= (RGBAf const &) noexcept = default;
    RGBAf& operator= (RGBAf &&) noexcept = default;

    inline constexpr RGBAf(f64 gray, f64 alpha = 1.0) noexcept
        : r ( gray  )
        , g ( gray  )
        , b ( gray  )
        , a ( alpha )
    {
        ASSERT(0.0 <= gray && gray <= 1.0);
        ASSERT(0.0 <= alpha && alpha <= 1.0);
    }

    inline constexpr RGBAf(f64 red, f64 green, f64 blue, f64 alpha = 1.0)
    noexcept
        : r ( red   )
        , g ( green )
        , b ( blue  )
        , a ( alpha )
    {
        ASSERT(0.0 <= red && red <= 1.0);
        ASSERT(0.0 <= green && green <= 1.0);
        ASSERT(0.0 <= blue && blue <= 1.0);
        ASSERT(0.0 <= alpha && alpha <= 1.0);
    }

    inline constexpr RGBAf(i32 red, i32 green, i32 blue, f64 alpha = 1.0)
    noexcept
        : r ( red   / 255.0 )
        , g ( green / 255.0 )
        , b ( blue  / 255.0 )
        , a ( alpha )
    {
        ASSERT(0 <= red && red <= 255);
        ASSERT(0 <= green && green <= 255);
        ASSERT(0 <= blue && blue <= 255);
        ASSERT(0.0 <= alpha && alpha <= 1.0);
    }
    inline constexpr RGBAf(i32 red, i32 green, i32 blue, i32 alpha)
    noexcept
        : r ( red   / 255.0 )
        , g ( green / 255.0 )
        , b ( blue  / 255.0 )
        , a ( alpha / 255.0 )
    {
        ASSERT(0 <= red && red <= 255);
        ASSERT(0 <= green && green <= 255);
        ASSERT(0 <= blue && blue <= 255);
        ASSERT(0 <= alpha && alpha <= 255);
    }

    inline constexpr RGBAf(RGBAu const & rhs) noexcept;
    inline constexpr operator RGBAu const () noexcept;
    inline constexpr explicit RGBAf(HSVA const & rhs) noexcept;
    inline constexpr explicit operator HSVA const () noexcept;

}; ENFORCE_POD(RGBAf); ENFORCE_SIZE(RGBAf, 16);


struct HSVA {
    angle h;
    f32   s;
    f32   v;
    f32   a;

    HSVA() noexcept = default;
    HSVA(HSVA const &) noexcept = default;
    HSVA(HSVA &&) noexcept = default;
    HSVA& operator= (HSVA const &) noexcept = default;
    HSVA& operator= (HSVA &&) noexcept = default;

    inline constexpr HSVA(angle hue,
                          f64   saturation = 1.0,
                          f64   value = 1.0,
                          f64   alpha = 1.0)
    noexcept
        : h ( hue        )
        , s ( saturation )
        , v ( value      )
        , a ( alpha      )
    {
        ASSERT(angle::cx::zero <= hue && hue <= angle::cx::tau);
        ASSERT(0.0 <= saturation && saturation <= 1.0);
        ASSERT(0.0 <= value && value <= 1.0);
        ASSERT(0.0 <= alpha && alpha <= 1.0);
    }

    inline constexpr HSVA(f64 value, f64 alpha = 1.0)
        : h ( angle::cx::zero )
        , s ( 0.0             )
        , v ( value           )
        , a ( alpha           )
    {
        ASSERT(0.0 <= value && value <= 1.0);
        ASSERT(0.0 <= alpha && alpha <= 1.0);
    }

    inline constexpr HSVA(RGBAu const & rhs) noexcept;
    inline constexpr explicit operator RGBAu const () noexcept;
    inline constexpr HSVA(RGBAf const & rhs) noexcept;
    inline constexpr explicit operator RGBAf const () noexcept;

}; ENFORCE_POD(HSVA); ENFORCE_SIZE(HSVA, 16);


/** Free Function Definitions
 *  =========================
 */
namespace detail {

    inline constexpr RGBAf rgba_u_to_f(RGBAu const & color) noexcept {
        return {
            color.r / 255.0,
            color.g / 255.0,
            color.b / 255.0,
            color.a / 255.0,
        };
    }
    inline constexpr RGBAu rgba_f_to_u(RGBAf const & color) noexcept {
        return {
            static_cast<u8>(color.r * 255.0),
            static_cast<u8>(color.g * 255.0),
            static_cast<u8>(color.b * 255.0),
            static_cast<u8>(color.a * 255.0),
        };
    }

    inline constexpr HSVA  rgba_to_hsva(RGBAf const & color) noexcept {
        // Thanks to stackoverflow.com/questions/3018313#answer-6930407 and
        // wikipedia for this implementation
        using nonstd::angle;

        f32 min_rg  = color.r < color.g ? color.r : color.g;
        f32 min_rgb = min_rg  < color.b ? min_rg  : color.b;
        f32 max_rg  = color.r > color.g ? color.r : color.g;
        f32 max_rgb = max_rg  > color.b ? max_rg  : color.b;
        f32 delta   = max_rgb - min_rgb;

        if (delta < 0.00001 || max_rgb <= 0.0) {
            return { angle::cx::zero, 0.0, max_rgb, 1.0 };
        }

        f32 v = max_rgb;
        f32 s = delta / max_rgb;

        f32 h2 = max_rgb == color.r
            ? (color.g - color.b) / delta
            : max_rgb == color.g
            ? (color.b - color.r) / delta + 2.0
            /* max_rgb == color.b */
            : (color.r - color.g) / delta + 4.0;
        f32 h1 = h2 < 0.0
            ? h2 + 6.0
            : h2;
        f32 h  = h1 / 6.0;

        return { h * angle::cx::tau, s, v, color.a };
    }
    inline constexpr RGBAf hsva_to_rgba(HSVA  const & color) noexcept {
        // Thanks to stackoverflow.com/questions/3018313#answer-36209005 and
        // wikipedia for this implementation
        using nonstd::angle;

        if (color.s == 0) { return { color.v, color.v, color.v, color.a }; }

        f32 h_ = color.h == angle::cx::tau
               ? 0.0
               : color.h.rads() / angle::cx::tau.rads() * 6.0; // [0.0, 6.0)
        f32 s_ = color.s;
        f32 v_ = color.v;
        f32 fract = h_ - std::floor(h_);

        f32 p = v_*(1. - s_);
        f32 q = v_*(1. - s_ * fract);
        f32 t = v_*(1. - s_ * (1. - fract));

        if      (0.0 <= h_ && h_ < 1.0) return { v_,  t,   p,   color.a };
        else if (1.0 <= h_ && h_ < 2.0) return { q,   v_,  p,   color.a };
        else if (2.0 <= h_ && h_ < 3.0) return { p,   v_,  t,   color.a };
        else if (3.0 <= h_ && h_ < 4.0) return { p,   q,   v_,  color.a };
        else if (4.0 <= h_ && h_ < 5.0) return { t,   p,   v_,  color.a };
        else if (5.0 <= h_ && h_ < 6.0) return { v_,  p,   q,   color.a };
        else /* (6.0 == h_)          */ return { 0.0, 0.0, 0.0, color.a };
    }

} /* namespace detail */


/** Class Member Definitions
 *  ========================
 */

/** RGBAu
 *  -----
 */
inline constexpr RGBAu::RGBAu(RGBAf const & rhs) noexcept
    : RGBAu ( detail::rgba_f_to_u(rhs) )
{ }
inline constexpr RGBAu::operator RGBAf const () noexcept {
    return detail::rgba_u_to_f(*this);
}
inline constexpr RGBAu::RGBAu(HSVA const & rhs) noexcept
    : RGBAu ( detail::hsva_to_rgba(rhs) )
{ }
inline constexpr RGBAu::operator HSVA const () noexcept {
    return detail::rgba_to_hsva(*this);
}

/** RGBAf
 *  -----
 */
inline constexpr RGBAf::RGBAf(RGBAu const & rhs) noexcept
    : RGBAf ( detail::rgba_u_to_f(rhs) )
{ }
inline constexpr RGBAf::operator RGBAu const () noexcept {
    return detail::rgba_f_to_u(*this);
}
inline constexpr RGBAf::RGBAf(HSVA const & rhs) noexcept
    : RGBAf ( detail::hsva_to_rgba(rhs) )
{ }
inline constexpr RGBAf::operator HSVA const () noexcept {
    return detail::rgba_to_hsva(*this);
}

/** HSVA
 *  ----
 */
inline constexpr HSVA::HSVA(RGBAu const & rhs) noexcept
    : HSVA ( detail::rgba_to_hsva(rhs) )
{ }
inline constexpr HSVA::operator RGBAu const () noexcept {
    return detail::hsva_to_rgba(*this);
}
inline constexpr HSVA::HSVA(RGBAf const & rhs) noexcept
    : HSVA ( detail::rgba_to_hsva(rhs) )
{ }
inline constexpr HSVA::operator RGBAf const () noexcept {
    return detail::hsva_to_rgba(*this);
}



/** Print Overloads
 *  ===============
 */
inline std::ostream& operator << (std::ostream & s, RGBAu const & c) {
    return s <<
        "color{{ #{:02x}{:02x}{:02x} a:{:03} }}"_format(c.r, c.g, c.b, c.a);
}
inline void format_arg(fmt::BasicFormatter<char> & f,
                       char const * & format_str,
                       RGBAu const & c) {
    f.writer().write(
        "color{{ #{:02x}{:02x}{:02x} a:{:03} }}"_format(c.r, c.g, c.b, c.a)
    );
}

inline std::ostream& operator << (std::ostream & s, RGBAf const & c) {
    return s <<
        "color{{ r:{:0.2f} g:{:0.2f} b:{:0.2f} a:{:0.2f} }}"_format(c.r, c.g, c.b, c.a);
}
inline void format_arg(fmt::BasicFormatter<char> & f,
                       char const * & format_str,
                       RGBAf const & c) {
    f.writer().write(
        "color{{ r:{:0.2f} g:{:0.2f} b:{:0.2f} a:{:0.2f} }}"_format(c.r, c.g, c.b, c.a)
    );
}

inline std::ostream& operator << (std::ostream & s, HSVA const & c) {
    return s <<
        "color{{ h:{:03} s:{:0.2f} v:{:0.2f} a:{:0.2f} }}"_format(c.h.degs(), c.s, c.v, c.a);
}
inline void format_arg(fmt::BasicFormatter<char> & f,
                       char const * & format_str,
                       HSVA const & c) {
    f.writer().write(
        "color{{ h:{:03} s:{:0.2f} v:{:0.2f} a:{:0.2f} }}"_format(c.h.degs(), c.s, c.v, c.a)
    );
}

} /* namespace nonstd */
