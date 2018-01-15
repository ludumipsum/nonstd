/** Iteration Tool Extensions
 *  =========================
 *  C++11/14 added great automatic deduction and iteration facilities, but it's
 *  often missing little conveniences to make the task of using them as simple
 *  as it could be.
 */

#pragma once

#include <iterator>
#include <nonstd/nonstd.h>


namespace nonstd {

/** Range
 *  =====
 *  Lazily yield Ts in the range provided. Emulates python 3's range()
 */
template <typename T> struct Range;

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
    template <typename U> struct iterator; //< forward decl

    /* named start / stop to avoid colliding with begin() / end() */
    iterator<T> start;
    iterator<T> stop;

    constexpr Range(T begin, T end, T step = 1) noexcept
        : start { begin, end, step }
        , stop  { end, end, step }
    { }
    constexpr Range(T end) noexcept
        : start { 0, end }
        , stop  { end, end }
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

        U       value = 0;
        U const max   = 0;
        U const step  = 1;

        constexpr iterator() = default;
        constexpr iterator(U value, U max, U step = 1) noexcept
            : value ( value )
            , max   ( max   )
            , step  ( step  )
        { }

        constexpr iterator(iterator const &)             = default;
        constexpr iterator & operator=(iterator const &) = default;
        constexpr iterator(iterator &&)                  = default;
        constexpr iterator & operator=(iterator &&)      = default;
        ~iterator() noexcept                             = default;

        constexpr U const & operator*() const noexcept {
            return value;
        }

        constexpr iterator & operator++() noexcept {
            value = n2min(value + step, max);
            return *this;
        }
        constexpr iterator & operator++(int) noexcept {
            iterator tmp{ *this };
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


/** Slice
 *  =====
 *  Lazily iterate over `count` elements from the typed pointer `data`.
 */
template<typename T>
class Slice {
public:
    T*  start;
    T*  stop;
    u64 stride;

    constexpr inline Slice(T* data, u64 count, u64 stride = 1)
        : start  ( data       )
        , stop   ( data+count )
        , stride ( stride     ) { }
    constexpr inline Slice(void* data, u64 count, u64 stride = 1)
        : start  ( (T*)data           )
        , stop   ( ((T*)data) + count )
        , stride ( stride             ) { }

    T& operator[](u64 index) {
        return *( start + (index * stride) );
    }

    class iterator {
    public:
        typedef std::output_iterator_tag iterator_category;

        Slice const& s;
        T* cursor;

        constexpr inline iterator(Slice const& s, u64 offset = 0)
            : s      ( s                )
            , cursor ( s.start + offset ) { }
        constexpr inline iterator(iterator const& i)
            : s      ( i.s      )
            , cursor ( i.cursor ) { }
        constexpr inline iterator& operator=(iterator const& i) {
            if (this != &i) {
                this->s      = i.s;
                this->cursor = i.cursor;
            }
        }

        constexpr inline T& operator*() { return *cursor; }

        constexpr inline bool operator==(iterator const& i) {
            return cursor == i.cursor;
        }
        constexpr inline bool operator!=(iterator const& i) {
            return cursor != i.cursor;
        }

        constexpr inline iterator& operator++() {
            cursor += s.stride;
            return *this;
        }
        constexpr inline iterator operator++(int) {
            iterator ret(*this);
            cursor += s.stride;
            return ret;
        }

        constexpr inline iterator& operator--() {
            cursor -= s.stride;
            return *this;
        }
        constexpr inline iterator operator--(int) {
            iterator ret(*this);
            cursor -= s.stride;
            return ret;
        }

        constexpr inline iterator& operator+=(u64 steps) {
            cursor += (steps * s.stride);
            return *this;
        }
        constexpr inline iterator& operator-=(u64 steps) {
            cursor -= (steps * s.stride);
            return *this;
        }
    };

    constexpr inline iterator begin() const {
        return Slice<T>::iterator(*this, 0);
    }
    constexpr inline iterator end()   const {
        return Slice<T>::iterator(*this, stop - start);
    }
};

/* Create a lazy iterator over `count` elements from the typed pointer `data`. */
template<typename T> inline
Slice<T> slice(T* data, u64 count, u64 stride = 1) {
    return Slice<T>(data, count, stride);
}

} /* namespace nonstd */
