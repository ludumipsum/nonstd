#pragma once

#include <ratio>
#include <chrono>
#include <nonstd/nonstd.h>

// A note on function-local type aliases;
// The C++ standard specifies some function-local types aliases when dealing
// with casts between different duration representations, such as `CD`, `CF`,
// and `CR`. My guess is these are short for 'Common Duration', 'Common
// Fraction' and 'Common Representation'. I'm not sure though, as it's never
// spelled out.
// This file takes inspiration from that assumption, using the following names;
//
//  - CD: Common Duration       -- ex; the type returned by `1s + 500ms`
//  - CF: Common Frequency      -- ex; the type returned by `1_kHz - 500_Hz`
//  - CR: Common Representation -- ex; `std::common_type_t<i32, i64>`
//  - CP: Common Period         -- ex; `std::ratio_divide<Period1, Period2>`
//                                     This is what the standard calls `CF`.


namespace nonstd::chrono {

    /** Subsumption of std::chrono
     *  --------------------------
     */
    using namespace std::chrono;


    /** Forward Declaration(s)
     *  ----------------------
     */
    template <typename Rep, typename Period = std::ratio<1>> class frequency;

} /* namespace nonstd::chrono */


/** Specialize `std::common_type` for `chrono::frequency`
 *  -----------------------------------------------------
 */
namespace std {

    template < typename Rep1, typename Period1
             , typename Rep2, typename Period2 >
    struct common_type< nonstd::chrono::frequency<Rep1, Period1>
                      , nonstd::chrono::frequency<Rep2, Period2> > {
    private:
        using Rep    = std::common_type_t<Rep1, Rep2>;
        using Period = nonstd::ratio::greatest_common_divisor_t< Period1
                                                               , Period2>;

    public:
        using type = nonstd::chrono::frequency<Rep, Period>;
    };

} /* namespace std */


/** (non)std::chrono
 *  ============================================================================
 */
namespace nonstd {
namespace chrono {

/** Type Traits
 *  ============================================================================
 *  These aren't useful outside the context of nonstd::chrono, so we define them
 *  here, rather than in nonstd/type_traits.h.
 *  Also, this file relies on our type trait extensions, and I don't want to
 *  introduce cyclic dependencies.
 */

/** is_duration<T>
 *  --------------
 */
template <typename T>
struct is_duration : std::false_type { };

template <typename Rep, typename Period>
struct is_duration<duration<Rep, Period>>                : std::true_type { };

template <typename Rep, typename Period>
struct is_duration<duration<Rep, Period> const>          : std::true_type { };

template <typename Rep, typename Period>
struct is_duration<duration<Rep, Period> volatile>       : std::true_type { };

template <typename Rep, typename Period>
struct is_duration<duration<Rep, Period> const volatile> : std::true_type { };

template <typename T>
inline constexpr bool is_duration_v = is_duration<T>::value;


/** is_frequency<T>
 *  ---------------
 */
template <typename T>
struct is_frequency : std::false_type { };

template <typename Rep, typename Period>
struct is_frequency<frequency<Rep, Period>>                : std::true_type { };

template <typename Rep, typename Period>
struct is_frequency<frequency<Rep, Period> const>          : std::true_type { };

template <typename Rep, typename Period>
struct is_frequency<frequency<Rep, Period> volatile>       : std::true_type { };

template <typename Rep, typename Period>
struct is_frequency<frequency<Rep, Period> const volatile> : std::true_type { };

template <typename T>
inline constexpr bool is_frequency_v = is_frequency<T>::value;


/** Frequency Cast
 *  ============================================================================
 *  As with std::duration_cast, we're using partial specializations on the
 *  implementation type to avoid multiplications and divides where possible.
 */
namespace detail {

/** Frequency Cast Implementation Details */
template < typename From
         , typename To
         , typename CP = typename std::ratio_divide< typename From::period
                                                   , typename To::period >::type
         , typename CR = std::common_type_t< typename To::rep
                                           , typename From::rep
                                           , intmax_t >
         , bool NumeratorIsOne   = CP::num == 1
         , bool DenominatorIsOne = CP::den == 1 >
struct frequency_cast_impl;

template <typename From, typename To, typename CP, typename CR>
struct frequency_cast_impl<From, To, CP, CR, true, true> {
    static constexpr To cast(From const & f) {
        return To { static_cast<typename To::rep>(f.count()) };
    }
};

template <typename From, typename To, typename CP, typename CR>
struct frequency_cast_impl<From, To, CP, CR, true, false> {
    static constexpr To cast(From const & f) {
        return To {
            static_cast<typename To::rep>(
                static_cast<CR>(f.count()) / static_cast<CR>(CP::den))
        };
    }
};

template <typename From, typename To, typename CP, typename CR>
struct frequency_cast_impl<From, To, CP, CR, false, true> {
    static constexpr To cast(From const & f) {
        return To {
            static_cast<typename To::rep>(
                static_cast<CR>(f.count()) * static_cast<CR>(CP::num))
        };
    }
};

template <typename From, typename To, typename CP, typename CR>
struct frequency_cast_impl<From, To, CP, CR, false, false> {
    static constexpr To cast(From const & f) {
        return To {
            static_cast<typename To::rep>(
                static_cast<CR>(f.count()) * static_cast<CR>(CP::num)
                                           / static_cast<CR>(CP::den))
        };
    }
};

} /* namespace detail */

/** Frequency Cast
 *  --------------
 */
template <typename ToFrequency, typename Rep, typename Period>
constexpr auto frequency_cast(frequency<Rep, Period> const & from)
-> /* ToFrequency */
std::enable_if_t<is_frequency_v<ToFrequency>, ToFrequency>
{
    using fc = detail::frequency_cast_impl<frequency<Rep, Period>, ToFrequency>;
    return fc::cast(from);
}


/** General Cast -- Floor
 *  ---------------------
 */
template <typename ToDuration, typename Rep, typename Period>
constexpr auto floor(duration<Rep, Period> const & d) {
    return ::std::chrono::floor<ToDuration>(d);
}
template <typename ToFrequency, typename Rep, typename Period>
constexpr auto floor(frequency<Rep, Period> const & f)
-> /* ToFrequency */
std::enable_if_t<is_frequency_v<ToFrequency>, ToFrequency>
{
    ToFrequency ret = frequency_cast<ToFrequency>(f);
    if (ret > f) { return ret - ToFrequency{1}; }
    return ret;
}

/** General Cast -- Ceil
 *  --------------------
 */
template <typename ToDuration, typename Rep, typename Period>
constexpr auto ceil(duration<Rep, Period> const & d) {
    return ::std::chrono::ceil<ToDuration>(d);
}
template <typename ToFrequency, typename Rep, typename Period>
constexpr auto ceil(frequency<Rep, Period> const & f)
-> /* ToFrequency */
std::enable_if_t<is_frequency_v<ToFrequency>, ToFrequency>
{
    ToFrequency ret = frequency_cast<ToFrequency>(f);
    if (ret < f) { return ret + ToFrequency{1}; }
    return ret;
}

/** General Cast -- Round
 *  ---------------------
 */
template <typename ToDuration, typename Rep, typename Period>
constexpr auto round(duration<Rep, Period> const & d) {
    return ::std::chrono::round<ToDuration>(d);
}
template <typename ToFrequency, typename Rep, typename Period>
constexpr auto round(frequency<Rep, Period> const & f)
-> /* ToFrequency */
std::enable_if_t< is_frequency_v<ToFrequency>
                  && !treat_as_floating_point_v<typename ToFrequency::rep>
                , ToFrequency > {
    ToFrequency t0 = floor<ToFrequency>(f);
    ToFrequency t1 = t0 + ToFrequency{1};
    auto diff0 = f - t0;
    auto diff1 = t1 - f;
    if (diff0 == diff1) {
        return t0.count() & 1
             ? t1
             : t0;
    } else if (diff0 < diff1) {
        return t0;
    }
    return t1;
}

/** Frequency Values Helpers
 *  ============================================================================
 *  The `frequency` template uses the `frequency_values` trait to construct
 *  special values of the `frequency`'s representation (`Rep`). This is done
 *  because the representation might be a class type with behavior which
 *  requires some other implementation to return these special values. In that
 *  case, the author of that class type should specialize `frequency_values` to
 *  return the indicated values.
 *  (Paraphrased from the standard.)
 */
template <typename Rep>
struct frequency_values {
    static constexpr Rep zero() { return Rep{0};                             }
    static constexpr Rep min()  { return std::numeric_limits<Rep>::lowest(); }
    static constexpr Rep max()  { return std::numeric_limits<Rep>::max();    }
};


/** Frequency
 *  ============================================================================
 */
template <typename Rep, typename Period>
class frequency {
    static_assert(!is_frequency_v<Rep>,
        "A frequency representation can not be a frequency");
    static_assert(!is_duration_v<Rep>,
        "A frequency representation can not be a duration");
    static_assert(nonstd::ratio::is_ratio_v<Period>,
        "The second template parameter of a frequency must be a std::ratio");
    static_assert(Period::num > 0, "frequency period must be positive");

public:
    using rep = Rep;
    using period = Period;

private:
    rep m_rep;

public:
    // Default Construction
    constexpr frequency() = default;

    // Value Construction
    template < typename Rep2
             , std::enable_if_t< std::is_convertible_v<Rep2, rep>
                                 && ( treat_as_floating_point_v<rep>
                                      || !treat_as_floating_point_v<Rep2>)
                               , int > = 0 >
    constexpr explicit frequency(Rep2 const & f)
        : m_rep ( f )
    { }

    // Conversion
    // TODO: If converting between ex; hz and mhz this constructor may introduce
    //       overflow. Such situations are explicitly disallowed by the standard
    //       (see §23.17.5.1: `duration` constructors). We should add a check
    //       and... static_assert when it fails? Go look at clang's impl and
    //       build something based on that?
    template < typename Rep2, typename Period2
             , std::enable_if_t< treat_as_floating_point_v<rep>
                                 || ( std::ratio_divide< Period2
                                                       , period >::den == 1
                                      && !treat_as_floating_point_v<Rep2>)
                               , int > = 0 >
    constexpr frequency(frequency<Rep2, Period2> const & rhs )
        : m_rep ( frequency_cast<frequency>(rhs).count() )
    { }

    // Observer
    constexpr rep count() const { return m_rep; }

    // Arithmetic
    constexpr frequency  operator+  () const { return *this;                 }
    constexpr frequency  operator-  () const { return frequency{ -m_rep };   }
    constexpr frequency& operator++ ()       { ++m_rep; return *this;        }
    constexpr frequency  operator++ (int)    { return frequency { m_rep++ }; }
    constexpr frequency& operator-- ()       { --m_rep; return *this;        }
    constexpr frequency  operator-- (int)    { return frequency { m_rep-- }; }

    constexpr frequency& operator+= (const frequency& f) {
        m_rep += f.count();
        return *this;
    }
    constexpr frequency& operator-= (const frequency& f) {
        m_rep -= f.count();
        return *this;
    }
    constexpr frequency& operator*= (const rep& r) {
        m_rep *= r;
        return *this;
    }
    constexpr frequency& operator/= (const rep& r) {
        m_rep /= r;
        return *this;
    }
    constexpr frequency& operator%= (const rep& r) {
        m_rep %= r;
        return *this;
    }
    constexpr frequency& operator%= (const frequency& f) {
        m_rep %= f.count();
        return *this;
    }

    // Special Values
    static constexpr frequency zero() {
        return frequency { frequency_values<rep>::zero() };
    }
    static constexpr frequency min()  {
        return frequency { frequency_values<rep>::min()  };
    }
    static constexpr frequency max()  {
        return frequency { frequency_values<rep>::max()  };
    }
};


/** Convenience Typedefs (or `using`s)
 *  ============================================================================
 */
namespace detail {
    // The integral type used for the representation (`Rep`) of
    // `std::chrono::seconds`, `::microseconds`, `::minutes`, etc. are
    // implementation-specific. We want to match the implementation's
    // representations, so we're going to extract the `::rep` type for the
    // durations relative to our frequencies: the period of one hertz is one
    // second, the period of one kilohertz is one millisecond, one megahertz is
    // one microsecond, etc..
    using hertz_rep_t     = typename         seconds::rep;
    using kilohertz_rep_t = typename    milliseconds::rep;
    using megahertz_rep_t = typename    microseconds::rep;
    using gigahertz_rep_t = typename     nanoseconds::rep;

    using bps_rep_t       = typename         seconds::rep;
    using bpm_rep_t       = typename         minutes::rep;
} /* namespace detail */

using hertz     = frequency<detail::hertz_rep_t               >;
using kilohertz = frequency<detail::kilohertz_rep_t, std::kilo>;
using megahertz = frequency<detail::megahertz_rep_t, std::mega>;
using gigahertz = frequency<detail::gigahertz_rep_t, std::giga>;
using terahertz = frequency<detail::gigahertz_rep_t, std::tera>;

using beats_per_second = frequency<detail::bps_rep_t                   >;
using beats_per_minute = frequency<detail::bpm_rep_t, std::ratio<1, 60>>;


/** Frequency Arithmetic
 *  ============================================================================
 */

// frequency + frequency -> frequency
template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr auto operator+ (frequency<Rep1, Period1> const & lhs,
                          frequency<Rep2, Period2> const & rhs)
-> std::common_type_t<frequency<Rep1, Period1>, frequency<Rep2, Period2>>
{
    using CF = std::common_type_t< frequency<Rep1, Period1>
                                 , frequency<Rep2, Period2> >;
    return CF { CF(lhs).count() + CF(rhs).count() };
}

// frequency - frequency -> frequency
template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr auto operator- (frequency<Rep1, Period1> const & lhs,
                          frequency<Rep2, Period2> const & rhs)
-> std::common_type_t<frequency<Rep1, Period1>, frequency<Rep2, Period2>>
{
    using CF = std::common_type_t< frequency<Rep1, Period1>
                                 , frequency<Rep2, Period2> >;
    return CF { CF(lhs).count() - CF(rhs).count() };
}

// frequency * scalar -> frequency
template <typename Rep, typename Period, typename ScalarRep>
constexpr auto operator* (frequency<Rep, Period> const & f, ScalarRep const & s)
-> /* frequency<std::common_type_t<Rep, ScalarRep>, Period> */
std::enable_if_t< std::is_convertible_v< ScalarRep
                                       , std::common_type_t<Rep, ScalarRep> >
                , frequency<std::common_type_t<Rep, ScalarRep>, Period> >
{
    using CF = frequency<std::common_type_t<Rep, ScalarRep>, Period>;
    return CF { CF(f).count() * s };
}

// scalar * frequency -> frequency
template <typename ScalarRep, typename Period, typename Rep>
constexpr auto operator* (ScalarRep const & s, frequency<Rep, Period> const & f)
-> /* frequency<std::common_type_t<Rep, ScalarRep>, Period> */
std::enable_if_t< std::is_convertible_v< ScalarRep
                                       , std::common_type_t<Rep, ScalarRep> >
                , frequency<std::common_type_t<Rep, ScalarRep>, Period> >
{
    return f * s;
}

// frequency / scalar -> frequency
template <typename Rep, typename Period, typename ScalarRep>
constexpr auto operator/ (frequency<Rep, Period> const & f, ScalarRep const & s)
-> /* frequency<std::common_type_t<Rep, ScalarRep>, Period> */
std::enable_if_t< std::is_convertible_v< ScalarRep
                                       , std::common_type_t<Rep, ScalarRep> >
                  && !is_frequency_v<ScalarRep>
                , frequency<std::common_type_t<Rep, ScalarRep>, Period> >
{
    using CF = frequency<std::common_type_t<Rep, ScalarRep>, Period>;
    return CF { CF(f).count() / s };
}

// frequency / frequency -> scalar
template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr auto operator/ (frequency<Rep1, Period1> const & lhs,
                          frequency<Rep2, Period2> const & rhs)
-> std::common_type_t<Rep1, Rep2>
{
    using CF = std::common_type_t< frequency<Rep1, Period1>
                                 , frequency<Rep2, Period2> >;
    return CF(lhs).count() / CF(rhs).count();
}

// scalar / frequency -> duration
template <typename ScalarRep, typename Rep, typename Period>
constexpr auto operator/ (ScalarRep const & s, frequency<Rep, Period> const & f)
-> /* duration< std::common_type_t<ScalarRep, Rep>
    *         , nonstd::ratio::invert_t<Period> > */
std::enable_if_t< std::is_convertible_v< ScalarRep
                                       , std::common_type_t<Rep, ScalarRep> >
                  && !is_frequency_v<ScalarRep>
                , duration< std::common_type_t<ScalarRep, Rep>
                          , nonstd::ratio::invert_t<Period> > >
{
    using CD = duration< std::common_type_t<ScalarRep, Rep>
                       , nonstd::ratio::invert_t<Period> >;
    using CF = frequency< std::common_type_t<ScalarRep, Rep>, Period >;
    return CD { s / CF(f).count() };
}

// frequency % scalar -> frequency
template <typename Rep, typename Period, typename ScalarRep>
constexpr auto operator% (frequency<Rep, Period> const & f, ScalarRep const & s)
-> /* frequency<std::common_type_t<Rep, ScalarRep>, Period> */
std::enable_if_t< std::is_convertible_v< ScalarRep
                                       , std::common_type_t<Rep, ScalarRep> >
                  && !is_frequency_v<ScalarRep>
                , frequency<std::common_type_t<Rep, ScalarRep>, Period> >
{
    using CF = frequency<std::common_type_t<Rep, ScalarRep>, Period>;
    return CF { CF(f).count() % s };
}

// frequency % frequency -> frequency
template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr auto operator% (frequency<Rep1, Period1> const & lhs,
                          frequency<Rep2, Period2> const & rhs)
-> std::common_type_t<Rep1, Rep2>
{
    using CF = std::common_type_t< frequency<Rep1, Period1>
                                 , frequency<Rep2, Period2> >;
    return CF { CF(lhs).count() % CF(rhs).count() };
}


// Frequency Comparisons
// =============================================================================
template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr bool operator== (frequency<Rep1, Period1> const & lhs,
                           frequency<Rep2, Period2> const & rhs)
{
    using CF = std::common_type_t< frequency<Rep1, Period1>
                                 , frequency<Rep2, Period2> >;
    return CF(lhs).count() == CT(rhs).count();
}
template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr bool operator!= (frequency<Rep1, Period1> const & lhs,
                           frequency<Rep2, Period2> const & rhs)
{
    return !(lhs == rhs);
}
template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr bool operator<  (frequency<Rep1, Period1> const & lhs,
                           frequency<Rep2, Period2> const & rhs)
{
    using CF = std::common_type_t< frequency<Rep1, Period1>
                                 , frequency<Rep2, Period2> >;
    return CF(lhs).count() < CF(rhs).count();
}
template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr bool operator<= (frequency<Rep1, Period1> const & lhs,
                           frequency<Rep2, Period2> const & rhs)
{
    return !(rhs < lhs);
}
template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr bool operator>  (frequency<Rep1, Period1> const & lhs,
                           frequency<Rep2, Period2> const & rhs)
{
    return rhs < lhs;
}
template <typename Rep1, typename Period1, typename Rep2, typename Period2>
constexpr bool operator>= (frequency<Rep1, Period1> const & lhs,
                           frequency<Rep2, Period2> const & rhs)
{
    return !(lhs < rhs);
}


/** Mixed Frequency and Duration Arithmetic
 *  ============================================================================
 *  TODO: If we want to be awesome ("awesome") like the standard, we can build
 *        partial class specializations to check if numerators/denominators are
 *        == 1, and skip divisions and/or multiplications for these conversions.
 */
// frequency * duration -> scalar
template <typename FRep, typename FPeriod, typename DRep, typename DPeriod>
constexpr auto operator* (frequency<FRep, FPeriod> const & f,
                          duration<DRep, DPeriod> const & d)
-> /* std::common_type_t<DRep, FRep> */
std::enable_if_t< std::is_convertible_v<DRep, std::common_type_t<DRep, FRep>>
                  && std::is_convertible_v<FRep, std::common_type_t<DRep, FRep>>
                , std::common_type_t<DRep, FRep> >
{
    using dp_t = typename duration<DRep, DPeriod>::period;
    using fp_t = typename frequency<FRep, FPeriod>::period;
    return f.count() * d.count() * dp_t::num * fp_t::num
         / (dp_t::den * fp_t::den);
}

// duration * frequency -> scalar
template <typename DRep, typename DPeriod, typename FRep, typename FPeriod>
constexpr auto operator* (duration<DRep, DPeriod> const & d,
                          frequency<FRep, FPeriod> const & f)
-> /* std::common_type_t<DRep, FRep> */
std::enable_if_t< std::is_convertible_v<DRep, std::common_type_t<DRep, FRep>>
                  && std::is_convertible_v<FRep, std::common_type_t<DRep, FRep>>
                , std::common_type_t<DRep, FRep> >
{
    using dp_t = typename duration<DRep, DPeriod>::period;
    using fp_t = typename frequency<FRep, FPeriod>::period;
    return f.count() * d.count() * dp_t::num * fp_t::num
         / (dp_t::den * fp_t::den);
}


/** `duration_of` Frequencies and `frequency_of` Durations Conversion
 *  ============================================================================
 *  TODO: If we want to be awesome ("awesome") like the standard, we can build
 *        partial class specializations to check if numerators/denominators are
 *        == 1, and skip divisions and/or multiplications for these conversions.
 */
template <typename ToFrequency, typename Rep, typename Period>
constexpr auto frequency_of(duration<Rep, Period> const & d)
-> /* ToFrequency */
std::enable_if_t< is_frequency_v<ToFrequency>
                  && std::is_convertible_v<Rep, typename ToFrequency::rep>
                , ToFrequency >
{
    using dp_t = typename duration<Rep, Period>::period;
    using fp_t = typename ToFrequency::period;
    return ToFrequency {
        dp_t::den * fp_t::den / (d.count() * dp_t::num * fp_t::num)
    };
}

template <typename ToDuration, typename Rep, typename Period>
constexpr auto duration_of(frequency<Rep, Period> const & f)
-> /* ToDuration */
std::enable_if_t< is_duration_v<ToDuration>
                  && std::is_convertible_v<Rep, typename ToDuration::rep>
                , ToDuration >
{
    using dp_t = typename ToDuration::period;
    using fp_t = typename frequency<Rep, Period>::period;
    return ToDuration {
        dp_t::den * fp_t::den / (f.count() * dp_t::num * fp_t::num)
    };
}


} /* namespace chrono */


/** User-Defined Chrono Literals
 *  ============================================================================
 */
namespace literals::chrono_literals {
using namespace std::literals::chrono_literals;

// TODO: Find out what happens when we try to build a literal that can't be
//       represented by the given `rep`. Make sure `frequency` and `duration` do
//       the same thing.
// TODO: Consider mapping ex; `_s` and `_min` to `s` and `min` s.t. we can keep
//       frequency and duration UDLs consistent in the code?

constexpr auto operator"" _Hz(udl_int_t   hz) {
    return chrono::hertz { hz };
}
constexpr auto operator"" _Hz(udl_float_t hz) {
    return chrono::frequency<f_long> { hz };
}

constexpr auto operator"" _kHz(udl_int_t   khz) {
    return chrono::kilohertz { khz };
}
constexpr auto operator"" _kHz(udl_float_t khz) {
    return chrono::frequency<f_long, std::kilo> { khz };
}

constexpr auto operator"" _MHz(udl_int_t   mhz) {
    return chrono::megahertz { mhz };
}
constexpr auto operator"" _MHz(udl_float_t mhz) {
    return chrono::frequency<f_long, std::mega> { mhz };
}

constexpr auto operator"" _GHz(udl_int_t   ghz) {
    return chrono::gigahertz { ghz };
}
constexpr auto operator"" _GHz(udl_float_t ghz) {
    return chrono::frequency<f_long, std::giga> { ghz };
}

constexpr auto operator"" _THz(udl_int_t   thz) {
    return chrono::gigahertz { thz };
}
constexpr auto operator"" _THz(udl_float_t thz) {
    return chrono::frequency<f_long, std::tera> { thz };
}


constexpr auto operator"" _bps(udl_int_t bps) {
    return chrono::beats_per_second { bps };
}
constexpr auto operator"" _bps(udl_float_t bps) {
    return chrono::frequency<f_long> { bps };
}

constexpr auto operator"" _bpm(udl_int_t bpm) {
    return chrono::beats_per_minute { bpm };
}
constexpr auto operator"" _bpm(udl_float_t bpm) {
    return chrono::frequency<f_long, std::ratio<1, 60>> { bpm };
}

} /* namespace literals::chrono_literals */

} /* namespace nonstd */


/** Operator Overloads that need to be in the global namespace
 *  ============================================================================
 *  I would much prefer to define this next to the `scalar / frequency ->
 *  duration` `operator/` overload, but it's not discoverable there; ADL won't
 *  apply (because std::chrono::duration isn't defined in nonstd::chrono::).
 */
// scalar / duration -> frequency
template <typename ScalarRep, typename Rep, typename Period>
constexpr auto operator/ (ScalarRep const & s,
                          nonstd::chrono::duration<Rep, Period> const & d)
-> /* frequency< std::common_type_t<ScalarRep, Rep>
    *          , nonstd::ratio::invert_t<Period> > */
std::enable_if_t< std::is_convertible_v< ScalarRep
                                       , std::common_type_t<Rep, ScalarRep> >
                  && !nonstd::chrono::is_duration_v<ScalarRep>
                , nonstd::chrono::frequency< std::common_type_t<ScalarRep, Rep>
                                           , nonstd::ratio::invert_t<Period> > >
{
    using CD = nonstd::chrono::duration< std::common_type_t<ScalarRep, Rep>
                                       , Period >;
    using CF = nonstd::chrono::frequency< std::common_type_t<ScalarRep, Rep>
                                        , nonstd::ratio::invert_t<Period> >;
    return CF { s / CD(d).count() };
}
