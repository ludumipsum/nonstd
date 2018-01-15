#pragma once

#include <cassert>
#include <cmath>

#include <nonstd/core/primitive_types.h>
#include <nonstd/type_traits_ext.h>
#include <nonstd/cx_math.h>


namespace nonstd {
namespace quantity {

struct angle {
private:
    // These functions will be defined out-of-line so that we are able to define
    // inline function local static storage as the addresses to take.
    static inline angle const & _get_pi();
    static inline angle const & _get_tau();
    static inline angle const & _get_zero();

public:
    /** Class Constants
     *  ---------------
     */
    // Mathematical π (pi). Half the diameter of the unit circle.
    static constexpr f64 pi_radians = 3.14159265358979323846264338327950288;
    static inline angle const & pi  = _get_pi();

    // Mathematical τ (tau). Whole diameter of the unit circle.
    static constexpr f64 tau_radians = 6.28318530717958647692528676655900576;
    static inline angle const & tau  = _get_tau();

    // Mathematical 0 (0.0). None of the diameter of the unit circle.
    static inline angle const & zero = _get_zero();

    // Ratio to convert radians to degrees.
    static constexpr f64 radians_to_degrees = 180.0/pi_radians;
    // Ratio to convert degrees to radians.
    static constexpr f64 degrees_to_radians = pi_radians/180.0;


    /** Inner type `angle::cx`
     *  ----------------------
     *  This type will define static constexpr members;
     *    * angle::cx::pi
     *    * angle::cx::tau
     *    * angle::cx::zero
     *  Unfortunately, they all need to be declared out-of-line -- after `angle`
     *  has been made a complete type -- in order to be correctly constexpr.
     */
    struct cx;

private:
    // The current value of this angle, stored in radians.
    // This value is explicitly private because users shouldn't know or care how
    // we encode this value. Additionally, we may want (or need) to modify
    // angular representations at some point. See std::chono::duration and
    // std::ratio for a similar variable-representation of a physical quantity.
    f32 m_rads;

    // Explicit Converting Constructor
    // Private to prevent leakage of the type of m_rads. Note that this takes an
    // f64 (rather than an f32) because of implicit interger promotion.
    constexpr explicit angle(f64 rads)
        : m_rads ( static_cast<f32>(rads) )
    { }

public:
    /** Implicit Constructor Set
     *  ------------------------
     */
    angle()                          noexcept = default;
    angle(angle const &)             noexcept = default;
    angle(angle &&)                  noexcept = default;
    angle& operator= (angle const &) noexcept = default;
    angle& operator= (angle &&)      noexcept = default;

    /** Implicit Destructor
     *  -------------------
     */
    ~angle() noexcept = default;

    /** Named "Constructors"
     *  --------------------
     */
    // Construct a new angle from a radian value.
    static constexpr inline angle in_radians(f32 radians) noexcept {
        return angle { radians };
    }
    static constexpr inline angle in_rads(f32 radians) noexcept {
        return angle::in_radians(radians);
    }
    // Construct a new angle from a degree value.
    static constexpr inline angle in_degrees(f32 degrees) noexcept {
        return angle { degrees * degrees_to_radians };
    }
    static constexpr inline angle in_degs(f32 degrees) noexcept {
        return angle::in_degrees(degrees);
    }

    /** Observers
     *  ---------
     */
    // Return this angle in radians.
    constexpr inline f32 radians() const noexcept {
        return this->m_rads;
    }
    // Return this angle in radians.
    constexpr inline f32 rads() const noexcept {
        return this->radians();
    }
    // Return this angle in radians, normalized to the range [0,Tau).
    inline f32 normalized_radians() const noexcept {
        f32 ret = std::fmod(this->m_rads, tau_radians);
        if (ret < 0.0) { ret += tau_radians; }

        assert(0.0 < ret); assert(ret < tau_radians);
        return ret;
    }
    // Return this angle in radians, normalized to the range [0,Tau).
    inline f32 rads_norm() const noexcept {
        return this->normalized_radians();
    }
    // Return this angle in radians, normalized to the range [0,Tau).
    // CONSTEXPR VARIANT; Only call this in a constexpr context. You can assume
    // the non-cx version of this function will run at least 10x more quickly.
    constexpr inline f32 normalized_radians_cx() const noexcept {
        f32 ret = nonstd::cx::fmod(this->m_rads, tau_radians);
        if (ret < 0.0) { ret += tau_radians; }

        assert(0.0 < ret); assert(ret < tau_radians);
        return ret;
    }
    // Return this angle in radians, normalized to the range [0,Tau).
    // CONSTEXPR VARIANT; Only call this in a constexpr context. You can assume
    // the non-cx version of this function will run at least 10x more quickly.
    constexpr inline f32 rads_norm_cx() const noexcept {
        return this->normalized_radians_cx();
    }

    // Return this angle in degrees.
    constexpr inline f32 degrees() const noexcept {
        return this->m_rads * radians_to_degrees;
    }
    // Return this angle in degrees.
    constexpr inline f32 degs() const noexcept {
        return this->degrees();
    }
    // Return this angle in degrees, normalized to the range [0º,360º).
    inline f32 normalized_degrees() const noexcept {
        f32 ret = std::fmod(this->degrees(), 360.f);
        if (ret < 0.0) { ret += 360.f; }

        assert(0.0 < ret); assert(ret < 360.f);
        return ret;
    }
    // Return this angle in degrees, normalized to the range [0º,360º).
    inline f32 degs_norm() const noexcept {
        return this->normalized_degrees();
    }
    // Return this angle in degrees, normalized to the range [0º,360º).
    // CONSTEXPR VARIANT; Only call this in a constexpr context. You can assume
    // the non-cx version of this function will run at least 10x more quickly.
    constexpr inline f32 normalized_degrees_cx() const noexcept {
        f32 ret = nonstd::cx::fmod(this->degrees(), 360.f);
        if (ret < 0.0) { ret += 360.f; }

        assert(0.0 < ret); assert(ret < 360.f);
        return ret;
    }
    // Return this angle in degrees, normalized to the range [0º,360º).
    // CONSTEXPR VARIANT; Only call this in a constexpr context. You can assume
    // the non-cx version of this function will run at least 10x more quickly.
    constexpr inline f32 degs_norm_cx() const noexcept {
        return this->normalized_degrees_cx();
    }

    // Return a new angle equal to this angle normalized to the range [0,tau].
    inline angle normalized() const noexcept {
        return angle { normalized_radians() };
    }
    inline angle norm() const noexcept {
        return normalized();
    }
    // Return a new angle equal to this angle normalized to the range [0,tau].
    // CONSTEXPR VARIANT; Only call this in a constexpr context. You can assume
    // the non-cx version of this function will run at least 10x more quickly.
    inline angle normalized_cx() const noexcept {
        return angle { normalized_radians_cx() };
    }
    inline angle norm_cx() const noexcept {
        return normalized_cx();
    }

    /** Simple Modifiers
     *  ----------------
     */
    // Set this angle to the given radian value (may be <0 and >Tau).
    constexpr inline void set_radians(f32 radians) noexcept {
        this->m_rads = radians;
    }
    // Set this angle to the given radian value (may be <0 and >Tau).
    constexpr inline void rads(f32 radians) noexcept {
        this->set_radians(radians);
    }
    // Set this angle to the given degree value (may be <0º and >360º).
    constexpr inline void set_degrees(f32 degrees) noexcept {
        this->m_rads = degrees * degrees_to_radians;
    }
    // Set this angle to the given degree value (may be <0º and >360º).
    constexpr inline void degs(f32 degrees) noexcept {
        this->set_degrees(degrees);
    }

    // Clamp the value of this angle to between [0,Tau) (or [0º,360º) ).
    inline angle& normalize() noexcept {
        this->m_rads = std::fmod(this->m_rads, tau_radians);
        if (this->m_rads < 0.0) { this->m_rads += tau_radians; }

        assert(0.0 <= this->m_rads); assert(this->m_rads < tau_radians);
        return *this;
    }
    // Clamp the value of this angle to between [0,Tau) (or [0º,360º) ).
    // CONSTEXPR VARIANT; Only call this in a constexpr context. You can assume
    // the non-cx version of this function will run at least 10x more quickly.
    constexpr inline angle& normalize_cx() noexcept {
        this->m_rads = nonstd::cx::fmod(this->m_rads, tau_radians);
        if (this->m_rads < 0.0) { this->m_rads += tau_radians; }

        assert(0.0 <= this->m_rads); assert(this->m_rads < tau_radians);
        return *this;
    }

    /** Arithmetic Operators
     *  --------------------
     */
    // Return a new angle equal to the sum of two angles.
    constexpr inline angle  operator+  (angle const & rhs) const noexcept {
        return angle { this->m_rads + rhs.m_rads };
    }
    // Return a new angle equal to the difference of two angles.
    constexpr inline angle  operator-  (angle const & rhs) const noexcept {
        return angle { this->m_rads - rhs.m_rads };
    }
    // Return a new angle equal to that angle multiplied by a scalar.
    friend constexpr inline angle operator* (f32 scalar, angle a) noexcept {
        return angle { a.m_rads * scalar };
    }
    friend constexpr inline angle operator* (angle a, f32 scalar) noexcept {
        return angle { a.m_rads * scalar };
    }
    // Return a new angle equal to this angle divided by a scalar. Division by
    // 0 is undefined behavior.
    friend constexpr inline angle operator/ (f32 scalar, angle a) noexcept {
        return angle { a.m_rads / scalar };
    }
    friend constexpr inline angle operator/ (angle a, f32 scalar) noexcept {
        return angle { a.m_rads / scalar };
    }

    /** Unary + and - Operators
     *  -----------------------
     */
    /* Construct a new angle equal to this angle * 1. */
    constexpr inline angle operator+ () const noexcept {
        return angle { this->m_rads };
    }
    /* Construct a new angle equal to this angle * -1. */
    constexpr inline angle operator- () const noexcept {
        return angle { -(this->m_rads) };
    }

    /** Compound Arithmetic Operators
     *  -----------------------------
     */
    // Increment this angle by the value of another.
    constexpr inline angle& operator+= (angle const & rhs) noexcept {
        this->m_rads += rhs.m_rads;
        return *this;
    }
    // Decrement this angle by the value of another.
    constexpr inline angle& operator-= (angle const & rhs) noexcept {
        this->m_rads -= rhs.m_rads;
        return *this;
    }
    // Multiply this angle by a scalar.
    constexpr inline angle& operator*= (f32 scalar) noexcept {
        this->m_rads *= scalar;
        return *this;
    }
    // Divide this angle by a scalar. Division by 0 is undefined behavior.
    constexpr inline angle& operator/= (f32 scalar) noexcept {
        this->m_rads /= scalar;
        return *this;
    }

    /** Equality Operators
     *  ------------------
     *  For simplicity's sake, we only allow comparisons between angles. Gotta
     *  have a unit to give the comparison some context.
     */
    // Compare two angles
    friend constexpr inline
    bool operator== (angle const & lhs, angle const & rhs) noexcept {
        return lhs.m_rads == rhs.m_rads;
    }
    friend constexpr inline
    bool operator!= (angle const & lhs, angle const & rhs) noexcept {
        return !(lhs == rhs);
    }
    friend constexpr inline
    bool operator<  (angle const & lhs, angle const & rhs) noexcept {
        return lhs.m_rads < rhs.m_rads;
    }
    friend constexpr inline
    bool operator>  (angle const & lhs, angle const & rhs) noexcept {
        return rhs < lhs;
    }
    friend constexpr inline
    bool operator<= (angle const & lhs, angle const & rhs) noexcept {
        return !(rhs < lhs);
    }
    friend constexpr inline
    bool operator>= (angle const & lhs, angle const & rhs) noexcept {
        return !(lhs < rhs);
    }

    // Check if two angles are within +/- an epsilon of one another.
    constexpr inline bool nearly_equal(
            angle const & other,
            angle const & epsilon = angle::in_radians(0.01))
    const noexcept {
        return ((other - epsilon) < *this) && (*this < (other + epsilon));
    }

}; ENFORCE_POD(angle);


/** Out-Of-Line Class Definitions
 *  -----------------------------
 *  For great storage. For great constexpr.
 */
inline angle const & angle::_get_pi() {
    static constexpr angle pi = angle::in_radians(angle::pi_radians);
    return pi;
}
inline angle const & angle::_get_tau() {
    static constexpr angle tau = angle::in_radians(angle::tau_radians);
    return tau;
}
inline angle const & angle::_get_zero() {
    static constexpr angle zero = angle::in_radians(0.0);
    return zero;
}

struct angle::cx {
    // Mathematical π (pi). Half the diameter of the unit circle.
    static constexpr angle pi = angle::in_radians(angle::pi_radians);

    // Mathematical τ (tau). Whole diameter of the unit circle.
    static constexpr angle tau = angle::in_radians(angle::tau_radians);

    // Mathematical 0 (0.0). None of the diameter of the unit circle.
    static constexpr angle zero = angle::in_radians(0.0);
};

} /* namespace quantity */


namespace literals {
namespace angle_literals {

/** User Defined Literals
 *  ---------------------
 */
constexpr inline
quantity::angle operator "" _radians (udl_float_t radians) noexcept {
    return quantity::angle::in_radians(static_cast<f32>(radians));
}
constexpr inline
quantity::angle operator "" _rads    (udl_float_t radians) noexcept {
    return quantity::angle::in_radians(static_cast<f32>(radians));
}

constexpr inline
quantity::angle operator "" _degrees (udl_float_t degrees) noexcept {
    return quantity::angle::in_degrees(static_cast<f32>(degrees));
}
constexpr inline
quantity::angle operator "" _degs    (udl_float_t degrees) noexcept {
    return quantity::angle::in_degrees(static_cast<f32>(degrees));
}
constexpr inline
quantity::angle operator "" _degrees (udl_int_t degrees) noexcept {
    return quantity::angle::in_degrees(degrees);
}
constexpr inline
quantity::angle operator "" _degs    (udl_int_t degrees) noexcept {
    return quantity::angle::in_degrees(degrees);
}

} /* namespace angle_literals */
} /* namespace literals */

} /* namespace nonstd */
