/** Range
 *  =====
 *  C++11/14 added great automatic deduction and iteration facilities, but it's
 *  often missing little conveniences to make the task of using them as simple
 *  as it could be. This iterator will lazily yield types of `T`. It's behavior
 *  is based on the python `range()` function.
 */
#pragma once

#include <iterator>
#include <type_traits>
#include "primitive_types.h"
#include "math.h"


namespace nonstd {

// Forward declaration
template <typename T> struct Range;

/** `nonstd::range` Free Functions
 *   =============================
 */
template<typename T>
constexpr Range<T> range(T begin, T end, T step = 1) {
    return Range<T>{ begin, end, step };
}

template<typename T>
constexpr Range<T> range(T end) {
    return Range<T>{ 0, end, 1 };
}


template <typename T>
struct Range {
    static_assert(std::is_arithmetic_v<T>,
        "nonstd::range is currently only safe with arithmetic types. I'm not "
        "saying it won't work for the type given, but you should test it. "
        "And then modify this assert.");

    template <typename U> struct iterator;

    /* named start / stop to avoid colliding with begin() / end() */
    iterator<T> start;
    iterator<T> stop;

    constexpr Range(T begin, T end, T step = 1) noexcept
        : start { begin, end, step }
        , stop  { end,   end, step }
    { }

    constexpr iterator<T> begin() const noexcept { return start; }
    constexpr iterator<T> end()   const noexcept { return stop; }


    // Redefining the template parameter is useless _except_ for the part where
    // it un-breaks an inscrutable MSVC error.
    template <typename U>
    struct iterator {
        /* Iterator type trait boilerplate. See,
         * http://en.cppreference.com/w/cpp/iterator/iterator_traits
         * http://www.cplusplus.com/reference/iterator/
         */
        using difference_type   = ptrdiff;
        using value_type        = U;
        using pointer           = U*;
        using reference         = U&;
        using iterator_category = std::input_iterator_tag;

        U       value;
        U const max;
        U const step;

        constexpr iterator(U value, U max, U step) noexcept
            : value ( value )
            , max   ( max   )
            , step  ( step  )
        { }

        constexpr iterator() = delete;
        constexpr iterator(iterator const &)            = default;
        constexpr iterator& operator=(iterator const &) = default;
        constexpr iterator(iterator &&)                 = default;
        constexpr iterator& operator=(iterator &&)      = default;
        ~iterator() noexcept                            = default;

        constexpr U const & operator*() const noexcept {
            return value;
        }

        constexpr iterator& operator++() noexcept {
            value = n2min(value + step, max);
            return *this;
        }
        constexpr iterator& operator++(int) noexcept {
            iterator tmp { *this };
            value = n2min(value + step, max);
            return tmp;
        }

        friend constexpr bool operator==(iterator const & lhs,
                                         iterator const & rhs) noexcept {
            return lhs.value == rhs.value;
        }
        friend constexpr bool operator!=(iterator const & lhs,
                                         iterator const & rhs) noexcept {
            return lhs.value != rhs.value;
        }
    };
};

} /* namespace nonstd */
