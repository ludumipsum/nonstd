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
#include <vector>

#include "error.h"
#include "math.h"
#include "primitive_types.h"


namespace nonstd {

// Forward declaration
template <typename T> struct range_t;

/** `nonstd::range` Free Functions
 *   =============================
 */
template<typename T>
constexpr range_t<T> range(T begin, T end, T step = 1) {
    return range_t<T>{ begin, end, step };
}

template<typename T>
constexpr range_t<T> range(T end) {
    return range_t<T>{ 0, end, 1 };
}


template <typename T>
struct range_t {
    static_assert(std::is_arithmetic_v<T>,
        "nonstd::range is currently only safe with arithmetic types. I'm not "
        "saying it won't work for the type given, but you should test it. "
        "And then modify this assert.");

    template <typename U> struct iterator;

    /* named start / stop to avoid colliding with begin() / end() */
    iterator<T> start;
    iterator<T> stop;

    constexpr range_t(T begin, T end, T step = 1) noexcept
        : start { begin, end, step }
        , stop  { end,   end, step }
    { }

    constexpr iterator<T> begin() const noexcept { return start; }
    constexpr iterator<T> end()   const noexcept { return stop; }

    /** container fill utilities
     *  ------------------------
     *  It's sometimes handy to have a vector or array that's prefilled with a
     *  sequential run of numbers, like those from a range.
     */
    template <typename IterableType>
    inline IterableType& fill(IterableType& iterable) {
        auto range_it = std::begin(*this);
        for (auto& element : iterable) {
            if(range_it == this->end()) {
                break;
            }
            element = *range_it;
            ++ range_it;
        }
        return iterable;
    }

    /** container fill-to-vector utilities
     *  ----------------------------------
     *  Same as the above, but specialized for vectors in particular.
     */
    template <typename ValueType>
    inline std::vector<ValueType>& fill(std::vector<ValueType>& vector) {
        for (auto const& element : *this) {
            vector.push_back(element);
        }
        return vector;
    }

    /** container implicit cast initializers
     *  ------------------------------------
     *  Call-through to fill for implicit casts. This permits assignment
     *  initialization for containers.
     */
    template <typename ValueType>
    inline operator std::vector<ValueType>() {
        std::vector<ValueType> ret { };
        fill(ret);
        return ret;
    }
    template <typename ValueType, size_t Length>
    inline operator std::array<ValueType, Length> () {
        std::array<ValueType, Length> ret { };
        fill(ret);
        return ret;
    }

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
